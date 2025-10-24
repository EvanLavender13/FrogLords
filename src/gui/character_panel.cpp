#include "gui/character_panel.h"
#include "gui/gui.h"
#include "sokol_app.h"
#include "imgui.h"
#include <glm/common.hpp>

namespace gui {

namespace {

std::vector<parameter_command> draw_character_tuning_section(const controller& character,
                                                              const character::tuning_params& params) {
    std::vector<parameter_command> commands;

    if (!ImGui::CollapsingHeader("Character Tuning", ImGuiTreeNodeFlags_DefaultOpen))
        return commands;

    // Local copies for slider interaction (GUI needs mutable values)
    float max_speed = params.max_speed;
    float accel = params.accel;
    float weight = params.weight;

    // Metadata-driven tunable parameters
    if (gui::widget::tunable_param(&max_speed, character::tuning_params::max_speed_meta)) {
        commands.push_back({parameter_type::MAX_SPEED, max_speed});
    }
    if (gui::widget::tunable_param(&accel, character::tuning_params::accel_meta)) {
        commands.push_back({parameter_type::ACCEL, accel});
    }
    if (gui::widget::tunable_param(&weight, character::tuning_params::weight_meta)) {
        commands.push_back({parameter_type::WEIGHT, weight});
    }

    // Real-time feedback: horizontal speed plot
    float horizontal_speed = glm::length(glm::vec3(character.velocity.x, 0.0f, character.velocity.z));
    gui::plot_histogram("Horizontal Speed (m/s)", horizontal_speed, 5.0f, 0.0f, params.max_speed * 1.2f);

    return commands;
}

std::vector<parameter_command> draw_landing_spring_section(const character_reactive_systems& visuals) {
    std::vector<parameter_command> commands;

    if (!ImGui::CollapsingHeader("Landing Spring"))
        return commands;

    // Local copies for slider interaction
    float landing_stiffness = visuals.animation.landing_spring.stiffness;
    float landing_damping = visuals.animation.landing_spring.damping;
    float landing_impulse_scale = visuals.animation.landing_impulse_scale;

    if (gui::widget::slider_float("Stiffness", &landing_stiffness, 100.0f, 1000.0f)) {
        commands.push_back({parameter_type::LANDING_STIFFNESS, landing_stiffness});
    }
    if (gui::widget::slider_float("Damping", &landing_damping, 10.0f, 100.0f)) {
        commands.push_back({parameter_type::LANDING_DAMPING, landing_damping});
    }
    if (gui::widget::slider_float("Impulse Scale", &landing_impulse_scale, 0.1f, 1.5f)) {
        commands.push_back({parameter_type::LANDING_IMPULSE_SCALE, landing_impulse_scale});
    }

    // Read-only spring state display
    gui::widget::text("Spring Position: %.3f", visuals.animation.get_vertical_offset());
    gui::widget::text("Spring Velocity: %.3f", visuals.animation.landing_spring.get_velocity());

    return commands;
}

void draw_orientation_section(const character_reactive_systems& visuals) {
    if (!ImGui::CollapsingHeader("Orientation"))
        return;

    // Read-only for now (no parameter commands needed for orientation spring)
    gui::widget::text("Spring Stiffness: %.2f", visuals.orientation.yaw_spring.stiffness);
    gui::widget::text("Spring Damping: %.2f", visuals.orientation.yaw_spring.damping);

    // Read-only orientation state display
    gui::widget::text("Current Yaw (rad): %.2f", visuals.orientation.get_yaw());
    gui::widget::text("Angular Velocity (rad/s): %.2f",
                      visuals.orientation.yaw_spring.get_velocity());
}

void draw_locomotion_state_section(const controller& character) {
    if (!ImGui::CollapsingHeader("Locomotion State"))
        return;

    // Traction level name
    const char* traction_name;
    switch (character.traction.level) {
    case controller::traction_level::SOFT:
        traction_name = "SOFT";
        break;
    case controller::traction_level::MEDIUM:
        traction_name = "MEDIUM";
        break;
    case controller::traction_level::HARD:
        traction_name = "HARD";
        break;
    default:
        traction_name = "UNKNOWN";
        break;
    }
    gui::widget::text("Traction: %s", traction_name);

    // Horizontal speed
    float speed = glm::length(glm::vec3(character.velocity.x, 0.0f, character.velocity.z));
    gui::widget::text("Speed: %.2f m/s", speed);

    // Speed-dependent steering multiplier
    float steering_multiplier = character.compute_steering_multiplier(speed);
    float effective_turn_rate = character.turn_rate * steering_multiplier;
    gui::widget::text("Steering Multiplier: %.2f", steering_multiplier);
    gui::widget::text("Effective Turn Rate: %.2f rad/s", effective_turn_rate);
}

} // anonymous namespace

std::vector<parameter_command> draw_character_panel(const character_panel_state& state,
                                                    const controller& character,
                                                    const character_reactive_systems& visuals,
                                                    const character::tuning_params& params) {
    std::vector<parameter_command> commands;

    if (!state.show)
        return commands;

    // Delegate to section helpers
    auto tuning_cmds = draw_character_tuning_section(character, params);
    commands.insert(commands.end(), tuning_cmds.begin(), tuning_cmds.end());

    auto landing_cmds = draw_landing_spring_section(visuals);
    commands.insert(commands.end(), landing_cmds.begin(), landing_cmds.end());

    draw_orientation_section(visuals);
    draw_locomotion_state_section(character);

    return commands;
}

} // namespace gui
