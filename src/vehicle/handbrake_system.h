#pragma once

// Forward declaration
struct controller;

/**
 * handbrake_system
 *
 * Applies braking force opposing velocity when handbrake engaged.
 * Composed into controller for semantic clarity.
 *
 * Physics: brake_force = -velocity * brake_strength
 *          acceleration += brake_force
 */
struct handbrake_system {
    // State
    bool active = false;

    // TUNED: Brake force multiplier
    // Applied as: brake_force = -velocity * brake_strength
    // Higher values = stronger braking
    float brake_strength = 2.0f; // dimensionless multiplier

    /**
     * Apply handbrake braking force to controller
     *
     * @param input Handbrake button state (true = engaged)
     * @param ctrl Controller to modify (adds brake force to acceleration)
     * @param dt Delta time in seconds
     */
    void apply(bool input, controller& ctrl, float dt);

    // Query current handbrake state
    bool is_active() const { return active; }
};
