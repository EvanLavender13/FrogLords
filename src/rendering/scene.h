#pragma once
#include "rendering/wireframe.h"
#include <vector>
#include <glm/glm.hpp>

struct sphere;

/// Axis-aligned bounding box collision primitive
struct aabb {
    glm::vec3 center;
    glm::vec3 half_extents;

    /// Check if sphere collides with box, resolve collision
    /// Returns true if collision occurred, updates sphere position
    bool resolve_sphere_collision(sphere& s, glm::vec3& normal) const;
};

class scene {
public:
    scene();
    ~scene();

    /// Add wireframe mesh to scene
    void add_object(const wireframe_mesh& mesh);

    /// Add collision box to scene
    void add_collision_box(const aabb& box);

    /// Remove all objects from scene
    void clear();

    /// Get number of objects in scene
    size_t object_count() const;

    /// Access scene objects for rendering
    const std::vector<wireframe_mesh>& objects() const;

    /// Access collision boxes
    const std::vector<aabb>& collision_boxes() const;

private:
    std::vector<wireframe_mesh> meshes;
    std::vector<aabb> boxes;
};
