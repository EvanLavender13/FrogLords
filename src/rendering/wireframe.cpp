#include "wireframe.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

wireframe_mesh::wireframe_mesh()
    : position(0.0f, 0.0f, 0.0f)
    , rotation(0.0f, 0.0f, 0.0f)
    , scale(1.0f, 1.0f, 1.0f)
{
}

glm::mat4 wireframe_mesh::get_model_matrix() const {
    glm::mat4 model = glm::mat4(1.0f);

    // Apply transformations: translate, rotate, scale
    model = glm::translate(model, position);

    // Apply rotations (X, Y, Z order)
    model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, scale);

    return model;
}

wireframe_mesh generate_sphere(int segments, int rings, float radius) {
    wireframe_mesh mesh;

    // Generate vertices using UV sphere topology
    // Top pole
    mesh.vertices.push_back(glm::vec3(0.0f, radius, 0.0f));

    // Intermediate rings
    for (int r = 1; r < rings; r++) {
        float phi = static_cast<float>(r) / rings * glm::pi<float>();
        float y = radius * std::cos(phi);
        float ringRadius = radius * std::sin(phi);

        for (int s = 0; s < segments; s++) {
            float theta = static_cast<float>(s) / segments * 2.0f * glm::pi<float>();
            float x = ringRadius * std::cos(theta);
            float z = ringRadius * std::sin(theta);
            mesh.vertices.push_back(glm::vec3(x, y, z));
        }
    }

    // Bottom pole
    mesh.vertices.push_back(glm::vec3(0.0f, -radius, 0.0f));

    // Generate edges
    int topPoleIndex = 0;
    int bottomPoleIndex = static_cast<int>(mesh.vertices.size()) - 1;

    // Top pole to first ring
    for (int s = 0; s < segments; s++) {
        int vertexIndex = 1 + s;
        mesh.edges.push_back(edge(topPoleIndex, vertexIndex));
    }

    // Intermediate rings (horizontal edges and vertical segments)
    for (int r = 0; r < rings - 2; r++) {
        int currentRingStart = 1 + r * segments;
        int nextRingStart = 1 + (r + 1) * segments;

        for (int s = 0; s < segments; s++) {
            int currentVertex = currentRingStart + s;
            int nextSegmentVertex = currentRingStart + (s + 1) % segments;
            int verticalVertex = nextRingStart + s;

            // Horizontal edge (along current ring)
            mesh.edges.push_back(edge(currentVertex, nextSegmentVertex));

            // Vertical edge (to next ring)
            mesh.edges.push_back(edge(currentVertex, verticalVertex));
        }
    }

    // Last ring horizontal edges
    int lastRingStart = 1 + (rings - 2) * segments;
    for (int s = 0; s < segments; s++) {
        int currentVertex = lastRingStart + s;
        int nextSegmentVertex = lastRingStart + (s + 1) % segments;
        mesh.edges.push_back(edge(currentVertex, nextSegmentVertex));
    }

    // Last ring to bottom pole
    for (int s = 0; s < segments; s++) {
        int vertexIndex = lastRingStart + s;
        mesh.edges.push_back(edge(vertexIndex, bottomPoleIndex));
    }

    return mesh;
}

wireframe_mesh generate_box(float width, float height, float depth) {
    wireframe_mesh mesh;

    // Half-extents
    float hw = width * 0.5f;
    float hh = height * 0.5f;
    float hd = depth * 0.5f;

    // 8 vertices (cube corners)
    mesh.vertices.push_back(glm::vec3(-hw, -hh, hd));  // 0: front-bottom-left
    mesh.vertices.push_back(glm::vec3(hw, -hh, hd));   // 1: front-bottom-right
    mesh.vertices.push_back(glm::vec3(-hw, hh, hd));   // 2: front-top-left
    mesh.vertices.push_back(glm::vec3(hw, hh, hd));    // 3: front-top-right
    mesh.vertices.push_back(glm::vec3(-hw, -hh, -hd)); // 4: back-bottom-left
    mesh.vertices.push_back(glm::vec3(hw, -hh, -hd));  // 5: back-bottom-right
    mesh.vertices.push_back(glm::vec3(-hw, hh, -hd));  // 6: back-top-left
    mesh.vertices.push_back(glm::vec3(hw, hh, -hd));   // 7: back-top-right

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

    float half_size = size * 0.5f;
    float step = size / divisions;

    // Generate vertices at grid intersections
    for (int z = 0; z <= divisions; z++) {
        for (int x = 0; x <= divisions; x++) {
            float px = -half_size + x * step;
            float pz = -half_size + z * step;
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
