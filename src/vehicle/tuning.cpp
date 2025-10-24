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

    FL_ASSERT_FINITE_SCALAR(turn_rate, "turn_rate");
    FL_ASSERT_FINITE_SCALAR(steering_reduction_factor, "steering_reduction_factor");

    // Copy parameters directly (no derivation)
    c.turn_rate = turn_rate;
    c.steering_reduction_factor = steering_reduction_factor;
}

} // namespace vehicle
