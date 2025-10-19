#pragma once

#include "foundation/collision_primitives.h"
#include <glm/glm.hpp>

struct sphere_collision {
    bool hit = false;
    glm::vec3 normal{0.0f};
    float penetration = 0.0f;
    const aabb* contact_box = nullptr;

    // Surface type tracking (for grounding logic)
    // True if ANY contact during multi-pass resolution was a floor
    // Prevents losing grounded state when simultaneously touching floor + wall
    bool contacted_floor = false;
    glm::vec3 floor_normal{0.0f};

    // Wall sliding debug info
    bool is_wall = false;
};

sphere_collision resolve_collisions(sphere&, const collision_world&, glm::vec3&, glm::vec3&,
                                    float wall_threshold);

sphere_collision resolve_sphere_aabb(const sphere& s, const aabb& box);
