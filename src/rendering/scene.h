#pragma once
#include "foundation/procedural_mesh.h"
#include "foundation/collision_primitives.h"
#include <vector>

class scene {
  public:
    scene() = default;
    ~scene() = default;

    void add_object(const foundation::wireframe_mesh& mesh);
    void add_collision_box(const aabb& box);
    void clear();

    size_t object_count() const;
    const std::vector<foundation::wireframe_mesh>& objects() const;
    const std::vector<aabb>& collision_boxes() const;

  private:
    std::vector<foundation::wireframe_mesh> meshes;
    std::vector<aabb> boxes;
};
