/// @file keyframe.cpp
/// Implementation of hardcoded skeletal poses for keyframe validation.
///
/// Bone Orientation Reference (T-pose baseline):
/// - Left arm bones:  point along -X axis (leftward)
/// - Right arm bones: point along +X axis (rightward)
/// - Leg bones:       point along -Y axis (downward)
/// - Spine bones:     point along +Y axis (upward)
///
/// Euler Angle Convention (standard: Pitch=X, Yaw=Y, Roll=Z):
/// Applied in rotation order: Y -> X -> Z (yaw, pitch, roll)
///
/// Effect per joint type:
/// - Arms (bones along ±X axis):
///   * Pitch (X-axis): twist along bone length (invisible on cylindrical debug geo)
///   * Yaw (Y-axis):   swing arm forward/backward
///   * Roll (Z-axis):  raise/lower arm (up/down motion)
/// - Legs (bones along -Y axis):
///   * Pitch (X-axis): swing leg forward/backward
///   * Yaw (Y-axis):   twist along bone length
///   * Roll (Z-axis):  spread leg in/out (abduction/adduction)

#include "keyframe.h"
#include "skeleton.h"
#include "t_pose.h"
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include <glm/gtc/type_ptr.hpp>

namespace character {

// Cached T-pose positions (translations only, no rotations)
// Extracted from create_t_pose() to avoid recreating skeleton every frame
static const std::array<glm::vec3, 17> T_POSE_POSITIONS = {{
    glm::vec3(0.0f, 0.0f, 0.0f),     // 0: root
    glm::vec3(0.0f, 0.1f, 0.0f),     // 1: spine_lower
    glm::vec3(0.0f, 0.1f, 0.0f),     // 2: spine_upper
    glm::vec3(0.0f, 0.05f, 0.0f),    // 3: neck
    glm::vec3(0.0f, 0.05f, 0.0f),    // 4: head
    glm::vec3(-0.1f, 0.05f, 0.0f),   // 5: left_shoulder
    glm::vec3(-0.15f, 0.0f, 0.0f),   // 6: left_elbow
    glm::vec3(-0.15f, 0.0f, 0.0f),   // 7: left_wrist
    glm::vec3(0.1f, 0.05f, 0.0f),    // 8: right_shoulder
    glm::vec3(0.15f, 0.0f, 0.0f),    // 9: right_elbow
    glm::vec3(0.15f, 0.0f, 0.0f),    // 10: right_wrist
    glm::vec3(-0.05f, -0.05f, 0.0f), // 11: left_hip
    glm::vec3(0.0f, -0.2f, 0.0f),    // 12: left_knee
    glm::vec3(0.0f, -0.2f, 0.0f),    // 13: left_ankle
    glm::vec3(0.05f, -0.05f, 0.0f),  // 14: right_hip
    glm::vec3(0.0f, -0.2f, 0.0f),    // 15: right_knee
    glm::vec3(0.0f, -0.2f, 0.0f)     // 16: right_ankle
}};

/// Reset skeleton to T-pose baseline without recreating structure
/// Preserves root transform, resets all other joints to cached T-pose positions
static void reset_to_t_pose(skeleton& skel) {
    // Reset all joints to T-pose positions (identity rotations)
    for (size_t i = 0; i < skel.joints.size() && i < T_POSE_POSITIONS.size(); ++i) {
        skel.joints[i].local_transform = glm::translate(glm::mat4(1.0f), T_POSE_POSITIONS[i]);
    }
}

/// Mirrors a keyframe by swapping its left and right joint rotations and inverting
/// the appropriate rotation axes.
/// @note This uses Euler angle conversion, which can be risky (gimbal lock), but it
/// correctly replicates the manual mirroring logic by negating yaw (Y) and roll (Z)
/// for the swapped joints. This is necessary because the local coordinate
/// systems are not perfectly symmetrical for all rotation axes.
static keyframe mirror_keyframe(const keyframe& kf) {
    keyframe mirrored;

    auto mirror_quat = [](const glm::quat& q) {
        glm::vec3 euler = glm::degrees(glm::eulerAngles(q));
        euler.y = -euler.y;
        euler.z = -euler.z;
        return glm::quat(glm::radians(euler));
    };

    mirrored.left_shoulder = mirror_quat(kf.right_shoulder);
    mirrored.left_elbow = mirror_quat(kf.right_elbow);
    mirrored.right_shoulder = mirror_quat(kf.left_shoulder);
    mirrored.right_elbow = mirror_quat(kf.left_elbow);

    mirrored.left_hip = mirror_quat(kf.right_hip);
    mirrored.left_knee = mirror_quat(kf.right_knee);
    mirrored.right_hip = mirror_quat(kf.left_hip);
    mirrored.right_knee = mirror_quat(kf.left_knee);

    return mirrored;
}

/// Create identity pose (all quaternions are identity rotations)
static keyframe create_identity_pose() {
    // Identity quaternion = (w=1, x=0, y=0, z=0) represents no rotation
    // NOTE: glm::quat() default constructor creates (w=1, x=0, y=0, z=0) which is correct
    // But for clarity, we explicitly construct identity quaternions
    glm::quat identity(1.0f, 0.0f, 0.0f, 0.0f);
    return keyframe{
        identity, // left_shoulder
        identity, // left_elbow
        identity, // right_shoulder
        identity, // right_elbow
        identity, // left_hip
        identity, // left_knee
        identity, // right_hip
        identity  // right_knee
    };
}

/// Create WALK_REACH_LEFT pose: left leg forward, right arm forward (counter-rotation)
static keyframe create_walk_reach_left_pose() {
    return keyframe{
        glm::quat(glm::radians(
            glm::vec3(0.0f, -45.0f, 90.0f))), // left_shoulder (swing back via Y-rotation)
        glm::quat(glm::radians(glm::vec3(0.0f, 15.0f, 0.0f))), // left_elbow (straight)
        glm::quat(glm::radians(
            glm::vec3(0.0f, -45.0f, -90.0f))), // right_shoulder (swing forward via Y-rotation)
        glm::quat(glm::radians(glm::vec3(0.0f, -15.0f, 0.0f))), // right_elbow (slight bend)
        glm::quat(glm::radians(glm::vec3(-20.0f, 0.0f, 0.0f))), // left_hip (flex forward)
        glm::quat(glm::radians(glm::vec3(20.0f, 0.0f, 0.0f))),  // left_knee (bend)
        glm::quat(glm::radians(glm::vec3(20.0f, 0.0f, 0.0f))),  // right_hip (extend back)
        glm::quat(glm::radians(glm::vec3(20.0f, 0.0f, 0.0f)))   // right_knee (straight)
    };
}

/// Create WALK_PASS_RIGHT pose: transitional stance between steps (right leg passing)
static keyframe create_walk_pass_right_pose() {
    // Arms down at sides: rotate shoulders -90° (left) and +90° (right) around Y-axis
    return keyframe{
        glm::quat(glm::radians(glm::vec3(0.0f, 0.0f, 90.0f))),  // left_shoulder (rotate down)
        glm::quat(),                                            // left_elbow
        glm::quat(glm::radians(glm::vec3(0.0f, 0.0f, -90.0f))), // right_shoulder (rotate down)
        glm::quat(),                                            // right_elbow
        glm::quat(),                                            // left_hip
        glm::quat(),                                            // left_knee
        glm::quat(),                                            // right_hip
        glm::quat()                                             // right_knee
    };
}

/// Create WALK_REACH_RIGHT pose: mirror of WALK_REACH_LEFT (swap left/right joints)
static keyframe create_walk_reach_right_pose() {
    return mirror_keyframe(create_walk_reach_left_pose());
}

/// Create WALK_PASS_LEFT pose: mirror of WALK_PASS_RIGHT (duplicate for symmetry)
static keyframe create_walk_pass_left_pose() {
    return mirror_keyframe(create_walk_pass_right_pose());
}

/// Create RUN_REACH_LEFT pose: exaggerated forward stride for left leg with extended trail leg
static keyframe create_run_reach_left_pose() {
    return keyframe{
        glm::quat(glm::radians(
            glm::vec3(0.0f, -65.0f, 90.0f))), // left_shoulder (swing back with extra roll)
        glm::quat(glm::radians(glm::vec3(0.0f, 25.0f, 0.0f))),    // left_elbow (additional bend)
        glm::quat(glm::radians(glm::vec3(0.0f, -65.0f, -90.0f))), // right_shoulder (forward reach)
        glm::quat(glm::radians(glm::vec3(0.0f, -25.0f, 0.0f))),   // right_elbow (pronounced bend)
        glm::quat(glm::radians(glm::vec3(-75.0f, 0.0f, 0.0f))),   // left_hip (deep flex)
        glm::quat(glm::radians(glm::vec3(45.0f, 0.0f, 0.0f))), // left_knee (front leg compression)
        glm::quat(glm::radians(glm::vec3(45.0f, 0.0f, 0.0f))), // right_hip (drive leg extension)
        glm::quat(
            glm::radians(glm::vec3(15.0f, 0.0f, 0.0f))) // right_knee (nearly straight trail leg)
    };
}

/// Create RUN_PASS_RIGHT pose: right leg crossing center with elevated energy
static keyframe create_run_pass_right_pose() {
    return keyframe{
        glm::quat(glm::radians(
            glm::vec3(0.0f, -20.0f, 90.0f))), // left_shoulder (back swing with added roll)
        glm::quat(glm::radians(glm::vec3(0.0f, 10.0f, 0.0f))),    // left_elbow (moderate bend)
        glm::quat(glm::radians(glm::vec3(0.0f, -20.0f, -90.0f))), // right_shoulder (forward swing)
        glm::quat(glm::radians(glm::vec3(0.0f, -10.0f, 0.0f))),   // right_elbow (light bend)
        glm::quat(
            glm::radians(glm::vec3(-15.0f, 0.0f, 0.0f))), // left_hip (prepping to drive forward)
        glm::quat(glm::radians(glm::vec3(25.0f, 0.0f, 0.0f))), // left_knee (soft bend)
        glm::quat(glm::radians(
            glm::vec3(20.0f, 0.0f, 0.0f))), // right_hip (extension as leg passes under)
        glm::quat(glm::radians(glm::vec3(10.0f, 0.0f, 0.0f))) // right_knee (near straight)
    };
}

/// Create RUN_REACH_RIGHT pose: mirror of RUN_REACH_LEFT (swap left/right roles)
static keyframe create_run_reach_right_pose() {
    return mirror_keyframe(create_run_reach_left_pose());
}

/// Create RUN_PASS_LEFT pose: mirror of RUN_PASS_RIGHT with left leg crossing center
static keyframe create_run_pass_left_pose() {
    return mirror_keyframe(create_run_pass_right_pose());
}

keyframe get_keyframe_data(pose_type pose) {
    switch (pose) {
    case pose_type::T_POSE:
        return create_identity_pose();
    case pose_type::WALK_REACH_LEFT:
        return create_walk_reach_left_pose();
    case pose_type::WALK_PASS_RIGHT:
        return create_walk_pass_right_pose();
    case pose_type::WALK_REACH_RIGHT:
        return create_walk_reach_right_pose();
    case pose_type::WALK_PASS_LEFT:
        return create_walk_pass_left_pose();
    case pose_type::RUN_REACH_LEFT:
        return create_run_reach_left_pose();
    case pose_type::RUN_PASS_RIGHT:
        return create_run_pass_right_pose();
    case pose_type::RUN_REACH_RIGHT:
        return create_run_reach_right_pose();
    case pose_type::RUN_PASS_LEFT:
        return create_run_pass_left_pose();
    default:
        return create_identity_pose();
    }
}

void apply_pose(skeleton& skel, pose_type pose) {
    // Store root transform (set by game_world)
    glm::mat4 root_transform = skel.joints[0].local_transform;

    // Reset to T-pose baseline (efficient: uses cached positions)
    reset_to_t_pose(skel);

    // Restore root transform
    skel.joints[0].local_transform = root_transform;

    // Get keyframe based on pose type
    keyframe kf = get_keyframe_data(pose);

    // Apply keyframe quaternions to skeleton joints using named indices
    set_joint_rotation(skel, joint_index::LEFT_SHOULDER, kf.left_shoulder);
    set_joint_rotation(skel, joint_index::LEFT_ELBOW, kf.left_elbow);
    set_joint_rotation(skel, joint_index::RIGHT_SHOULDER, kf.right_shoulder);
    set_joint_rotation(skel, joint_index::RIGHT_ELBOW, kf.right_elbow);
    set_joint_rotation(skel, joint_index::LEFT_HIP, kf.left_hip);
    set_joint_rotation(skel, joint_index::LEFT_KNEE, kf.left_knee);
    set_joint_rotation(skel, joint_index::RIGHT_HIP, kf.right_hip);
    set_joint_rotation(skel, joint_index::RIGHT_KNEE, kf.right_knee);
}

void apply_pose_with_overrides(skeleton& skel, pose_type pose,
                               const glm::vec3& left_shoulder_angles,
                               const glm::vec3& left_elbow_angles,
                               const glm::vec3& right_shoulder_angles,
                               const glm::vec3& right_elbow_angles,
                               const glm::vec3& left_hip_angles, const glm::vec3& left_knee_angles,
                               const glm::vec3& right_hip_angles,
                               const glm::vec3& right_knee_angles) {
    // Store root transform (set by game_world)
    glm::mat4 root_transform = skel.joints[0].local_transform;

    // Reset to T-pose baseline (efficient: uses cached positions)
    reset_to_t_pose(skel);

    // Restore root transform
    skel.joints[0].local_transform = root_transform;

    // Get base pose keyframe
    keyframe base_kf;
    switch (pose) {
    case pose_type::T_POSE:
        base_kf = create_identity_pose();
        break;
    case pose_type::WALK_REACH_LEFT:
        base_kf = create_walk_reach_left_pose();
        break;
    case pose_type::WALK_PASS_RIGHT:
        base_kf = create_walk_pass_right_pose();
        break;
    case pose_type::WALK_REACH_RIGHT:
        base_kf = create_walk_reach_right_pose();
        break;
    case pose_type::WALK_PASS_LEFT:
        base_kf = create_walk_pass_left_pose();
        break;
    case pose_type::RUN_REACH_LEFT:
        base_kf = create_run_reach_left_pose();
        break;
    case pose_type::RUN_PASS_RIGHT:
        base_kf = create_run_pass_right_pose();
        break;
    case pose_type::RUN_REACH_RIGHT:
        base_kf = create_run_reach_right_pose();
        break;
    case pose_type::RUN_PASS_LEFT:
        base_kf = create_run_pass_left_pose();
        break;
    default:
        base_kf = create_identity_pose();
        break;
    }

    // Convert Euler angles (degrees) to quaternions
    auto to_quat = [](const glm::vec3& euler_degrees) {
        return glm::quat(glm::radians(euler_degrees));
    };

    // Compose base pose rotation with override rotation (multiplicative)
    auto compose_rotation = [](const glm::quat& base, const glm::quat& override_rot) {
        return override_rot * base;
    };

    // Apply composed rotations (base pose + user overrides) to all joints
    set_joint_rotation(skel, joint_index::LEFT_SHOULDER,
                       compose_rotation(base_kf.left_shoulder, to_quat(left_shoulder_angles)));
    set_joint_rotation(skel, joint_index::LEFT_ELBOW,
                       compose_rotation(base_kf.left_elbow, to_quat(left_elbow_angles)));
    set_joint_rotation(skel, joint_index::RIGHT_SHOULDER,
                       compose_rotation(base_kf.right_shoulder, to_quat(right_shoulder_angles)));
    set_joint_rotation(skel, joint_index::RIGHT_ELBOW,
                       compose_rotation(base_kf.right_elbow, to_quat(right_elbow_angles)));
    set_joint_rotation(skel, joint_index::LEFT_HIP,
                       compose_rotation(base_kf.left_hip, to_quat(left_hip_angles)));
    set_joint_rotation(skel, joint_index::LEFT_KNEE,
                       compose_rotation(base_kf.left_knee, to_quat(left_knee_angles)));
    set_joint_rotation(skel, joint_index::RIGHT_HIP,
                       compose_rotation(base_kf.right_hip, to_quat(right_hip_angles)));
    set_joint_rotation(skel, joint_index::RIGHT_KNEE,
                       compose_rotation(base_kf.right_knee, to_quat(right_knee_angles)));
}

} // namespace character