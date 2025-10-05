#include "foundation/collision.h"
#include <algorithm>
#include <cmath>

namespace {
constexpr float EPSILON = 0.0001f;
}

sphere_collision resolve_sphere_aabb(sphere& s, const aabb& box) {
    sphere_collision result{};

    glm::vec3 min_corner = box.center - box.half_extents;
    glm::vec3 max_corner = box.center + box.half_extents;
    glm::vec3 closest = glm::clamp(s.center, min_corner, max_corner);

    glm::vec3 delta = s.center - closest;
    float distance_sq = glm::dot(delta, delta);
    float radius_sq = s.radius * s.radius;

    if (distance_sq >= radius_sq) {
        return result;
    }

    float distance = std::sqrt(std::max(distance_sq, 0.0f));

    if (distance < EPSILON) {
        glm::vec3 offset_from_center = s.center - box.center;
        glm::vec3 abs_offset = glm::abs(offset_from_center);
        glm::vec3 distance_to_surface = box.half_extents - abs_offset;

        if (distance_to_surface.x < distance_to_surface.y &&
            distance_to_surface.x < distance_to_surface.z) {
            result.normal = glm::vec3(offset_from_center.x > 0 ? 1.0f : -1.0f, 0.0f, 0.0f);
            result.penetration = distance_to_surface.x + s.radius;
        } else if (distance_to_surface.y < distance_to_surface.z) {
            result.normal = glm::vec3(0.0f, offset_from_center.y > 0 ? 1.0f : -1.0f, 0.0f);
            result.penetration = distance_to_surface.y + s.radius;
        } else {
            result.normal = glm::vec3(0.0f, 0.0f, offset_from_center.z > 0 ? 1.0f : -1.0f);
            result.penetration = distance_to_surface.z + s.radius;
        }
    } else {
        result.normal = delta / distance;
        result.penetration = s.radius - distance;
    }

    result.hit = true;
    return result;
}
