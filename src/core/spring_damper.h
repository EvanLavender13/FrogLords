#pragma once

struct spring_damper {
    float position = 0.0f;
    float velocity = 0.0f;

    float stiffness = 100.0f;
    float damping = 20.0f;

    void update(float target, float dt);
    void add_impulse(float impulse);
    void reset(float pos = 0.0f);

    float get_position() const { return position; }
    float get_velocity() const { return velocity; }
};

float critical_damping(float stiffness, float mass = 1.0f);
