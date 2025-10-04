#include "rendering/scene.h"

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
