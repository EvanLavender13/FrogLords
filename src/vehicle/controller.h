#pragma once
#include <glm/glm.hpp>
#include "foundation/collision_primitives.h"
#include "foundation/param_meta.h"

struct collision_world;

struct controller {
    struct camera_input_params {
        glm::vec3 forward;
        glm::vec3 right;
    };

    struct controller_input_params {
        glm::vec2 move_direction; // Normalized WASD-equivalent [-1,1] per axis
        float turn_input;         // Turn input for heading integration [-1,1]
    };

    // Collision volumes
    sphere collision_sphere; // Single sphere used for all collision

    // State (source of truth)
    // NOTE: Position and velocity are ACCUMULATED STATE (integrated over time).
    // This is the CORRECT pattern for physics simulation - see PRINCIPLES.md.
    // Physics integration requires accumulation - this is NOT a violation.
    // Exception: Time integration of motion is different from caching/memoization.
    glm::vec3 position; // meters (integrated from velocity)
    glm::vec3 velocity; // meters/second (integrated from acceleration)
    glm::vec3 acceleration;

    // Input intent (for animation/orientation when velocity is blocked)
    glm::vec3 input_direction;

    struct contact_debug_info {
        bool active = false;
        bool from_box = false;
        glm::vec3 normal{0.0f};
        float penetration = 0.0f;
        float vertical_penetration = 0.0f;
    };

    contact_debug_info collision_contact_debug;

    // Properties
    // NOTE: Friction removed - drag now derived from accel/max_speed
    // See controller::update for exponential drag model: k = accel / max_speed

    // Ground state
    bool is_grounded = false;

    // Tunable parameters
    // TUNED: Horizontal acceleration (direct from tuning system)
    // Controls responsiveness - higher = snappier movement feel
    // Used in: apply_input to set acceleration magnitude
    // Used in: update to calculate drag coefficient k = accel / max_speed
    float accel = 5.0f; // m/s²

    // TUNED: Maximum horizontal movement speed
    // Real-world context: 8.0 m/s ≈ 17.9 mph (fast jogging pace)
    // Foundation for acceleration calculations in tuning system
    // Used in: clamp_horizontal_speed, update_reactive_systems
    float max_speed = 8.0f; // m/s

    // PHYSICAL: Vehicle weight force (downward)
    // Acts as constant downward force keeping vehicle on track
    // Sign convention: Negative because Y-up coordinate system (weight pulls down)
    // Used in: velocity integration for vertical physics
    float weight = -9.8f; // m/s² (acceleration, will convert to force with mass later)

    // TUNED: Maximum walkable slope angle threshold (SINGLE SOURCE OF TRUTH)
    // Surfaces steeper than this are walls, not ground
    // Industry standard: 30-50° (Quake/Half-Life: ~45°)
    // Used in: controller::move - derived as wall_threshold = cos(radians(max_slope_angle))
    //          Passed to collision system for surface classification
    // Note: This is the authoritative value - collision system derives threshold from this
    float max_slope_angle = 45.0f; // degrees

    // TUNED: Turn rate for car-like control heading
    // Controls rotational speed when using heading-based movement
    // Higher values = faster turning (arcade feel)
    // Integrated in: controller::apply_input() - heading_yaw += -input.x * turn_rate * dt
    float turn_rate = 3.0f; // radians/second

    // TUNED: Steering reduction factor for speed-dependent steering limits
    // Controls how much steering authority decreases at high speeds
    // Formula: multiplier = 1.0 - (speed/max_speed) * reduction_factor
    // At max_speed with reduction_factor=0.7: 30% steering authority remains
    // Used in: compute_steering_multiplier() to scale turn_rate
    float steering_reduction_factor = 0.7f; // dimensionless [0,1]

    // Car-like control heading (physics state)
    // Updated from turn input (A/D keys), used when composition layer
    // selects heading-based movement basis (instead of camera basis)
    float heading_yaw = 0.0f; // radians

    // Angular velocity (derived per-frame from heading change)
    // DERIVED STATE: Calculated from heading_yaw delta, not accumulated
    // Used for lateral g-force calculation and drift detection
    float angular_velocity = 0.0f; // radians/second

    // Previous heading for angular velocity derivation
    float previous_heading_yaw = 0.0f; // radians

    // Speed thresholds for traction classification (m/s)
    float soft_threshold = 3.0f;   // soft < 3 m/s (low speed, high grip)
    float medium_threshold = 6.0f; // medium: 3-6 m/s, hard ≥ 6 m/s (high speed)

    controller();

    void apply_input(const controller_input_params& input_params,
                     const camera_input_params& cam_params, float dt);
    void update(const collision_world* world, float dt);

    // Compute speed-dependent steering multiplier [0, 1]
    // Returns 1.0 at zero speed (full steering), decreases with speed
    // Clamped to prevent negative values when speed exceeds max_speed
    float compute_steering_multiplier(float horizontal_speed) const;

    // Calculate slip angle between velocity direction and heading direction
    // Returns signed angle in radians [-π, π]:
    //   Positive: velocity points right of heading
    //   Negative: velocity points left of heading
    //   Zero: moving straight or stationary
    float calculate_slip_angle() const;

    // Calculate lateral g-force from current speed and angular velocity
    // Returns dimensionless g-force multiplier:
    //   Positive: centripetal acceleration points right (right turn)
    //   Negative: centripetal acceleration points left (left turn)
    //   Zero: moving straight or stationary
    float calculate_lateral_g_force() const;

  private:
    // Physics integration: weight, drag, velocity, position
    void update_physics(float dt);
    // Collision resolution and grounding detection
    void update_collision(const collision_world* world, float dt);
};
