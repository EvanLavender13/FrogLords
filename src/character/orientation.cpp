#include "character/orientation.h"
#include <glm/gtc/constants.hpp>
#include <cmath>

void orientation_system::update(glm::vec3 velocity, float dt) {
    float speed = glm::length(velocity);

    if (speed > 0.01f) {
        target_yaw = atan2f(velocity.x, velocity.z);

        // Handle angle wrapping (shortest path)
        float delta = target_yaw - current_yaw;
        while (delta > glm::pi<float>())
            delta -= 2.0f * glm::pi<float>();
        while (delta < -glm::pi<float>())
            delta += 2.0f * glm::pi<float>();

        current_yaw += delta * yaw_smoothing * dt;

        // Normalize to [-π, π]
        while (current_yaw > glm::pi<float>())
            current_yaw -= 2.0f * glm::pi<float>();
        while (current_yaw < -glm::pi<float>())
            current_yaw += 2.0f * glm::pi<float>();
    }
}
