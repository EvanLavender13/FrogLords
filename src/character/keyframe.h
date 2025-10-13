/// @file keyframe.h
/// Hardcoded skeletal poses for quaternion-based keyframe validation.
/// Minimum viable structure: 8 joints, 4 poses, instant switching.

#pragma once

#include <glm/gtc/quaternion.hpp>
#include "skeleton.h"

namespace character {

/// Hardcoded pose types for manual selection
enum class pose_type {
    T_POSE,
    WALK_REACH_LEFT,
    WALK_PASS_RIGHT,
    WALK_REACH_RIGHT,
    WALK_PASS_LEFT,
    RUN_REACH_LEFT,
    RUN_PASS_RIGHT,
    RUN_REACH_RIGHT,
    RUN_PASS_LEFT
};

/// Keyframe data: quaternion rotations for minimum joint set (8 joints)
struct keyframe {
    glm::quat left_shoulder;
    glm::quat left_elbow;
    glm::quat right_shoulder;
    glm::quat right_elbow;
    glm::quat left_hip;
    glm::quat left_knee;
    glm::quat right_hip;
    glm::quat right_knee;
};

/// Apply hardcoded pose to skeleton. Composes quaternions with T-pose baseline.
/// Joint indices: shoulders(5,8), elbows(6,9), hips(11,14), knees(12,15)
void apply_pose(skeleton& skel, pose_type pose);

/// Get raw keyframe data for a pose without applying to skeleton.
/// Returns quaternions for all 8 joints (shoulders, elbows, hips, knees).
keyframe get_keyframe_data(pose_type pose);

/// Apply custom Euler angles (degrees) to skeleton joints for real-time tuning.
/// Overrides hardcoded pose with user-specified rotations.
/// @param *_angles vec3 format: (X-rotation, Y-rotation, Z-rotation) in degrees
///                 Rotations applied in order: Y -> X -> Z (yaw -> pitch -> roll)
///                 See keyframe.cpp header for bone-local axis meanings per joint type
void apply_pose_with_overrides(skeleton& skel, pose_type pose,
                               const glm::vec3& left_shoulder_angles,
                               const glm::vec3& left_elbow_angles,
                               const glm::vec3& right_shoulder_angles,
                               const glm::vec3& right_elbow_angles,
                               const glm::vec3& left_hip_angles, const glm::vec3& left_knee_angles,
                               const glm::vec3& right_hip_angles,
                               const glm::vec3& right_knee_angles);

} // namespace character
