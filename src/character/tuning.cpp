#include "character/tuning.h"
#include <algorithm>
#include <cmath>

namespace {
// COEFFICIENT: Fraction of ground_accel used to overcome friction
// Physical meaning: 75% of applied acceleration counters friction, 25% is net
// Design intent: High friction ratio creates snappy stopping behavior
// Used in: apply_to (line 19, 24) to decompose acceleration and calculate friction
constexpr float FRICTION_RATIO = 0.75f; // dimensionless

// CALCULATED: Fraction of ground_accel that produces net acceleration
// Derived: NET_FRACTION = 1.0 - FRICTION_RATIO = 1.0 - 0.75 = 0.25
// Physical meaning: Only 25% of ground_accel contributes to speed change
// Used in: apply_to (line 19) and read_from (line 31) for accel decomposition
constexpr float NET_FRACTION = 1.0f - FRICTION_RATIO; // dimensionless (= 0.25)
} // namespace

namespace character {

void tuning_params::apply_to(controller& c) const {
    // Copy high-level parameters (source of truth)
    c.max_speed = max_speed;
    c.gravity = gravity;

    // Calculate derived values
    if (time_to_max_speed <= 0.0f) {
        return;
    }

    float desired_accel = max_speed / time_to_max_speed;

    c.ground_accel = desired_accel / NET_FRACTION;
    c.air_accel = desired_accel;

    float gravity_mag = std::abs(gravity);
    if (gravity_mag > 0.0f) {
        c.friction = (c.ground_accel * FRICTION_RATIO) / gravity_mag;
        c.jump_velocity = std::sqrt(2.0f * gravity_mag * jump_height);
    }
}

void tuning_params::read_from(const controller& c) {
    if (c.ground_accel > 0.0f) {
        float net_accel = c.ground_accel * NET_FRACTION;
        time_to_max_speed = c.max_speed / net_accel;
    }

    float gravity_mag = std::abs(c.gravity);
    if (gravity_mag > 0.0f) {
        jump_height = (c.jump_velocity * c.jump_velocity) / (2.0f * gravity_mag);
    }
}

} // namespace character