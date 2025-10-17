#include "gui/character_panel.h"
#include "gui/gui.h"
#include "sokol_app.h"
#include "imgui.h"
#include <glm/common.hpp>

namespace gui {

void draw_character_panel(character_panel_state& state, controller& character,
                          orientation_system& orientation,
                          character::tuning_params& params) {
    if (!state.show)
        return;

    // Draw as collapsing sections (no window wrapper)
    // Main tuning parameters
    if (ImGui::CollapsingHeader("Character Tuning", ImGuiTreeNodeFlags_DefaultOpen)) {
        bool changed = false;
        changed |= gui::widget::slider_float("Max Speed (m/s)", &character.max_speed, 1.0f, 15.0f);
        changed |= gui::widget::slider_float("Time to Max Speed (s)", &params.time_to_max_speed,
                                             0.1f, 2.0f);
        changed |= gui::widget::slider_float("Jump Height (m)", &params.jump_height, 0.5f, 3.0f);
        changed |= gui::widget::slider_float("Gravity (m/s^2)", &character.gravity, -20.0f, -5.0f);
        gui::widget::slider_float("Coyote Window (s)", &character.coyote_window, 0.0f, 0.5f);
        gui::widget::slider_float("Jump Buffer Window (s)", &character.jump_buffer_window, 0.0f,
                                  0.5f);

        // Debug displays for jump timing forgiveness
        gui::widget::text("Coyote Timer: %.3f s", character.coyote_timer);
        gui::widget::text("Jump Buffer Timer: %.3f s", character.jump_buffer_timer);
        gui::widget::text(
            "Can Jump (Coyote): %s",
            (character.is_grounded || character.coyote_timer < character.coyote_window) ? "YES"
                                                                                        : "NO");

        if (changed) {
            params.apply_to(character);
        }
    }

    // Landing spring parameters
    if (ImGui::CollapsingHeader("Landing Spring")) {
        gui::widget::slider_float("Stiffness", &character.animation.landing_spring.stiffness,
                                  100.0f, 1000.0f);
        gui::widget::slider_float("Damping", &character.animation.landing_spring.damping, 10.0f,
                                  100.0f);
        gui::widget::slider_float("Impulse Scale", &character.animation.landing_impulse_scale, 0.1f,
                                  1.5f);

        // Read-only spring state display
        gui::widget::text("Spring Position: %.3f", character.animation.get_vertical_offset());
        gui::widget::text("Spring Velocity: %.3f",
                          character.animation.landing_spring.get_velocity());
    }

    // Orientation parameters
    if (ImGui::CollapsingHeader("Orientation")) {
        gui::widget::slider_float("Yaw Smoothing", &orientation.yaw_smoothing, 1.0f, 20.0f);

        // Read-only orientation state display
        gui::widget::text("Current Yaw (rad): %.2f", orientation.current_yaw);
        gui::widget::text("Target Yaw (rad): %.2f", orientation.target_yaw);
    }
}

} // namespace gui
