#include "vehicle/vehicle_visual_systems.h"
#include "vehicle/controller.h"
#include "foundation/math_utils.h"
#include "foundation/debug_assert.h"

vehicle_visual_systems::vehicle_visual_systems() {
    // Initialize springs with critical damping for smooth, non-overshooting response
    lean_spring.stiffness = tilt_stiffness;
    lean_spring.damping = critical_damping(tilt_stiffness);

    pitch_spring.stiffness = tilt_stiffness;
    pitch_spring.damping = critical_damping(tilt_stiffness);
}

void vehicle_visual_systems::update(const controller& ctrl, float dt) {
    FL_PRECONDITION(dt > 0.0f && std::isfinite(dt), "dt must be positive and finite");

    // Update orientation from actual velocity (not input direction or heading)
    orientation.update(ctrl.velocity, dt);

    // Calculate target lean from lateral g-force (lean into turns)
    float lateral_g = ctrl.calculate_lateral_g_force();
    float target_lean = lateral_g * lean_multiplier;

    // Derive longitudinal acceleration from velocity change
    glm::vec3 acceleration = (ctrl.velocity - previous_velocity) / dt;

    // Project acceleration onto orientation direction for forward/back component
    float yaw = orientation.get_yaw();
    glm::vec3 forward = math::yaw_to_forward(yaw);
    float forward_accel = glm::dot(acceleration, forward);

    // Calculate target pitch from forward acceleration (pitch back when accelerating)
    float target_pitch = forward_accel * pitch_multiplier;

    // Spring-damp toward targets for smooth transitions
    lean_spring.update({.target = target_lean, .delta_time = dt});
    pitch_spring.update({.target = target_pitch, .delta_time = dt});

    // Store velocity for next frame
    previous_velocity = ctrl.velocity;

    FL_POSTCONDITION(std::isfinite(lean_spring.position), "lean position must be finite");
    FL_POSTCONDITION(std::isfinite(pitch_spring.position), "pitch position must be finite");
}

glm::mat4 vehicle_visual_systems::get_visual_transform(const controller& ctrl) const {
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
