#include "gui/character_panel.h"
#include "gui/gui.h"
#include "character/tuning.h"
#include "sokol_app.h"
#include "ImGui.h"

namespace gui {

void draw_character_panel(character_panel_state& state, controller& character,
                          locomotion_system& locomotion, orientation_system& orientation,
                          character::tuning_params& params) {
    ImGui::SetNextWindowSize(ImVec2(350.0f, 0.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(static_cast<float>(sapp_width()) - 370.0f, 20.0f),
                            ImGuiCond_FirstUseEver);

    if (gui::panel::begin("Character Tuning", &state.show)) {
        bool changed = false;
        changed |= gui::widget::slider_float("Max Speed (m/s)", &character.max_speed, 1.0f, 15.0f);
        changed |= gui::widget::slider_float("Time to Max Speed (s)", &params.time_to_max_speed,
                                             0.1f, 2.0f);
        changed |= gui::widget::slider_float("Jump Height (m)", &params.jump_height, 0.5f, 3.0f);
        changed |= gui::widget::slider_float("Gravity (m/s^2)", &character.gravity, -20.0f, -5.0f);

        if (changed) {
            params.apply_to(character);
            character::sync_locomotion_targets(character, locomotion);
        }

        // Landing spring parameters
        if (ImGui::CollapsingHeader("Landing Spring")) {
            gui::widget::slider_float("Stiffness", &character.animation.landing_spring.stiffness,
                                      100.0f, 1000.0f);
            gui::widget::slider_float("Damping", &character.animation.landing_spring.damping, 10.0f,
                                      100.0f);
            gui::widget::slider_float("Impulse Scale", &character.animation.landing_impulse_scale,
                                      0.1f, 1.5f);

            // Read-only spring state display
            gui::widget::text("Spring Position: %.3f", character.animation.get_vertical_offset());
            gui::widget::text("Spring Velocity: %.3f",
                              character.animation.landing_spring.get_velocity());
        }

        // Acceleration tilt parameters
        if (ImGui::CollapsingHeader("Acceleration Tilt")) {
            gui::widget::slider_float("Tilt Smoothing", &character.animation.tilt_smoothing, 1.0f,
                                      20.0f);
            gui::widget::slider_float("Tilt Magnitude (rad)", &character.animation.tilt_magnitude,
                                      0.0f, 1.0f);

            // Read-only tilt state display
            gui::widget::text("Current Tilt (pitch, roll): (%.2f, %.2f)",
                              character.animation.tilt_angles.x, character.animation.tilt_angles.z);
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
        }

        if (ImGui::CollapsingHeader("Skeleton")) {
            gui::widget::checkbox("Show Skeleton", &state.show_skeleton);
            gui::widget::checkbox("Show Joint Labels", &state.show_joint_labels);
            gui::widget::checkbox("Animate Skeleton", &state.animate_skeleton);
        }

        gui::widget::text("");
        gui::widget::text("FPS: %.1f", 1.0f / sapp_frame_duration());
    }
    gui::panel::end();
}

} // namespace gui
