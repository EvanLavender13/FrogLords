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
#include "t_pose.h"
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include <glm/gtc/type_ptr.hpp>

namespace character {

// Joint indices in T-pose skeleton (must match order in t_pose.cpp)
namespace joint_index {
constexpr int ROOT = 0;
constexpr int SPINE_LOWER = 1;
constexpr int SPINE_UPPER = 2;
constexpr int NECK = 3;
constexpr int HEAD = 4;
constexpr int LEFT_SHOULDER = 5;
constexpr int LEFT_ELBOW = 6;
constexpr int LEFT_WRIST = 7;
constexpr int RIGHT_SHOULDER = 8;
constexpr int RIGHT_ELBOW = 9;
constexpr int RIGHT_WRIST = 10;
constexpr int LEFT_HIP = 11;
constexpr int LEFT_KNEE = 12;
constexpr int LEFT_ANKLE = 13;
constexpr int RIGHT_HIP = 14;
constexpr int RIGHT_KNEE = 15;
constexpr int RIGHT_ANKLE = 16;
} // namespace joint_index

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

/// Create STEP_LEFT pose: left leg forward, right arm forward (counter-rotation)
static keyframe create_step_left_pose() {
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

/// Create NEUTRAL pose: transitional stance between steps
static keyframe create_neutral_pose() {
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

/// Create STEP_RIGHT pose: mirror of STEP_LEFT (swap left/right joints)
static keyframe create_step_right_pose() {
    keyframe step_left = create_step_left_pose();
    return keyframe{
        glm::quat(glm::radians(
            glm::vec3(0.0f, 45.0f, 90.0f))), // left_shoulder (swing back via Y-rotation)
        glm::quat(glm::radians(glm::vec3(0.0f, 15.0f, 0.0f))), // left_elbow (straight)
        glm::quat(glm::radians(
            glm::vec3(0.0f, 45.0f, -90.0f))), // right_shoulder (swing forward via Y-rotation)
        glm::quat(glm::radians(glm::vec3(0.0f, -15.0f, 0.0f))), // right_elbow (slight bend)
        step_left.right_hip,                                    // left_hip <- right_hip
        step_left.right_knee,                                   // left_knee <- right_knee
        step_left.left_hip,                                     // right_hip <- left_hip
        step_left.left_knee                                     // right_knee <- left_knee
    };
}

keyframe get_keyframe_data(pose_type pose) {
    switch (pose) {
    case pose_type::T_POSE:
        return create_identity_pose();
    case pose_type::STEP_LEFT:
        return create_step_left_pose();
    case pose_type::NEUTRAL:
        return create_neutral_pose();
    case pose_type::STEP_RIGHT:
        return create_step_right_pose();
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

    // Helper lambda to apply quaternion to joint
    auto apply_joint = [&](int joint_idx, const glm::quat& rotation) {
        // Extract T-pose position (T-pose rotations are identity)
        glm::vec3 t_pose_pos = glm::vec3(skel.joints[joint_idx].local_transform[3]);
        // Build new transform: translation * rotation (position bone, then apply joint rotation)
        skel.joints[joint_idx].local_transform =
            glm::translate(glm::mat4(1.0f), t_pose_pos) * glm::mat4_cast(rotation);
    };

    apply_joint(joint_index::LEFT_SHOULDER, kf.left_shoulder);
    apply_joint(joint_index::LEFT_ELBOW, kf.left_elbow);
    apply_joint(joint_index::RIGHT_SHOULDER, kf.right_shoulder);
    apply_joint(joint_index::RIGHT_ELBOW, kf.right_elbow);
    apply_joint(joint_index::LEFT_HIP, kf.left_hip);
    apply_joint(joint_index::LEFT_KNEE, kf.left_knee);
    apply_joint(joint_index::RIGHT_HIP, kf.right_hip);
    apply_joint(joint_index::RIGHT_KNEE, kf.right_knee);
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
    case pose_type::STEP_LEFT:
        base_kf = create_step_left_pose();
        break;
    case pose_type::NEUTRAL:
        base_kf = create_neutral_pose();
        break;
    case pose_type::STEP_RIGHT:
        base_kf = create_step_right_pose();
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

    // Helper lambda to apply composed quaternion to joint
    auto apply_joint = [&](int joint_idx, const glm::quat& rotation) {
        glm::vec3 t_pose_pos = glm::vec3(skel.joints[joint_idx].local_transform[3]);
        skel.joints[joint_idx].local_transform =
            glm::translate(glm::mat4(1.0f), t_pose_pos) * glm::mat4_cast(rotation);
    };

    // Apply composed rotations (base pose + user overrides) to all joints
    apply_joint(joint_index::LEFT_SHOULDER,
                compose_rotation(base_kf.left_shoulder, to_quat(left_shoulder_angles)));
    apply_joint(joint_index::LEFT_ELBOW,
                compose_rotation(base_kf.left_elbow, to_quat(left_elbow_angles)));
    apply_joint(joint_index::RIGHT_SHOULDER,
                compose_rotation(base_kf.right_shoulder, to_quat(right_shoulder_angles)));
    apply_joint(joint_index::RIGHT_ELBOW,
                compose_rotation(base_kf.right_elbow, to_quat(right_elbow_angles)));
    apply_joint(joint_index::LEFT_HIP,
                compose_rotation(base_kf.left_hip, to_quat(left_hip_angles)));
    apply_joint(joint_index::LEFT_KNEE,
                compose_rotation(base_kf.left_knee, to_quat(left_knee_angles)));
    apply_joint(joint_index::RIGHT_HIP,
                compose_rotation(base_kf.right_hip, to_quat(right_hip_angles)));
    apply_joint(joint_index::RIGHT_KNEE,
                compose_rotation(base_kf.right_knee, to_quat(right_knee_angles)));
}

} // namespace character
