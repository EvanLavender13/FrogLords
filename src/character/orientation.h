#pragma once
#include <glm/glm.hpp>

struct orientation_system {
    float target_yaw = 0.0f;
    float current_yaw = 0.0f;

    // TUNED: Yaw rotation smoothing rate constant (exponential decay)
    // Formula: smoothing_factor = 1 - exp(-yaw_smoothing · dt)
    // Time constant: τ = 1/rate = 1/5 = 0.2s (reaches ~63% in 0.2s)
    // Effect: Smooth but responsive rotation toward movement direction
    // Used in: orientation_system::update (line 16) for framerate-independent smoothing
    float yaw_smoothing = 5.0f; // 1/seconds (Hz)

    void update(glm::vec3 velocity, float dt);
    float get_yaw() const { return current_yaw; }
};
