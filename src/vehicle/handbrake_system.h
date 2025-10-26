#pragma once

/**
 * handbrake_system
 *
 * Contributes additional drag coefficient to friction_model when engaged.
 * Composed into friction_model for semantic clarity.
 *
 * Physics: Adds brake_rate to total drag coefficient k_total
 *          Used in unified exponential integrator: v *= exp(-k_total*dt)
 *          Time-independent: identical behavior at any framerate
 */
struct handbrake_system {
    // State
    bool active = false;

    // TUNED: Drag coefficient contribution when handbrake engaged
    // Added to base drag in friction_model::compute_total_drag()
    // Higher values = stronger braking
    // Units: 1/s (inverse time, drag coefficient)
    float brake_rate = 2.0f;

    /**
     * Update handbrake state from input
     *
     * @param input Handbrake button state (true = engaged)
     */
    void update(bool input);

    // Query current handbrake state
    bool is_active() const { return active; }
};
