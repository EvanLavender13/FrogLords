#include "foundation/spring_damper.h"
#include <cmath>

void spring_damper::update(float target, float dt) {
    // F = -k * (x - target) - c * v
    float spring_force = -stiffness * (position - target);
    float damping_force = -damping * velocity;
    float acceleration = spring_force + damping_force;

    // Semi-implicit Euler: update velocity first, then position
    velocity += acceleration * dt;
    position += velocity * dt;
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
