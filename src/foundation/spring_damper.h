#pragma once

struct spring_step {
    float target = 0.0f;
    float delta_time = 0.0f;
};

struct spring_damper {
    float position = 0.0f;
    float velocity = 0.0f;

    float stiffness = 100.0f;
    float damping = 20.0f;

    void update(spring_step step);
    void add_impulse(float impulse);
    void reset(float pos = 0.0f);

    float get_position() const { return position; }
    float get_velocity() const { return velocity; }
};

float critical_damping(float stiffness, float mass = 1.0f);
