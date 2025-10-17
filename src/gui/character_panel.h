#pragma once

#include "character/controller.h"
#include "character/orientation.h"
#include "character/tuning.h"
#include <glm/glm.hpp>

namespace gui {

struct character_panel_state {
    bool show = true;
    bool show_velocity_trail = true;
};

void draw_character_panel(character_panel_state& state, controller& character,
                          orientation_system& orientation,
                          character::tuning_params& params);

} // namespace gui
