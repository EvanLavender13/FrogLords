#pragma once

#include "character/controller.h"
#include "foundation/param_meta.h"

namespace character {

struct tuning_params {
    // TUNED: Maximum horizontal movement speed (source of truth)
    // Real-world context: 8.0 m/s ≈ 17.9 mph (fast jogging pace)
    // Used in: apply_to to copy to controller.max_speed
    float max_speed = 8.0f; // m/s

    // TUNED: Horizontal acceleration (direct physical parameter)
    // Controls responsiveness - higher = snappier feel
    // Used in: apply_to to copy to controller.accel
    float accel = 5.0f; // m/s²

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

    // NOTE: Friction removed - drag now derived from accel/max_speed
    // Drag coefficient k = accel / max_speed guarantees equilibrium at max_speed
    // See controller::update for exponential drag model implementation

    // Parameter metadata for GUI presentation
    static constexpr param_meta max_speed_meta = {
        "Max Speed", "m/s", 1.0f, 15.0f, param_type::TUNABLE
    };

    static constexpr param_meta accel_meta = {
        "Acceleration", "m/s²", 1.0f, 50.0f, param_type::TUNABLE
    };

    static constexpr param_meta jump_height_meta = {
        "Jump Height", "m", 0.5f, 3.0f, param_type::TUNABLE
    };

    static constexpr param_meta gravity_meta = {
        "Gravity", "m/s²", -20.0f, -5.0f, param_type::TUNABLE
    };

    void apply_to(controller& c) const;
};

} // namespace character
