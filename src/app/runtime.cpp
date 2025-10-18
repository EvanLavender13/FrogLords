#include "app/runtime.h"
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"
#include "input/input.h"
#include "gui/gui.h"
#include "gui/character_panel.h"
#include "rendering/debug_draw.h"
#include "rendering/debug_visualization.h"
#include "rendering/debug_validation.h"
#include "app/debug_generation.h"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#ifdef ENABLE_QUATERNION_TESTS
#include <cstdlib>
#endif

app_runtime& runtime() {
    static app_runtime instance;
    return instance;
}

void app_runtime::initialize() {
#ifdef ENABLE_QUATERNION_TESTS
    // Run quaternion validation test suite and exit immediately
    extern bool run_quaternion_validation_suite();
    bool tests_passed = run_quaternion_validation_suite();
    std::exit(tests_passed ? 0 : 1);
#endif

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

    world.init();

    // Verify mathematical assumptions about coordinate system
    debug_viz::validation::run_startup_checks();

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
            world.cam.orbit(-delta_x, delta_y);
        }

        float scroll_delta = input::mouse_scroll_y();
        if (scroll_delta != 0.0f) {
            world.cam.zoom(-scroll_delta * 0.5f);
        }

        last_mouse_x = input::mouse_x();
        last_mouse_y = input::mouse_y();
    }

    world.update(dt, panel_state);

    // Handle F3 key press to toggle debug visualization
    if (input::is_key_pressed(SAPP_KEYCODE_F3)) {
        debug_viz::toggle();
    }

    input::update();

    gui::begin_frame();

    // Create unified debug panel on left side
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(0.0f, static_cast<float>(sapp_height())), ImGuiCond_Always);
    ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;

    if (ImGui::Begin("Debug Panel", nullptr, flags)) {
        // Character tuning sections
        gui::draw_character_panel(panel_state, world.character, world.character_visuals,
                                  world.character_params);

        // Camera section
        gui::draw_camera_panel(camera_panel_state, world.cam);

        // FPS display at bottom
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("FPS: %.1f", 1.0f / sapp_frame_duration());
        gui::plot_histogram("FPS", 1.0f / sapp_frame_duration(), 5.0f, 0.0f, 200.0f, 60);
    }
    ImGui::End();

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

    unit_circle = foundation::generate_circle(glm::vec3(0.0f), {1.0f});
    unit_sphere_8 = foundation::generate_sphere({8, 8, 1.0f});
    unit_sphere_6 = foundation::generate_sphere({6, 6, 1.0f});
    unit_sphere_4 = foundation::generate_sphere({4, 4, 1.0f});

    static_meshes_initialized = true;
}

void app_runtime::render_world() {
    sg_pass pass = {};
    pass.action = pass_action;
    pass.swapchain = sglue_swapchain();
    sg_begin_pass(&pass);

    float aspect = static_cast<float>(sapp_width()) / static_cast<float>(sapp_height());

    glm::vec4 color(wireframe_color[0], wireframe_color[1], wireframe_color[2], wireframe_color[3]);
    for (const auto& mesh : world.scn.objects()) {
        renderer.draw(mesh, world.cam, aspect, color);
    }

    // Debug visualization (toggle with F3)
    if (debug_viz::is_enabled()) {
        debug::draw_context debug_ctx{renderer,      world.cam,     aspect,       unit_circle,
                                      unit_sphere_8, unit_sphere_6, unit_sphere_4};

        // Generate all debug primitives from the current world state.
        app::generate_debug_primitives(world.debug_list, world, panel_state);

        // Pass the populated list to the dumb renderer.
        debug::draw_primitives(debug_ctx, world.debug_list);
    }

    gui::render();

    sg_end_pass();
    sg_commit();
}
