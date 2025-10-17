#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "foundation/spring_damper.h"

namespace character {

struct animation_state {
    // Landing spring
    spring_damper landing_spring;
    float landing_impulse_scale = 0.5f;

    animation_state();

    void update_landing_spring(bool just_landed,
                               float vertical_velocity, // NOLINT
                               float dt);
    glm::mat4 get_vertical_offset_matrix() const;
    float get_vertical_offset() const;
};

} // namespace character
