#include "vehicle/handbrake_system.h"
#include "vehicle/controller.h"
#include "foundation/debug_assert.h"
#include <glm/glm.hpp>

void handbrake_system::apply(bool input, controller& ctrl, float dt) {
    FL_PRECONDITION(dt > 0.0f && std::isfinite(dt), "dt must be positive and finite");
    FL_PRECONDITION(brake_strength >= 0.0f, "brake_strength must be non-negative");

    // Store handbrake state
    active = input;

    if (active) {
        // Brake force opposes velocity (like drag, but stronger)
        glm::vec3 brake_force = -ctrl.velocity * brake_strength;

        FL_ASSERT(std::isfinite(brake_force.x) && std::isfinite(brake_force.y) &&
                      std::isfinite(brake_force.z),
                  "brake_force must be finite");

        // Add brake force to acceleration (will be integrated in physics update)
        ctrl.acceleration += brake_force;
    }
}
