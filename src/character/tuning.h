#pragma once

#include "character/character_controller.h"
#include "character/locomotion.h"

namespace character {

struct tuning_params {
    float time_to_max_speed = 0.4f;
    float jump_height = 1.3f;

    void apply_to(character_controller& c) const;
    void read_from(const character_controller& c);
};

void sync_locomotion_targets(character_controller& c, locomotion_system& loco);

} // namespace character
