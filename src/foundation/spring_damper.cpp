#include "foundation/spring_damper.h"
#include <cmath>

void spring_damper::update(spring_step step) {
    // F = -k * (x - target) - c * v
    float spring_force = -stiffness * (position - step.target);
    float damping_force = -damping * velocity;
    float acceleration = spring_force + damping_force;

    // Semi-implicit Euler: update velocity first, then position
    velocity += acceleration * step.delta_time;
    position += velocity * step.delta_time;
}

void spring_damper::add_impulse(float impulse) {
    velocity += impulse;
}

float critical_damping(float stiffness, float mass) {
    // DERIVED: Critical damping formula from harmonic oscillator theory
    // Equation: c = 2√(k·m)
    // Derivation:
    //   - Spring-damper ODE: mẍ + cẋ + kx = 0
    //   - Characteristic equation: mλ² + cλ + k = 0
    //   - Critical damping (ζ=1): Discriminant = 0 → c² = 4km → c = 2√(km)
    // Coefficient: 2.0 is the mathematical constant from the derivation
    // Result: Fastest return to equilibrium without overshoot
    return 2.0f * sqrtf(stiffness * mass); // DERIVED coefficient: 2.0 (dimensionless)
}
