#include "camera/camera.h"
#include <cmath>
#include <algorithm>

camera::camera(glm::vec3 center, float distance, float latitude, float longitude)
    : center(center)
    , distance(distance)
    , latitude(latitude)
    , longitude(longitude) {
    update_eye_position();
}

void camera::update_eye_position() {
    float lat_rad = glm::radians(latitude);
    float lon_rad = glm::radians(longitude);

    eye_pos.x = center.x + distance * cosf(lat_rad) * sinf(lon_rad);
    eye_pos.y = center.y + distance * sinf(lat_rad);
    eye_pos.z = center.z + distance * cosf(lat_rad) * cosf(lon_rad);
}

glm::mat4 camera::get_view_matrix() const {
    return glm::lookAt(eye_pos, center, up);
}

glm::mat4 camera::get_projection_matrix(float aspect_ratio) const {
    return glm::perspective(glm::radians(fov_degrees), aspect_ratio, z_near, z_far);
}

void camera::orbit(float delta_x, float delta_y) {
    longitude += delta_x;
    latitude = std::clamp(latitude + delta_y, min_latitude, max_latitude);

    // Wrap longitude
    if (longitude < -180.0f)
        longitude += 360.0f;
    if (longitude > 180.0f)
        longitude -= 360.0f;

    update_eye_position();
}

void camera::zoom(float delta) {
    distance = std::clamp(distance + delta, min_distance, max_distance);
    update_eye_position();
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
    return glm::normalize(glm::cross(get_forward_horizontal(), glm::vec3(0, 1, 0)));
}

float camera::get_yaw() const {
    glm::vec3 forward = get_forward_horizontal();
    return atan2f(forward.x, forward.z);
}

void camera::follow_update(const glm::vec3& target_position, float dt) {
    if (mode != camera_mode::follow)
        return;

    // Update center to follow target
    center = target_position + glm::vec3(0, follow_height_offset, 0);

    // Calculate position using spherical coordinates (reuse orbit logic)
    float lat_rad = glm::radians(latitude);
    float lon_rad = glm::radians(longitude);

    eye_pos.x = center.x + follow_distance * cosf(lat_rad) * sinf(lon_rad);
    eye_pos.y = center.y + follow_distance * sinf(lat_rad);
    eye_pos.z = center.z + follow_distance * cosf(lat_rad) * cosf(lon_rad);
}
