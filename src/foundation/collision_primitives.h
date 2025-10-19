#pragma once
#include <glm/glm.hpp>
#include <vector>

struct sphere {
    glm::vec3 center{0.0f};
    float radius = 0.0f;
};

enum class collision_surface_type {
    NONE,     // Default, generic obstacle
    FLOOR,    // Supports standing, slope walking
    WALL,     // Blocks horizontal movement, enables wall-slide
    PLATFORM, // One-way passthrough from below
    DYNAMIC   // Movable objects (future)
};

struct aabb {
    glm::vec3 center{0.0f};
    glm::vec3 half_extents{0.0f};
};

struct collision_box {
    aabb bounds;
    collision_surface_type type = collision_surface_type::NONE;
};

struct collision_world {
    std::vector<collision_box> boxes;
};
