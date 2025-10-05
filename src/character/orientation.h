#pragma once
#include <glm/glm.hpp>

struct orientation_system {
    float target_yaw = 0.0f;
    float current_yaw = 0.0f;
    float yaw_smoothing = 5.0f;

    void update(glm::vec3 velocity, float dt);
    float get_yaw() const { return current_yaw; }
};
