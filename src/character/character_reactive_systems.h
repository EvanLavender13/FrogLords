#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "character/animation.h"
#include "foundation/orientation.h"

// Forward declaration
struct controller;

/**
 * character_reactive_systems
 *
 * Visual systems that react to controller physics state.
 * Data flows unidirectionally: controller → reactive_systems → rendering
 *
 * Composition: physics (controller) drives visual response (reactive_systems)
 */
struct character_reactive_systems {
    // Orientation system - smooth rotation from velocity
    orientation_system orientation;

    // Animation state - landing springs and vertical offset
    character::animation_state animation;

    character_reactive_systems() = default;

    /**
     * Update reactive systems from controller physics state
     * Call after controller.update() each frame
     *
     * @param ctrl Controller providing physics state (position, velocity, grounded)
     * @param dt Delta time in seconds
     */
    void update(const controller& ctrl, float dt);

    /**
     * Get visual transform for rendering
     * Composes controller position with orientation and animation offset
     *
     * @param ctrl Controller providing base position
     * @return Transform matrix for rendering character
     */
    glm::mat4 get_visual_transform(const controller& ctrl) const;
};
