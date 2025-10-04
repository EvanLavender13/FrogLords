#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#include "input/input.h"
#include "gui/gui.h"
#include "core/camera.h"
#include "core/character_controller.h"
#include "core/orientation.h"
#include "core/locomotion.h"
#include "rendering/wireframe.h"
#include "rendering/renderer.h"
#include "core/scene.h"
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>
#include "imgui.h"

static sg_pass_action pass_action;
static bool show_demo_window = true;
static float demo_value = 0.5f;
static camera* cam = nullptr;
static scene* scn = nullptr;
static wireframe_renderer* renderer = nullptr;
static character_controller* character = nullptr;
static orientation_system* orientation = nullptr;
static locomotion_system* locomotion = nullptr;
static float wireframe_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static glm::vec3 wheel_direction = glm::vec3(0, 0, 1);  // Debug: wheel orientation
static float wheel_distance = 0.0f;  // Debug: signed distance for wheel rotation

static void init(void) {
    sg_desc desc = {};
    desc.environment = sglue_environment();
    desc.logger.func = slog_func;
    sg_setup(&desc);

    pass_action = {};
    pass_action.colors[0].load_action = SG_LOADACTION_CLEAR;
    pass_action.colors[0].clear_value = { 0.1f, 0.1f, 0.1f, 1.0f };

    // Initialize input system
    Input::init();

    // Initialize GUI system
    GUI::init();

    // Initialize renderer
    renderer = new wireframe_renderer();
    renderer->init();

    // Initialize character
    character = new character_controller();

    // Initialize animation systems
    orientation = new orientation_system();
    locomotion = new locomotion_system();

    // Initialize camera in follow mode
    cam = new camera(character->position, 5.0f, 15.0f, 0.0f);
    cam->set_mode(camera_mode::follow);

    // Initialize scene
    scn = new scene();

    // Add static grid floor
    wireframe_mesh floor = generate_grid_floor(10.0f, 10);
    scn->add_object(floor);
}

static void frame(void) {
    float dt = (float)sapp_frame_duration();

    // Handle camera controls (when not over GUI)
    if (!GUI::wants_mouse()) {
        static float lastMouseX = 0.0f;
        static float lastMouseY = 0.0f;

        if (Input::is_mouse_button_down(SAPP_MOUSEBUTTON_RIGHT)) {
            float deltaX = (Input::mouse_x() - lastMouseX) * 0.5f;
            float deltaY = (Input::mouse_y() - lastMouseY) * 0.5f;
            cam->orbit(-deltaX, deltaY);
        }

        float scrollDelta = Input::mouse_scroll_y();
        if (scrollDelta != 0.0f) {
            cam->zoom(-scrollDelta * 0.5f);
        }

        lastMouseX = Input::mouse_x();
        lastMouseY = Input::mouse_y();
    }

    // Character physics update
    character->apply_input(*cam, dt);
    character->update(dt);

    // Animation systems update
    glm::vec3 horizontal_velocity = character->velocity;
    horizontal_velocity.y = 0.0f;
    orientation->update(horizontal_velocity, dt);
    locomotion->update(horizontal_velocity, dt, character->is_grounded);

    // Camera follow character
    if (cam->get_mode() == camera_mode::follow) {
        cam->follow_update(character->position, dt);
    }

    // Update input state (resets scroll delta)
    Input::update();

    // Begin GUI frame
    GUI::begin_frame();

    // Character debug window
    ImGui::SetNextWindowSize(ImVec2(350, 500), ImGuiCond_FirstUseEver);
    if (GUI::Panel::begin("Character Debug", &show_demo_window)) {
        GUI::Widget::text("Position: (%.2f, %.2f, %.2f)",
            character->position.x, character->position.y, character->position.z);
        GUI::Widget::text("Velocity: (%.2f, %.2f, %.2f)",
            character->velocity.x, character->velocity.y, character->velocity.z);
        GUI::Widget::text("Speed: %.2f m/s", glm::length(character->velocity));
        GUI::Widget::text("Grounded: %s", character->is_grounded ? "YES" : "NO");

        if (character->is_grounded) {
            GUI::Widget::text("Ground Normal: (%.2f, %.2f, %.2f)",
                character->ground_normal.x, character->ground_normal.y, character->ground_normal.z);
        }

        GUI::Widget::text("");
        GUI::Widget::text("--- Animation ---");
        GUI::Widget::text("Phase: %.2f", locomotion->phase);
        GUI::Widget::text("Distance: %.2f m", locomotion->distance_traveled);
        GUI::Widget::text("Yaw: %.1f deg", orientation->current_yaw * 180.0f / 3.14159f);

        GUI::Widget::text("");
        GUI::Widget::slider_float("Jump Velocity", &character->jump_velocity, 0.0f, 10.0f);
        GUI::Widget::slider_float("Ground Accel", &character->ground_accel, 0.0f, 100.0f);
        GUI::Widget::slider_float("Air Accel", &character->air_accel, 0.0f, 50.0f);
        GUI::Widget::slider_float("Max Speed", &character->max_speed, 0.0f, 20.0f);
        GUI::Widget::slider_float("Friction", &character->friction, 0.0f, 2.0f);
        GUI::Widget::slider_float("Gravity", &character->gravity, -20.0f, 0.0f);

        GUI::Widget::text("");
        GUI::Widget::slider_float("Yaw Smoothing", &orientation->yaw_smoothing, 0.1f, 20.0f);
        GUI::Widget::slider_float("Stride Length", &locomotion->run_state.stride_length, 0.5f, 5.0f);

        GUI::Widget::text("");
        GUI::Widget::text("FPS: %.1f", 1.0f / sapp_frame_duration());
    }
    GUI::Panel::end();

    // Render pass
    sg_pass pass = {};
    pass.action = pass_action;
    pass.swapchain = sglue_swapchain();
    sg_begin_pass(&pass);

    // Render all 3D wireframes
    float aspect = (float)sapp_width() / (float)sapp_height();
    glm::vec4 color(wireframe_color[0], wireframe_color[1], wireframe_color[2], wireframe_color[3]);
    for (const auto& mesh : scn->objects()) {
        renderer->draw(mesh, *cam, aspect, color);
    }

    // Draw character debug spheres with pose offset
    simple_pose current_pose = locomotion->get_current_pose();
    glm::vec3 pose_offset = current_pose.root_offset;

    wireframe_mesh bumper_vis = generate_sphere(8, 8, character->bumper.radius);
    bumper_vis.position = character->position + pose_offset;
    renderer->draw(bumper_vis, *cam, aspect, glm::vec4(0, 1, 1, 1));  // Cyan

    wireframe_mesh weightlifter_vis = generate_sphere(6, 6, character->weightlifter.radius);
    weightlifter_vis.position = character->position + pose_offset + glm::vec3(0, -0.4f, 0);
    renderer->draw(weightlifter_vis, *cam, aspect, glm::vec4(1, 1, 0, 1));  // Yellow

    // Draw orientation indicator (small sphere in forward direction)
    float yaw = orientation->get_yaw();
    glm::vec3 forward_dir(sinf(yaw), 0, cosf(yaw));
    glm::vec3 right_dir(-cosf(yaw), 0, sinf(yaw));
    wireframe_mesh yaw_indicator = generate_sphere(4, 4, 0.1f);
    yaw_indicator.position = character->position + pose_offset + forward_dir * 0.8f;
    renderer->draw(yaw_indicator, *cam, aspect, glm::vec4(0, 1, 0, 1));  // Green

    // Draw phase trail (breadcrumb spheres showing distance-phased motion)
    for (int i = 0; i < 10; i++) {
        float phase_offset = (float)i * 0.1f;
        float trail_phase = fmodf(locomotion->phase + phase_offset, 1.0f);
        float brightness = 1.0f - (float)i * 0.08f;

        wireframe_mesh trail_sphere = generate_sphere(4, 4, 0.12f);
        trail_sphere.position = character->position + pose_offset - forward_dir * (float)i * 0.25f;
        trail_sphere.position.y += sinf(trail_phase * 2.0f * 3.14159f) * 0.25f;
        renderer->draw(trail_sphere, *cam, aspect, glm::vec4(brightness, brightness, brightness, 1));
    }

    // Draw foot positions (only when grounded)
    if (character->is_grounded) {
        // Left foot oscillates based on animation phase
        float left_foot_offset = sinf(locomotion->phase * 2.0f * 3.14159f) * 0.4f;
        glm::vec3 left_foot_pos = character->position + forward_dir * left_foot_offset + right_dir * 0.2f;
        left_foot_pos.y = 0.0f;  // Ground level
        wireframe_mesh left_foot = generate_sphere(4, 4, 0.08f);
        left_foot.position = left_foot_pos;
        renderer->draw(left_foot, *cam, aspect, glm::vec4(1, 0, 1, 1));  // Magenta

        // Right foot (opposite phase - offset by 0.5)
        float right_phase = fmodf(locomotion->phase + 0.5f, 1.0f);
        float right_foot_offset = sinf(right_phase * 2.0f * 3.14159f) * 0.4f;
        glm::vec3 right_foot_pos = character->position + forward_dir * right_foot_offset + right_dir * -0.2f;
        right_foot_pos.y = 0.0f;  // Ground level
        wireframe_mesh right_foot = generate_sphere(4, 4, 0.08f);
        right_foot.position = right_foot_pos;
        renderer->draw(right_foot, *cam, aspect, glm::vec4(1, 0.5f, 0, 1));  // Orange
    }

    // Render GUI
    GUI::render();

    sg_end_pass();
    sg_commit();
}

static void cleanup(void) {
    delete locomotion;
    delete orientation;
    delete character;
    delete scn;
    delete cam;
    delete renderer;
    GUI::shutdown();
    sg_shutdown();
}

static void event(const sapp_event* e) {
    // Forward events to GUI system first (for mouse/keyboard capture)
    GUI::handle_event(e);

    // Forward events to input system
    Input::process_event(e);
}

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    sapp_desc desc = {};
    desc.init_cb = init;
    desc.frame_cb = frame;
    desc.cleanup_cb = cleanup;
    desc.event_cb = event;
    desc.width = 1920;
    desc.height = 1080;
    desc.window_title = "FrogLords - GUI Demo";
    desc.icon.sokol_default = true;
    desc.enable_clipboard = true;  // Enable clipboard for ImGui
    desc.logger.func = slog_func;
    return desc;
}
