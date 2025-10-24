#pragma once
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>

/// Mathematical utility functions for common vector operations
namespace math {

/// PHYSICAL: World up axis in project coordinate system
/// Convention: Y-up, Z-forward, X-right (OpenGL/GLM standard)
/// Alternative systems: Some engines use Z-up (Unreal) or Y-forward (Unity legacy)
/// Components: (0, 1, 0) → X=0, Y=1 (up), Z=0
/// Used throughout: Gravity direction, ground normal checks, vertical projections
inline constexpr glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f); // dimensionless (unit vector)

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
    float two_pi = glm::two_pi<float>();
    float wrapped = std::fmod(angle + glm::pi<float>(), two_pi);
    if (wrapped < 0.0f) {
        wrapped += two_pi;
    }
    return wrapped - glm::pi<float>();
}

/// Wrap angle in degrees to the range [-180, 180].
inline float wrap_angle_degrees(float angle) {
    float wrapped = std::fmod(angle + 180.0f, 360.0f);
    if (wrapped < 0.0f) {
        wrapped += 360.0f;
    }
    return wrapped - 180.0f;
}

/// Calculate the shortest difference between two angles in radians.
inline float angle_difference_radians(float target, float current) {
    float delta = target - current;
    return wrap_angle_radians(delta);
}

/// Safely normalize a vector, returning a fallback if the length is close to zero.
inline glm::vec3 safe_normalize(const glm::vec3& v, const glm::vec3& fallback) {
    float len = glm::length(v);

    // TUNED: Epsilon threshold for zero-vector detection
    // Purpose: Prevent division by zero in normalization (numerical stability)
    // Scale: 0.0001 = 0.1mm threshold (conservative for meter-scale world)
    // Could be smaller (1e-6) but current value is safe across typical scales
    // Context-dependent: Implicitly assumes meter-scale vectors (position/velocity)
    if (len > 0.0001f) { // meters (or unitless, context-dependent)
        return v / len;
    }
    return fallback;
}

} // namespace math
