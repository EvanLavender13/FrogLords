#include "camera/camera_follow.h"
#include "foundation/math_utils.h"
#include <algorithm>
#include <cmath>

namespace {
/// Compute 3D position from spherical coordinates
glm::vec3 compute_spherical_position(const glm::vec3& center, float dist, float lat, float lon) {
    float lat_rad = glm::radians(lat);
    float lon_rad = glm::radians(lon);
    glm::vec3 pos;
    pos.x = center.x + dist * cosf(lat_rad) * sinf(lon_rad);
    pos.y = center.y + dist * sinf(lat_rad);
    pos.z = center.z + dist * cosf(lat_rad) * cosf(lon_rad);
    return pos;
}
} // namespace

void camera_follow::orbit(float delta_x, float delta_y) {
    longitude += delta_x;
    latitude = std::clamp(latitude + delta_y, min_latitude, max_latitude);

    // Wrap longitude to [-180, 180]
    longitude = math::wrap_angle_degrees(longitude);
}

void camera_follow::zoom(float delta) {
    distance = std::clamp(distance + delta, min_distance, max_distance);
}

glm::vec3 camera_follow::compute_eye_position(const glm::vec3& target_position) const {
    glm::vec3 center = target_position + glm::vec3(0, height_offset, 0);
    return compute_spherical_position(center, distance, latitude, longitude);
}

glm::vec3 camera_follow::compute_look_target(const glm::vec3& target_position) const {
    return target_position + glm::vec3(0, height_offset, 0);
}
