#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "foundation/spring_damper.h"
#include "foundation/orientation.h"

// Forward declaration
struct controller;

/**
 * vehicle_visual_systems
 *
 * Visual systems that react to vehicle controller physics state.
 * Data flows unidirectionally: controller → vehicle_visual_systems → rendering
 *
 * Composition: physics (controller) drives visual response (vehicle_visual_systems)
 */
struct vehicle_visual_systems {
    // Orientation system - smooth rotation from velocity direction
    orientation_system orientation;

    // Spring-damped visual state (derived from physics, not affecting physics)
    spring_damper lean_spring;   // radians (roll around forward axis)
    spring_damper pitch_spring;  // radians (pitch around right axis)

    // History for acceleration derivation
    glm::vec3 previous_velocity{0.0f};

    // Tunable parameters
    float lean_multiplier = 0.3f;   // radians per g (how much to lean in corners)
    float pitch_multiplier = 0.05f; // radians per m/s² (how much to pitch during accel)

    // Spring tuning
    float tilt_stiffness = 150.0f;  // Spring stiffness for tilt response

    vehicle_visual_systems();

    /**
     * Update visual systems from controller physics state
     * Call after controller.update() each frame
     *
     * @param ctrl Controller providing physics state (velocity, lateral g-force, heading)
     * @param dt Delta time in seconds
     */
    void update(const controller& ctrl, float dt);

    /**
     * Get visual transform for rendering
     * Composes controller position with heading and tilt rotations
     *
     * @param ctrl Controller providing base position and heading
     * @return Transform matrix for rendering vehicle
     */
    glm::mat4 get_visual_transform(const controller& ctrl) const;

    // Derived value getters for debugging/visualization
    float get_lean_angle() const { return lean_spring.get_position(); }
    float get_pitch_angle() const { return pitch_spring.get_position(); }
    float get_orientation_yaw() const { return orientation.get_yaw(); }
};
