#include "camera/dynamic_fov.h"
#include "camera/camera.h"
#include "vehicle/controller.h"
#include "foundation/debug_assert.h"
#include <glm/glm.hpp>
#include <algorithm>
#include <cmath>

dynamic_fov_system::dynamic_fov_system() {
    // Initialize spring with moderate stiffness for smooth FOV transitions
    // Mid-range value balances responsiveness and smoothness
    constexpr float default_fov_stiffness = 150.0f;

    fov_spring.position = base_fov; // Start at base FOV
    fov_spring.stiffness = default_fov_stiffness;
    fov_spring.damping = critical_damping(default_fov_stiffness);
}

void dynamic_fov_system::update(const controller& ctrl, camera& cam, float dt) {
    // Precondition: delta_time must be positive
    FL_PRECONDITION(dt > 0.0f, "Delta time must be positive for spring integration");

    // Update spring damping to maintain critical damping (no overshoot)
    fov_spring.damping = critical_damping(fov_spring.stiffness);

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

    // Clamp target to valid range before spring integration
    float max_allowed_fov = base_fov + max_fov_range;
    float clamped_target_fov = std::clamp(target_fov, base_fov, max_allowed_fov);

    // Integrate spring toward clamped target
    spring_step step{.target = clamped_target_fov, .delta_time = dt};
    fov_spring.update(step);

    // Get smoothed FOV from spring position
    float smoothed_fov = fov_spring.get_position();

    // Clamp after spring integration to enforce hard bounds
    smoothed_fov = std::clamp(smoothed_fov, base_fov, max_allowed_fov);

    FL_POSTCONDITION(smoothed_fov >= base_fov && smoothed_fov <= max_allowed_fov,
                     "FOV must be within [base_fov, base_fov + max_fov_range]");

    // Write to camera
    cam.set_fov(smoothed_fov);
}
