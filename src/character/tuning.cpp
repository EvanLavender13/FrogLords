#include "character/tuning.h"
#include "foundation/debug_assert.h"
#include <algorithm>
#include <cmath>

namespace character {

void tuning_params::apply_to(controller& c) const {
    // Validate preconditions: parameters must be within metadata-defined ranges
    // Single source of truth: metadata defines allowable domain
    FL_PRECONDITION(max_speed >= max_speed_meta.min && max_speed <= max_speed_meta.max,
                    "max_speed must be within metadata range");
    FL_PRECONDITION(accel >= accel_meta.min && accel <= accel_meta.max,
                    "accel must be within metadata range");
    FL_PRECONDITION(weight >= weight_meta.min && weight <= weight_meta.max,
                    "weight must be within metadata range");
    FL_ASSERT_FINITE_SCALAR(max_speed, "max_speed");
    FL_ASSERT_FINITE_SCALAR(accel, "accel");
    FL_ASSERT_FINITE_SCALAR(weight, "weight");

    // Copy parameters directly (no derivation)
    c.max_speed = max_speed;
    c.weight = weight;
    c.accel = accel;
}

} // namespace character