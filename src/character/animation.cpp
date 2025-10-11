#include "animation.h"
#include "foundation/math_utils.h"
#include <cmath>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

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
    glm::vec3 horizontal_velocity = math::project_to_horizontal(velocity);
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

    // Determine blend segment and calculate interpolation factor
    pose_type source_pose, target_pose;
    float segment_start, segment_end, t;

    if (phase < 0.25f) {
        // Segment 1: STEP_LEFT → NEUTRAL
        source_pose = pose_type::STEP_LEFT;
        target_pose = pose_type::NEUTRAL;
        segment_start = 0.0f;
        segment_end = 0.25f;
    } else if (phase < 0.5f) {
        // Segment 2: NEUTRAL → STEP_RIGHT
        source_pose = pose_type::NEUTRAL;
        target_pose = pose_type::STEP_RIGHT;
        segment_start = 0.25f;
        segment_end = 0.5f;
    } else if (phase < 0.75f) {
        // Segment 3: STEP_RIGHT → NEUTRAL
        source_pose = pose_type::STEP_RIGHT;
        target_pose = pose_type::NEUTRAL;
        segment_start = 0.5f;
        segment_end = 0.75f;
    } else {
        // Segment 4: NEUTRAL → STEP_LEFT (wrap continuity)
        source_pose = pose_type::NEUTRAL;
        target_pose = pose_type::STEP_LEFT;
        segment_start = 0.75f;
        segment_end = 1.0f;
    }

    // Calculate blend factor within current segment
    t = (phase - segment_start) / (segment_end - segment_start);

    // Get keyframe data for blending
    keyframe source_kf = get_keyframe_data(source_pose);
    keyframe target_kf = get_keyframe_data(target_pose);

    // Store root transform (set by game_world)
    glm::mat4 root_transform = skel.joints[0].local_transform;

    // Helper lambda to apply blended quaternion to joint
    auto apply_blended_joint = [&](int joint_idx, const glm::quat& blended_rotation) {
        // Extract T-pose position (T-pose rotations are identity)
        glm::vec3 t_pose_pos = glm::vec3(skel.joints[joint_idx].local_transform[3]);
        // Build new transform: translation * rotation
        skel.joints[joint_idx].local_transform =
            glm::translate(glm::mat4(1.0f), t_pose_pos) * glm::mat4_cast(blended_rotation);
    };

    // Blend and apply all 8 joints using slerp
    apply_blended_joint(joint_index::LEFT_SHOULDER,
                        glm::slerp(source_kf.left_shoulder, target_kf.left_shoulder, t));
    apply_blended_joint(joint_index::LEFT_ELBOW,
                        glm::slerp(source_kf.left_elbow, target_kf.left_elbow, t));
    apply_blended_joint(joint_index::RIGHT_SHOULDER,
                        glm::slerp(source_kf.right_shoulder, target_kf.right_shoulder, t));
    apply_blended_joint(joint_index::RIGHT_ELBOW,
                        glm::slerp(source_kf.right_elbow, target_kf.right_elbow, t));
    apply_blended_joint(joint_index::LEFT_HIP,
                        glm::slerp(source_kf.left_hip, target_kf.left_hip, t));
    apply_blended_joint(joint_index::LEFT_KNEE,
                        glm::slerp(source_kf.left_knee, target_kf.left_knee, t));
    apply_blended_joint(joint_index::RIGHT_HIP,
                        glm::slerp(source_kf.right_hip, target_kf.right_hip, t));
    apply_blended_joint(joint_index::RIGHT_KNEE,
                        glm::slerp(source_kf.right_knee, target_kf.right_knee, t));

    // Restore root transform
    skel.joints[0].local_transform = root_transform;

    // Update global transforms after applying blended poses
    update_global_transforms(skel);

    // Update GUI state to reflect target pose of current segment
    current_automatic_pose = target_pose;

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

        // Ensure current rotation is in same hemisphere as previous (shortest path)
        if (glm::dot(current_parent_rot, prev_parent_rot) < 0.0f) {
            current_parent_rot = -current_parent_rot;
        }

        // Detect parent rotation change (parent's motion that child should follow)
        glm::quat delta_rot = current_parent_rot * glm::inverse(prev_parent_rot);
        float angle = glm::angle(delta_rot);

        // Inject parent's rotational motion as velocity into child's spring
        if (angle > 0.001f && dt > 0.0f) {
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
