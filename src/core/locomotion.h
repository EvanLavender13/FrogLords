#pragma once
#include <glm/glm.hpp>
#include "spring_damper.h"

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

    // Vertical motion (spring-damper)
    spring_damper vertical_spring;
    float time_since_last_step = 0.0f;
    float step_period = 0.5f;
    float bounce_impulse_scale = 0.5f;
    float idle_speed_threshold = 0.15f;  // m/s before easing toward idle pose
    float idle_relax_rate = 8.0f;        // exponential relax speed for phase settling
    bool is_relaxing = false;
    float relax_stride = 1.0f;

    locomotion_system();

    void update(glm::vec3 ground_velocity, float dt, bool is_grounded);
    simple_pose get_current_pose() const;

private:
    simple_pose lerp(const simple_pose& a, const simple_pose& b, float t) const;
    simple_pose cubic_interp(const simple_pose& a, const simple_pose& b, float t) const;
};
