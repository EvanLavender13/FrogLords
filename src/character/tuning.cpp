#include "character/tuning.h"
#include <algorithm>
#include <cmath>

namespace character {

void tuning_params::apply_to(controller& c) const {
    // Copy high-level parameters (source of truth)
    c.max_speed = max_speed;
    c.gravity = gravity;

    // Clamp friction coefficient to physically valid range [0.0, 1.0]
    // μ > 1.0 is non-physical for kinetic friction
    c.friction = std::clamp(friction_coefficient, 0.0f, 1.0f);

    // Calculate derived values
    if (time_to_max_speed <= 0.0f) {
        return;
    }

    float desired_accel = max_speed / time_to_max_speed;

    // Direct acceleration assignment (no inflation)
    c.ground_accel = desired_accel;
    c.air_accel = desired_accel;

    // Calculate jump velocity (gravity_mag required for this calculation)
    float gravity_mag = std::abs(gravity);
    if (gravity_mag > 0.0f) {
        c.jump_velocity = std::sqrt(2.0f * gravity_mag * jump_height);
    }
}

} // namespace character