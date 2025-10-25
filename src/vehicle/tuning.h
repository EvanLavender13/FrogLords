#pragma once

#include "vehicle/controller.h"
#include "foundation/param_meta.h"

namespace vehicle {

struct tuning_params {
    // TUNED: Maximum horizontal movement speed (source of truth)
    // Real-world context: 8.0 m/s ≈ 17.9 mph (fast jogging pace)
    // Used in: apply_to to copy to controller.max_speed
    float max_speed = 8.0f; // m/s

    // TUNED: Horizontal acceleration (direct physical parameter)
    // Controls responsiveness - higher = snappier feel
    // Used in: apply_to to copy to controller.accel
    float accel = 5.0f; // m/s²

    // TUNED: Vehicle weight force (source of truth)
    // Acts as constant downward force keeping vehicle on track
    // Sign convention: Negative because Y-up coordinate system (weight pulls down)
    // Used in: apply_to to copy to controller.weight
    float weight = -9.8f; // m/s² (acceleration, will convert to force with mass later)

    // TUNED: Turn rate for car-like control heading (source of truth)
    // Controls rotational speed when using heading-based movement
    // Higher values = faster turning (arcade feel)
    // Real-world context: 3.0 rad/s ≈ 172°/s turning rate
    // Used in: apply_to to copy to controller.turn_rate
    float turn_rate = 3.0f; // radians/second

    // TUNED: Steering reduction factor for speed-dependent steering limits (source of truth)
    // Controls how much steering authority decreases at high speeds
    // Formula: multiplier = 1.0 - (speed/max_speed) * reduction_factor
    // At max_speed with reduction_factor=0.7: 30% steering authority remains
    // Used in: apply_to to copy to controller.steering_reduction_factor
    float steering_reduction_factor = 0.7f; // dimensionless [0,1]

    // NOTE: Friction removed - drag now derived from accel/max_speed
    // Drag coefficient k = accel / max_speed guarantees equilibrium at max_speed
    // See controller::update for exponential drag model implementation

    // Parameter metadata for GUI presentation
    static constexpr param_meta max_speed_meta = {
        "Max Speed", "m/s", 1.0f, 15.0f
    };

    static constexpr param_meta accel_meta = {
        "Acceleration", "m/s²", 1.0f, 50.0f
    };

    static constexpr param_meta weight_meta = {
        "Weight", "m/s²", -20.0f, -5.0f
    };

    static constexpr param_meta turn_rate_meta = {
        "Turn Rate", "rad/s", 1.0f, 10.0f
    };

    static constexpr param_meta steering_reduction_factor_meta = {
        "Steering Reduction", "", 0.0f, 1.0f
    };

    void apply_to(controller& c) const;
};

} // namespace vehicle
