#pragma once
#include <glm/glm.hpp>

constexpr float WHEEL_RADIUS = 0.45f;
constexpr float TWO_PI = 6.28318530718f;

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
    float speed_smoothing = 10.0f;     // Smoothing rate
    float walk_speed_threshold = 2.0f; // m/s
    float run_speed_threshold = 6.0f;  // m/s

    locomotion_system();

    void update(glm::vec3 ground_velocity, float dt);
    simple_pose get_current_pose() const;
    float get_run_blend() const;
    float get_blended_stride() const;

  private:
    float blended_stride = 2.0f; // Cached surveyor wheel size from last update
    simple_pose lerp(const simple_pose& a, const simple_pose& b, float t) const;
    simple_pose cubic_interp(const simple_pose& a, const simple_pose& b, float t) const;
};
