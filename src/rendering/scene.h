#pragma once
#include "foundation/procedural_mesh.h"
#include <vector>

class scene {
  public:
    scene() = default;
    ~scene() = default;

    void add_object(const foundation::wireframe_mesh& mesh);
    void clear();

    size_t object_count() const;
    const std::vector<foundation::wireframe_mesh>& objects() const;

  private:
    std::vector<foundation::wireframe_mesh> meshes;
};
