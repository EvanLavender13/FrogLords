#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "foundation/spring_damper.h"

namespace character {

struct animation_state {
    // Landing spring
    spring_damper landing_spring;

    // COEFFICIENT: Scales vertical landing velocity to spring impulse magnitude
    // Physical meaning: Spring receives 50% of impact velocity as impulse
    // Purpose: Aesthetic tuning for landing crouch intensity (prevents excessive crouch)
    // Used in: update_landing_spring (line 18) when just_landed is true
    // Dimensionless multiplier: impulse = -|v_vertical| · landing_impulse_scale
    float landing_impulse_scale = 0.5f; // dimensionless

    animation_state();

    void update_landing_spring(bool just_landed,
                               float vertical_velocity, // NOLINT
                               float dt);
    glm::mat4 get_vertical_offset_matrix() const;
    float get_vertical_offset() const;
};

} // namespace character
