#include "camera/camera.h"
#include "foundation/debug_assert.h"

glm::mat4 camera::get_view_matrix() const {
    return glm::lookAt(eye_pos, center, up);
}

glm::mat4 camera::get_projection_matrix(float aspect_ratio) const {
    return glm::perspective(glm::radians(fov_degrees), aspect_ratio, z_near, z_far);
}

glm::vec3 camera::get_forward_horizontal() const {
    // Guard: Normalize 3D forward direction (eye → target)
    // Fallback: If eye == center, use standard -Z forward
    glm::vec3 forward_3d = math::safe_normalize(center - eye_pos, glm::vec3(0.0f, 0.0f, -1.0f));

    // Project to horizontal plane by zeroing Y
    glm::vec3 forward_2d = glm::vec3(forward_3d.x, 0.0f, forward_3d.z);

    // Guard: Normalize horizontal projection
    // Fallback: If camera looking straight up/down, use -Z forward
    glm::vec3 result = math::safe_normalize(forward_2d, glm::vec3(0.0f, 0.0f, -1.0f));

    FL_POSTCONDITION(std::isfinite(result.x) && std::isfinite(result.y) && std::isfinite(result.z),
                     "forward_horizontal must be finite");
    FL_ASSERT_NORMALIZED(result, "forward_horizontal");

    return result;
}

glm::vec3 camera::get_right() const {
    glm::vec3 forward = get_forward_horizontal();

    // Compute cross product (UP × forward)
    glm::vec3 right = glm::cross(math::UP, forward);

    // Guard: Normalize cross product
    // Fallback: If forward parallel to UP (degenerate), use standard X axis
    glm::vec3 result = math::safe_normalize(right, glm::vec3(1.0f, 0.0f, 0.0f));

    FL_POSTCONDITION(std::isfinite(result.x) && std::isfinite(result.y) && std::isfinite(result.z),
                     "right must be finite");
    FL_ASSERT_NORMALIZED(result, "right");

    return result;
}

float camera::get_yaw() const {
    glm::vec3 forward = get_forward_horizontal();
    return atan2f(forward.x, forward.z);
}
