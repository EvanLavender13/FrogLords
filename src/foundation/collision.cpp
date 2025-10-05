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

    float distance = std::sqrt(distance_sq);

    // Special case: sphere center is inside the AABB.
    // We need to find the smallest push-out vector.
    if (distance < EPSILON) {
        glm::vec3 dist_to_max = max_corner - s.center;
        glm::vec3 dist_to_min = s.center - min_corner;

        float min_dist = dist_to_max.x;
        result.normal = glm::vec3(1, 0, 0);

        if (dist_to_min.x < min_dist) {
            min_dist = dist_to_min.x;
            result.normal = glm::vec3(-1, 0, 0);
        }
        if (dist_to_max.y < min_dist) {
            min_dist = dist_to_max.y;
            result.normal = glm::vec3(0, 1, 0);
        }
        if (dist_to_min.y < min_dist) {
            min_dist = dist_to_min.y;
            result.normal = glm::vec3(0, -1, 0);
        }
        if (dist_to_max.z < min_dist) {
            min_dist = dist_to_max.z;
            result.normal = glm::vec3(0, 0, 1);
        }
        if (dist_to_min.z < min_dist) {
            min_dist = dist_to_min.z;
            result.normal = glm::vec3(0, 0, -1);
        }
        result.penetration = s.radius + min_dist;
    } else {
        result.normal = delta / distance;
        result.penetration = s.radius - distance;
    }

    result.hit = true;
    return result;
}
