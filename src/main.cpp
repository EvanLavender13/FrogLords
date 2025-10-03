#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#include "input/input.h"
#include "gui/gui.h"
#include "core/camera.h"
#include "rendering/wireframe.h"
#include "rendering/renderer.h"
#include "core/scene.h"
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>

static sg_pass_action pass_action;
static bool show_demo_window = true;
static float demo_value = 0.5f;
static camera* cam = nullptr;
static scene* scn = nullptr;
static wireframe_renderer* renderer = nullptr;
static float wireframe_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

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

    // Initialize camera (center, distance, latitude, longitude)
    cam = new camera(glm::vec3(0.0f, 0.0f, 0.0f), 8.0f, 15.0f, 0.0f);

    // Initialize scene
    scn = new scene();

    // Add static grid floor
    wireframe_mesh floor = generate_grid_floor(10.0f, 10);
    scn->add_object(floor);
}

static void frame(void) {
    // Handle camera controls (when not over GUI)
    if (!GUI::wants_mouse()) {
        static float lastMouseX = 0.0f;
        static float lastMouseY = 0.0f;

        if (Input::is_mouse_button_down(SAPP_MOUSEBUTTON_LEFT)) {
            float deltaX = (Input::mouse_x() - lastMouseX) * 0.5f;
            float deltaY = (Input::mouse_y() - lastMouseY) * 0.5f;
            cam->orbit(deltaX, -deltaY);
        }

        float scrollDelta = Input::mouse_scroll_y();
        if (scrollDelta != 0.0f) {
            cam->zoom(-scrollDelta * 0.5f);
        }

        lastMouseX = Input::mouse_x();
        lastMouseY = Input::mouse_y();
    }

    // Update input state (resets scroll delta)
    Input::update();

    // Begin GUI frame
    GUI::begin_frame();

    // Wireframe spawner window
    if (GUI::Panel::begin("Wireframe Spawner", &show_demo_window)) {
        GUI::Widget::text("3D Wireframe Scene");
        GUI::Widget::text("Objects: %zu", scn->object_count());

        if (GUI::Widget::button("Spawn Sphere")) {
            // Generate sphere at random position
            float x = ((float)(rand() % 1000) / 100.0f) - 5.0f;  // -5 to 5
            float y = ((float)(rand() % 1000) / 100.0f) - 5.0f;
            float z = ((float)(rand() % 1000) / 100.0f) - 5.0f;

            wireframe_mesh sphere = generate_sphere(8, 8, 1.0f);
            sphere.position = glm::vec3(x, y, z);
            scn->add_object(sphere);
        }

        if (GUI::Widget::button("Spawn Box")) {
            float x = ((float)(rand() % 1000) / 100.0f) - 5.0f;  // -5 to 5
            float y = ((float)(rand() % 1000) / 100.0f) - 5.0f;
            float z = ((float)(rand() % 1000) / 100.0f) - 5.0f;

            wireframe_mesh box = generate_box(1.0f, 1.0f, 1.0f);
            box.position = glm::vec3(x, y, z);
            scn->add_object(box);
        }

        if (GUI::Widget::button("Clear All")) {
            scn->clear();
        }

        GUI::Widget::color_edit("Wireframe Color", wireframe_color);
        GUI::Widget::color_edit("Clear Color", &pass_action.colors[0].clear_value.r);
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

    // Render GUI
    GUI::render();

    sg_end_pass();
    sg_commit();
}

static void cleanup(void) {
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
    desc.width = 800;
    desc.height = 600;
    desc.window_title = "FrogLords - GUI Demo";
    desc.icon.sokol_default = true;
    desc.enable_clipboard = true;  // Enable clipboard for ImGui
    desc.logger.func = slog_func;
    return desc;
}
