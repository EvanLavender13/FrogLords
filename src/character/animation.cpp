#include "animation.h"
#include "foundation/math_utils.h"
#include <cmath>
#include <glm/gtc/constants.hpp>
#include <glm/common.hpp>
#include <glm/gtc/quaternion.hpp>

#include <glm/gtx/quaternion.hpp>

namespace character {

animation_state::animation_state()
    : tilt_angles(0.0f) {
    landing_spring.stiffness = 400.0f;
    landing_spring.damping = critical_damping(landing_spring.stiffness);

    // Initialize contact weight spring (start grounded)
    contact_weight_spring.position = 1.0f;
    float omega = contact_weight_frequency * glm::two_pi<float>();
    contact_weight_spring.stiffness = omega * omega;
    contact_weight_spring.damping = critical_damping(contact_weight_spring.stiffness);
}

void animation_state::update_acceleration_tilt(glm::vec3 acceleration, glm::vec3 velocity,
                                               float reference_speed,
                                               float orientation_yaw, // NOLINT
                                               float dt) {
    // Extract horizontal acceleration (ignore vertical/gravity)
    glm::vec3 horizontal_accel = glm::vec3(acceleration.x, 0.0f, acceleration.z);
    glm::vec3 horizontal_velocity = math::project_to_horizontal(velocity);
    float accel_magnitude = glm::length(horizontal_accel);
    float velocity_magnitude = glm::length(horizontal_velocity);

    if (accel_magnitude > 0.01f) {
        // Transform world-space acceleration to character-local space
        // Rotate acceleration vector by -yaw to get it in character's frame of reference
        glm::mat2 rot = glm::mat2(cos(-orientation_yaw), -sin(-orientation_yaw),
                                  sin(-orientation_yaw), cos(-orientation_yaw));
        glm::vec2 local_accel = rot * glm::vec2(horizontal_accel.x, horizontal_accel.z);
        float local_forward = local_accel.y;
        float local_right = local_accel.x;

        // Normalize local acceleration direction (tilt should show direction, not magnitude)
        float local_accel_magnitude =
            std::sqrt(local_forward * local_forward + local_right * local_right);
        if (local_accel_magnitude > 0.01f) {
            local_forward /= local_accel_magnitude;
            local_right /= local_accel_magnitude;
        }

        // Scale tilt based on velocity (faster movement = more tilt, but clamped)
        // Use velocity relative to the reference speed for scaling (0.5x to 1.5x tilt magnitude)
        float ref_speed = glm::max(reference_speed, 0.01f);
        float velocity_scale = glm::clamp(velocity_magnitude / ref_speed, 0.0f, 1.0f);
        float effective_tilt_magnitude = tilt_magnitude * (0.5f + velocity_scale * 1.0f);

        // Map normalized local acceleration to tilt angles
        // Forward acceleration → pitch forward (positive pitch)
        // Right acceleration → roll right (positive roll)
        float target_pitch = local_forward * effective_tilt_magnitude;
        float target_roll = local_right * effective_tilt_magnitude;

        // Scale by contact weight (no tilt when airborne)
        float contact_weight = contact_weight_spring.get_position();
        target_pitch *= contact_weight;
        target_roll *= contact_weight;

        // Smooth toward target (exponential decay)
        float blend = 1.0f - glm::exp(-tilt_smoothing * dt);
        tilt_angles.x = glm::mix(tilt_angles.x, target_pitch, blend);
        tilt_angles.z = glm::mix(tilt_angles.z, target_roll, blend);
    } else {
        // Return to neutral when no acceleration
        float blend = 1.0f - glm::exp(-tilt_smoothing * dt);
        tilt_angles = glm::mix(tilt_angles, glm::vec3(0.0f), blend);
    }
}

void animation_state::update_landing_spring(bool just_landed,
                                            float vertical_velocity, // NOLINT
                                            float dt) {
    if (just_landed) {
        float impulse = -glm::abs(vertical_velocity) * landing_impulse_scale;
        landing_spring.add_impulse(impulse);
    }

    spring_step step{.target = 0.0f, .delta_time = dt};
    landing_spring.update(step);
}

void animation_state::update_contact_weight(bool is_grounded, float dt) {
    contact_weight_target = is_grounded ? 1.0f : 0.0f;

    // Convert frequency (Hz) to spring stiffness (API consistency: use glm::two_pi)
    float omega = contact_weight_frequency * glm::two_pi<float>();
    contact_weight_spring.stiffness = omega * omega;
    contact_weight_spring.damping = critical_damping(contact_weight_spring.stiffness);

    // Update spring toward target
    spring_step step = {.target = contact_weight_target, .delta_time = dt};
    contact_weight_spring.update(step);
}

glm::mat4 animation_state::get_tilt_matrix() const {
    // Build rotation matrix: roll around Z, then pitch around X
    glm::mat4 tilt = glm::mat4(1.0f);
    tilt = glm::rotate(tilt, tilt_angles.z, glm::vec3(0, 0, 1)); // Roll
    tilt = glm::rotate(tilt, tilt_angles.x, glm::vec3(1, 0, 0)); // Pitch
    return tilt;
}

glm::mat4 animation_state::get_vertical_offset_matrix() const {
    return glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, landing_spring.get_position(), 0.0f));
}

float animation_state::get_vertical_offset() const {
    return landing_spring.get_position();
}

} // namespace character
