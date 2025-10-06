#pragma once

#include "character/controller.h"
#include "character/locomotion.h"

namespace character {

struct tuning_params {
    float time_to_max_speed = 0.4f;
    float jump_height = 1.3f;

    void apply_to(controller& c) const;
    void read_from(const controller& c);
};

void sync_locomotion_targets(const controller& c, locomotion_system& loco);

} // namespace character
