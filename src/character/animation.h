#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "foundation/spring_damper.h"
#include "keyframe.h"

namespace character {

struct animation_update_params {
    float dt;
    float orientation_yaw;
    glm::vec3 acceleration;
    glm::vec3 velocity;
};

struct secondary_motion_state {
    // Per-joint spring states (offset + velocity)
    float left_elbow_offset = 0.0f;   // Current angle offset (radians)
    float left_elbow_velocity = 0.0f; // Angular velocity (radians/s)
    float right_elbow_offset = 0.0f;
    float right_elbow_velocity = 0.0f;
    float left_knee_offset = 0.0f;
    float left_knee_velocity = 0.0f;
    float right_knee_offset = 0.0f;
    float right_knee_velocity = 0.0f;

    // Previous rotations for detecting pose changes (track PARENT rotations)
    glm::quat prev_left_shoulder = glm::quat();
    glm::quat prev_right_shoulder = glm::quat();
    glm::quat prev_left_hip = glm::quat();
    glm::quat prev_right_hip = glm::quat();

    // Tuning parameters
    float stiffness = 15.0f;      // Spring response speed (Hz, range 10.0-20.0) - lower = more lag
    float damping_ratio = 1.0f;   // Critically damped by default (range 0.5-2.0)
    float response_scale = 0.02f; // How much to amplify rotation changes (higher = more wobble)
};

struct animation_state {
    // Acceleration tilt
    glm::vec3 tilt_angles;       // Current smoothed tilt (pitch, roll in radians)
    float tilt_smoothing = 8.0f; // Response speed (higher = snappier)
    float tilt_magnitude = 0.3f; // Max tilt angle in radians (~17 degrees)

    // Landing spring
    spring_damper landing_spring;
    float landing_impulse_scale = 0.5f;

    // Distance-phased skeletal animation
    float cycle_length = 2.0f;
    pose_type current_automatic_pose = pose_type::NEUTRAL;

    // Secondary motion (skeletal follow-through)
    secondary_motion_state secondary_motion;

    animation_state();

    void update_acceleration_tilt(glm::vec3 acceleration, glm::vec3 velocity, float max_speed,
                                  float orientation_yaw, // NOLINT
                                  float dt);
    void update_landing_spring(bool just_landed,
                               float vertical_velocity, // NOLINT
                               float dt);
    glm::mat4 get_tilt_matrix() const;
    glm::mat4 get_vertical_offset_matrix() const;
    float get_vertical_offset() const;

    // Legacy update method (calls update_acceleration_tilt for compatibility)
    void update(const animation_update_params& params);

    // Distance-phased skeletal animation update
    void update_skeletal_animation(skeleton& skel, float distance_traveled,
                                   pose_type manual_override_pose, bool use_manual_override,
                                   bool enable_secondary_motion, float dt);
};

} // namespace character
