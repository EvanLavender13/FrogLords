#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "foundation/spring_damper.h"

namespace character {

struct animation_state {
    // Acceleration tilt
    glm::vec3 tilt_angles;       // Current smoothed tilt (pitch, roll in radians)
    float tilt_smoothing = 8.0f; // Response speed (higher = snappier)
    float tilt_magnitude = 0.3f; // Max tilt angle in radians (~17 degrees)

    // Landing spring
    spring_damper landing_spring;
    float landing_impulse_scale = 0.5f;

    // Contact/air weight system (phase continuity)
    spring_damper contact_weight_spring;
    float contact_weight_target = 1.0f;    // Dual-reference pattern (instant 0/1 flip)
    float contact_weight_frequency = 8.0f; // Tuning parameter (Hz)

    animation_state();

    void update_acceleration_tilt(glm::vec3 acceleration, glm::vec3 velocity,
                                  float reference_speed, // Absolute speed baseline for tilt scaling
                                  float orientation_yaw, // NOLINT
                                  float dt);
    void update_landing_spring(bool just_landed,
                               float vertical_velocity, // NOLINT
                               float dt);
    void update_contact_weight(bool is_grounded, float dt);
    glm::mat4 get_tilt_matrix() const;
    glm::mat4 get_vertical_offset_matrix() const;
    float get_vertical_offset() const;
};

} // namespace character
