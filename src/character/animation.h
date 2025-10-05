#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "foundation/spring_damper.h"

namespace character {

struct animation_update_params {
    float dt;
    float orientation_yaw;
    glm::vec3 acceleration;
    glm::vec3 velocity;
};

struct animation_state {
    // Acceleration tilt
    glm::vec3 tilt_angles;       // Current smoothed tilt (pitch, roll in radians)
    float tilt_smoothing = 8.0f; // Response speed (higher = snappier)
    float tilt_magnitude = 0.3f; // Max tilt angle in radians (~17 degrees)

    // Landing spring
    spring_damper landing_spring;
    float landing_impulse_scale = 0.5f;

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
};

} // namespace character
