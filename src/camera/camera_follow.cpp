#include "camera/camera_follow.h"
#include "foundation/math_utils.h"
#include "foundation/debug_assert.h"
#include <algorithm>
#include <cmath>

namespace {
/// Compute 3D position from spherical coordinates
glm::vec3 compute_spherical_position(const glm::vec3& center, float dist, float lat, float lon) {
    FL_PRECONDITION(dist > 0.0f, "distance must be positive");
    FL_PRECONDITION(lat >= -90.0f && lat <= 90.0f, "latitude must be in range [-90, 90]");

    float lat_rad = glm::radians(lat);
    float lon_rad = glm::radians(lon);
    glm::vec3 pos;
    pos.x = center.x + dist * cosf(lat_rad) * sinf(lon_rad);
    pos.y = center.y + dist * sinf(lat_rad);
    pos.z = center.z + dist * cosf(lat_rad) * cosf(lon_rad);

    FL_POSTCONDITION(std::isfinite(pos.x) && std::isfinite(pos.y) && std::isfinite(pos.z),
                     "output position must be finite");
    return pos;
}
} // namespace

void camera_follow::orbit(float delta_x, float delta_y) {
    FL_PRECONDITION(std::isfinite(delta_x) && std::isfinite(delta_y),
                    "orbit deltas must be finite");
    FL_PRECONDITION(min_latitude <= max_latitude, "min_latitude must be <= max_latitude");

    longitude += delta_x;
    latitude = std::clamp(latitude + delta_y, min_latitude, max_latitude);

    // Wrap longitude to [-180, 180]
    longitude = math::wrap_angle_degrees(longitude);

    FL_POSTCONDITION(latitude >= min_latitude && latitude <= max_latitude,
                     "latitude must be clamped to limits");
    FL_POSTCONDITION(longitude >= -180.0f && longitude <= 180.0f,
                     "longitude must be wrapped to [-180, 180]");
}

void camera_follow::zoom(float delta) {
    FL_PRECONDITION(std::isfinite(delta), "zoom delta must be finite");
    FL_PRECONDITION(min_distance <= max_distance, "min_distance must be <= max_distance");
    FL_PRECONDITION(min_distance > 0.0f, "min_distance must be positive");

    distance = std::clamp(distance + delta, min_distance, max_distance);

    FL_POSTCONDITION(distance >= min_distance && distance <= max_distance,
                     "distance must be clamped to limits");
    FL_POSTCONDITION(distance > 0.0f, "distance must remain positive");
}

glm::vec3 camera_follow::compute_eye_position(const glm::vec3& target_position) const {
    FL_PRECONDITION(std::isfinite(target_position.x) && std::isfinite(target_position.y) &&
                    std::isfinite(target_position.z), "target_position must be finite");
    FL_PRECONDITION(distance > 0.0f, "distance must be positive");

    glm::vec3 center = target_position + glm::vec3(0, height_offset, 0);
    glm::vec3 result = compute_spherical_position(center, distance, latitude, longitude);

    FL_POSTCONDITION(std::isfinite(result.x) && std::isfinite(result.y) && std::isfinite(result.z),
                     "eye position must be finite");
    return result;
}

glm::vec3 camera_follow::compute_look_target(const glm::vec3& target_position) const {
    FL_PRECONDITION(std::isfinite(target_position.x) && std::isfinite(target_position.y) &&
                    std::isfinite(target_position.z), "target_position must be finite");

    glm::vec3 result = target_position + glm::vec3(0, height_offset, 0);

    FL_POSTCONDITION(std::isfinite(result.x) && std::isfinite(result.y) && std::isfinite(result.z),
                     "look target must be finite");
    return result;
}
