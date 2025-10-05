#include "gui/character_panel.h"
#include "gui/gui.h"
#include "character/tuning.h"
#include "sokol_app.h"
#include "ImGui.h"

namespace gui {

void draw_character_panel(character_panel_state& state, character_controller& character,
                          locomotion_system& locomotion, character::tuning_params& params) {
    ImGui::SetNextWindowSize(ImVec2(350.0f, 0.0f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(static_cast<float>(sapp_width()) - 370.0f, 20.0f),
                            ImGuiCond_FirstUseEver);

    if (gui::panel::begin("Character Tuning", &state.show)) {
        bool changed = false;
        changed |= gui::widget::slider_float("Max Speed (m/s)", &character.max_speed, 1.0f, 15.0f);
        changed |= gui::widget::slider_float("Time to Max Speed (s)",
                                             &params.time_to_max_speed, 0.1f, 2.0f);
        changed |= gui::widget::slider_float("Jump Height (m)", &params.jump_height, 0.5f, 3.0f);
        changed |= gui::widget::slider_float("Gravity (m/s^2)", &character.gravity, -20.0f, -5.0f);

        if (changed) {
            params.apply_to(character);
            character::sync_locomotion_targets(character, locomotion);
        }

        if (ImGui::CollapsingHeader("Debug Info")) {
        }

        gui::widget::text("");
        gui::widget::text("FPS: %.1f", 1.0f / sapp_frame_duration());
    }
    gui::panel::end();
}

} // namespace gui
