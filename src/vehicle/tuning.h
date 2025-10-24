#pragma once

#include "vehicle/controller.h"
#include "foundation/param_meta.h"

namespace vehicle {

struct tuning_params {
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

    // VEHICLE: Wheelbase - distance between front and rear axles (source of truth)
    // Used for circle-based turning physics (arcade racing model)
    // Real-world context: 2.5m typical sedan wheelbase
    // Used in: apply_to to copy to controller.wheelbase
    float wheelbase = 2.5f; // meters

    // VEHICLE: Maximum steering angle at front wheels (source of truth)
    // Used with speed-dependent steering limits for arcade handling
    // Real-world context: 25° = moderate steering angle for arcade feel
    // Used in: apply_to to copy to controller.max_steering_angle
    float max_steering_angle = 25.0f; // degrees

    // VEHICLE: Lateral grip coefficient for drift threshold (source of truth)
    // Multiplier for g-force calculation (1.0 = 1g lateral acceleration)
    // Higher values = tighter turns before drifting
    // Used in: apply_to to copy to controller.grip_coefficient
    float grip_coefficient = 1.2f; // dimensionless

    // Parameter metadata for GUI presentation
    static constexpr param_meta turn_rate_meta = {
        "Turn Rate", "rad/s", 1.0f, 10.0f
    };

    static constexpr param_meta steering_reduction_factor_meta = {
        "Steering Reduction", "", 0.0f, 1.0f
    };

    static constexpr param_meta wheelbase_meta = {
        "Wheelbase", "m", 1.5f, 4.0f
    };

    static constexpr param_meta max_steering_angle_meta = {
        "Max Steering Angle", "deg", 10.0f, 45.0f
    };

    static constexpr param_meta grip_coefficient_meta = {
        "Grip Coefficient", "", 0.5f, 2.0f
    };

    void apply_to(controller& c) const;
};

} // namespace vehicle
