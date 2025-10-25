#include "character/character_reactive_systems.h"
#include "vehicle/controller.h"
#include "foundation/math_utils.h"

void character_reactive_systems::update(const controller& ctrl, float dt) {
    // Update orientation from intended velocity (input direction × max speed)
    // Using input_direction ensures orientation responds even when velocity is blocked by walls
    glm::vec3 intended_velocity = ctrl.input_direction * ctrl.max_speed;
    orientation.update(intended_velocity, dt);
}

glm::mat4 character_reactive_systems::get_visual_transform(const controller& ctrl) const {
    glm::mat4 transform = glm::mat4(1.0f);

    // Translate to character position
    transform = glm::translate(transform, ctrl.position);

    // Apply orientation (yaw rotation around Y axis)
    float yaw = orientation.get_yaw();
    transform = glm::rotate(transform, yaw, math::UP);

    // Apply landing spring vertical offset (crouch effect)
    transform *= animation.get_vertical_offset_matrix();

    return transform;
}
