#include "character/tuning.h"
#include <algorithm>
#include <cmath>

namespace character {

void tuning_params::apply_to(controller& c) const {
    // Copy high-level parameters (source of truth)
    c.max_speed = max_speed;
    c.gravity = gravity;

    // Copy acceleration directly (no derivation)
    c.ground_accel = accel;
    c.air_accel = accel; // Will be removed in Step 7

    // Calculate jump velocity
    float gravity_mag = std::abs(gravity);
    if (gravity_mag > 0.0f) {
        c.jump_velocity = std::sqrt(2.0f * gravity_mag * jump_height);
    }
}

} // namespace character