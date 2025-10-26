#include "vehicle/tuning.h"
#include "vehicle/vehicle_reactive_systems.h"
#include "foundation/debug_assert.h"
#include <algorithm>
#include <cmath>

namespace vehicle {

void tuning_params::apply_to(controller& c, vehicle_reactive_systems& visuals) const {
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

    // Validate visual parameters
    FL_PRECONDITION(lean_multiplier >= lean_multiplier_meta.min &&
                        lean_multiplier <= lean_multiplier_meta.max,
                    "lean_multiplier must be within metadata range");
    FL_PRECONDITION(pitch_multiplier >= pitch_multiplier_meta.min &&
                        pitch_multiplier <= pitch_multiplier_meta.max,
                    "pitch_multiplier must be within metadata range");
    FL_PRECONDITION(tilt_stiffness >= tilt_stiffness_meta.min &&
                        tilt_stiffness <= tilt_stiffness_meta.max,
                    "tilt_stiffness must be within metadata range");
    FL_PRECONDITION(orientation_stiffness >= orientation_stiffness_meta.min &&
                        orientation_stiffness <= orientation_stiffness_meta.max,
                    "orientation_stiffness must be within metadata range");

    FL_ASSERT_FINITE_SCALAR(lean_multiplier, "lean_multiplier");
    FL_ASSERT_FINITE_SCALAR(pitch_multiplier, "pitch_multiplier");
    FL_ASSERT_FINITE_SCALAR(tilt_stiffness, "tilt_stiffness");
    FL_ASSERT_FINITE_SCALAR(orientation_stiffness, "orientation_stiffness");

    // Copy controller parameters directly (no derivation)
    c.max_speed = max_speed;
    c.accel = accel;
    c.weight = weight;
    c.turn_rate = turn_rate;
    c.steering_reduction_factor = steering_reduction_factor;

    // Copy visual system parameters
    visuals.lean_multiplier = lean_multiplier;
    visuals.pitch_multiplier = pitch_multiplier;

    // Update tilt spring parameters (stiffness changed, recalculate damping)
    // Single source of truth: springs own their stiffness values
    visuals.lean_spring.stiffness = tilt_stiffness;
    visuals.lean_spring.damping = critical_damping(tilt_stiffness);
    visuals.pitch_spring.stiffness = tilt_stiffness;
    visuals.pitch_spring.damping = critical_damping(tilt_stiffness);

    // Update orientation spring parameters
    visuals.orientation.yaw_spring.stiffness = orientation_stiffness;
    visuals.orientation.yaw_spring.damping = critical_damping(orientation_stiffness);
}

} // namespace vehicle
