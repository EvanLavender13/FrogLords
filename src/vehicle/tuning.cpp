#include "vehicle/tuning.h"
#include "foundation/debug_assert.h"
#include <algorithm>
#include <cmath>

namespace vehicle {

void tuning_params::apply_to(controller& c) const {
    // Validate preconditions: parameters must be within metadata-defined ranges
    // Single source of truth: metadata defines allowable domain
    FL_PRECONDITION(max_speed >= max_speed_meta.min && max_speed <= max_speed_meta.max,
                    "max_speed must be within metadata range");
    FL_PRECONDITION(accel >= accel_meta.min && accel <= accel_meta.max,
                    "accel must be within metadata range");
    FL_PRECONDITION(weight >= weight_meta.min && weight <= weight_meta.max,
                    "weight must be within metadata range");
    FL_PRECONDITION(turn_rate >= turn_rate_meta.min && turn_rate <= turn_rate_meta.max,
                    "turn_rate must be within metadata range");
    FL_PRECONDITION(steering_reduction_factor >= steering_reduction_factor_meta.min &&
                        steering_reduction_factor <= steering_reduction_factor_meta.max,
                    "steering_reduction_factor must be within metadata range");

    FL_ASSERT_FINITE_SCALAR(max_speed, "max_speed");
    FL_ASSERT_FINITE_SCALAR(accel, "accel");
    FL_ASSERT_FINITE_SCALAR(weight, "weight");
    FL_ASSERT_FINITE_SCALAR(turn_rate, "turn_rate");
    FL_ASSERT_FINITE_SCALAR(steering_reduction_factor, "steering_reduction_factor");

    // Copy parameters directly (no derivation)
    c.max_speed = max_speed;
    c.accel = accel;
    c.weight = weight;
    c.turn_rate = turn_rate;
    c.steering_reduction_factor = steering_reduction_factor;
}

} // namespace vehicle
