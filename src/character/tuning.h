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

/// Synchronizes tunable controller parameters with the locomotion system's internal state.
/// This ensures that changes made in the tuning UI (e.g., to run_speed) are propagated
/// to the locomotion system's thresholds and stride lengths.
void sync_locomotion_targets(const controller& c, locomotion_system& loco);

} // namespace character
