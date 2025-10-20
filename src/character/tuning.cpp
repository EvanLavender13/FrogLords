#include "character/tuning.h"
#include <algorithm>
#include <cmath>

namespace character {

void tuning_params::apply_to(controller& c) const {
    // Copy parameters directly (no derivation)
    c.max_speed = max_speed;
    c.gravity = gravity;
    c.accel = accel;

    // Calculate jump velocity
    float gravity_mag = std::abs(gravity);
    if (gravity_mag > 0.0f) {
        c.jump_velocity = std::sqrt(2.0f * gravity_mag * jump_height);
    }
}

} // namespace character