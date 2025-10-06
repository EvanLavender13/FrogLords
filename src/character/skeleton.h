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

struct joint {
    glm::mat4 model_transform; // Computed global transform (parent.model * local)
    glm::mat4 local_transform; // Local transform relative to parent
    int parent_index;          // Index of parent joint, or NO_PARENT for root
    const char* name;          // Joint name for debugging
};

struct skeleton {
    std::vector<joint> joints; // Hierarchical joint array, root at index 0
};

} // namespace character