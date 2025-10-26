#pragma once
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include "foundation/debug_assert.h"

/// Mathematical utility functions for common vector operations
namespace math {

/// PHYSICAL: Earth gravity constant (standard acceleration due to gravity)
/// Used for force calculations (F = m * g) and g-force normalization
constexpr float GRAVITY = 9.8f; // m/s²

/// PHYSICAL: World up axis in project coordinate system
/// Convention: Y-up, Z-forward, +X-right (right-handed, standard OpenGL)
/// Alternative systems: Some engines use Z-up (Unreal, right-handed) or left-handed variants
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

/// Convert yaw angle to right direction vector (positive X in world space).
/// Uses project's Y-up, Z-forward coordinate system convention.
inline glm::vec3 yaw_to_right(float yaw) {
    return glm::vec3(std::cos(yaw), 0.0f, -std::sin(yaw));
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

/// Calculate slip angle between velocity vector and forward direction vector.
/// Slip angle is the angle between the direction of motion (velocity) and the
/// direction the object is facing (forward). Used for drift detection and vehicle dynamics.
///
/// @param horizontal_velocity Velocity vector projected to horizontal plane (XZ)
/// @param forward Unit vector indicating forward direction (horizontal)
/// @return Signed angle in radians [-π, π]:
///         - Positive: velocity points right of forward
///         - Negative: velocity points left of forward
///         - Zero: moving straight or stationary
///         - ≈±π: moving backward relative to forward
inline float calculate_slip_angle(const glm::vec3& horizontal_velocity, const glm::vec3& forward) {
    // Validate preconditions in debug builds
    FL_PRECONDITION(glm::abs(glm::length(forward) - 1.0f) < 0.01f, "forward must be unit length");
    FL_PRECONDITION(glm::abs(horizontal_velocity.y) < 0.01f,
                    "horizontal_velocity must be projected to XZ plane");
    FL_PRECONDITION(glm::abs(forward.y) < 0.01f,
                    "forward must be horizontal (Y component near zero)");

    // Zero-velocity edge case: no slip when stationary
    constexpr float VELOCITY_EPSILON = 0.0001f; // m/s (below perceptible threshold)
    if (glm::length(horizontal_velocity) < VELOCITY_EPSILON) {
        return 0.0f;
    }

    // Compute right vector perpendicular to forward (in horizontal plane)
    // cross(UP, forward) yields +X when forward is +Z (matches +X-right coordinate system)
    glm::vec3 right = glm::normalize(glm::cross(UP, forward));

    // Project velocity onto local coordinate frame
    float forward_speed = glm::dot(horizontal_velocity, forward);
    float lateral_speed = glm::dot(horizontal_velocity, right);

    // Calculate slip angle via atan2
    // atan2(y, x) returns angle from +X axis, where:
    //   - lateral_speed is the "Y" component (perpendicular)
    //   - forward_speed is the "X" component (parallel)
    return std::atan2(lateral_speed, forward_speed);
}

/// Calculate lateral g-force from speed and angular velocity.
/// Returns centripetal acceleration as a multiple of Earth gravity (9.8 m/s²).
///
/// @param speed Horizontal speed in m/s (magnitude, always non-negative)
/// @param angular_velocity Rotational velocity in rad/s (negative = right turn, positive = left
/// turn)
/// @return Lateral g-force multiplier (dimensionless):
///         - Negative: centripetal acceleration points right (right turn)
///         - Positive: centripetal acceleration points left (left turn)
///         - Zero: moving straight or stationary
inline float calculate_lateral_g_force(float speed, float angular_velocity) {
    FL_PRECONDITION(speed >= 0.0f, "speed must be non-negative (magnitude)");
    FL_PRECONDITION(std::isfinite(speed), "speed must be finite");
    FL_PRECONDITION(std::isfinite(angular_velocity), "angular_velocity must be finite");

    // Zero-velocity edge case: no centripetal force when stationary
    constexpr float SPEED_EPSILON = 0.0001f; // m/s (below perceptible threshold)
    if (speed < SPEED_EPSILON) {
        return 0.0f;
    }

    // Centripetal acceleration: a = v * ω
    // Derivation: For circular motion, a = v²/r and ω = v/r, therefore a = v*ω
    // Sign: negative ω (right turn) × positive v = negative a (rightward, toward center)
    float lateral_accel = speed * angular_velocity; // m/s²

    // Convert to g-force (dimensionless multiplier of Earth gravity)
    return lateral_accel / GRAVITY;
}

} // namespace math
