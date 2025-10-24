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

    // Parameter metadata for GUI presentation
    static constexpr param_meta turn_rate_meta = {
        "Turn Rate", "rad/s", 1.0f, 10.0f
    };

    static constexpr param_meta steering_reduction_factor_meta = {
        "Steering Reduction", "", 0.0f, 1.0f
    };

    void apply_to(controller& c) const;
};

} // namespace vehicle
