#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class camera_mode {
    orbit,   // Rotate around fixed center
    follow   // Follow character with free rotation
};

class camera {
public:
    camera(glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f),
           float distance = 8.0f,
           float latitude = 15.0f,
           float longitude = 0.0f);

    /// Generate view matrix from current eye position
    glm::mat4 get_view_matrix() const;

    /// Generate perspective projection matrix
    /// @param aspect_ratio Viewport width/height ratio
    glm::mat4 get_projection_matrix(float aspect_ratio) const;

    /// Orbit camera around center point (Maya-style)
    /// @param delta_x Horizontal rotation delta (screen space)
    /// @param delta_y Vertical rotation delta (screen space)
    void orbit(float delta_x, float delta_y);

    /// Zoom camera toward/away from center
    /// @param delta Distance delta (positive = closer)
    void zoom(float delta);

    /// Set field of view angle
    /// @param fov Field of view in degrees
    void set_fov(float fov) { fov_degrees = fov; }

    /// Set near/far clip planes
    /// @param near_plane Near clip distance
    /// @param far_plane Far clip distance
    void set_near_far(float near_plane, float far_plane) {
        z_near = near_plane;
        z_far = far_plane;
    }

    /// Get current camera eye position in world space
    const glm::vec3& get_position() const { return eye_pos; }

    /// Get current field of view in degrees
    float get_fov() const { return fov_degrees; }

    /// Get camera forward direction projected to horizontal plane
    glm::vec3 get_forward_horizontal() const;

    /// Get camera right direction
    glm::vec3 get_right() const;

    /// Get camera yaw angle (horizontal rotation)
    float get_yaw() const;

    /// Switch camera mode
    void set_mode(camera_mode new_mode) { mode = new_mode; }

    /// Get current camera mode
    camera_mode get_mode() const { return mode; }

    /// Update follow camera (call each frame when in follow mode)
    void follow_update(const glm::vec3& target_position, float dt);

    /// Set follow camera parameters
    void set_follow_distance(float dist) { follow_distance = dist; }
    void set_follow_height(float height) { follow_height_offset = height; }

private:
    void update_eye_position();

    glm::vec3 center;
    glm::vec3 eye_pos;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    float distance;
    float latitude;
    float longitude;

    float min_distance = 2.0f;
    float max_distance = 30.0f;
    float min_latitude = -85.0f;
    float max_latitude = 85.0f;

    float fov_degrees = 60.0f;
    float z_near = 0.1f;
    float z_far = 100.0f;

    camera_mode mode = camera_mode::orbit;

    // Follow mode state
    float follow_distance = 5.0f;
    float follow_height_offset = 1.5f;
};
