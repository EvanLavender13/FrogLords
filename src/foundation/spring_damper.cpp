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

void spring_damper::reset(float pos) {
    position = pos;
    velocity = 0.0f;
}

float critical_damping(float stiffness, float mass) {
    return 2.0f * sqrtf(stiffness * mass);
}
