/// @file skeleton.h
/// Hierarchical skeleton data structures for procedural animation.
/// Transform conventions:
/// - Units: meters; Up-axis: Y (project default).
/// - Column-major glm; model = parent.model * local.
/// - Root at index 0 uses NO_PARENT.

#pragma once

#include <glm/mat4x4.hpp>
#include <vector>

namespace character {

constexpr int NO_PARENT = -1;

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

struct joint {
    glm::mat4 model_transform; // Computed global transform (parent.model * local)
    glm::mat4 local_transform; // Local transform relative to parent
    int parent_index;          // Index of parent joint, or NO_PARENT for root
    const char* name;          // Joint name for debugging
};

struct skeleton {
    std::vector<joint> joints; // Hierarchical joint array, root at index 0
};

/// Compute model (global) transforms for the entire skeleton from local transforms.
/// Assumes parent-before-child ordering in the joints array and that joint 0 is the root.
void update_global_transforms(skeleton& skel);

} // namespace character