#include "gui/character_panel.h"
#include "gui/gui.h"
#include "character/tuning.h"
#include "sokol_app.h"
#include "ImGui.h"

namespace gui {

void draw_character_panel(character_panel_state& state, controller& character,
                          locomotion_system& locomotion, character::tuning_params& params) {
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

        if (ImGui::CollapsingHeader("Debug Info")) {
            gui::widget::text("Grounded: %s", character.is_grounded ? "yes" : "no");
            gui::widget::text("Position Y: %.3f", character.position.y);
            gui::widget::text("Velocity Y: %.3f", character.velocity.y);
            gui::widget::text("Bumper center Y: %.3f", character.bumper.center.y);
            gui::widget::text("Weightlifter center Y: %.3f", character.weightlifter.center.y);

            if (character.is_grounded) {
                gui::widget::text("Ground height: %.3f", character.ground_height);
                gui::widget::text("Ground normal: (%.2f, %.2f, %.2f)", character.ground_normal.x,
                                  character.ground_normal.y, character.ground_normal.z);

                float bumper_surface_offset =
                    character.bumper.center.y - (character.ground_height + character.bumper.radius);
                float weightlifter_burial = character.ground_height +
                                            character.weightlifter.radius -
                                            character.weightlifter.center.y;

                gui::widget::text("Bumper surface offset: %.3f", bumper_surface_offset);
                gui::widget::text("Weightlifter burial: %.3f", weightlifter_burial);
            }

            gui::widget::text("");

            const controller::contact_debug_info& lifter_dbg = character.weightlifter_contact_debug;
            gui::widget::text("Weightlifter contact: %s",
                              lifter_dbg.active ? (lifter_dbg.from_box ? "box" : "ground plane")
                                                : "none");
            if (lifter_dbg.active) {
                gui::widget::text("  Normal: (%.2f, %.2f, %.2f)", lifter_dbg.normal.x,
                                  lifter_dbg.normal.y, lifter_dbg.normal.z);
                gui::widget::text("  Penetration (normal): %.4f", lifter_dbg.penetration);
                gui::widget::text("  Vertical penetration: %.4f", lifter_dbg.vertical_penetration);
            }

            const controller::contact_debug_info& bumper_dbg = character.bumper_contact_debug;
            gui::widget::text("Bumper contact: %s",
                              bumper_dbg.active ? (bumper_dbg.from_box ? "box" : "ground plane")
                                                : "none");
            if (bumper_dbg.active) {
                gui::widget::text("  Normal: (%.2f, %.2f, %.2f)", bumper_dbg.normal.x,
                                  bumper_dbg.normal.y, bumper_dbg.normal.z);
                gui::widget::text("  Penetration (normal): %.4f", bumper_dbg.penetration);
                gui::widget::text("  Vertical penetration: %.4f", bumper_dbg.vertical_penetration);
            }
        }

        gui::widget::text("");
        gui::widget::text("FPS: %.1f", 1.0f / sapp_frame_duration());
    }
    gui::panel::end();
}

} // namespace gui
