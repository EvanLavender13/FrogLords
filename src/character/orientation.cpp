#include "character/orientation.h"
#include "foundation/math_utils.h"
#include <glm/gtc/constants.hpp>
#include <cmath>

void orientation_system::update(glm::vec3 velocity, float dt) {
    float speed = glm::length(velocity);

    if (speed > 0.01f) {
        target_yaw = atan2f(velocity.x, velocity.z);

        // Handle angle wrapping (shortest path)
        float delta = math::angle_difference_radians(target_yaw, current_yaw);

        current_yaw += delta * yaw_smoothing * dt;

        // Normalize to [-π, π]
        current_yaw = math::wrap_angle_radians(current_yaw);
    }
}
