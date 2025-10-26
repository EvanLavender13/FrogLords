#pragma once
#include "foundation/debug_assert.h"
#include <cmath>

/**
 * handbrake_system
 *
 * Provides drag coefficient contribution when handbrake engaged.
 * Owned by controller, queried by friction_model and future rear_axle_system.
 *
 * Physics: Contributes brake_rate to total drag coefficient k_total
 *          Used in unified exponential integrator:
 *            v(t+dt) = v(t)*exp(-k*dt) + (a/k)*(1 - exp(-k*dt))
 *          Time-independent: identical behavior at any framerate
 */
struct handbrake_system {
    // State
    bool active = false;

    // TUNED: Drag coefficient contribution when handbrake engaged
    // Higher values = stronger braking
    // Units: 1/s (inverse time, drag coefficient)
    // Default provided by tuning system initialization
    float brake_rate;

    /**
     * Update handbrake state from input
     *
     * @param input Handbrake button state (true = engaged)
     */
    void update(bool input);

    /**
     * Compute drag coefficient contribution for friction model
     * Encapsulates handbrake logic: returns brake_rate if active, 0 otherwise
     *
     * @return Drag coefficient contribution (1/s)
     */
    float get_drag_contribution() const {
        FL_ASSERT_NON_NEGATIVE(brake_rate, "Handbrake brake_rate");
        FL_ASSERT_FINITE_SCALAR(brake_rate, "Handbrake brake_rate");
        return active ? brake_rate : 0.0f;
    }

    // Query current handbrake state
    bool is_active() const { return active; }
};
