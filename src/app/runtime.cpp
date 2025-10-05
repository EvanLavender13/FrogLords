#include "app/runtime.h"
#include "sokol_app.h"
#include "sokol_glue.h"
#include "sokol_log.h"
#include "input/input.h"
#include "gui/gui.h"
#include "gui/character_panel.h"
#include "character/tuning.h"
#include "rendering/debug_draw.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <algorithm>

namespace {
constexpr float WHEEL_RADIUS = 0.45f;
constexpr float TWO_PI = 6.28318530718f;
} // namespace

app_runtime& runtime() {
    static app_runtime instance;
    return instance;
}

void app_runtime::initialize() {
    if (initialized) {
        return;
    }

    sg_desc desc = {};
    desc.environment = sglue_environment();
    desc.logger.func = slog_func;
    sg_setup(&desc);

    pass_action = {};
    pass_action.colors[0].load_action = SG_LOADACTION_CLEAR;
    pass_action.colors[0].clear_value = {0.1f, 0.1f, 0.1f, 1.0f};

    input::init();
    gui::init();

    renderer.init();

    character = character_controller();
    orientation = orientation_system();
    locomotion = locomotion_system();
    character_params.read_from(character);

    character::sync_locomotion_targets(character, locomotion);

    cam = camera(character.position, orbit_config{5.0f, 15.0f, 0.0f});
    cam.set_mode(camera_mode::follow);

    scn = scene();
    wireframe_mesh floor = generate_grid_floor(20.0f, 20);
    scn.add_object(floor);

    aabb low_platform;
    low_platform.center = glm::vec3(8.0f, 0.75f, 0.0f);
    low_platform.half_extents = glm::vec3(1.5f, 0.75f, 1.5f);
    scn.add_collision_box(low_platform);

    aabb high_platform;
    high_platform.center = glm::vec3(12.0f, 1.5f, 0.0f);
    high_platform.half_extents = glm::vec3(1.5f, 1.5f, 1.5f);
    scn.add_collision_box(high_platform);

    aabb test_platform;
    test_platform.center = glm::vec3(0.0f, 0.05f, 15.0f);
    test_platform.half_extents = glm::vec3(10.0f, 0.05f, 10.0f);
    scn.add_collision_box(test_platform);

    initialized = true;
}

void app_runtime::shutdown() {
    if (!initialized) {
        return;
    }

    renderer.shutdown();
    gui::shutdown();
    sg_shutdown();

    initialized = false;
}

void app_runtime::frame() {
    if (!initialized) {
        return;
    }

    float dt = static_cast<float>(sapp_frame_duration());

    ensure_static_meshes();

    if (!gui::wants_mouse()) {
        static float last_mouse_x = 0.0f;
        static float last_mouse_y = 0.0f;

        if (input::is_mouse_button_down(SAPP_MOUSEBUTTON_RIGHT)) {
            float delta_x = (input::mouse_x() - last_mouse_x) * 0.5f;
            float delta_y = (input::mouse_y() - last_mouse_y) * 0.5f;
            cam.orbit(-delta_x, delta_y);
        }

        float scroll_delta = input::mouse_scroll_y();
        if (scroll_delta != 0.0f) {
            cam.zoom(-scroll_delta * 0.5f);
        }

        last_mouse_x = input::mouse_x();
        last_mouse_y = input::mouse_y();
    }

    update_simulation(dt);

    input::update();

    gui::begin_frame();
    gui::draw_character_panel(panel_state, character, locomotion, character_params);

    render_world();
}

void app_runtime::handle_event(const sapp_event* e) {
    gui::handle_event(e);
    input::process_event(e);
}

void app_runtime::ensure_static_meshes() {
    if (static_meshes_initialized) {
        return;
    }

    unit_circle = generate_circle(glm::vec3(0.0f), circle_config{1.0f});
    unit_sphere_8 = generate_sphere(sphere_config{8, 8, 1.0f});
    unit_sphere_6 = generate_sphere(sphere_config{6, 6, 1.0f});
    unit_sphere_4 = generate_sphere(sphere_config{4, 4, 1.0f});

    static_meshes_initialized = true;
}

void app_runtime::update_simulation(float dt) {
    character.apply_input(cam, dt);
    character.update(&scn, dt);

    float landing_impact = character.get_landing_impact();
    if (landing_impact > 0.0f) {
        locomotion.vertical_spring.add_impulse(-landing_impact * 0.5f);
    }

    glm::vec3 horizontal_velocity = character.velocity;
    horizontal_velocity.y = 0.0f;

    // Use input intent for animation/orientation (allows running against walls)
    glm::vec3 intended_velocity = character.input_direction * character.max_speed;

    if (input::is_mouse_button_down(SAPP_MOUSEBUTTON_RIGHT)) {
        orientation.update_forced(forced_orientation_input{cam.get_yaw(), dt});
    } else {
        orientation.update(intended_velocity, dt);
    }

    character::sync_locomotion_targets(character, locomotion);
    locomotion.update(intended_velocity, dt, character.is_grounded, character.ground_height);

    float angular_speed = 0.0f;
    if (WHEEL_RADIUS > 0.0001f) {
        angular_speed = locomotion.smoothed_speed / WHEEL_RADIUS;
    }
    wheel_spin_angle += angular_speed * dt;
    if (wheel_spin_angle > TWO_PI) {
        wheel_spin_angle = std::fmod(wheel_spin_angle, TWO_PI);
    }

    if (cam.get_mode() == camera_mode::follow) {
        cam.follow_update(character.position, dt);
    }
}

void app_runtime::render_world() {
    sg_pass pass = {};
    pass.action = pass_action;
    pass.swapchain = sglue_swapchain();
    sg_begin_pass(&pass);

    float aspect = static_cast<float>(sapp_width()) / static_cast<float>(sapp_height());

    glm::vec4 color(wireframe_color[0], wireframe_color[1], wireframe_color[2], wireframe_color[3]);
    for (const auto& mesh : scn.objects()) {
        renderer.draw(mesh, cam, aspect, color);
    }

    for (const auto& box : scn.collision_boxes()) {
        box_dimensions box_size{box.half_extents.x * 2.0f, box.half_extents.y * 2.0f,
                                box.half_extents.z * 2.0f};
        wireframe_mesh box_mesh = generate_box(box_size);
        box_mesh.position = box.center;

        glm::vec4 box_color = box.center.y < 1.0f ? glm::vec4(1, 1, 0, 1) : glm::vec4(1, 0, 0, 1);
        renderer.draw(box_mesh, cam, aspect, box_color);
    }

    debug::draw_context debug_ctx{renderer, cam,      aspect,        unit_circle,
                                   unit_sphere_8, unit_sphere_6, unit_sphere_4};

    debug::draw_character_state(debug_ctx, character, locomotion, orientation);
    debug::draw_physics_springs(debug_ctx, character, locomotion);
    debug::draw_locomotion_wheel(debug_ctx, character, locomotion, orientation, wheel_spin_angle);
    debug::draw_foot_positions(debug_ctx, character, locomotion, orientation);

    gui::render();

    sg_end_pass();
    sg_commit();
}
