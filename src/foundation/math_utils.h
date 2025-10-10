#pragma once
#include <glm/vec3.hpp>
#include <cmath>

/// Mathematical utility functions for common vector operations
namespace math {

/// Project a 3D vector onto the horizontal (XZ) plane by zeroing the Y component.
/// Useful for extracting horizontal velocity/acceleration while ignoring vertical motion.
inline glm::vec3 project_to_horizontal(const glm::vec3& v) {
    return glm::vec3(v.x, 0.0f, v.z);
}

/// Convert yaw angle to forward direction vector (positive Z in world space).
/// Uses project's Y-up, Z-forward coordinate system convention.
inline glm::vec3 yaw_to_forward(float yaw) {
    return glm::vec3(std::sin(yaw), 0.0f, std::cos(yaw));
}

/// Convert yaw angle to right direction vector (negative X in world space).
/// Uses project's Y-up, Z-forward coordinate system convention.
inline glm::vec3 yaw_to_right(float yaw) {
    return glm::vec3(-std::cos(yaw), 0.0f, std::sin(yaw));
}

} // namespace math
