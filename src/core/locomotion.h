#pragma once
#include <glm/glm.hpp>

struct simple_pose {
    glm::vec3 root_offset;
    float leg_phase_offset;
};

struct locomotion_state {
    simple_pose pass_pose;
    simple_pose reach_pose;
    float stride_length;
};

struct locomotion_system {
    locomotion_state walk_state;
    locomotion_state run_state;

    float distance_traveled = 0.0f;
    float phase = 0.0f;

    // Speed-based blending
    float current_speed = 0.0f;
    float smoothed_speed = 0.0f;
    float speed_smoothing = 10.0f;      // Smoothing rate
    float walk_speed_threshold = 2.0f;  // m/s
    float run_speed_threshold = 6.0f;   // m/s

    locomotion_system();

    void update(glm::vec3 ground_velocity, float dt, bool is_grounded);
    simple_pose get_current_pose() const;

private:
    simple_pose lerp(const simple_pose& a, const simple_pose& b, float t) const;
    simple_pose cubic_interp(const simple_pose& a, const simple_pose& b, float t) const;
};
