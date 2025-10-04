#pragma once
#include <glm/glm.hpp>

struct forced_orientation_input {
    float yaw = 0.0f;
    float delta_time = 0.0f;
};

struct orientation_system {
    float target_yaw = 0.0f;
    float current_yaw = 0.0f;
    float yaw_smoothing = 5.0f;

    void update(glm::vec3 velocity, float dt);
    void update_forced(forced_orientation_input input);
    float get_yaw() const { return current_yaw; }
};
