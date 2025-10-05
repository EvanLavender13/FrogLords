#pragma once

#include "character/controller.h"
#include "character/locomotion.h"
#include "character/tuning.h"

namespace gui {

struct character_panel_state {
    bool show = true;
};

void draw_character_panel(character_panel_state& state, controller& character,
                          locomotion_system& locomotion, character::tuning_params& params);

} // namespace gui
