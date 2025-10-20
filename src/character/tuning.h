#pragma once

#include "character/controller.h"

namespace character {

struct tuning_params {
    // TUNED: Maximum horizontal movement speed (equilibrium point)
    // Real-world context: 8.0 m/s ≈ 17.9 mph (fast jogging pace)
    // At this speed: ground_accel = base_friction·|g| + drag·max_speed (net zero)
    float max_speed = 8.0f; // m/s

    // TUNED: Ground acceleration (propulsion force)
    // How hard the character pushes against the ground
    // Higher = faster acceleration, requires more friction/drag to balance
    // Default: 20.0 m/s² (reaches max_speed in ~0.4s)
    float ground_accel = 20.0f; // m/s²

    // TUNED: Base friction coefficient (velocity-independent)
    // Ground contact friction: F = base_friction · |g|
    // Range: [0.0, 1.0] physically valid
    //   - 0.0 = ice (no contact friction)
    //   - 0.6 = current default (smooth but controlled)
    //   - 1.0 = maximum practical friction
    // Note: Auto-reduces if ground_accel too low to maintain equilibrium
    float base_friction = 0.6f; // dimensionless

    // TUNED: Target vertical jump height
    // With BUMPER_RADIUS=0.5m: Character can reach platforms at Y = 0.5 + 1.3 = 1.8m
    // Used to calculate jump_velocity via v = √(2·|g|·h)
    float jump_height = 1.3f; // meters

    // TUNED: Gravitational acceleration
    // Standard value: -9.8 m/s² (Earth's gravity at sea level)
    // Sign convention: Negative because Y-up coordinate system
    float gravity = -9.8f; // m/s²

    // TUNED: Dash impulse (instant velocity boost)
    // Applied directly to velocity in input direction
    // Higher values = stronger burst
    // Deceleration time depends on drag_coefficient (influenced by ground_accel/max_speed ratio)
    float dash_impulse = 6.0f; // m/s

    void apply_to(controller& c) const;
};

} // namespace character
