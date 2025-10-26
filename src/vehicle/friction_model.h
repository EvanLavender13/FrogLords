#pragma once

// Unified friction/drag model for vehicle physics
//
// Computes drag coefficient for horizontal physics integration from:
// - Base drag: Maintains equilibrium at max_speed with full throttle
// - Handbrake drag: Additional drag when handbrake active (state passed as parameter)
// - Future: Surface friction modifiers, drift friction scaling, etc.
//
// Physics integration uses unified drag in exact exponential solution:
//   dv/dt = a - k_total*v
//   v(t+dt) = v(t)*exp(-k_total*dt) + (a/k_total)*(1 - exp(-k_total*dt))
//
// This ensures time-independence when multiple drag sources are active.
//
// Note: Handbrake state now owned by controller (query pattern, not composition)
struct friction_model {
    // Compute total drag coefficient for physics integration
    // Returns k_total = k_base + k_handbrake (+ future modifiers)
    // Units: 1/s (drag coefficient)
    //
    // @param accel Horizontal acceleration magnitude (m/s²)
    // @param max_speed Maximum equilibrium speed (m/s)
    // @param handbrake_active Whether handbrake is engaged
    // @param brake_rate Handbrake drag coefficient contribution (1/s)
    float compute_total_drag(float accel, float max_speed, bool handbrake_active, float brake_rate) const;

    // Get base drag rate for GUI/debug display (derived parameter)
    // Formula: k_base = accel / max_speed (ensures equilibrium at max_speed)
    // Units: 1/s (drag coefficient)
    float get_base_drag_rate(float accel, float max_speed) const;
};
