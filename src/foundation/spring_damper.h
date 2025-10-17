#pragma once

struct spring_step {
    float target = 0.0f;
    float delta_time = 0.0f;
};

struct spring_damper {
    float position = 0.0f;
    float velocity = 0.0f;

    // TUNED: Default spring stiffness (generic moderate value)
    // Natural frequency: ω = √(k/m) = √(100/1) = 10 rad/s ≈ 1.59 Hz
    // Typical usage: Override with specific values (e.g., landing_spring uses 400.0)
    // Used in: Spring force calculation F = -k·(x - target)
    float stiffness = 100.0f; // 1/s² (unitless spring, assumes mass=1)

    // TUNED: Default damping coefficient (critically damped with default stiffness)
    // Damping ratio: ζ = c/(2√(km)) = 20/(2√(100·1)) = 20/20 = 1.0 (critical)
    // Result: No overshoot, smooth settle to target
    // Typical usage: Override via critical_damping() for specific stiffness values
    float damping = 20.0f; // 1/s (unitless damper)

    void update(spring_step step);
    void add_impulse(float impulse);
    void reset(float pos = 0.0f);

    float get_position() const { return position; }
    float get_velocity() const { return velocity; }
};

// DERIVED: Calculate critical damping coefficient for given stiffness
// Formula: c = 2√(k·m) from harmonic oscillator theory (ζ=1 condition)
// Parameters:
//   - stiffness: Spring constant k (1/s² for unitless springs)
//   - mass: System mass (default 1.0 for unit mass assumption)
// Returns: Damping coefficient c that produces ζ=1 (no overshoot)
// Used in: animation_state constructor to set landing_spring damping
float critical_damping(float stiffness, float mass = 1.0f);
