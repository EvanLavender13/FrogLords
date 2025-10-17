#include "animation.h"
#include "foundation/math_utils.h"
#include <cmath>
#include <glm/gtc/constants.hpp>
#include <glm/common.hpp>

namespace character {

animation_state::animation_state() {
    // TUNED: Landing spring stiffness (controls oscillation frequency)
    // Natural frequency: ω = √(k/m) = √(400/1) = 20 rad/s ≈ 3.18 Hz
    // With critical damping: Settles in ~2τ = 2/(ζω) = 2/20 = 0.1s (fast, snappy)
    // Used in: spring force calculation F = -k·(x - target)
    landing_spring.stiffness = 400.0f; // 1/s² (unitless spring, assumes mass=1)

    // DERIVED: Critical damping coefficient (prevents overshoot/oscillation)
    // Formula: c = 2√(k·m) with k=400, m=1 → c = 2√400 = 40.0
    // Damping ratio ζ = 1.0 (critically damped, no overshoot)
    // Result: Landing crouch settles smoothly to rest without bounce
    landing_spring.damping = critical_damping(landing_spring.stiffness); // 1/s (= 40.0)
}

void animation_state::update_landing_spring(bool just_landed,
                                            float vertical_velocity, // NOLINT
                                            float dt) {
    if (just_landed) {
        float impulse = -glm::abs(vertical_velocity) * landing_impulse_scale;
        landing_spring.add_impulse(impulse);
    }

    spring_step step{.target = 0.0f, .delta_time = dt};
    landing_spring.update(step);
}

glm::mat4 animation_state::get_vertical_offset_matrix() const {
    return glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, landing_spring.get_position(), 0.0f));
}

float animation_state::get_vertical_offset() const {
    return landing_spring.get_position();
}

} // namespace character
