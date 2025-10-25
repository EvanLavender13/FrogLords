#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "foundation/math_utils.h"

struct clip_planes {
    float near_plane = 0.1f;
    float far_plane = 100.0f;
};

/// Camera - pure view/projection matrix generation
class camera {
  public:
    camera() = default;

    /// Generate view matrix from current eye position and target
    glm::mat4 get_view_matrix() const;

    /// Generate perspective projection matrix
    /// @param aspect_ratio Viewport width/height ratio
    glm::mat4 get_projection_matrix(float aspect_ratio) const;

    /// Set camera eye position in world space
    void set_position(const glm::vec3& pos) { eye_pos = pos; }

    /// Set camera look-at target in world space
    void set_target(const glm::vec3& target) { center = target; }

    /// Get current camera eye position in world space
    const glm::vec3& get_position() const { return eye_pos; }

    /// Get current field of view in degrees
    float get_fov() const { return fov_degrees; }

    /// Set field of view in degrees
    /// @param fov Field of view in degrees (must be positive and finite)
    void set_fov(float fov);

    /// Get camera forward direction projected to horizontal plane
    glm::vec3 get_forward_horizontal() const;

    /// Get camera right direction
    glm::vec3 get_right() const;

    /// Get camera yaw angle (horizontal rotation)
    float get_yaw() const;

  private:
    glm::vec3 center = glm::vec3(0.0f);
    glm::vec3 eye_pos = glm::vec3(0.0f, 5.0f, 10.0f);
    glm::vec3 up = math::UP;

    float fov_degrees = 60.0f;
    // Use a slightly larger near plane to get better floating-point precision in the
    // depth buffer (reduces z-fighting/aliasing when rendering distant thin geometry).
    float z_near = 0.5f;
    float z_far = 100.0f;
};
