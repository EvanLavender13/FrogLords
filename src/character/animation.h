#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace character {

struct animation_update_params {
    float dt;
    float orientation_yaw;
    glm::vec3 acceleration;
};

struct animation_state {
    // Acceleration tilt
    glm::vec3 tilt_angles;       // Current smoothed tilt (pitch, roll in radians)
    float tilt_smoothing = 8.0f; // Response speed (higher = snappier)
    float tilt_magnitude = 0.3f; // Max tilt angle in radians (~17 degrees)

    void update(const animation_update_params& params);
    glm::mat4 get_tilt_matrix() const;
};

} // namespace character
