#pragma once
#include <glm/glm.hpp>
#include "foundation/collision_primitives.h"

struct collision_world;

struct controller {
    struct camera_input_params {
        glm::vec3 forward;
        glm::vec3 right;
    };

    struct controller_input_params {
        glm::vec2 move_direction; // Normalized WASD-equivalent [-1,1] per axis
        bool jump_pressed;        // True on frame of jump press
        bool dash_pressed;        // True on frame of dash press
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
    glm::vec3 last_acceleration; // Previous frame's acceleration (for animation)

    // Input intent (for animation/orientation when velocity is blocked)
    glm::vec3 input_direction;

    struct contact_debug_info {
        bool active = false;
        bool from_box = false;
        glm::vec3 normal{0.0f};
        float penetration = 0.0f;
        float vertical_penetration = 0.0f;

        // Wall sliding debug info
        bool is_wall = false;
    };

    contact_debug_info collision_contact_debug;

    // Properties
    // COEFFICIENT: Friction as multiple of gravity magnitude
    // Deceleration when grounded: friction · |g| = 0.9 · 9.8 ≈ 8.82 m/s²
    // Provides rapid stopping while maintaining control
    // Used in: controller::update (line 92)
    float friction = 0.9f; // dimensionless

    // Ground state
    bool is_grounded = false;
    bool was_grounded = false;
    bool just_landed = false; // Set when landing occurs, cleared after animation reads it
    float vertical_velocity_on_land = 0.0f;
    glm::vec3 ground_normal;
    float ground_height = 0.0f;

    // Jump timing forgiveness
    float coyote_timer = 0.0f;      // Time since leaving ground (for coyote time)
    float jump_buffer_timer = 0.0f; // Time since jump input pressed (for buffered jump)

    // Dash state
    float dash_timer = 0.0f; // Time remaining until next dash available (cooldown)
    float dash_active_timer = 0.0f; // Time remaining in committed dash state

    // Jump state (to preserve momentum when jumping)
    bool skip_friction_this_frame = false; // True when jump triggered, prevents friction from eating momentum

    // Tunable parameters
    // CALCULATED: Ground acceleration (derived from tuning.h defaults)
    // Derived from time_to_max_speed via: ground_accel = max_speed / time
    // Default: 20.0 m/s² matches tuning.h time_to_max_speed=0.4s
    // Formula: 8.0 m/s / 0.4s = 20.0 m/s²
    // NOTE: Will be overwritten if tuning_params::apply_to() is called
    // Used in: apply_input (line 64) to set acceleration magnitude when grounded
    float ground_accel = 20.0f; // m/s²

    // CALCULATED: Air acceleration (derived from tuning.h defaults)
    // Derived from time_to_max_speed via: air_accel = max_speed / time
    // Default: 20.0 m/s² matches tuning.h time_to_max_speed=0.4s
    // Formula: 8.0 m/s / 0.4s = 20.0 m/s²
    // NOTE: Will be overwritten if tuning_params::apply_to() is called
    // Used in: apply_input (line 64) to set acceleration magnitude when airborne
    float air_accel = 20.0f; // m/s²

    // TUNED: Maximum horizontal movement speed
    // Real-world context: 8.0 m/s ≈ 17.9 mph (fast jogging pace)
    // Foundation for acceleration calculations in tuning system
    // Used in: clamp_horizontal_speed, update_reactive_systems
    float max_speed = 8.0f; // m/s

    // PHYSICAL: Earth's gravitational acceleration at sea level
    // Standard value in game physics for familiar jump feel
    // Sign convention: Negative because Y-up coordinate system (gravity pulls down)
    // Used in: velocity integration (line 83), friction calculation (line 92), jump calculations
    float gravity = -9.8f; // m/s²

    // TUNED: Maximum walkable slope angle threshold (SINGLE SOURCE OF TRUTH)
    // Surfaces steeper than this are walls, not ground
    // Industry standard: 30-50° (Quake/Half-Life: ~45°)
    // Used in: controller::move - derived as wall_threshold = cos(radians(max_slope_angle))
    //          Passed to collision system for surface classification (is_wall)
    // Note: This is the authoritative value - collision system derives threshold from this
    float max_slope_angle = 45.0f; // degrees

    // CALCULATED: Vertical velocity required to reach target jump height
    // Using kinematic equation: v = √(2·|g|·h)
    // With h=1.3m, g=-9.8m/s²: v = √(2·9.8·1.3) ≈ 5.048 m/s
    // Current value 5.0 m/s is 1% lower (acceptable tuning adjustment)
    // Modified by tuning system: jump_velocity = √(2·|gravity|·jump_height)
    // Used in: apply_input (lines 41, 72) when jump triggered
    float jump_velocity = 5.0f; // m/s

    // TUNED: Coyote time - jump grace period after leaving ground
    // Industry standard: 100-200ms (Celeste: 140ms, Super Meat Boy: 160ms)
    // Allows jump input slightly after leaving ledge (forgiving platforming)
    // Player can jump if (is_grounded || coyote_timer < coyote_window)
    // Used in: apply_input (line 69) for jump eligibility check
    float coyote_window = 0.15f; // seconds (150ms)

    // TUNED: Jump buffer time - pre-input grace period before landing
    // Industry standard: 100-200ms (matches coyote_window timing)
    // Allows jump input slightly before landing (forgiving platforming)
    // Buffered jump executes on next grounded frame (line 40)
    // Used in: apply_input (lines 40, 77) for buffered jump handling
    float jump_buffer_window = 0.15f; // seconds (150ms)

    // TUNED: Dash impulse - instant velocity boost magnitude
    // Applied directly to velocity in input direction
    // Higher values = stronger burst, longer time to decelerate
    // Used in: apply_input for dash mechanic
    float dash_impulse = 6.0f; // m/s (velocity boost)
    float dash_active_duration = 0.3f; // seconds (committed dash state duration)

    // TUNED: Dash cooldown - time between dash uses
    // Prevents spam while allowing fluid movement
    // Industry standard: 0.5-1.5s (depends on game feel)
    // Used in: apply_input to reset timer, update to decay timer
    float dash_cooldown = 0.8f; // seconds

    // Locomotion state (speed tiers + phase for cyclic motion)
    enum class locomotion_speed_state { walk, run, sprint };

    struct locomotion_state {
        locomotion_speed_state state;
        float phase;        // 0-1 normalized position within cycle
        float cycle_length; // Current cycle length for this state (meters)
    };

    locomotion_state locomotion;

    // Internal state (not exposed in locomotion_state output)
    float distance_traveled = 0.0f; // Accumulated horizontal distance (meters)

    // Speed thresholds for state classification (m/s)
    float walk_threshold = 3.0f; // walk < 3 m/s
    float run_threshold = 6.0f;  // run: 3-6 m/s, sprint ≥ 6 m/s

    // Cycle lengths per state (meters)
    // Faster gaits = longer strides
    float walk_cycle_length = 2.0f;   // walk: 2m per cycle (two steps)
    float run_cycle_length = 3.0f;    // run: 3m per cycle
    float sprint_cycle_length = 4.0f; // sprint: 4m per cycle

    controller();

    void apply_input(const controller_input_params& input_params,
                     const camera_input_params& cam_params, float dt);
    void update(const collision_world* world, float dt);

    // Query helpers (used by debug viz)
    bool can_dash() const { return is_grounded && dash_timer <= 0.0f && dash_active_timer <= 0.0f; }
    bool is_dashing() const { return dash_active_timer > 0.0f; }

  private:
    // Pure function: map state → cycle length (INTERNAL USE ONLY)
    // External consumers should use locomotion.cycle_length (the output struct)
    float get_cycle_length(locomotion_speed_state state) const;
};
