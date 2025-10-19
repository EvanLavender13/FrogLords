#include "camera/camera.h"

glm::mat4 camera::get_view_matrix() const {
    return glm::lookAt(eye_pos, center, up);
}

glm::mat4 camera::get_projection_matrix(float aspect_ratio) const {
    return glm::perspective(glm::radians(fov_degrees), aspect_ratio, z_near, z_far);
}

glm::vec3 camera::get_forward_horizontal() const {
    glm::vec3 forward = glm::normalize(center - eye_pos);
    forward.y = 0;
    if (glm::length(forward) > 0.0f) {
        forward = glm::normalize(forward);
    }
    return forward;
}

glm::vec3 camera::get_right() const {
    return glm::normalize(glm::cross(get_forward_horizontal(), math::UP));
}

float camera::get_yaw() const {
    glm::vec3 forward = get_forward_horizontal();
    return atan2f(forward.x, forward.z);
}
