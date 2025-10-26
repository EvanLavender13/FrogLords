#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace foundation {

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

struct sphere_config {
    int segments = 8;
    int rings = 8;
    float radius = 1.0f;
};

struct box_dimensions {
    float width = 1.0f;
    float height = 1.0f;
    float depth = 1.0f;
};

struct circle_config {
    float radius = 1.0f;
    int segments = 32;
};

/// Generate UV sphere wireframe
wireframe_mesh generate_sphere(sphere_config config = {});

/// Generate box wireframe
wireframe_mesh generate_box(box_dimensions dims = {});

/// Generate grid floor wireframe
wireframe_mesh generate_grid_floor(float size = 10.0f, int divisions = 10);

/// Generate arrow wireframe (line with cone head)
wireframe_mesh generate_arrow(const glm::vec3& start, const glm::vec3& end, float head_size = 0.1f);

/// Generate horizontal circle wireframe
wireframe_mesh generate_circle(const glm::vec3& center, circle_config config = {});

/// Generate horizontal arc wireframe between two directions
/// Arc sweeps from start_dir to end_dir in the horizontal plane (XZ)
/// @param center Arc center point (world position)
/// @param start_dir Starting direction (must be unit vector, horizontal)
/// @param end_dir Ending direction (must be unit vector, horizontal)
/// @param radius Arc radius (visualization scale)
/// @param segments Number of line segments (higher = smoother arc)
wireframe_mesh generate_arc(const glm::vec3& center, const glm::vec3& start_dir,
                            const glm::vec3& end_dir, float radius, int segments = 32);

/// Generate helix-style spring between two points (world-space vertices)
wireframe_mesh generate_spring(const glm::vec3& start, const glm::vec3& end, int coils = 6,
                               float radius = 0.05f);

} // namespace foundation
