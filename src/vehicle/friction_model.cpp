#include "vehicle/friction_model.h"
#include "foundation/debug_assert.h"

float friction_model::compute_total_drag(float accel, float max_speed, float handbrake_drag) const {
    FL_PRECONDITION(accel > 0.0f, "accel must be positive");
    FL_PRECONDITION(max_speed > 0.0f, "max_speed must be positive");
    FL_PRECONDITION(handbrake_drag >= 0.0f, "handbrake_drag must be non-negative");
    FL_ASSERT_FINITE_SCALAR(accel, "accel");
    FL_ASSERT_FINITE_SCALAR(max_speed, "max_speed");
    FL_ASSERT_FINITE_SCALAR(handbrake_drag, "handbrake_drag");

    // Base drag: maintains equilibrium at max_speed with full throttle
    // Derivation: At equilibrium dv/dt = 0, so a - k*v_eq = 0
    //             Want v_eq = max_speed when a = accel
    //             Therefore: k_base = accel / max_speed
    float k_base = accel / max_speed;

    // Add drag contributions from systems
    // handbrake_drag provided by handbrake_system::get_drag_contribution()
    // Future: surface_drag, drift_drag, etc.
    float k_total = k_base + handbrake_drag;

    FL_POSTCONDITION(k_total > 0.0f && std::isfinite(k_total),
                     "total drag coefficient must be positive and finite");

    return k_total;
}

float friction_model::get_base_drag_rate(float accel, float max_speed) const {
    FL_PRECONDITION(accel > 0.0f, "accel must be positive");
    FL_PRECONDITION(max_speed > 0.0f, "max_speed must be positive");

    return accel / max_speed;
}
