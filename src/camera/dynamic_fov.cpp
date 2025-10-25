#include "camera/dynamic_fov.h"
#include "camera/camera.h"
#include "vehicle/controller.h"
#include "foundation/debug_assert.h"
#include <glm/glm.hpp>
#include <algorithm>
#include <cmath>

void dynamic_fov_system::update(const controller& ctrl, camera& cam) {
    // Calculate current speed (magnitude of velocity)
    float speed = glm::length(ctrl.velocity);

    // Calculate lateral g-force from physics state
    float lateral_g = ctrl.calculate_lateral_g_force();

    // Guard against division by zero
    constexpr float SPEED_EPSILON = 0.001f; // m/s
    float safe_max_speed = std::max(ctrl.max_speed, SPEED_EPSILON);

    // Calculate speed contribution: saturate to [0,1] before scaling
    float speed_ratio = speed / safe_max_speed;
    float speed_factor = std::clamp(speed_ratio, 0.0f, 1.0f);
    float speed_contribution = speed_factor * max_fov_range;

    // Calculate g-force contribution: use absolute value (symmetric effect)
    float g_contribution = std::abs(lateral_g) * g_multiplier;

    // Combine contributions
    float target_fov = base_fov + speed_contribution + g_contribution;

    // Clamp to valid range before writing
    float max_allowed_fov = base_fov + max_fov_range;
    float clamped_fov = std::clamp(target_fov, base_fov, max_allowed_fov);

    // Postcondition: FOV always within valid range
    FL_POSTCONDITION(clamped_fov >= base_fov && clamped_fov <= max_allowed_fov,
                     "FOV must be within [base_fov, base_fov + max_fov_range]");

    // Write to camera
    cam.set_fov(clamped_fov);
}
