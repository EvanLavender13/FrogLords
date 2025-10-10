#pragma once
#include <glm/vec3.hpp>

/// Mathematical utility functions for common vector operations
namespace math {

/// Project a 3D vector onto the horizontal (XZ) plane by zeroing the Y component.
/// Useful for extracting horizontal velocity/acceleration while ignoring vertical motion.
inline glm::vec3 project_to_horizontal(const glm::vec3& v) {
    return glm::vec3(v.x, 0.0f, v.z);
}

} // namespace math
