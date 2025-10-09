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
///   * Pitch (X-axis): raise/lower arm (up/down motion)
///   * Yaw (Y-axis):   swing arm forward/backward
///   * Roll (Z-axis):  twist along bone length (invisible on cylindrical debug geo)
/// - Legs (bones along -Y axis):
///   * Pitch (X-axis): swing leg forward/backward
///   * Yaw (Y-axis):   twist along bone length
///   * Roll (Z-axis):  spread leg in/out (abduction/adduction)

#include "keyframe.h"
#include "t_pose.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace character {

/// Create identity pose (all quaternions are identity rotations)
static keyframe create_identity_pose() {
    return keyframe{
        glm::quat(), // left_shoulder
        glm::quat(), // left_elbow
        glm::quat(), // right_shoulder
        glm::quat(), // right_elbow
        glm::quat(), // left_hip
        glm::quat(), // left_knee
        glm::quat(), // right_hip
        glm::quat()  // right_knee
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

void apply_pose(skeleton& skel, pose_type pose) {
    // Store root transform (set by game_world)
    glm::mat4 root_transform = skel.joints[0].local_transform;

    // Recreate T-pose to get clean baseline
    create_t_pose(skel);

    // Restore root transform
    skel.joints[0].local_transform = root_transform;

    // T-pose is the default; no modification needed
    if (pose == pose_type::T_POSE) {
        update_global_transforms(skel);
        return;
    }

    // Get keyframe based on pose type
    keyframe kf;
    switch (pose) {
    case pose_type::STEP_LEFT:
        kf = create_step_left_pose();
        break;
    case pose_type::NEUTRAL:
        kf = create_neutral_pose();
        break;
    case pose_type::STEP_RIGHT:
        kf = create_step_right_pose();
        break;
    default:
        kf = create_identity_pose();
        break;
    }

    // Apply keyframe quaternions to skeleton joints
    // Joint indices: 5=left_shoulder, 6=left_elbow, 8=right_shoulder, 9=right_elbow,
    //                11=left_hip, 12=left_knee, 14=right_hip, 15=right_knee

    // Helper lambda to apply quaternion to joint
    auto apply_joint = [&](int joint_idx, const glm::quat& rotation) {
        // Extract T-pose position (T-pose rotations are identity)
        glm::vec3 t_pose_pos = glm::vec3(skel.joints[joint_idx].local_transform[3]);
        // Build new transform: translation * rotation (position bone, then apply joint rotation)
        skel.joints[joint_idx].local_transform =
            glm::translate(glm::mat4(1.0f), t_pose_pos) * glm::mat4_cast(rotation);
    };

    apply_joint(5, kf.left_shoulder);
    apply_joint(6, kf.left_elbow);
    apply_joint(8, kf.right_shoulder);
    apply_joint(9, kf.right_elbow);
    apply_joint(11, kf.left_hip);
    apply_joint(12, kf.left_knee);
    apply_joint(14, kf.right_hip);
    apply_joint(15, kf.right_knee);

    // Propagate transforms through hierarchy
    update_global_transforms(skel);
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

    // Recreate T-pose to get clean baseline
    create_t_pose(skel);

    // Restore root transform
    skel.joints[0].local_transform = root_transform;

    // Convert Euler angles (degrees) to quaternions
    // Uses GLM's built-in Euler angle conversion (same as hardcoded poses)
    auto to_quat = [](const glm::vec3& euler_degrees) {
        return glm::quat(glm::radians(euler_degrees));
    };

    // Helper lambda to apply quaternion to joint
    auto apply_joint = [&](int joint_idx, const glm::quat& rotation) {
        glm::vec3 t_pose_pos = glm::vec3(skel.joints[joint_idx].local_transform[3]);
        skel.joints[joint_idx].local_transform =
            glm::translate(glm::mat4(1.0f), t_pose_pos) * glm::mat4_cast(rotation);
    };

    // Apply custom quaternions to all joints
    apply_joint(5, to_quat(left_shoulder_angles));
    apply_joint(6, to_quat(left_elbow_angles));
    apply_joint(8, to_quat(right_shoulder_angles));
    apply_joint(9, to_quat(right_elbow_angles));
    apply_joint(11, to_quat(left_hip_angles));
    apply_joint(12, to_quat(left_knee_angles));
    apply_joint(14, to_quat(right_hip_angles));
    apply_joint(15, to_quat(right_knee_angles));

    // Propagate transforms through hierarchy
    update_global_transforms(skel);
}

} // namespace character
