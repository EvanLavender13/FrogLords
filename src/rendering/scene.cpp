#include "rendering/scene.h"
#include "character/character_controller.h"
#include <algorithm>

using namespace glm;

bool aabb::resolve_sphere_collision(sphere& s, vec3& normal) const {
    // Find closest point on box to sphere center
    vec3 closest = clamp(s.center, center - half_extents, center + half_extents);

    // Vector from closest point to sphere center
    vec3 delta = s.center - closest;
    float distance_sq = dot(delta, delta);

    // Check if collision occurred
    if (distance_sq < s.radius * s.radius) {
        float distance = sqrtf(distance_sq);

        // Edge case: sphere center inside box (closest point is on surface, not inside)
        if (distance < 0.0001f) {
            // Sphere center inside box - push out along shortest axis
            vec3 offset_from_center = s.center - center;
            vec3 abs_offset = abs(offset_from_center);
            vec3 distance_to_surface = half_extents - abs_offset;

            // Find minimum distance to surface (shortest push-out)
            if (distance_to_surface.x < distance_to_surface.y && distance_to_surface.x < distance_to_surface.z) {
                // Push out along X
                normal = vec3(offset_from_center.x > 0 ? 1.0f : -1.0f, 0, 0);
                float push_distance = distance_to_surface.x + s.radius;
                s.center += normal * push_distance;
            } else if (distance_to_surface.y < distance_to_surface.z) {
                // Push out along Y
                normal = vec3(0, offset_from_center.y > 0 ? 1.0f : -1.0f, 0);
                float push_distance = distance_to_surface.y + s.radius;
                s.center += normal * push_distance;
            } else {
                // Push out along Z
                normal = vec3(0, 0, offset_from_center.z > 0 ? 1.0f : -1.0f);
                float push_distance = distance_to_surface.z + s.radius;
                s.center += normal * push_distance;
            }
        } else {
            // Normal collision - push out along delta direction
            normal = delta / distance;
            float penetration = s.radius - distance;
            s.center += normal * penetration;
        }

        return true;
    }

    return false;
}

scene::scene() {
}

scene::~scene() {
}

void scene::add_object(const wireframe_mesh& mesh) {
    meshes.push_back(mesh);
}

void scene::add_collision_box(const aabb& box) {
    boxes.push_back(box);
}

void scene::clear() {
    meshes.clear();
    boxes.clear();
}

size_t scene::object_count() const {
    return meshes.size();
}

const std::vector<wireframe_mesh>& scene::objects() const {
    return meshes;
}

const std::vector<aabb>& scene::collision_boxes() const {
    return boxes;
}
