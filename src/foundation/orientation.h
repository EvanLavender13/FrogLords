#pragma once
#include <glm/glm.hpp>
#include "foundation/spring_damper.h"

struct orientation_system {
    spring_damper yaw_spring;
    float min_speed = 0.01f; // m/s - minimum speed to update orientation

    orientation_system();

    void update(glm::vec3 velocity, float dt);
    float get_yaw() const { return yaw_spring.get_position(); }
};
