#pragma once

#include <glm/glm.hpp>

/// Follow camera component - maintains spherical offset from target
struct camera_follow {
    // State
    float distance = 5.0f;
    float latitude = 15.0f; // degrees
    float longitude = 0.0f; // degrees
    float height_offset = 1.5f;

    // Limits
    float min_distance = 1.5f;
    float max_distance = 15.0f;
    float min_latitude = -85.0f;
    float max_latitude = 85.0f;

    /// Update camera orientation (Maya-style orbit)
    /// @param delta_x Horizontal rotation delta (degrees)
    /// @param delta_y Vertical rotation delta (degrees)
    void orbit(float delta_x, float delta_y);

    /// Update camera distance from target
    /// @param delta Distance delta (negative = closer)
    void zoom(float delta);

    /// Compute camera eye position from target
    /// @param target_position Target world position to follow
    /// @return Eye position in world space
    glm::vec3 compute_eye_position(const glm::vec3& target_position) const;

    /// Compute camera look-at target from target position
    /// @param target_position Target world position to follow
    /// @return Look-at target in world space
    glm::vec3 compute_look_target(const glm::vec3& target_position) const;
};
