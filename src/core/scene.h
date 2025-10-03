#pragma once
#include "rendering/wireframe.h"
#include <vector>

class scene {
public:
    scene();
    ~scene();

    /// Add wireframe mesh to scene
    void add_object(const wireframe_mesh& mesh);

    /// Remove all objects from scene
    void clear();

    /// Get number of objects in scene
    size_t object_count() const;

    /// Access scene objects for rendering
    const std::vector<wireframe_mesh>& objects() const;

private:
    std::vector<wireframe_mesh> meshes;
};
