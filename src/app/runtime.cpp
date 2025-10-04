#include "app/runtime.h"
#include "sokol_app.h"
#include "sokol_glue.h"
#include "sokol_log.h"
#include "input/input.h"
#include "gui/gui.h"
#include "ImGui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <algorithm>

void intuitive_character_params::apply_to(character_controller& c) const {
    if (time_to_max_speed <= 0.0f) {
        return;
    }

    float desired_accel = c.max_speed / time_to_max_speed;
    constexpr float friction_ratio = 0.75f;
    constexpr float net_fraction = 1.0f - friction_ratio;
    if (net_fraction <= 0.0f) {
        return;
    }

    c.ground_accel = desired_accel / net_fraction;
    c.air_accel = desired_accel;

    float gravity_mag = std::abs(c.gravity);
    if (gravity_mag > 0.0f) {
        c.friction = (c.ground_accel * 0.75f) / gravity_mag;
        c.jump_velocity = std::sqrt(2.0f * gravity_mag * jump_height);
    }
}

void intuitive_character_params::read_from(const character_controller& c) {
    constexpr float friction_ratio = 0.75f;
    constexpr float net_fraction = 1.0f - friction_ratio;
    if (c.ground_accel > 0.0f && net_fraction > 0.0f) {
        float net_accel = c.ground_accel * net_fraction;
        time_to_max_speed = c.max_speed / net_accel;
    }

    float gravity_mag = std::abs(c.gravity);
    if (gravity_mag > 0.0f) {
        jump_height = (c.jump_velocity * c.jump_velocity) / (2.0f * gravity_mag);
    }
}

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

    sync_locomotion_speed_targets();

    cam = camera(character.position, 5.0f, 15.0f, 0.0f);
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
    update_spring_history();

    input::update();

    begin_gui_frame();
    build_character_panel();

    render_world();
}

void app_runtime::handle_event(const sapp_event* e) {
    gui::handle_event(e);
    input::process_event(e);
}

void app_runtime::sync_locomotion_speed_targets() {
    float run_speed = character.max_speed;
    locomotion.run_speed_threshold = run_speed;
    locomotion.walk_speed_threshold = run_speed * 0.33f;
    locomotion.walk_state.stride_length = locomotion.walk_speed_threshold * 1.0f;
    locomotion.run_state.stride_length = run_speed * 0.8f;
}

void app_runtime::ensure_static_meshes() {
    if (static_meshes_initialized) {
        return;
    }

    unit_circle = generate_circle(glm::vec3(0.0f), 1.0f);
    unit_sphere_8 = generate_sphere(8, 8, 1.0f);
    unit_sphere_6 = generate_sphere(6, 6, 1.0f);
    unit_sphere_4 = generate_sphere(4, 4, 1.0f);

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

    if (input::is_mouse_button_down(SAPP_MOUSEBUTTON_RIGHT)) {
        orientation.update_forced(cam.get_yaw(), dt);
    } else {
        orientation.update(horizontal_velocity, dt);
    }

    sync_locomotion_speed_targets();
    locomotion.update(horizontal_velocity, dt, character.is_grounded, character.ground_height);

    if (cam.get_mode() == camera_mode::follow) {
        cam.follow_update(character.position, dt);
    }
}

void app_runtime::begin_gui_frame() {
    gui::begin_frame();
}

void app_runtime::build_character_panel() {
    ImGui::SetNextWindowSize(ImVec2(350.0f, 0.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(static_cast<float>(sapp_width()) - 370.0f, 20.0f), ImGuiCond_FirstUseEver);

    if (gui::panel::begin("Character Tuning", &show_character_panel)) {
        bool changed = false;
        changed |= gui::widget::slider_float("Max Speed (m/s)", &character.max_speed, 1.0f, 15.0f);
        changed |= gui::widget::slider_float("Time to Max Speed (s)", &character_params.time_to_max_speed, 0.1f, 2.0f);
        changed |= gui::widget::slider_float("Jump Height (m)", &character_params.jump_height, 0.5f, 3.0f);
        changed |= gui::widget::slider_float("Gravity (m/s^2)", &character.gravity, -20.0f, -5.0f);

        if (changed) {
            character_params.apply_to(character);
            sync_locomotion_speed_targets();
        }

        if (ImGui::CollapsingHeader("Locomotion")) {
            gui::widget::text("Run speed threshold: %.1f m/s", locomotion.run_speed_threshold);
            gui::widget::text("Walk threshold: %.1f m/s", locomotion.walk_speed_threshold);
            gui::widget::text("Walk stride: %.1f m", locomotion.walk_state.stride_length);
            gui::widget::text("Run stride: %.1f m", locomotion.run_state.stride_length);
        }

        if (ImGui::CollapsingHeader("Debug Info")) {
            gui::widget::text("Grounded: %s", character.is_grounded ? "YES" : "NO");
            gui::widget::text("Phase: %.2f", locomotion.phase);
            gui::widget::text("Distance: %.2f m", locomotion.distance_traveled);

            gui::widget::text("");
            gui::widget::text("Ground Height: %.2f m", character.ground_height);
            gui::widget::text("Ground Normal: (%.2f, %.2f, %.2f)", character.ground_normal.x, character.ground_normal.y, character.ground_normal.z);
            gui::widget::text("Landing Impact: %.2f m/s", character.landing_impact_velocity);

            gui::widget::text("");
            gui::widget::text("Spring Pos: %.3f m", locomotion.vertical_spring.get_position());

            simple_pose debug_pose = locomotion.get_current_pose();
            glm::vec3 spring_base = character.weightlifter.center;
            glm::vec3 spring_tip = character.position + debug_pose.root_offset;
            float base_to_bumper = glm::length(character.position - spring_base);
            float rest_length = base_to_bumper + locomotion.get_vertical_target_offset();
            float current_length = glm::length(spring_tip - spring_base);
            float compression_ratio = 0.0f;
            if (rest_length > 0.0001f) {
                compression_ratio = (rest_length - current_length) / rest_length;
            }
            gui::widget::text("Spring Rest: %.3f m", rest_length);
            gui::widget::text("Spring Length: %.3f m", current_length);
            gui::widget::text("Compression: %.1f%%", compression_ratio * 100.0f);

            gui::widget::text("Spring Vel: %.3f m/s", locomotion.vertical_spring.get_velocity());

            ImGui::PlotLines("Position", spring_position_history, spring_history_size, spring_history_index, nullptr, -0.2f, 3.5f, ImVec2(0, 60));
            ImGui::PlotLines("Velocity", spring_velocity_history, spring_history_size, spring_history_index, nullptr, -8.0f, 8.0f, ImVec2(0, 60));
        }

        gui::widget::text("");
        gui::widget::text("FPS: %.1f", 1.0f / sapp_frame_duration());
    }
    gui::panel::end();
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
        wireframe_mesh box_mesh = generate_box(box.half_extents.x * 2.0f, box.half_extents.y * 2.0f, box.half_extents.z * 2.0f);
        box_mesh.position = box.center;

        glm::vec4 box_color = box.center.y < 1.0f ? glm::vec4(1, 1, 0, 1) : glm::vec4(1, 0, 0, 1);
        renderer.draw(box_mesh, cam, aspect, box_color);
    }

    simple_pose current_pose = locomotion.get_current_pose();
    glm::vec3 pose_offset = current_pose.root_offset;

    wireframe_mesh bumper_vis = unit_sphere_8;
    bumper_vis.position = character.position + pose_offset;
    bumper_vis.scale = glm::vec3(character.bumper.radius);
    renderer.draw(bumper_vis, cam, aspect, glm::vec4(0, 1, 1, 1));

    wireframe_mesh weightlifter_vis = unit_sphere_6;
    weightlifter_vis.position = character.weightlifter.center;
    weightlifter_vis.scale = glm::vec3(character.weightlifter.radius);
    renderer.draw(weightlifter_vis, cam, aspect, glm::vec4(1, 1, 0, 1));

    glm::vec3 spring_base = character.weightlifter.center;
    glm::vec3 spring_tip = character.position + pose_offset;
    float base_to_bumper = glm::length(character.position - spring_base);
    float rest_length = base_to_bumper + locomotion.get_vertical_target_offset();
    float current_length = glm::length(spring_tip - spring_base);
    float compression_ratio = 0.0f;
    if (rest_length > 0.0001f) {
        compression_ratio = (rest_length - current_length) / rest_length;
    }
    int spring_coils = 6;
    float spring_radius = glm::clamp(rest_length * 0.12f, 0.05f, 0.18f);
    wireframe_mesh spring_debug = generate_spring(spring_base, spring_tip, spring_coils, spring_radius);

    glm::vec4 spring_color;
    if (compression_ratio >= 0.0f) {
        float heat = glm::clamp(compression_ratio * 3.0f, 0.0f, 1.0f);
        spring_color = glm::vec4(1.0f, 1.0f - heat * 0.6f, 0.0f, 1.0f);
    } else {
        float stretch = glm::clamp(-compression_ratio * 2.0f, 0.0f, 1.0f);
        spring_color = glm::vec4(0.0f, 0.6f + (1.0f - stretch) * 0.3f, 1.0f, 1.0f);
    }
    renderer.draw(spring_debug, cam, aspect, spring_color);

    glm::vec3 rest_tip = spring_base + glm::vec3(0.0f, rest_length, 0.0f);
    wireframe_mesh rest_line = generate_arrow(spring_base, rest_tip, 0.05f);
    renderer.draw(rest_line, cam, aspect, glm::vec4(0.4f, 0.4f, 0.4f, 0.5f));

    wireframe_mesh rest_marker = unit_sphere_4;
    rest_marker.position = rest_tip;
    rest_marker.scale = glm::vec3(0.06f);
    renderer.draw(rest_marker, cam, aspect, glm::vec4(0.4f, 0.4f, 0.4f, 0.6f));

    glm::vec3 vel = character.velocity;
    if (glm::length(vel) > 0.1f) {
        wireframe_mesh velocity_indicator = unit_sphere_4;
        velocity_indicator.position = character.position + pose_offset + vel;
        velocity_indicator.scale = glm::vec3(0.1f);
        renderer.draw(velocity_indicator, cam, aspect, glm::vec4(1, 0, 0, 1));
    }

    wireframe_mesh walk_circle = unit_circle;
    walk_circle.position = character.position;
    walk_circle.scale = glm::vec3(locomotion.walk_speed_threshold, 1.0f, locomotion.walk_speed_threshold);
    renderer.draw(walk_circle, cam, aspect, glm::vec4(0, 1, 0, 0.5f));

    wireframe_mesh run_circle = unit_circle;
    run_circle.position = character.position;
    run_circle.scale = glm::vec3(locomotion.run_speed_threshold, 1.0f, locomotion.run_speed_threshold);
    renderer.draw(run_circle, cam, aspect, glm::vec4(1, 1, 0, 0.5f));

    glm::vec3 horiz_vel = character.velocity;
    horiz_vel.y = 0.0f;
    float current_speed = glm::length(horiz_vel);
    if (current_speed > 0.05f) {
        wireframe_mesh speed_circle = unit_circle;
        speed_circle.position = character.position;
        speed_circle.scale = glm::vec3(current_speed, 1.0f, current_speed);
        renderer.draw(speed_circle, cam, aspect, glm::vec4(1, 0, 0, 0.8f));
    }

    float yaw = orientation.get_yaw();
    glm::vec3 forward_dir(std::sin(yaw), 0, std::cos(yaw));
    glm::vec3 right_dir(-std::cos(yaw), 0, std::sin(yaw));
    wireframe_mesh yaw_indicator = unit_sphere_4;
    yaw_indicator.position = character.position + pose_offset + forward_dir * 0.8f;
    yaw_indicator.scale = glm::vec3(0.1f);
    renderer.draw(yaw_indicator, cam, aspect, glm::vec4(0, 1, 0, 1));

    for (int i = 0; i < 10; i++) {
        float phase_offset = static_cast<float>(i) * 0.1f;
        float trail_phase = std::fmod(locomotion.phase + phase_offset, 1.0f);
        float brightness = 1.0f - static_cast<float>(i) * 0.08f;

        wireframe_mesh trail_sphere = unit_sphere_4;
        trail_sphere.position = character.position + pose_offset - forward_dir * static_cast<float>(i) * 0.25f;
        trail_sphere.position.y += std::sin(trail_phase * 2.0f * 3.14159f) * 0.25f;
        trail_sphere.scale = glm::vec3(0.12f);
        renderer.draw(trail_sphere, cam, aspect, glm::vec4(brightness, brightness, brightness, 1));
    }

    float ground_contact_y = character.weightlifter.center.y - character.weightlifter.radius;

    float left_foot_offset = std::sin(locomotion.phase * 2.0f * 3.14159f) * 0.4f;
    glm::vec3 left_foot_pos = character.position + forward_dir * left_foot_offset + right_dir * 0.2f;
    left_foot_pos.y = ground_contact_y;
    wireframe_mesh left_foot = unit_sphere_4;
    left_foot.position = left_foot_pos;
    left_foot.scale = glm::vec3(0.08f);
    renderer.draw(left_foot, cam, aspect, glm::vec4(1, 0, 1, 1));

    float right_phase = std::fmod(locomotion.phase + 0.5f, 1.0f);
    float right_foot_offset = std::sin(right_phase * 2.0f * 3.14159f) * 0.4f;
    glm::vec3 right_foot_pos = character.position + forward_dir * right_foot_offset + right_dir * -0.2f;
    right_foot_pos.y = ground_contact_y;
    wireframe_mesh right_foot = unit_sphere_4;
    right_foot.position = right_foot_pos;
    right_foot.scale = glm::vec3(0.08f);
    renderer.draw(right_foot, cam, aspect, glm::vec4(1, 0.5f, 0, 1));

    gui::render();

    sg_end_pass();
    sg_commit();
}

void app_runtime::update_spring_history() {
    spring_position_history[spring_history_index] = locomotion.vertical_spring.get_position();
    spring_velocity_history[spring_history_index] = locomotion.vertical_spring.get_velocity();
    spring_history_index = (spring_history_index + 1) % spring_history_size;
}


