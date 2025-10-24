#include "vehicle/tuning.h"
#include "foundation/debug_assert.h"
#include <algorithm>
#include <cmath>

namespace vehicle {

void tuning_params::apply_to(controller& c) const {
    // Validate preconditions: parameters must be within metadata-defined ranges
    // Single source of truth: metadata defines allowable domain
    FL_PRECONDITION(turn_rate >= turn_rate_meta.min && turn_rate <= turn_rate_meta.max,
                    "turn_rate must be within metadata range");
    FL_PRECONDITION(steering_reduction_factor >= steering_reduction_factor_meta.min &&
                        steering_reduction_factor <= steering_reduction_factor_meta.max,
                    "steering_reduction_factor must be within metadata range");
    FL_PRECONDITION(wheelbase >= wheelbase_meta.min && wheelbase <= wheelbase_meta.max,
                    "wheelbase must be within metadata range");
    FL_PRECONDITION(max_steering_angle >= max_steering_angle_meta.min &&
                        max_steering_angle <= max_steering_angle_meta.max,
                    "max_steering_angle must be within metadata range");
    FL_PRECONDITION(grip_coefficient >= grip_coefficient_meta.min &&
                        grip_coefficient <= grip_coefficient_meta.max,
                    "grip_coefficient must be within metadata range");

    FL_ASSERT_FINITE_SCALAR(turn_rate, "turn_rate");
    FL_ASSERT_FINITE_SCALAR(steering_reduction_factor, "steering_reduction_factor");
    FL_ASSERT_FINITE_SCALAR(wheelbase, "wheelbase");
    FL_ASSERT_FINITE_SCALAR(max_steering_angle, "max_steering_angle");
    FL_ASSERT_FINITE_SCALAR(grip_coefficient, "grip_coefficient");

    // Copy parameters directly (no derivation)
    c.turn_rate = turn_rate;
    c.steering_reduction_factor = steering_reduction_factor;
    c.wheelbase = wheelbase;
    c.max_steering_angle = max_steering_angle;
    c.grip_coefficient = grip_coefficient;
}

} // namespace vehicle
