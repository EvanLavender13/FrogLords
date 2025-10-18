#pragma once

#include "character/controller.h"

namespace character {

struct tuning_params {
    // TUNED: Maximum horizontal movement speed (source of truth)
    // Real-world context: 8.0 m/s ≈ 17.9 mph (fast jogging pace)
    // Foundation for acceleration calculations
    // Used in: apply_to to copy to controller.max_speed
    float max_speed = 8.0f; // m/s

    // TUNED: Time from rest to max_speed (responsiveness feel)
    // Platformer typical range: 300-500ms (fighting games: 100-200ms)
    // Current: 400ms provides responsive but not twitchy control
    // Used in: apply_to to calculate ground_accel and air_accel
    float time_to_max_speed = 0.4f; // seconds

    // TUNED: Target vertical jump height (world-space distance)
    // With BUMPER_RADIUS=0.5m: Character can reach platforms at Y = 0.5 + 1.3 = 1.8m
    // Used in: apply_to to calculate jump_velocity via v = √(2·|g|·h)
    // Reverse calculated: Current jump_velocity (5.0 m/s) implies h ≈ 1.276m (2% difference)
    float jump_height = 1.3f; // meters

    // TUNED: Gravitational acceleration (source of truth)
    // Standard value: -9.8 m/s² (Earth's gravity at sea level)
    // Sign convention: Negative because Y-up coordinate system (gravity pulls down)
    // Used in: apply_to to copy to controller.gravity and calculate jump_velocity
    float gravity = -9.8f; // m/s²

    void apply_to(controller& c) const;
};

} // namespace character
