#include "foundation/collision.h"
#include "foundation/math_utils.h"
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <algorithm>

sphere_collision resolve_sphere_aabb(const sphere& s, const aabb& box) {
    sphere_collision result;

    // Find the closest point on the AABB to the center of the sphere
    glm::vec3 closest_point;
    closest_point.x = std::max(box.center.x - box.half_extents.x,
                               std::min(s.center.x, box.center.x + box.half_extents.x));
    closest_point.y = std::max(box.center.y - box.half_extents.y,
                               std::min(s.center.y, box.center.y + box.half_extents.y));
    closest_point.z = std::max(box.center.z - box.half_extents.z,
                               std::min(s.center.z, box.center.z + box.half_extents.z));

    // Calculate the distance between the sphere's center and the closest point
    glm::vec3 distance = s.center - closest_point;

    float distance_squared = glm::dot(distance, distance);

    // If the distance is less than the sphere's radius, there is a collision
    if (distance_squared < (s.radius * s.radius)) {
        result.hit = true;
        float distance_magnitude = std::sqrt(distance_squared);

        // Safe normalize: prevents division by zero when sphere center inside/on AABB
        // Fallback to UP pushes sphere upward when distance is degenerate
        result.normal = math::safe_normalize(distance, math::UP);
        result.penetration = s.radius - distance_magnitude;
        result.contact_box = &box;
    }

    return result;
}

sphere_collision resolve_box_collisions(sphere& collision_sphere, const collision_world& world,
                                        glm::vec3& position, glm::vec3& velocity) {
    sphere_collision final_contact; // Default: hit=false, contact_box=nullptr

    // TUNED: Multi-pass collision resolution iteration limit
    // Purpose: Handle corner/edge cases where single pass insufficient (sliding along edges)
    // Common range: 2-4 passes (trade-off: accuracy vs performance)
    // Assessment: 3 passes handles most multi-collision scenarios in practice
    // Used in: Loop iterations for resolving multiple simultaneous collisions
    for (int pass = 0; pass < 3; ++pass) { // iterations (dimensionless)
        bool any_collision = false;

        for (const auto& box : world.boxes) {
            sphere_collision col = resolve_sphere_aabb(collision_sphere, box);

            if (col.hit) {
                // Push out of collision
                position += col.normal * col.penetration;
                collision_sphere.center = position;

                // Remove velocity into surface
                float vel_into_surface = glm::dot(velocity, col.normal);
                if (vel_into_surface < 0.0f) {
                    velocity -= col.normal * vel_into_surface;
                }

                // Track final contact (last valid collision from multi-pass)
                final_contact = col;
                any_collision = true;
            }
        }

        if (!any_collision)
            break; // Early exit if no collisions in this pass
    }

    return final_contact;
}

sphere_collision resolve_collisions(sphere& collision_sphere, const collision_world& world,
                                    glm::vec3& position, glm::vec3& velocity) {
    // Update collision sphere position to match integrated position
    collision_sphere.center = position;

    // Box collision resolution (unified collision system)
    return resolve_box_collisions(collision_sphere, world, position, velocity);
}