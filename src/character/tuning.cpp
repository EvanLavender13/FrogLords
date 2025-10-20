#include "character/tuning.h"
#include <algorithm>
#include <cmath>

namespace character {

void tuning_params::apply_to(controller& c) const {
    // Copy physics parameters (source of truth)
    c.max_speed = max_speed;
    c.gravity = gravity;
    c.ground_accel = ground_accel;
    c.air_accel = ground_accel; // Use same acceleration for air and ground
    c.dash_impulse = dash_impulse;

    // Clamp friction coefficient to physically valid range [0.0, 1.0]
    c.base_friction = std::clamp(base_friction, 0.0f, 1.0f);

    // Recalculate drag coefficient to maintain equilibrium at max_speed
    // This auto-adjusts base_friction if ground_accel is too low
    c.recalculate_drag_coefficient();

    // Calculate jump velocity from desired jump height
    float gravity_mag = std::abs(gravity);
    if (gravity_mag > 0.0f) {
        c.jump_velocity = std::sqrt(2.0f * gravity_mag * jump_height);
    }
}

} // namespace character