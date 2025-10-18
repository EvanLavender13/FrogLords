#include "rendering/scene.h"

void scene::add_object(const foundation::wireframe_mesh& mesh) {
    meshes.push_back(mesh);
}

void scene::clear() {
    meshes.clear();
}

size_t scene::object_count() const {
    return meshes.size();
}

const std::vector<foundation::wireframe_mesh>& scene::objects() const {
    return meshes;
}
