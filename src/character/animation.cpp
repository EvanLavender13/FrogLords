#include "animation.h"
#include "foundation/math_utils.h"
#include "skeleton.h"
#include <cmath>
#include <glm/gtc/constants.hpp>
#include <glm/common.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace character {

// Forward declaration
static void update_secondary_motion(skeleton& skel, secondary_motion_state& state, float dt);

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

void animation_state::update_skeletal_animation(skeleton& skel, float phase, float walk_factor,
                                                pose_type manual_override_pose,
                                                bool use_manual_override, float dt) {
    if (use_manual_override) {
        apply_pose(skel, manual_override_pose);
        return;
    }

    // Use phase directly from locomotion (already calculated correctly with blended stride)

    // Determine blend segment and calculate interpolation factor
    pose_type walk_source_pose;
    pose_type walk_target_pose;
    pose_type run_source_pose;
    pose_type run_target_pose;
    float segment_start;
    float segment_end;
    float t;

    if (phase < 0.25f) {
        // Segment 1: REACH_L → PASS_R
        walk_source_pose = pose_type::WALK_REACH_LEFT;
        walk_target_pose = pose_type::WALK_PASS_RIGHT;
        run_source_pose = pose_type::RUN_REACH_LEFT;
        run_target_pose = pose_type::RUN_PASS_RIGHT;
        segment_start = 0.0f;
        segment_end = 0.25f;
    } else if (phase < 0.5f) {
        // Segment 2: PASS_R → REACH_R
        walk_source_pose = pose_type::WALK_PASS_RIGHT;
        walk_target_pose = pose_type::WALK_REACH_RIGHT;
        run_source_pose = pose_type::RUN_PASS_RIGHT;
        run_target_pose = pose_type::RUN_REACH_RIGHT;
        segment_start = 0.25f;
        segment_end = 0.5f;
    } else if (phase < 0.75f) {
        // Segment 3: REACH_R → PASS_L
        walk_source_pose = pose_type::WALK_REACH_RIGHT;
        walk_target_pose = pose_type::WALK_PASS_LEFT;
        run_source_pose = pose_type::RUN_REACH_RIGHT;
        run_target_pose = pose_type::RUN_PASS_LEFT;
        segment_start = 0.5f;
        segment_end = 0.75f;
    } else {
        // Segment 4: PASS_L → REACH_L (wrap continuity)
        walk_source_pose = pose_type::WALK_PASS_LEFT;
        walk_target_pose = pose_type::WALK_REACH_LEFT;
        run_source_pose = pose_type::RUN_PASS_LEFT;
        run_target_pose = pose_type::RUN_REACH_LEFT;
        segment_start = 0.75f;
        segment_end = 1.0f;
    }

    // Calculate blend factor within current segment
    t = (phase - segment_start) / (segment_end - segment_start);

    // Fetch keyframe data for blending
    keyframe walk_source_kf = get_keyframe_data(walk_source_pose);
    keyframe walk_target_kf = get_keyframe_data(walk_target_pose);
    keyframe run_source_kf = get_keyframe_data(run_source_pose);
    keyframe run_target_kf = get_keyframe_data(run_target_pose);

    // Get contact weight for amplitude suppression (1.0 grounded, 0.0 airborne)
    float contact_weight = contact_weight_spring.get_position();
    float run_weight = glm::clamp(1.0f - walk_factor, 0.0f, 1.0f);

    // T_POSE uses identity quaternion (no rotation)
    glm::quat t_pose_rotation(1.0f, 0.0f, 0.0f, 0.0f);

    // Store root transform (set by game_world)
    glm::mat4 root_transform = skel.joints[0].local_transform;

    auto blend_channel = [&](const glm::quat& walk_source, const glm::quat& walk_target,
                             const glm::quat& run_source, const glm::quat& run_target) {
        glm::quat walk_blend = glm::slerp(walk_source, walk_target, t);
        glm::quat run_blend = glm::slerp(run_source, run_target, t);
        glm::quat gait_blend = glm::slerp(walk_blend, run_blend, run_weight);
        return glm::slerp(t_pose_rotation, gait_blend, contact_weight);
    };

    // Blend and apply all 8 joints using bilinear slerp
    set_joint_rotation(skel, joint_index::LEFT_SHOULDER,
                       blend_channel(walk_source_kf.left_shoulder, walk_target_kf.left_shoulder,
                                     run_source_kf.left_shoulder, run_target_kf.left_shoulder));
    set_joint_rotation(skel, joint_index::LEFT_ELBOW,
                       blend_channel(walk_source_kf.left_elbow, walk_target_kf.left_elbow,
                                     run_source_kf.left_elbow, run_target_kf.left_elbow));
    set_joint_rotation(skel, joint_index::RIGHT_SHOULDER,
                       blend_channel(walk_source_kf.right_shoulder, walk_target_kf.right_shoulder,
                                     run_source_kf.right_shoulder, run_target_kf.right_shoulder));
    set_joint_rotation(skel, joint_index::RIGHT_ELBOW,
                       blend_channel(walk_source_kf.right_elbow, walk_target_kf.right_elbow,
                                     run_source_kf.right_elbow, run_target_kf.right_elbow));
    set_joint_rotation(skel, joint_index::LEFT_HIP,
                       blend_channel(walk_source_kf.left_hip, walk_target_kf.left_hip,
                                     run_source_kf.left_hip, run_target_kf.left_hip));
    set_joint_rotation(skel, joint_index::LEFT_KNEE,
                       blend_channel(walk_source_kf.left_knee, walk_target_kf.left_knee,
                                     run_source_kf.left_knee, run_target_kf.left_knee));
    set_joint_rotation(skel, joint_index::RIGHT_HIP,
                       blend_channel(walk_source_kf.right_hip, walk_target_kf.right_hip,
                                     run_source_kf.right_hip, run_target_kf.right_hip));
    set_joint_rotation(skel, joint_index::RIGHT_KNEE,
                       blend_channel(walk_source_kf.right_knee, walk_target_kf.right_knee,
                                     run_source_kf.right_knee, run_target_kf.right_knee));

    // Restore root transform
    skel.joints[0].local_transform = root_transform;

    // Update GUI state to reflect dominant gait pose for current segment
    pose_type display_pose = run_weight > 0.5f ? run_target_pose : walk_target_pose;
    current_automatic_pose = display_pose;
}

/// Apply per-joint spring-damper lag to create natural follow-through.
/// Child joints lag behind their parent's motion, creating visible wobble.
void animation_state::update_secondary_motion(skeleton& skel, float dt) {
    auto update_spring = [&](int parent_idx, int joint_idx, float& offset, float& velocity,
                             glm::quat& prev_parent_rot, const glm::vec3& rotation_axis,
                             float min_limit, float max_limit) {
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

            // Use projected rotation component (preserves sign for direction)
            float effective_angle = angle * axis_component;

            float angular_velocity_change = effective_angle / dt;

            // Add parent's angular velocity to child's spring (child follows parent)
            velocity += angular_velocity_change * secondary_motion.response_scale;
        }

        // Spring toward zero offset (child catches up to parent's motion)
        float spring_force = secondary_motion.stiffness * (0.0f - offset);
        float damping =
            critical_damping(secondary_motion.stiffness) * secondary_motion.damping_ratio;
        float damping_force = damping * velocity;
        float acceleration = spring_force - damping_force;

        // Integrate
        velocity += acceleration * dt;
        offset += velocity * dt;

        // Clamp spring offset to reasonable wobble range  
        offset = glm::clamp(offset, min_limit, max_limit);

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
    // Elbow hinge: bend axis = math::UP (Y+)
    update_spring(joint_index::LEFT_SHOULDER, joint_index::LEFT_ELBOW,
                  secondary_motion.left_elbow_offset, secondary_motion.left_elbow_velocity,
                  secondary_motion.prev_left_shoulder, math::UP,
                  secondary_motion.left_elbow_min_offset, secondary_motion.left_elbow_max_offset);
    // Elbow hinge: bend axis = math::UP (Y+)
    update_spring(joint_index::RIGHT_SHOULDER, joint_index::RIGHT_ELBOW,
                  secondary_motion.right_elbow_offset, secondary_motion.right_elbow_velocity,
                  secondary_motion.prev_right_shoulder, math::UP,
                  secondary_motion.right_elbow_min_offset, secondary_motion.right_elbow_max_offset);
    // Knee hinge: bend axis = X+ (side-to-side)
    update_spring(joint_index::LEFT_HIP, joint_index::LEFT_KNEE, secondary_motion.left_knee_offset,
                  secondary_motion.left_knee_velocity, secondary_motion.prev_left_hip,
                  glm::vec3(1, 0, 0), secondary_motion.left_knee_min_offset,
                  secondary_motion.left_knee_max_offset);
    // Knee hinge: bend axis = X+ (side-to-side)
    update_spring(joint_index::RIGHT_HIP, joint_index::RIGHT_KNEE,
                  secondary_motion.right_knee_offset, secondary_motion.right_knee_velocity,
                  secondary_motion.prev_right_hip, glm::vec3(-1, 0, 0),
                  secondary_motion.right_knee_min_offset, secondary_motion.right_knee_max_offset);
}

} // namespace character
