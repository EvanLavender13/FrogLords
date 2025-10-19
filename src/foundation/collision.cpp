#include "foundation/collision.h"
#include "foundation/debug_assert.h"
#include "foundation/math_utils.h"
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <algorithm>

namespace {

// Compute AABB face normal from closest point on box surface
// Used as fallback when sphere center is inside/on box (degenerate distance vector)
// Returns the normal of the face that closest_point lies on
glm::vec3 compute_face_normal(const glm::vec3& closest_point, const aabb& box) {
    // Find which face the closest_point is on by checking which component
    // is at the box extent (within epsilon for numerical stability)
    constexpr float epsilon = 0.0001f;

    glm::vec3 box_min = box.center - box.half_extents;
    glm::vec3 box_max = box.center + box.half_extents;

    // Check each face (prioritize Y for floor/ceiling, then X, then Z)
    if (std::abs(closest_point.y - box_max.y) < epsilon)
        return glm::vec3(0, 1, 0); // Top
    if (std::abs(closest_point.y - box_min.y) < epsilon)
        return glm::vec3(0, -1, 0); // Bottom
    if (std::abs(closest_point.x - box_max.x) < epsilon)
        return glm::vec3(1, 0, 0); // Right
    if (std::abs(closest_point.x - box_min.x) < epsilon)
        return glm::vec3(-1, 0, 0); // Left
    if (std::abs(closest_point.z - box_max.z) < epsilon)
        return glm::vec3(0, 0, 1); // Front
    if (std::abs(closest_point.z - box_min.z) < epsilon)
        return glm::vec3(0, 0, -1); // Back

    // Fallback: should not reach here if closest_point is truly on box surface
    // If we do, use UP as last resort (same as old behavior)
    return math::UP;
}

// Surface classification: Determine if collision normal represents a wall
// A wall is defined as a surface that is more vertical than the threshold angle
// Returns true if surface is a wall (not floor or ceiling)
//
// wall_threshold: cos of the maximum walkable slope angle (from controller)
//   Example: 45° slope → threshold = cos(45°) ≈ 0.707
//   Single source of truth: derived from controller.max_slope_angle
bool is_wall(const glm::vec3& normal, float wall_threshold) {
    // Compare absolute value to handle both upward (floor) and downward (ceiling) normals
    // abs(dot(normal, UP)) gives how "vertical" the surface is:
    //   1.0 = horizontal surface (floor/ceiling)
    //   0.0 = vertical surface (wall)
    return std::abs(normal.y) < wall_threshold;
}

// Project velocity along wall surface (remove component into wall normal)
// This preserves player intent to move parallel to the wall
// Returns velocity vector tangent to the wall surface
glm::vec3 project_along_wall(const glm::vec3& velocity, const glm::vec3& wall_normal) {
    // Contract validation (ITERATE phase assertions)
    FL_ASSERT_FINITE(velocity, "velocity");
    FL_ASSERT_FINITE(wall_normal, "wall_normal");
    FL_ASSERT_NORMALIZED(wall_normal, "wall_normal");

    // Remove the component of velocity that points into the wall
    // Formula: v_tangent = v - n * dot(v, n)
    // This is the standard vector projection formula for removing a component
    glm::vec3 projected = velocity - wall_normal * glm::dot(velocity, wall_normal);

    // Contract postconditions
    // 1. Projection never amplifies velocity: |v_projected| ≤ |v_original|
    float original_mag = glm::length(velocity);
    float projected_mag = glm::length(projected);
    FL_POSTCONDITION(projected_mag <= original_mag + FL_EPSILON,
                     "projection must not amplify velocity");

    // 2. Projected velocity is orthogonal to normal: dot(v_projected, normal) ≈ 0
    float dot_result = glm::dot(projected, wall_normal);
    FL_POSTCONDITION(std::abs(dot_result) < FL_EPSILON,
                     "projected velocity must be orthogonal to wall normal");

    // 3. Result is finite
    FL_ASSERT_FINITE(projected, "projected velocity");

    return projected;
}

} // namespace

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

        // Compute collision normal
        // Normal case: normalize the distance vector (points from surface to sphere center)
        // Degenerate case: sphere center on/inside box → use face normal from closest_point
        // This prevents misclassifying deep wall penetrations as floors (no UP fallback)
        glm::vec3 face_normal = compute_face_normal(closest_point, box);
        result.normal = math::safe_normalize(distance, face_normal);
        result.penetration = s.radius - distance_magnitude;
        result.contact_box = &box;

        // Validate collision normal (ITERATE phase)
        FL_ASSERT_NORMALIZED(result.normal, "collision normal");
        FL_ASSERT_NON_NEGATIVE(result.penetration, "penetration depth");
    }

    return result;
}

sphere_collision resolve_box_collisions(sphere& collision_sphere, const collision_world& world,
                                        glm::vec3& position, glm::vec3& velocity,
                                        float wall_threshold) {
    sphere_collision final_contact; // Default: hit=false, contact_box=nullptr

    // Sequential iteration resolves multi-wall collisions (industry standard approach)
    // Handles N-wall cases, converges to stable solution, deterministic physics outcome
    // 3 passes handles most scenarios
    for (int pass = 0; pass < 3; ++pass) { // iterations (dimensionless)
        bool any_collision = false;

        for (const auto& box : world.boxes) {
            sphere_collision col = resolve_sphere_aabb(collision_sphere, box.bounds);

            if (col.hit) {
                // Push out of collision
                position += col.normal * col.penetration;
                collision_sphere.center = position;

                // Wall sliding: Classify surface and apply appropriate velocity response
                bool is_wall_contact = is_wall(col.normal, wall_threshold);
                if (is_wall_contact) {
                    // Wall collision: Project velocity along wall surface
                    // This preserves player intent to move parallel to the wall
                    velocity = project_along_wall(velocity, col.normal);
                } else {
                    // Floor/ceiling collision: Remove velocity into surface (original behavior)
                    float vel_into_surface = glm::dot(velocity, col.normal);
                    if (vel_into_surface < 0.0f) {
                        velocity -= col.normal * vel_into_surface;
                    }

                    // Track floor contact (for grounding logic)
                    // Prevents losing grounded state when touching floor + wall simultaneously
                    if (col.normal.y > 0.0f) { // Upward-facing surface = floor
                        final_contact.contacted_floor = true;
                        final_contact.floor_normal = col.normal;
                        final_contact.contact_box =
                            col.contact_box; // Store floor box for height query
                    }
                }

                // Track final contact (last valid collision from multi-pass)
                final_contact.hit = col.hit;
                final_contact.normal = col.normal;
                final_contact.penetration = col.penetration;
                final_contact.is_wall = is_wall_contact;
                // Note: contacted_floor and floor_normal persist across contacts

                any_collision = true;
            }
        }

        if (!any_collision)
            break; // Early exit if no collisions in this pass
    }

    return final_contact;
}

sphere_collision resolve_collisions(sphere& collision_sphere, const collision_world& world,
                                    glm::vec3& position, glm::vec3& velocity,
                                    float wall_threshold) {
    // Update collision sphere position to match integrated position
    collision_sphere.center = position;

    // Box collision resolution (unified collision system)
    return resolve_box_collisions(collision_sphere, world, position, velocity, wall_threshold);
}