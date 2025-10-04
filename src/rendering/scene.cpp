#include "rendering/scene.h"

scene::scene() {
}

scene::~scene() {
}

void scene::add_object(const wireframe_mesh& mesh) {
    meshes.push_back(mesh);
}

void scene::clear() {
    meshes.clear();
}

size_t scene::object_count() const {
    return meshes.size();
}

const std::vector<wireframe_mesh>& scene::objects() const {
    return meshes;
}
