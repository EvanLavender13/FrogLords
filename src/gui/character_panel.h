#pragma once

#include "character/controller.h"
#include "character/locomotion.h"
#include "character/orientation.h"
#include "character/tuning.h"
#include <glm/glm.hpp>

namespace gui {

struct character_panel_state {
    bool show = true;
    bool show_velocity_trail = true;

    enum class blend_mode { WALK_ONLY, MIXED, RUN_ONLY };
    blend_mode gait_blend_mode = blend_mode::MIXED;
};

void draw_character_panel(character_panel_state& state, controller& character,
                          locomotion_system& locomotion, orientation_system& orientation,
                          character::tuning_params& params);

float compute_walk_factor_override(const character_panel_state& state, float base_walk_factor);

} // namespace gui
