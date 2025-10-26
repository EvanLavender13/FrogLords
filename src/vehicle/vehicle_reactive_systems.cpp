#include "vehicle/vehicle_reactive_systems.h"
#include "vehicle/controller.h"
#include "foundation/math_utils.h"
#include "foundation/debug_assert.h"

vehicle_reactive_systems::vehicle_reactive_systems() {
    // Initialize springs with critical damping for smooth, non-overshooting response
    // Default stiffness: 150 N/m (mid-range for tilt response, tunable at runtime)
    constexpr float default_tilt_stiffness = 150.0f;

    lean_spring.stiffness = default_tilt_stiffness;
    lean_spring.damping = critical_damping(default_tilt_stiffness);

    pitch_spring.stiffness = default_tilt_stiffness;
    pitch_spring.damping = critical_damping(default_tilt_stiffness);
}

void vehicle_reactive_systems::update(const controller& ctrl, float dt) {
    FL_PRECONDITION(dt > 0.0f && std::isfinite(dt), "dt must be positive and finite");
    FL_PRECONDITION(lean_multiplier >= 0.0f && lean_multiplier <= 1.0f,
                    "lean_multiplier must be in valid range [0, 1] rad/g");
    FL_PRECONDITION(pitch_multiplier >= 0.0f && pitch_multiplier <= 0.2f,
                    "pitch_multiplier must be in valid range [0, 0.2] rad/(m/s²)");
    FL_PRECONDITION(lean_spring.stiffness >= 10.0f && lean_spring.stiffness <= 500.0f,
                    "lean_spring stiffness must be in valid range [10, 500] N/m");
    FL_PRECONDITION(pitch_spring.stiffness >= 10.0f && pitch_spring.stiffness <= 500.0f,
                    "pitch_spring stiffness must be in valid range [10, 500] N/m");

    // Update orientation from intended velocity (input direction × max speed)
    // Using input_direction ensures orientation responds immediately to player intent,
    // creating drift-like behavior where the vehicle can face different from travel direction
    glm::vec3 intended_velocity = ctrl.input_direction * ctrl.max_speed;
    orientation.update(intended_velocity, dt);

    // Calculate target lean from lateral g-force (lean into turns)
    float lateral_g = ctrl.calculate_lateral_g_force();
    FL_ASSERT(std::isfinite(lateral_g), "lateral_g must be finite");

    float target_lean = lateral_g * lean_multiplier;
    FL_ASSERT(std::isfinite(target_lean), "target_lean must be finite");

    // Derive longitudinal acceleration from velocity change
    glm::vec3 acceleration = (ctrl.velocity - previous_velocity) / dt;
    FL_ASSERT(std::isfinite(acceleration.x) && std::isfinite(acceleration.y) &&
                  std::isfinite(acceleration.z),
              "acceleration must be finite");

    // Project acceleration onto orientation direction for forward/back component
    float yaw = orientation.get_yaw();
    glm::vec3 forward = math::yaw_to_forward(yaw);
    float forward_accel = glm::dot(acceleration, forward);
    FL_ASSERT(std::isfinite(forward_accel), "forward_accel must be finite");

    // Calculate target pitch from forward acceleration (pitch back when accelerating)
    // Negate because positive forward_accel should pitch nose UP (negative rotation around right
    // axis)
    float target_pitch = -forward_accel * pitch_multiplier;
    FL_ASSERT(std::isfinite(target_pitch), "target_pitch must be finite");

    // Spring-damp toward targets for smooth transitions
    lean_spring.update({.target = target_lean, .delta_time = dt});
    pitch_spring.update({.target = target_pitch, .delta_time = dt});

    // Store velocity for next frame
    previous_velocity = ctrl.velocity;

    FL_POSTCONDITION(std::isfinite(lean_spring.position), "lean position must be finite");
    FL_POSTCONDITION(std::isfinite(pitch_spring.position), "pitch position must be finite");
}

glm::mat4 vehicle_reactive_systems::get_visual_transform(const controller& ctrl) const {
    glm::mat4 transform = glm::mat4(1.0f);

    // Translate to vehicle position
    transform = glm::translate(transform, ctrl.position);

    // Apply orientation rotation (yaw from velocity, not heading)
    float yaw = orientation.get_yaw();
    transform = glm::rotate(transform, yaw, math::UP);

    // Apply tilt rotations in local space (after yaw rotation)
    // Lean: roll around local forward axis (Z)
    transform = glm::rotate(transform, lean_spring.position, glm::vec3(0, 0, 1));

    // Pitch: rotation around local right axis (X)
    transform = glm::rotate(transform, pitch_spring.position, glm::vec3(1, 0, 0));

    return transform;
}
