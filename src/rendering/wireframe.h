#pragma once

#include <glm/glm.hpp>
#include <vector>

// Simple edge structure - pair of vertex indices
struct edge {
    int v0, v1;

    edge(int vertex0, int vertex1)
        : v0(vertex0)
        , v1(vertex1) {}
};

/// Wireframe mesh: vertices, edges, and transform
struct wireframe_mesh {
    std::vector<glm::vec3> vertices;
    std::vector<edge> edges;

    glm::vec3 position; // World position
    glm::vec3 rotation; // Euler angles (radians)
    glm::vec3 scale;    // Per-axis scale

    wireframe_mesh();

    /// Compute model matrix from position/rotation/scale
    glm::mat4 get_model_matrix() const;
};

/// Generate UV sphere wireframe
/// @param segments Vertical divisions (longitude lines)
/// @param rings Horizontal divisions (latitude lines)
/// @param radius Sphere radius
wireframe_mesh generate_sphere(int segments = 8, int rings = 8, float radius = 1.0f);

/// Generate box wireframe
/// @param width Box extent along X axis
/// @param height Box extent along Y axis
/// @param depth Box extent along Z axis
wireframe_mesh generate_box(float width = 1.0f, float height = 1.0f, float depth = 1.0f);

/// Generate grid floor wireframe
/// @param size Total size of the floor
/// @param divisions Number of grid divisions per axis
wireframe_mesh generate_grid_floor(float size = 10.0f, int divisions = 10);

/// Generate arrow wireframe (line with cone head)
/// @param start Arrow start position
/// @param end Arrow end position
/// @param head_size Size of arrowhead cone
wireframe_mesh generate_arrow(const glm::vec3& start, const glm::vec3& end, float head_size = 0.1f);

/// Generate horizontal circle wireframe
/// @param center Circle center position
/// @param radius Circle radius
/// @param segments Number of line segments
wireframe_mesh generate_circle(const glm::vec3& center, float radius, int segments = 32);

/// Generate helix-style spring between two points (world-space vertices)
/// @param start Spring start point
/// @param end Spring end point
/// @param coils Number of helical turns
/// @param radius Spring radius
wireframe_mesh generate_spring(const glm::vec3& start, const glm::vec3& end, int coils = 6,
                               float radius = 0.05f);
