#include "gui/character_panel.h"
#include "gui/gui.h"
#include "sokol_app.h"
#include "imgui.h"
#include <glm/common.hpp>

namespace gui {

void draw_character_panel(character_panel_state& state, controller& character,
                          locomotion_system& locomotion, orientation_system& orientation,
                          character::tuning_params& params) {
    if (!state.show)
        return;

    // Draw as collapsing sections (no window wrapper)
    // Main tuning parameters
    if (ImGui::CollapsingHeader("Character Tuning", ImGuiTreeNodeFlags_DefaultOpen)) {
        bool changed = false;
        changed |= gui::widget::slider_float("Run Speed (m/s)", &character.run_speed, 1.0f, 15.0f);
        changed |= gui::widget::slider_float("Time to Max Speed (s)", &params.time_to_max_speed,
                                             0.1f, 2.0f);
        changed |= gui::widget::slider_float("Jump Height (m)", &params.jump_height, 0.5f, 3.0f);
        changed |= gui::widget::slider_float("Gravity (m/s^2)", &character.gravity, -20.0f, -5.0f);
        gui::widget::slider_float("Walk Transition Rate", &character.walk_transition_rate, 1.0f,
                                  30.0f);
        gui::widget::slider_float("Coyote Window (s)", &character.coyote_window, 0.0f, 0.5f);
        gui::widget::slider_float("Jump Buffer Window (s)", &character.jump_buffer_window, 0.0f,
                                  0.5f);

        // Debug displays for walk/run transition
        gui::widget::text("Target Max Speed: %.2f m/s", character.target_max_speed);
        gui::widget::text("Current Max Speed: %.2f m/s", character.max_speed);

        // Debug displays for jump timing forgiveness
        gui::widget::text("Coyote Timer: %.3f s", character.coyote_timer);
        gui::widget::text("Jump Buffer Timer: %.3f s", character.jump_buffer_timer);
        gui::widget::text(
            "Can Jump (Coyote): %s",
            (character.is_grounded || character.coyote_timer < character.coyote_window) ? "YES"
                                                                                        : "NO");

        if (changed) {
            params.apply_to(character);
            character::sync_locomotion_targets(character, locomotion);
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

    // Acceleration tilt parameters
    if (ImGui::CollapsingHeader("Acceleration Tilt")) {
        gui::widget::slider_float("Tilt Smoothing", &character.animation.tilt_smoothing, 1.0f,
                                  20.0f);
        gui::widget::slider_float("Tilt Magnitude (rad)", &character.animation.tilt_magnitude, 0.0f,
                                  1.0f);

        // Read-only tilt state display
        gui::widget::text("Current Tilt (pitch, roll): (%.2f, %.2f)",
                          character.animation.tilt_angles.x, character.animation.tilt_angles.z);
    }

    // Air locomotion weights (phase continuity)
    if (ImGui::CollapsingHeader("Air Locomotion Weights")) {
        gui::widget::slider_float("Contact Weight Frequency (Hz)",
                                  &character.animation.contact_weight_frequency, 3.0f, 12.0f);

        // Read-only weight state display
        gui::widget::text("Contact Weight Target: %.3f", character.animation.contact_weight_target);
        gui::widget::text("Contact Weight (smoothed): %.3f",
                          character.animation.contact_weight_spring.get_position());
        gui::widget::text("Air Weight: %.3f",
                          1.0f - character.animation.contact_weight_spring.get_position());
        gui::widget::text("Spring Velocity: %.3f",
                          character.animation.contact_weight_spring.get_velocity());
    }

    // Orientation parameters
    if (ImGui::CollapsingHeader("Orientation")) {
        gui::widget::slider_float("Yaw Smoothing", &orientation.yaw_smoothing, 1.0f, 20.0f);

        // Read-only orientation state display
        gui::widget::text("Current Yaw (rad): %.2f", orientation.current_yaw);
        gui::widget::text("Target Yaw (rad): %.2f", orientation.target_yaw);
    }

    // Locomotion parameters
    if (ImGui::CollapsingHeader("Locomotion")) {
        gui::widget::slider_float("Speed Smoothing", &locomotion.speed_smoothing, 1.0f, 20.0f);

        // Read-only locomotion state display
        gui::widget::text("Current Speed: %.2f m/s", locomotion.current_speed);
        gui::widget::text("Smoothed Speed: %.2f m/s", locomotion.smoothed_speed);
        gui::widget::text("Animation Phase: %.2f", locomotion.phase);
        gui::widget::text("Stride: %.2fm (auto from locomotion)", locomotion.get_blended_stride());
    }

    if (ImGui::CollapsingHeader("Locomotion Blending", ImGuiTreeNodeFlags_DefaultOpen)) {
        const char* mode_labels[] = {"Walk Only", "Mixed (Speed)", "Run Only"};
        int current_mode = static_cast<int>(state.gait_blend_mode);
        if (ImGui::Combo("Blend Mode", &current_mode, mode_labels, 3)) {
            state.gait_blend_mode = static_cast<character_panel_state::blend_mode>(current_mode);
        }
        float base_walk_factor = 1.0f - locomotion.get_run_blend();
        float applied_walk_factor = compute_walk_factor_override(state, base_walk_factor);
        gui::widget::text("Walk Factor (locomotion): %.3f", base_walk_factor);
        gui::widget::text("Walk Factor (applied): %.3f", applied_walk_factor);
        gui::widget::text("Contact Weight: %.3f",
                          character.animation.contact_weight_spring.get_position());
    }
}

float compute_walk_factor_override(const character_panel_state& state, float base_walk_factor) {
    switch (state.gait_blend_mode) {
    case character_panel_state::blend_mode::WALK_ONLY:
        return 1.0f;
    case character_panel_state::blend_mode::RUN_ONLY:
        return 0.0f;
    case character_panel_state::blend_mode::MIXED:
    default:
        return base_walk_factor;
    }
}

} // namespace gui
