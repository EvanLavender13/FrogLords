#pragma once

#include "character/controller.h"
#include "character/character_reactive_systems.h"
#include "character/tuning.h"
#include "gui/parameter_command.h"
#include <glm/glm.hpp>
#include <vector>

namespace gui {

enum class camera_mode { FREE_ORBIT, LOCK_TO_ORIENTATION };

struct character_panel_state {
    bool show = true;
    bool show_velocity_trail = true;
    camera_mode cam_mode = camera_mode::FREE_ORBIT;
};

std::vector<parameter_command> draw_character_panel(character_panel_state& state,
                                                    const controller& character,
                                                    const character_reactive_systems& visuals,
                                                    const character::tuning_params& params);

} // namespace gui
