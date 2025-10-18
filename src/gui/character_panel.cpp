#include "gui/character_panel.h"
#include "gui/gui.h"
#include "sokol_app.h"
#include "imgui.h"
#include <glm/common.hpp>

namespace gui {

std::vector<parameter_command> draw_character_panel(character_panel_state& state,
                                                     const controller& character,
                                                     const character_reactive_systems& visuals,
                                                     const character::tuning_params& params) {
    std::vector<parameter_command> commands;

    if (!state.show)
        return commands;

    // Draw as collapsing sections (no window wrapper)
    // Main tuning parameters
    if (ImGui::CollapsingHeader("Character Tuning", ImGuiTreeNodeFlags_DefaultOpen)) {
        // Local copies for slider interaction (GUI needs mutable values)
        float max_speed = params.max_speed;
        float time_to_max_speed = params.time_to_max_speed;
        float jump_height = params.jump_height;
        float gravity = params.gravity;
        float coyote_window = character.coyote_window;
        float jump_buffer_window = character.jump_buffer_window;

        if (gui::widget::slider_float("Max Speed (m/s)", &max_speed, 1.0f, 15.0f)) {
            commands.push_back({parameter_type::max_speed, max_speed});
        }
        if (gui::widget::slider_float("Time to Max Speed (s)", &time_to_max_speed, 0.1f, 2.0f)) {
            commands.push_back({parameter_type::time_to_max_speed, time_to_max_speed});
        }
        if (gui::widget::slider_float("Jump Height (m)", &jump_height, 0.5f, 3.0f)) {
            commands.push_back({parameter_type::jump_height, jump_height});
        }
        if (gui::widget::slider_float("Gravity (m/s^2)", &gravity, -20.0f, -5.0f)) {
            commands.push_back({parameter_type::gravity, gravity});
        }
        if (gui::widget::slider_float("Coyote Window (s)", &coyote_window, 0.0f, 0.5f)) {
            commands.push_back({parameter_type::coyote_window, coyote_window});
        }
        if (gui::widget::slider_float("Jump Buffer Window (s)", &jump_buffer_window, 0.0f, 0.5f)) {
            commands.push_back({parameter_type::jump_buffer_window, jump_buffer_window});
        }

        // Debug displays for jump timing forgiveness
        gui::widget::text("Coyote Timer: %.3f s", character.coyote_timer);
        gui::widget::text("Jump Buffer Timer: %.3f s", character.jump_buffer_timer);
        gui::widget::text(
            "Can Jump (Coyote): %s",
            (character.is_grounded || character.coyote_timer < character.coyote_window) ? "YES"
                                                                                        : "NO");
    }

    // Landing spring parameters
    if (ImGui::CollapsingHeader("Landing Spring")) {
        // Local copies for slider interaction
        float landing_stiffness = visuals.animation.landing_spring.stiffness;
        float landing_damping = visuals.animation.landing_spring.damping;
        float landing_impulse_scale = visuals.animation.landing_impulse_scale;

        if (gui::widget::slider_float("Stiffness", &landing_stiffness, 100.0f, 1000.0f)) {
            commands.push_back({parameter_type::landing_stiffness, landing_stiffness});
        }
        if (gui::widget::slider_float("Damping", &landing_damping, 10.0f, 100.0f)) {
            commands.push_back({parameter_type::landing_damping, landing_damping});
        }
        if (gui::widget::slider_float("Impulse Scale", &landing_impulse_scale, 0.1f, 1.5f)) {
            commands.push_back({parameter_type::landing_impulse_scale, landing_impulse_scale});
        }

        // Read-only spring state display
        gui::widget::text("Spring Position: %.3f", visuals.animation.get_vertical_offset());
        gui::widget::text("Spring Velocity: %.3f", visuals.animation.landing_spring.get_velocity());
    }

    // Orientation parameters
    if (ImGui::CollapsingHeader("Orientation")) {
        // Read-only for now (no parameter commands needed for orientation spring)
        // These were being modified directly, but not part of the initial violation scope
        gui::widget::text("Spring Stiffness: %.2f", visuals.orientation.yaw_spring.stiffness);
        gui::widget::text("Spring Damping: %.2f", visuals.orientation.yaw_spring.damping);

        // Read-only orientation state display
        gui::widget::text("Current Yaw (rad): %.2f", visuals.orientation.get_yaw());
        gui::widget::text("Angular Velocity (rad/s): %.2f",
                          visuals.orientation.yaw_spring.get_velocity());
    }

    return commands;
}

} // namespace gui
