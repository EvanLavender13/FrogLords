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
        result.normal = distance / distance_magnitude;
        result.penetration = s.radius - distance_magnitude;
    }

    return result;
}

void resolve_ground_collision(sphere& collision_sphere, glm::vec3& position, glm::vec3& velocity,
                              bool& is_grounded, glm::vec3& ground_normal, float& ground_height) {
    using namespace glm;

    // Only check ground plane if not already grounded by box collision
    if (is_grounded) {
        return;
    }

    // Simple ground plane at y=0 - single sphere collision
    float ground_y = 0.0f;
    float distance_to_ground = collision_sphere.center.y - ground_y;
    float penetration = collision_sphere.radius - distance_to_ground;

    if (penetration > 0.0f) {
        // Push sphere up to rest on surface
        collision_sphere.center.y += penetration;
        position = collision_sphere.center;

        // Remove velocity into ground
        if (velocity.y < 0.0f) {
            velocity.y = 0.0f;
        }

        is_grounded = true;
        ground_normal = math::UP;
        ground_height = ground_y;
    } else {
        is_grounded = false;
    }
}

void resolve_box_collisions(sphere& collision_sphere, const collision_world& world,
                            glm::vec3& position, glm::vec3& velocity, bool& is_grounded,
                            glm::vec3& ground_normal, float& ground_height, float max_slope_angle) {
    // Multi-pass collision resolution to handle corner cases
    for (int pass = 0; pass < 3; ++pass) {
        bool any_collision = false;

        for (const auto& box : world.boxes) {
            sphere_collision col = resolve_sphere_aabb(collision_sphere, box);

            if (col.hit) {
                // Push out of collision
                position += col.normal * col.penetration;
                collision_sphere.center = position;

                // Check if this is a ground surface (upward-facing normal)
                if (col.normal.y >= glm::cos(glm::radians(max_slope_angle))) {
                    // Set grounded state
                    is_grounded = true;
                    ground_normal = col.normal;
                    ground_height = box.center.y + box.half_extents.y;
                }

                // Remove velocity into surface
                float vel_into_surface = glm::dot(velocity, col.normal);
                if (vel_into_surface < 0.0f) {
                    velocity -= col.normal * vel_into_surface;
                }

                any_collision = true;
            }
        }

        if (!any_collision)
            break; // Early exit if no collisions in this pass
    }
}

void resolve_collisions(sphere& collision_sphere, const collision_world& world, glm::vec3& position,
                        glm::vec3& velocity, bool& is_grounded, glm::vec3& ground_normal,
                        float& ground_height, float max_slope_angle) {
    // Reset grounded state before collision checks
    is_grounded = false;

    // Update collision sphere position to match integrated position
    collision_sphere.center = position;

    // Box collision resolution (main environment)
    resolve_box_collisions(collision_sphere, world, position, velocity, is_grounded, ground_normal,
                           ground_height, max_slope_angle);

    // Ground plane (fallback if no box collision)
    resolve_ground_collision(collision_sphere, position, velocity, is_grounded, ground_normal,
                             ground_height);
}