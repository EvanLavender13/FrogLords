#include "gui/vehicle_panel.h"
#include "gui/gui.h"
#include "imgui.h"
#include <glm/common.hpp>

namespace gui {

namespace {

std::vector<parameter_command> draw_vehicle_tuning_section(const vehicle::tuning_params& params) {
    std::vector<parameter_command> commands;

    if (!ImGui::CollapsingHeader("Vehicle Tuning", ImGuiTreeNodeFlags_DefaultOpen))
        return commands;

    // Local copies for slider interaction (GUI needs mutable values)
    float turn_rate = params.turn_rate;
    float steering_reduction_factor = params.steering_reduction_factor;

    // Metadata-driven tunable parameters
    if (gui::widget::tunable_param(&turn_rate, vehicle::tuning_params::turn_rate_meta)) {
        commands.push_back({parameter_type::TURN_RATE, turn_rate});
    }
    if (gui::widget::tunable_param(&steering_reduction_factor,
                                    vehicle::tuning_params::steering_reduction_factor_meta)) {
        commands.push_back({parameter_type::STEERING_REDUCTION_FACTOR, steering_reduction_factor});
    }

    return commands;
}

void draw_vehicle_state_section(const controller& vehicle) {
    if (!ImGui::CollapsingHeader("Vehicle State"))
        return;

    // Read-only vehicle state display (metadata-driven)
    static constexpr param_meta heading_yaw_meta = {"Heading Yaw", "rad", -3.15f, 3.15f};
    static constexpr param_meta steering_multiplier_meta = {"Steering Multiplier", "", 0.0f, 1.0f};
    static constexpr param_meta effective_turn_rate_meta = {
        "Effective Turn Rate", "rad/s", 0.0f, 10.0f};

    gui::widget::readonly_param(vehicle.heading_yaw, heading_yaw_meta);

    // Steering multiplier: derived from speed ratio and reduction factor
    float horizontal_speed = glm::length(glm::vec3(vehicle.velocity.x, 0.0f, vehicle.velocity.z));
    float steering_multiplier = vehicle.compute_steering_multiplier(horizontal_speed);
    gui::widget::derived_param(steering_multiplier, steering_multiplier_meta, "1 - (v/v_max)·r");

    // Effective turn rate: base rate scaled by steering multiplier
    float effective_turn_rate = vehicle.turn_rate * steering_multiplier;
    gui::widget::derived_param(effective_turn_rate, effective_turn_rate_meta, "w_base · m");

    // Slip angle: angle between velocity direction and heading direction
    static constexpr param_meta slip_angle_meta = {"Slip Angle", "deg", -180.0f, 180.0f};
    float slip_angle_deg = glm::degrees(vehicle.calculate_slip_angle());
    gui::widget::derived_param(slip_angle_deg, slip_angle_meta, "atan2(v_lat, v_fwd)");
}

} // namespace

std::vector<parameter_command> draw_vehicle_panel(const vehicle_panel_state& state,
                                                   const controller& vehicle,
                                                   const vehicle::tuning_params& params) {
    std::vector<parameter_command> commands;

    if (!state.show)
        return commands;

    // Vehicle tuning section (returns commands)
    auto tuning_commands = draw_vehicle_tuning_section(params);
    commands.insert(commands.end(), tuning_commands.begin(), tuning_commands.end());

    // Vehicle state section (read-only)
    draw_vehicle_state_section(vehicle);

    return commands;
}

} // namespace gui
