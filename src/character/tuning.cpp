#include "character/tuning.h"
#include "foundation/debug_assert.h"
#include <algorithm>
#include <cmath>

namespace character {

void tuning_params::apply_to(controller& c) const {
    // Validate preconditions: parameters must be within valid ranges
    FL_PRECONDITION(max_speed > 0.0f, "max_speed must be positive");
    FL_PRECONDITION(accel > 0.0f, "accel must be positive");
    FL_PRECONDITION(jump_height > 0.0f, "jump_height must be positive");
    FL_PRECONDITION(gravity < 0.0f, "gravity must be negative (Y-up coordinate system)");
    FL_ASSERT_FINITE_SCALAR(max_speed, "max_speed");
    FL_ASSERT_FINITE_SCALAR(accel, "accel");
    FL_ASSERT_FINITE_SCALAR(jump_height, "jump_height");
    FL_ASSERT_FINITE_SCALAR(gravity, "gravity");

    // Copy parameters directly (no derivation)
    c.max_speed = max_speed;
    c.gravity = gravity;
    c.accel = accel;

    // Calculate jump velocity from jump height and gravity
    // Formula: v = √(2·|g|·h) derived from kinematic equation v² = 2gh
    float gravity_mag = std::abs(gravity);
    c.jump_velocity = std::sqrt(2.0f * gravity_mag * jump_height);

    // Validate postcondition: jump velocity is positive and finite
    FL_POSTCONDITION(c.jump_velocity > 0.0f, "jump_velocity must be positive");
    FL_ASSERT_FINITE_SCALAR(c.jump_velocity, "jump_velocity");
}

} // namespace character