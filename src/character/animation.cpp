#include "animation.h"
#include <cmath>
#include <glm/gtc/quaternion.hpp>

namespace character {

// Joint indices (must match order in t_pose.cpp)
namespace joint_index {
constexpr int LEFT_SHOULDER = 5;
constexpr int LEFT_ELBOW = 6;
constexpr int RIGHT_SHOULDER = 8;
constexpr int RIGHT_ELBOW = 9;
constexpr int LEFT_HIP = 11;
constexpr int LEFT_KNEE = 12;
constexpr int RIGHT_HIP = 14;
constexpr int RIGHT_KNEE = 15;
} // namespace joint_index

// Forward declaration
static void update_secondary_motion(skeleton& skel, secondary_motion_state& state, float dt);

animation_state::animation_state()
    : tilt_angles(0.0f) {
    landing_spring.stiffness = 400.0f;
    landing_spring.damping = critical_damping(landing_spring.stiffness);
}

void animation_state::update_acceleration_tilt(glm::vec3 acceleration, glm::vec3 velocity,
                                               float max_speed,
                                               float orientation_yaw, // NOLINT
                                               float dt) {
    // Extract horizontal acceleration (ignore vertical/gravity)
    glm::vec3 horizontal_accel = glm::vec3(acceleration.x, 0.0f, acceleration.z);
    glm::vec3 horizontal_velocity = glm::vec3(velocity.x, 0.0f, velocity.z);
    float accel_magnitude = glm::length(horizontal_accel);
    float velocity_magnitude = glm::length(horizontal_velocity);

    if (accel_magnitude > 0.01f) {
        // Transform world-space acceleration to character-local space
        // Rotate acceleration vector by -yaw to get it in character's frame of reference
        float cos_yaw = std::cos(-orientation_yaw);
        float sin_yaw = std::sin(-orientation_yaw);
        float local_forward = horizontal_accel.z * cos_yaw - horizontal_accel.x * sin_yaw;
        float local_right = horizontal_accel.x * cos_yaw + horizontal_accel.z * sin_yaw;

        // Normalize local acceleration direction (tilt should show direction, not magnitude)
        float local_accel_magnitude =
            std::sqrt(local_forward * local_forward + local_right * local_right);
        if (local_accel_magnitude > 0.01f) {
            local_forward /= local_accel_magnitude;
            local_right /= local_accel_magnitude;
        }

        // Scale tilt based on velocity (faster movement = more tilt, but clamped)
        // Use velocity relative to max speed for scaling (0.5x to 1.5x tilt magnitude)
        float velocity_scale = glm::clamp(velocity_magnitude / max_speed, 0.0f, 1.0f);
        float effective_tilt_magnitude = tilt_magnitude * (0.5f + velocity_scale * 1.0f);

        // Map normalized local acceleration to tilt angles
        // Forward acceleration → pitch forward (positive pitch)
        // Right acceleration → roll right (positive roll)
        float target_pitch = local_forward * effective_tilt_magnitude;
        float target_roll = local_right * effective_tilt_magnitude;

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

void animation_state::update(const animation_update_params& params) {
    update_acceleration_tilt(params.acceleration, params.velocity, 8.0f, params.orientation_yaw,
                             params.dt);
}

void animation_state::update_skeletal_animation(skeleton& skel, float distance_traveled,
                                                pose_type manual_override_pose,
                                                bool use_manual_override,
                                                bool enable_secondary_motion, float dt) {
    if (use_manual_override) {
        apply_pose(skel, manual_override_pose);
        if (enable_secondary_motion) {
            update_secondary_motion(skel, secondary_motion, dt);
        }
        return;
    }

    // Calculate phase (0.0-1.0) from distance traveled
    float phase = std::fmod(distance_traveled, cycle_length) / cycle_length;

    // Threshold-based pose selection (STEP_LEFT → NEUTRAL → STEP_RIGHT → NEUTRAL)
    // Two NEUTRAL ranges (0.25-0.5, 0.75-1.0) frame steps symmetrically
    if (phase < 0.25f) {
        current_automatic_pose = pose_type::STEP_LEFT;
    } else if (phase < 0.5f) { // NOLINT(bugprone-branch-clone)
        current_automatic_pose = pose_type::NEUTRAL;
    } else if (phase < 0.75f) {
        current_automatic_pose = pose_type::STEP_RIGHT;
    } else {
        current_automatic_pose = pose_type::NEUTRAL;
    }

    // Apply selected pose
    apply_pose(skel, current_automatic_pose);

    // Apply secondary motion (spring-based follow-through)
    if (enable_secondary_motion) {
        update_secondary_motion(skel, secondary_motion, dt);
    }
}

/// Apply per-joint spring-damper lag to create natural follow-through.
/// Child joints lag behind their parent's motion, creating visible wobble.
static void update_secondary_motion(skeleton& skel, secondary_motion_state& state, float dt) {
    auto update_spring = [&](int parent_idx, int joint_idx, float& offset, float& velocity,
                             glm::quat& prev_parent_rot, const glm::vec3& rotation_axis) {
        // Get parent's current rotation (this is what drives the motion)
        glm::quat current_parent_rot = glm::quat_cast(skel.joints[parent_idx].local_transform);

        // Detect parent rotation change (parent's motion that child should follow)
        glm::quat delta_rot = current_parent_rot * glm::inverse(prev_parent_rot);
        float angle = glm::angle(delta_rot);

        // When parent rotates, inject that motion as velocity into child's spring
        if (angle > 0.001f && dt > 0.0f) { // Lower threshold to catch smaller rotations
            glm::vec3 axis = glm::axis(delta_rot);
            // Project parent's rotation onto our tracking axis
            float axis_component = glm::dot(axis, rotation_axis);

            // Use full rotation magnitude if axis alignment is weak
            // This captures rotation even when it's not perfectly aligned with tracking axis
            float effective_angle =
                std::abs(axis_component) > 0.1f
                    ? (angle * axis_component)           // Use projected rotation if well-aligned
                    : angle * glm::sign(axis_component); // Use full magnitude with sign

            float angular_velocity_change = effective_angle / dt;

            // Add parent's angular velocity to child's spring (child follows parent)
            velocity += angular_velocity_change * state.response_scale;
        }

        // Spring toward zero offset (child catches up to parent's motion)
        float spring_force = state.stiffness * (0.0f - offset);
        float damping = critical_damping(state.stiffness) * state.damping_ratio;
        float damping_force = damping * velocity;
        float acceleration = spring_force - damping_force;

        // Integrate
        velocity += acceleration * dt;
        offset += velocity * dt;

        // Apply offset to child joint (lag behind parent's motion)
        glm::vec3 position = glm::vec3(skel.joints[joint_idx].local_transform[3]);
        glm::quat child_rot = glm::quat_cast(skel.joints[joint_idx].local_transform);
        glm::quat lag_rot = glm::angleAxis(offset, rotation_axis);
        glm::quat final_rot = lag_rot * child_rot;

        skel.joints[joint_idx].local_transform =
            glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(final_rot);

        // Store parent rotation for next frame
        prev_parent_rot = current_parent_rot;
    };

    // Child joints follow parent motion
    update_spring(joint_index::LEFT_SHOULDER, joint_index::LEFT_ELBOW, state.left_elbow_offset,
                  state.left_elbow_velocity, state.prev_left_shoulder, glm::vec3(0, 1, 0));
    update_spring(joint_index::RIGHT_SHOULDER, joint_index::RIGHT_ELBOW, state.right_elbow_offset,
                  state.right_elbow_velocity, state.prev_right_shoulder, glm::vec3(0, 1, 0));
    update_spring(joint_index::LEFT_HIP, joint_index::LEFT_KNEE, state.left_knee_offset,
                  state.left_knee_velocity, state.prev_left_hip, glm::vec3(1, 0, 0));
    update_spring(joint_index::RIGHT_HIP, joint_index::RIGHT_KNEE, state.right_knee_offset,
                  state.right_knee_velocity, state.prev_right_hip, glm::vec3(1, 0, 0));
}

} // namespace character
