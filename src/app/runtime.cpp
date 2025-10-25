#include "app/runtime.h"
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"
#include "input/input.h"
#include "gui/gui.h"
#include "rendering/debug_draw.h"
#include "rendering/debug_visualization.h"
#include "app/debug_generation.h"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

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

    world.init();

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

    // Track mouse position every frame to prevent stale delta accumulation
    static float last_mouse_x = 0.0f;
    static float last_mouse_y = 0.0f;

    if (!gui::wants_mouse()) {
        if (input::is_mouse_button_down(SAPP_MOUSEBUTTON_RIGHT)) {
            float delta_x = input::mouse_x() - last_mouse_x;
            float delta_y = input::mouse_y() - last_mouse_y;
            world.apply_camera_orbit(-delta_x, delta_y);
        }

        float scroll_delta = input::mouse_scroll_y();
        if (scroll_delta != 0.0f) {
            world.apply_camera_zoom(-scroll_delta);
        }
    }

    // Always update last mouse position (prevents camera jump when GUI releases control)
    last_mouse_x = input::mouse_x();
    last_mouse_y = input::mouse_y();

    world.update(dt);

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
        // Vehicle section
        auto vehicle_commands = gui::draw_vehicle_panel(
            vehicle_panel_state, world.character, world.vehicle_params, world.vehicle_visuals);

        // Apply vehicle parameter commands (unidirectional flow: GUI → commands → game state)
        apply_parameter_commands(vehicle_commands);

        // Camera section
        auto camera_commands =
            gui::draw_camera_panel(camera_panel_state, world.cam, world.cam_follow);

        // Apply camera commands (unidirectional flow: GUI → commands → game state)
        apply_camera_commands(camera_commands);

        // FOV section
        auto fov_commands = gui::draw_fov_panel(fov_panel_state, world.dynamic_fov);

        // Apply FOV commands (unidirectional flow: GUI → commands → game state)
        apply_fov_commands(fov_commands);

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

void app_runtime::apply_parameter_commands(const std::vector<gui::parameter_command>& commands) {
    for (const auto& cmd : commands) {
        switch (cmd.type) {
        case gui::parameter_type::MAX_SPEED:
            world.vehicle_params.max_speed = cmd.value;
            world.vehicle_params.apply_to(world.character, world.vehicle_visuals);
            break;
        case gui::parameter_type::ACCEL:
            world.vehicle_params.accel = cmd.value;
            world.vehicle_params.apply_to(world.character, world.vehicle_visuals);
            break;
        case gui::parameter_type::WEIGHT:
            world.vehicle_params.weight = cmd.value;
            world.vehicle_params.apply_to(world.character, world.vehicle_visuals);
            break;
        case gui::parameter_type::TURN_RATE:
            world.vehicle_params.turn_rate = cmd.value;
            world.vehicle_params.apply_to(world.character, world.vehicle_visuals);
            break;
        case gui::parameter_type::STEERING_REDUCTION_FACTOR:
            world.vehicle_params.steering_reduction_factor = cmd.value;
            world.vehicle_params.apply_to(world.character, world.vehicle_visuals);
            break;
        case gui::parameter_type::LEAN_MULTIPLIER:
            world.vehicle_params.lean_multiplier = cmd.value;
            world.vehicle_params.apply_to(world.character, world.vehicle_visuals);
            break;
        case gui::parameter_type::PITCH_MULTIPLIER:
            world.vehicle_params.pitch_multiplier = cmd.value;
            world.vehicle_params.apply_to(world.character, world.vehicle_visuals);
            break;
        case gui::parameter_type::TILT_STIFFNESS:
            world.vehicle_params.tilt_stiffness = cmd.value;
            world.vehicle_params.apply_to(world.character, world.vehicle_visuals);
            break;
        case gui::parameter_type::ORIENTATION_STIFFNESS:
            world.vehicle_params.orientation_stiffness = cmd.value;
            world.vehicle_params.apply_to(world.character, world.vehicle_visuals);
            break;
        }
    }
}

void app_runtime::apply_camera_commands(const std::vector<gui::camera_command>& commands) {
    // Enforce invariants: min_distance <= distance <= max_distance
    for (const auto& cmd : commands) {
        switch (cmd.type) {
        case gui::camera_parameter_type::DISTANCE:
            world.cam_follow.distance =
                std::clamp(cmd.value, world.cam_follow.min_distance, world.cam_follow.max_distance);
            break;
        case gui::camera_parameter_type::HEIGHT_OFFSET:
            world.cam_follow.height_offset = cmd.value;
            break;
        case gui::camera_parameter_type::MIN_DISTANCE:
            world.cam_follow.min_distance = cmd.value;
            // Clamp distance and max_distance to respect new minimum
            world.cam_follow.distance = std::max(world.cam_follow.distance, cmd.value);
            world.cam_follow.max_distance = std::max(world.cam_follow.max_distance, cmd.value);
            break;
        case gui::camera_parameter_type::MAX_DISTANCE:
            world.cam_follow.max_distance = cmd.value;
            // Clamp distance and min_distance to respect new maximum
            world.cam_follow.distance = std::min(world.cam_follow.distance, cmd.value);
            world.cam_follow.min_distance = std::min(world.cam_follow.min_distance, cmd.value);
            break;
        case gui::camera_parameter_type::MODE:
            world.cam_follow.mode = cmd.mode;
            break;
        }
    }
}

void app_runtime::apply_fov_commands(const std::vector<gui::fov_command>& commands) {
    for (const auto& cmd : commands) {
        switch (cmd.type) {
        case gui::fov_parameter_type::BASE_FOV:
            world.dynamic_fov.base_fov = cmd.value;
            break;
        case gui::fov_parameter_type::MAX_FOV_RANGE:
            world.dynamic_fov.max_fov_range = cmd.value;
            break;
        case gui::fov_parameter_type::G_MULTIPLIER:
            world.dynamic_fov.g_multiplier = cmd.value;
            break;
        }
    }
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
        app::generate_debug_primitives(world.debug_list, world);

        // Pass the populated list to the dumb renderer.
        debug::draw_primitives(debug_ctx, world.debug_list);
    }

    gui::render();

    sg_end_pass();
    sg_commit();
}
