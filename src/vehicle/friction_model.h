#pragma once
#include "vehicle/handbrake_system.h"
#include "vehicle/controller_input_params.h"

// Forward declare controller to access nested type
struct controller;

// Unified friction/drag model for vehicle physics
//
// Composes all sources of drag coefficient for horizontal physics integration:
// - Base drag: Maintains equilibrium at max_speed with full throttle
// - Handbrake drag: Additional drag when handbrake active
// - Future: Surface friction modifiers, drift friction scaling, etc.
//
// Physics integration uses unified drag in exact exponential solution:
//   dv/dt = a - k_total*v
//   v(t+dt) = v(t)*exp(-k_total*dt) + (a/k_total)*(1 - exp(-k_total*dt))
//
// This ensures time-independence when multiple drag sources are active.
struct friction_model {
    // Composed drag sources
    handbrake_system handbrake;

    // Compute total drag coefficient for physics integration
    // Returns k_total = k_base + k_handbrake (+ future modifiers)
    // Units: 1/s (drag coefficient)
    float compute_total_drag(float accel, float max_speed) const;

    // Get base drag rate for GUI/debug display (derived parameter)
    // Formula: k_base = accel / max_speed (ensures equilibrium at max_speed)
    // Units: 1/s (drag coefficient)
    float get_base_drag_rate(float accel, float max_speed) const;

    // Update active friction modifiers (handbrake state, etc.)
    void update(const controller_input_params& input);
};
