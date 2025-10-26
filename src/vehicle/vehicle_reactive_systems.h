#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "foundation/spring_damper.h"
#include "foundation/orientation.h"

// Forward declaration
struct controller;

/**
 * vehicle_reactive_systems
 *
 * Reactive systems respond to controller physics state (read-only).
 * Data flows unidirectionally: controller → vehicle_reactive_systems → rendering
 *
 * Contrast with active systems (handbrake_system) which modify physics.
 * Composition: physics (controller) drives visual response (vehicle_reactive_systems)
 */
struct vehicle_reactive_systems {
    // Orientation system - smooth rotation from velocity direction
    orientation_system orientation;

    // Spring-damped visual state (derived from physics, not affecting physics)
    spring_damper lean_spring;  // radians (roll around forward axis)
    spring_damper pitch_spring; // radians (pitch around right axis)

    // History for acceleration derivation
    glm::vec3 previous_velocity{0.0f};

    // Tunable parameters
    // NOTE: These are conversion factors for visual-only arcade feedback, not physical properties.
    // Trade-off: Visual weight transfer for gameplay feel vs physical realism.
    // Real vehicles don't tilt bodies proportional to g-force (suspension compresses instead).
    // These parameters control exaggerated tilt for enhanced motion readability.
    float lean_multiplier = 0.3f;   // radians per g (arcade lean in corners, not physical)
    float pitch_multiplier = 0.05f; // radians per m/s² (arcade pitch during accel, not physical)

    vehicle_reactive_systems();

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

    // Spring parameter getters (single source of truth: springs own their parameters)
    float get_tilt_stiffness() const { return lean_spring.stiffness; }
};
