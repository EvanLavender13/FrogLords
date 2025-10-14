#include "foundation/procedural_mesh.h"
#include "foundation/math_utils.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <algorithm>

namespace foundation {

wireframe_mesh::wireframe_mesh()
    : position(0.0f, 0.0f, 0.0f)
    , rotation(0.0f, 0.0f, 0.0f)
    , scale(1.0f, 1.0f, 1.0f) {}

glm::mat4 wireframe_mesh::get_model_matrix() const {
    glm::mat4 model = glm::mat4(1.0f);

    // Apply transformations: translate, rotate, scale
    model = glm::translate(model, position);

    // Apply rotations (X, Y, Z order)
    model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotation.y, math::UP);
    model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, scale);

    return model;
}

wireframe_mesh generate_sphere(sphere_config config) {
    wireframe_mesh mesh;

    if (config.radius <= 0.0f) {
        mesh.vertices.push_back(glm::vec3(0.0f));
        return mesh;
    }

    int segments = std::max(3, config.segments);
    int rings = std::max(3, config.rings);
    float radius = config.radius;

    // Generate vertices using UV sphere topology
    mesh.vertices.push_back(glm::vec3(0.0f, radius, 0.0f));

    for (int r = 1; r < rings; r++) {
        float phi = static_cast<float>(r) / static_cast<float>(rings) * glm::pi<float>();
        float y = radius * std::cos(phi);
        float ring_radius = radius * std::sin(phi);

        for (int s = 0; s < segments; s++) {
            float theta =
                static_cast<float>(s) / static_cast<float>(segments) * 2.0f * glm::pi<float>();
            float x = ring_radius * std::cos(theta);
            float z = ring_radius * std::sin(theta);
            mesh.vertices.push_back(glm::vec3(x, y, z));
        }
    }

    mesh.vertices.push_back(glm::vec3(0.0f, -radius, 0.0f));

    int top_pole_index = 0;
    int bottom_pole_index = static_cast<int>(mesh.vertices.size()) - 1;

    for (int s = 0; s < segments; s++) {
        int vertex_index = 1 + s;
        mesh.edges.push_back(edge(top_pole_index, vertex_index));
    }

    for (int r = 0; r < rings - 2; r++) {
        int current_ring_start = 1 + r * segments;
        int next_ring_start = 1 + (r + 1) * segments;

        for (int s = 0; s < segments; s++) {
            int current_vertex = current_ring_start + s;
            int next_segment_vertex = current_ring_start + (s + 1) % segments;
            int vertical_vertex = next_ring_start + s;

            mesh.edges.push_back(edge(current_vertex, next_segment_vertex));
            mesh.edges.push_back(edge(current_vertex, vertical_vertex));
        }
    }

    int last_ring_start = 1 + (rings - 2) * segments;
    for (int s = 0; s < segments; s++) {
        int current_vertex = last_ring_start + s;
        int next_segment_vertex = last_ring_start + (s + 1) % segments;
        mesh.edges.push_back(edge(current_vertex, next_segment_vertex));
    }

    for (int s = 0; s < segments; s++) {
        int vertex_index = last_ring_start + s;
        mesh.edges.push_back(edge(vertex_index, bottom_pole_index));
    }

    return mesh;
}

wireframe_mesh generate_box(box_dimensions dims) {
    wireframe_mesh mesh;

    // Half-extents
    float half_width = dims.width * 0.5f;
    float half_height = dims.height * 0.5f;
    float half_depth = dims.depth * 0.5f;

    // 8 vertices (cube corners)
    mesh.vertices.push_back(
        glm::vec3(-half_width, -half_height, half_depth)); // 0: front-bottom-left
    mesh.vertices.push_back(
        glm::vec3(half_width, -half_height, half_depth)); // 1: front-bottom-right
    mesh.vertices.push_back(glm::vec3(-half_width, half_height, half_depth)); // 2: front-top-left
    mesh.vertices.push_back(glm::vec3(half_width, half_height, half_depth));  // 3: front-top-right
    mesh.vertices.push_back(
        glm::vec3(-half_width, -half_height, -half_depth)); // 4: back-bottom-left
    mesh.vertices.push_back(
        glm::vec3(half_width, -half_height, -half_depth)); // 5: back-bottom-right
    mesh.vertices.push_back(glm::vec3(-half_width, half_height, -half_depth)); // 6: back-top-left
    mesh.vertices.push_back(glm::vec3(half_width, half_height, -half_depth));  // 7: back-top-right

    // 12 edges
    // Bottom quad
    mesh.edges.push_back(edge(0, 1));
    mesh.edges.push_back(edge(1, 5));
    mesh.edges.push_back(edge(5, 4));
    mesh.edges.push_back(edge(4, 0));

    // Top quad
    mesh.edges.push_back(edge(2, 3));
    mesh.edges.push_back(edge(3, 7));
    mesh.edges.push_back(edge(7, 6));
    mesh.edges.push_back(edge(6, 2));

    // Vertical pillars
    mesh.edges.push_back(edge(0, 2));
    mesh.edges.push_back(edge(1, 3));
    mesh.edges.push_back(edge(5, 7));
    mesh.edges.push_back(edge(4, 6));

    return mesh;
}

wireframe_mesh generate_grid_floor(float size, int divisions) {
    wireframe_mesh mesh;

    if (divisions <= 0) {
        return mesh;
    }

    float half_size = size * 0.5f;
    float step = size / static_cast<float>(divisions);

    // Generate vertices at grid intersections
    for (int z = 0; z <= divisions; z++) {
        for (int x = 0; x <= divisions; x++) {
            float px = -half_size + static_cast<float>(x) * step;
            float pz = -half_size + static_cast<float>(z) * step;
            mesh.vertices.push_back(glm::vec3(px, 0.0f, pz));
        }
    }

    int grid_width = divisions + 1;

    // Generate horizontal edges (along X axis)
    for (int z = 0; z <= divisions; z++) {
        for (int x = 0; x < divisions; x++) {
            int v0 = z * grid_width + x;
            int v1 = z * grid_width + (x + 1);
            mesh.edges.push_back(edge(v0, v1));
        }
    }

    // Generate vertical edges (along Z axis)
    for (int z = 0; z < divisions; z++) {
        for (int x = 0; x <= divisions; x++) {
            int v0 = z * grid_width + x;
            int v1 = (z + 1) * grid_width + x;
            mesh.edges.push_back(edge(v0, v1));
        }
    }

    return mesh;
}

wireframe_mesh generate_arrow(const glm::vec3& start, const glm::vec3& end, float head_size) {
    wireframe_mesh mesh;

    glm::vec3 direction = end - start;
    float length = glm::length(direction);

    if (length < 0.001f) {
        return mesh; // Degenerate arrow
    }

    direction = glm::normalize(direction);

    // Arrow shaft (just a line from start to end)
    mesh.vertices.push_back(start);
    mesh.vertices.push_back(end);
    mesh.edges.push_back(edge(0, 1));

    // Cone head at end
    glm::vec3 perpendicular;
    if (std::abs(direction.y) < 0.9f) {
        perpendicular = glm::normalize(glm::cross(direction, math::UP));
    } else {
        perpendicular = glm::normalize(glm::cross(direction, glm::vec3(1, 0, 0)));
    }
    glm::vec3 other_perp = glm::cross(direction, perpendicular);

    glm::vec3 cone_base = end - direction * head_size;
    float cone_radius = head_size * 0.3f;

    // 4 vertices around cone base
    int base_start = static_cast<int>(mesh.vertices.size());
    for (int i = 0; i < 4; i++) {
        float angle = static_cast<float>(i) / 4.0f * 2.0f * glm::pi<float>();
        glm::vec3 offset =
            cone_radius * (std::cos(angle) * perpendicular + std::sin(angle) * other_perp);
        mesh.vertices.push_back(cone_base + offset);
    }

    // Connect cone vertices to tip
    for (int i = 0; i < 4; i++) {
        mesh.edges.push_back(edge(1, base_start + i));
    }

    // Connect cone base vertices
    for (int i = 0; i < 4; i++) {
        mesh.edges.push_back(edge(base_start + i, base_start + (i + 1) % 4));
    }

    return mesh;
}

wireframe_mesh generate_circle(const glm::vec3& center, circle_config config) {
    wireframe_mesh mesh;

    if (config.segments <= 0) {
        return mesh;
    }

    // Generate vertices around circle (horizontal, in XZ plane)
    for (int i = 0; i < config.segments; i++) {
        float angle =
            static_cast<float>(i) / static_cast<float>(config.segments) * 2.0f * glm::pi<float>();
        float x = center.x + config.radius * std::cos(angle);
        float z = center.z + config.radius * std::sin(angle);
        mesh.vertices.push_back(glm::vec3(x, center.y, z));
    }

    // Connect adjacent vertices
    for (int i = 0; i < config.segments; i++) {
        mesh.edges.push_back(edge(i, (i + 1) % config.segments));
    }

    return mesh;
}

wireframe_mesh generate_spring(const glm::vec3& start, const glm::vec3& end, int coils,
                               float radius) {
    wireframe_mesh mesh;

    glm::vec3 axis = end - start;
    float length = glm::length(axis);

    if (length < 0.0001f || coils <= 0 || radius <= 0.0f) {
        mesh.vertices.push_back(start);
        mesh.vertices.push_back(end);
        mesh.edges.push_back(edge(0, 1));
        return mesh;
    }

    glm::vec3 direction = axis / length;
    glm::vec3 fallback = std::abs(direction.y) < 0.95f ? math::UP : glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 tangent = glm::normalize(glm::cross(direction, fallback));
    glm::vec3 bitangent = glm::cross(direction, tangent);

    int segments_per_coil = std::max(6, coils * 2);
    int total_segments = coils * segments_per_coil;

    mesh.vertices.reserve(total_segments + 1);
    for (int i = 0; i <= total_segments; i++) {
        float t = static_cast<float>(i) / static_cast<float>(total_segments);
        float angle = t * static_cast<float>(coils) * 2.0f * glm::pi<float>();
        float envelope = std::sin(t * glm::pi<float>());
        glm::vec3 radial =
            (std::cos(angle) * tangent + std::sin(angle) * bitangent) * (radius * envelope);
        glm::vec3 point = start + direction * (t * length) + radial;
        mesh.vertices.push_back(point);

        if (i > 0) {
            mesh.edges.push_back(edge(i - 1, i));
        }
    }

    return mesh;
}

} // namespace foundation
