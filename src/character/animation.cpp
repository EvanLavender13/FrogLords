#include "animation.h"
#include <cmath>

namespace character {

void animation_state::update(const animation_update_params& params) {
    // Extract horizontal acceleration (ignore vertical/gravity)
    glm::vec3 horizontal_accel = glm::vec3(params.acceleration.x, 0.0f, params.acceleration.z);
    float accel_magnitude = glm::length(horizontal_accel);

    if (accel_magnitude > 0.01f) {
        // Transform world-space acceleration to character-local space
        // Rotate acceleration vector by -yaw to get it in character's frame of reference
        float cos_yaw = std::cos(-params.orientation_yaw);
        float sin_yaw = std::sin(-params.orientation_yaw);
        float local_forward = horizontal_accel.z * cos_yaw - horizontal_accel.x * sin_yaw;
        float local_right = horizontal_accel.x * cos_yaw + horizontal_accel.z * sin_yaw;

        // Map local acceleration to tilt angles
        // Forward acceleration → pitch forward (positive pitch)
        // Right acceleration → roll right (positive roll)
        float target_pitch = local_forward * tilt_magnitude / 20.0f;
        float target_roll = local_right * tilt_magnitude / 20.0f;

        // Smooth toward target (exponential decay)
        float blend = 1.0f - glm::exp(-tilt_smoothing * params.dt);
        tilt_angles.x = glm::mix(tilt_angles.x, target_pitch, blend);
        tilt_angles.z = glm::mix(tilt_angles.z, target_roll, blend);
    } else {
        // Return to neutral when no acceleration
        float blend = 1.0f - glm::exp(-tilt_smoothing * params.dt);
        tilt_angles = glm::mix(tilt_angles, glm::vec3(0.0f), blend);
    }
}

glm::mat4 animation_state::get_tilt_matrix() const {
    // Build rotation matrix: roll around Z, then pitch around X
    glm::mat4 tilt = glm::mat4(1.0f);
    tilt = glm::rotate(tilt, tilt_angles.z, glm::vec3(0, 0, 1)); // Roll
    tilt = glm::rotate(tilt, tilt_angles.x, glm::vec3(1, 0, 0)); // Pitch
    return tilt;
}

} // namespace character
