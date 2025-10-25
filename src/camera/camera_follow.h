#pragma once

#include "foundation/param_meta.h"
#include <glm/glm.hpp>

enum class camera_mode { FREE_ORBIT, LOCK_TO_ORIENTATION };

/// Follow camera component - maintains spherical offset from target
struct camera_follow {
    // State
    camera_mode mode = camera_mode::FREE_ORBIT;
    float distance = 5.0f;
    float latitude = 15.0f;  // degrees
    float longitude = 180.0f; // degrees (behind target, aligned with +Z forward)
    float height_offset = 1.5f;

    // Limits
    float min_distance = 1.5f;
    float max_distance = 15.0f;
    float min_latitude = -85.0f;
    float max_latitude = 85.0f;

    // Sensitivity
    float orbit_sensitivity = 0.5f; // degrees per pixel
    float zoom_sensitivity = 0.5f;  // distance per scroll unit

    // Parameter metadata for GUI presentation
    static inline param_meta make_distance_meta(float min, float max) {
        // GUI requires strict inequality, but runtime allows min == max
        // Add epsilon to max if equal to prevent assertion
        if (min >= max) {
            max = min + 0.01f;
        }
        return {"Distance", "m", min, max};
    }

    static constexpr param_meta height_offset_meta = {"Height Offset", "m", 0.0f, 3.0f};

    static constexpr param_meta min_distance_meta = {"Min Distance", "m", 0.5f, 10.0f};

    static constexpr param_meta max_distance_meta = {"Max Distance", "m", 5.0f, 30.0f};

    /// Update camera orientation (Maya-style orbit)
    /// @param delta_x Horizontal rotation delta (pixels)
    /// @param delta_y Vertical rotation delta (pixels)
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

    /// Compute camera eye position locked behind a direction vector
    /// @param target_position Target world position to follow
    /// @param forward_dir Direction to lock behind (will be normalized)
    /// @param distance Distance from target
    /// @param height_offset Height offset above target
    /// @return Eye position in world space
    static glm::vec3 compute_locked_eye_position(const glm::vec3& target_position,
                                                 const glm::vec3& forward_dir, float distance,
                                                 float height_offset);
};
