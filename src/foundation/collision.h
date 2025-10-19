#pragma once

#include "foundation/collision_primitives.h"
#include <glm/glm.hpp>

struct sphere_collision {
    bool hit = false;
    glm::vec3 normal{0.0f};
    float penetration = 0.0f;
    const aabb* contact_box = nullptr;

    // Wall sliding debug info
    bool is_wall = false;
    glm::vec3 velocity_before{0.0f};
    glm::vec3 velocity_after{0.0f};
};

sphere_collision resolve_collisions(sphere&, const collision_world&, glm::vec3&, glm::vec3&);

sphere_collision resolve_sphere_aabb(const sphere& s, const aabb& box);
