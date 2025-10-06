#pragma once

#include "foundation/collision_primitives.h"
#include <glm/glm.hpp>

struct sphere_collision {
    bool hit = false;
    glm::vec3 normal{0.0f};
    float penetration = 0.0f;
};

sphere_collision resolve_sphere_aabb(const sphere& s, const aabb& box);
