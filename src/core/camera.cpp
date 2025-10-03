#include "camera.h"
#include <cmath>
#include <algorithm>

camera::camera(glm::vec3 center, float distance, float latitude, float longitude)
    : center(center), distance(distance), latitude(latitude), longitude(longitude) {
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
    if (longitude < -180.0f) longitude += 360.0f;
    if (longitude > 180.0f) longitude -= 360.0f;

    update_eye_position();
}

void camera::zoom(float delta) {
    distance = std::clamp(distance + delta, min_distance, max_distance);
    update_eye_position();
}
