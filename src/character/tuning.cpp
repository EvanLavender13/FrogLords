#include "character/tuning.h"
#include <algorithm>
#include <cmath>

namespace character {

void tuning_params::apply_to(controller& c) const {
    // Copy high-level parameters (source of truth)
    c.max_speed = max_speed;
    c.gravity = gravity;

    // Friction removed - drag now derived from accel/max_speed (Step 4)
    // Was: c.friction = std::clamp(friction_coefficient, 0.0f, 1.0f);

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