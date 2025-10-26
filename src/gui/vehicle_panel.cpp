#include "gui/vehicle_panel.h"
#include "gui/gui.h"
#include "vehicle/vehicle_reactive_systems.h"
#include "imgui.h"
#include <glm/common.hpp>

namespace gui {

namespace {

std::vector<parameter_command> draw_vehicle_tuning_section(const controller& vehicle,
                                                           const vehicle::tuning_params& params) {
    std::vector<parameter_command> commands;

    if (!ImGui::CollapsingHeader("Vehicle Tuning", ImGuiTreeNodeFlags_DefaultOpen))
        return commands;

    // Local copies for slider interaction (GUI needs mutable values)
    float max_speed = params.max_speed;
    float accel = params.accel;
    float mass = params.mass;
    float turn_rate = params.turn_rate;
    float steering_reduction_factor = params.steering_reduction_factor;
    float brake_rate = params.brake_rate;

    // Metadata-driven tunable parameters
    if (gui::widget::tunable_param(&max_speed, vehicle::tuning_params::max_speed_meta)) {
        commands.push_back({parameter_type::MAX_SPEED, max_speed});
    }
    if (gui::widget::tunable_param(&accel, vehicle::tuning_params::accel_meta)) {
        commands.push_back({parameter_type::ACCEL, accel});
    }
    if (gui::widget::tunable_param(&mass, vehicle::tuning_params::mass_meta)) {
        commands.push_back({parameter_type::MASS, mass});
    }
    if (gui::widget::tunable_param(&turn_rate, vehicle::tuning_params::turn_rate_meta)) {
        commands.push_back({parameter_type::TURN_RATE, turn_rate});
    }
    if (gui::widget::tunable_param(&steering_reduction_factor,
                                   vehicle::tuning_params::steering_reduction_factor_meta)) {
        commands.push_back({parameter_type::STEERING_REDUCTION_FACTOR, steering_reduction_factor});
    }
    if (gui::widget::tunable_param(&brake_rate, vehicle::tuning_params::brake_rate_meta)) {
        commands.push_back({parameter_type::BRAKE_RATE, brake_rate});
    }

    ImGui::Separator();
    ImGui::Text("Visual Tilt");

    // Tilt parameters
    float lean_multiplier = params.lean_multiplier;
    float pitch_multiplier = params.pitch_multiplier;
    float tilt_stiffness = params.tilt_stiffness;

    if (gui::widget::tunable_param(&lean_multiplier,
                                   vehicle::tuning_params::lean_multiplier_meta)) {
        commands.push_back({parameter_type::LEAN_MULTIPLIER, lean_multiplier});
    }
    if (gui::widget::tunable_param(&pitch_multiplier,
                                   vehicle::tuning_params::pitch_multiplier_meta)) {
        commands.push_back({parameter_type::PITCH_MULTIPLIER, pitch_multiplier});
    }
    if (gui::widget::tunable_param(&tilt_stiffness, vehicle::tuning_params::tilt_stiffness_meta)) {
        commands.push_back({parameter_type::TILT_STIFFNESS, tilt_stiffness});
    }

    ImGui::Separator();
    ImGui::Text("Orientation");

    // Orientation parameters
    float orientation_stiffness = params.orientation_stiffness;

    if (gui::widget::tunable_param(&orientation_stiffness,
                                   vehicle::tuning_params::orientation_stiffness_meta)) {
        commands.push_back({parameter_type::ORIENTATION_STIFFNESS, orientation_stiffness});
    }

    // Real-time feedback: horizontal speed plot
    float horizontal_speed = glm::length(glm::vec3(vehicle.velocity.x, 0.0f, vehicle.velocity.z));
    gui::plot_histogram("Horizontal Speed (m/s)", horizontal_speed, 5.0f, 0.0f,
                        params.max_speed * 1.2f);

    return commands;
}

void draw_vehicle_state_section(const controller& vehicle, const vehicle_reactive_systems& visuals) {
    if (!ImGui::CollapsingHeader("Vehicle State"))
        return;

    // Read-only vehicle state display (metadata-driven)
    static constexpr param_meta heading_yaw_meta = {"Heading Yaw", "rad", -3.15f, 3.15f};
    static constexpr param_meta steering_multiplier_meta = {"Steering Multiplier", "", 0.0f, 1.0f};
    static constexpr param_meta effective_turn_rate_meta = {"Effective Turn Rate", "rad/s", 0.0f,
                                                            10.0f};

    gui::widget::readonly_param(vehicle.heading_yaw, heading_yaw_meta);

    // Steering multiplier: derived from speed ratio and reduction factor
    float horizontal_speed = glm::length(glm::vec3(vehicle.velocity.x, 0.0f, vehicle.velocity.z));
    float steering_multiplier = vehicle.compute_steering_multiplier(horizontal_speed);
    gui::widget::derived_param(steering_multiplier, steering_multiplier_meta, "1 - (v/v_max)·r");

    // Effective turn rate: base rate scaled by steering multiplier
    float effective_turn_rate = vehicle.turn_rate * steering_multiplier;
    gui::widget::derived_param(effective_turn_rate, effective_turn_rate_meta, "ω_base · m");

    // Slip angle: angle between velocity direction and heading direction
    static constexpr param_meta slip_angle_meta = {"Slip Angle", "deg", -180.0f, 180.0f};
    float slip_angle_deg = glm::degrees(vehicle.calculate_slip_angle());
    gui::widget::derived_param(slip_angle_deg, slip_angle_meta, "atan2(v_lat, v_fwd)");

    // Friction model derived parameters
    ImGui::Separator();
    ImGui::Text("Handbrake: %s", vehicle.friction.handbrake.is_active() ? "ACTIVE" : "INACTIVE");

    static constexpr param_meta base_drag_meta = {"Base Drag Rate", "/s", 0.0f, 10.0f};
    float base_drag = vehicle.friction.get_base_drag_rate(vehicle.accel, vehicle.max_speed);
    gui::widget::derived_param(base_drag, base_drag_meta, "accel / max_speed");

    ImGui::Separator();
    ImGui::Text("Visual State");

    // Visual tilt derived values
    static constexpr param_meta lean_angle_meta = {"Lean Angle", "deg", -45.0f, 45.0f};
    static constexpr param_meta pitch_angle_meta = {"Pitch Angle", "deg", -45.0f, 45.0f};
    static constexpr param_meta orientation_yaw_meta = {"Orientation Yaw", "rad", -3.15f, 3.15f};

    float lean_angle_deg = glm::degrees(visuals.get_lean_angle());
    float pitch_angle_deg = glm::degrees(visuals.get_pitch_angle());

    gui::widget::derived_param(lean_angle_deg, lean_angle_meta, "g_lateral · k_lean");
    gui::widget::derived_param(pitch_angle_deg, pitch_angle_meta, "a_forward · k_pitch");
    gui::widget::readonly_param(visuals.get_orientation_yaw(), orientation_yaw_meta);
}

} // namespace

std::vector<parameter_command> draw_vehicle_panel(const vehicle_panel_state& state,
                                                  const controller& vehicle,
                                                  const vehicle::tuning_params& params,
                                                  const vehicle_reactive_systems& visuals) {
    std::vector<parameter_command> commands;

    if (!state.show)
        return commands;

    // Vehicle tuning section (returns commands)
    auto tuning_commands = draw_vehicle_tuning_section(vehicle, params);
    commands.insert(commands.end(), tuning_commands.begin(), tuning_commands.end());

    // Vehicle state section (read-only)
    draw_vehicle_state_section(vehicle, visuals);

    return commands;
}

} // namespace gui
