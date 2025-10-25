#include "foundation/orientation.h"
#include "foundation/math_utils.h"
#include <glm/gtc/constants.hpp>

// TUNED: Spring parameters for yaw orientation smoothing
// Stiffness k=25 → natural frequency ω=√k=5 rad/s → response time ~0.2s
// Damping ζ=1.0 (critical) → no overshoot, smooth settle
// Matches previous exponential smoothing feel (rate=5.0, τ=0.2s)
orientation_system::orientation_system() {
    yaw_spring.stiffness = 25.0f;
    yaw_spring.damping = critical_damping(25.0f); // = 10.0
    yaw_spring.position = 0.0f;
    yaw_spring.velocity = 0.0f;
}

void orientation_system::update(glm::vec3 velocity, float dt) {
    float speed = glm::length(velocity);

    // TUNED: Minimum speed threshold to update yaw orientation
    // Purpose: Prevent jitter/chatter when velocity is near-zero
    // Value: 0.01 m/s (1 cm/s) is well below perceptual threshold
    // Ensures character maintains orientation when effectively stationary
    if (speed > 0.01f) { // m/s
        float target_yaw = glm::atan(velocity.x, velocity.z);

        // Handle angle wrapping (shortest path)
        float current = yaw_spring.get_position();
        float delta = math::angle_difference_radians(target_yaw, current);
        float wrapped_target = current + delta;

        // Update spring toward wrapped target
        yaw_spring.update({wrapped_target, dt});

        // Normalize output to [-π, π]
        yaw_spring.position = math::wrap_angle_radians(yaw_spring.position);
    }
}
