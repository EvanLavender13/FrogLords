#pragma once
#include <glm/glm.hpp>
#include <vector>

struct sphere {
    glm::vec3 center{0.0f};
    float radius = 0.0f;
};

struct aabb {
    glm::vec3 center{0.0f};
    glm::vec3 half_extents{0.0f};
};

struct collision_world {
    std::vector<aabb> boxes;
};
