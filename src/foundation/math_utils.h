#pragma once
#include <glm/vec3.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

/// Mathematical utility functions for common vector operations
namespace math {

/// World up axis in project coordinate system (Y-up, Z-forward).
inline constexpr glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);

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

/// Wrap angle in radians to the range [-π, π].
inline float wrap_angle_radians(float angle) {
    while (angle > glm::pi<float>())
        angle -= 2.0f * glm::pi<float>();
    while (angle < -glm::pi<float>())
        angle += 2.0f * glm::pi<float>();
    return angle;
}

/// Wrap angle in degrees to the range [-180, 180].
inline float wrap_angle_degrees(float angle) {
    while (angle > 180.0f)
        angle -= 360.0f;
    while (angle < -180.0f)
        angle += 360.0f;
    return angle;
}

/// Calculate the shortest difference between two angles in radians.
inline float angle_difference_radians(float target, float current) {
    float delta = target - current;
    return wrap_angle_radians(delta);
}

} // namespace math
