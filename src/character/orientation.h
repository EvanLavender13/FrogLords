#pragma once
#include <glm/glm.hpp>
#include "foundation/spring_damper.h"

struct orientation_system {
    spring_damper yaw_spring;

    orientation_system();

    void update(glm::vec3 velocity, float dt);
    float get_yaw() const { return yaw_spring.get_position(); }
};
