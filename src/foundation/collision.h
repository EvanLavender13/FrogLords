#pragma once

#include "foundation/collision_primitives.h"
#include <glm/glm.hpp>

struct sphere_collision {
    bool hit = false;
    glm::vec3 normal{0.0f};
    float penetration = 0.0f;
    const aabb* contact_box = nullptr;
};

void resolve_collisions(sphere&, const collision_world&, glm::vec3&, glm::vec3&, bool&, glm::vec3&,
                        float&, float);

sphere_collision resolve_sphere_aabb(const sphere& s, const aabb& box);
