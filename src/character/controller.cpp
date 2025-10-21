#include "character/controller.h"
#include "foundation/collision.h"
#include "foundation/math_utils.h"
#include "foundation/debug_assert.h"
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <algorithm>
#include <cmath>

namespace {
// Single-sphere collision configuration (experiment branch)
// Primary sphere handles all collision resolution

// TUNED: Collision sphere radius (character physical size)
// World scale: 0.5m radius = 1.0m diameter sphere (human-scale)
// Defines capsule/sphere collision volume for all character interactions
// Used in: controller constructor (line 35) to initialize collision_sphere.radius
constexpr float BUMPER_RADIUS = 0.50f; // meters

// CALCULATED: Initial spawn height for sphere resting on ground plane
// Geometric derivation: Sphere resting on Y=0 has center at Y = radius
// Contact point: center.y - radius = 0 → center.y = radius
// Used in: controller constructor (line 29) to set initial position.y
constexpr float STANDING_HEIGHT = BUMPER_RADIUS; // meters (= 0.5m)

void clamp_horizontal_speed(glm::vec3& velocity, float max_speed) {
    glm::vec3 horizontal_velocity = math::project_to_horizontal(velocity);
    float speed = glm::length(horizontal_velocity);
    if (speed > max_speed) {
        horizontal_velocity = horizontal_velocity * (max_speed / speed);
        velocity.x = horizontal_velocity.x;
        velocity.z = horizontal_velocity.z;
    }
}

} // namespace

controller::controller()
    : position(0.0f, STANDING_HEIGHT, 0.0f)
    , velocity(0.0f)
    , acceleration(0.0f)
    , locomotion{locomotion_speed_state::WALK, 0.0f, walk_cycle_length} {
    // Validate threshold ordering (prevents state collapse)
    FL_PRECONDITION(walk_threshold < run_threshold,
                    "walk_threshold must be less than run_threshold to define distinct states");

    // Initialize single collision sphere
    collision_sphere.center = position;
    collision_sphere.radius = BUMPER_RADIUS;
}

void controller::apply_input(const controller_input_params& input_params,
                             const camera_input_params& cam_params,
                             float dt) {
    // Execute buffered jump on next grounded frame (jump buffer forgiveness)
    // PRINCIPLE TRADE-OFF: See "PRINCIPLE TRADE-OFF: Coyote time and jump buffering" below for full
    // rationale
    if (is_grounded && jump_buffer_timer > 0.0f) {
        velocity.y = jump_velocity;
        coyote_timer = coyote_window; // Exhaust coyote window
        jump_buffer_timer = 0.0f;     // Clear buffer
    }

    // Integrate heading from turn input (physics primitive - always runs)
    // Time-independent heading integration: heading_yaw += turn_input * turn_rate * dt
    // Coordinate system: Y-up right-handed, positive yaw = CCW rotation from above
    // Input convention: negative = left turn, positive = right turn
    // Turn convention: left turn = +yaw (CCW), right turn = -yaw (CW)
    // Therefore: negate turn_input to map left → +yaw, right → -yaw
    FL_PRECONDITION(dt > 0.0f && std::isfinite(dt), "dt must be positive and finite for time-independent integration");
    FL_PRECONDITION(std::isfinite(turn_rate), "turn_rate must be finite");
    FL_PRECONDITION(std::isfinite(input_params.turn_input), "turn_input must be finite");
    FL_PRECONDITION(std::isfinite(heading_yaw), "heading_yaw must be finite before integration");

    heading_yaw += -input_params.turn_input * turn_rate * dt;
    heading_yaw = math::wrap_angle_radians(heading_yaw);

    FL_POSTCONDITION(std::isfinite(heading_yaw), "heading_yaw must remain finite after integration");
    FL_POSTCONDITION(heading_yaw >= -glm::pi<float>() && heading_yaw <= glm::pi<float>(),
                     "heading_yaw must be wrapped to [-π, π]");

    // Convert 2D input to 3D acceleration (basis-relative)
    // Basis provided by composition layer: camera vectors or heading vectors
    glm::vec3 forward = cam_params.forward;
    glm::vec3 right = cam_params.right;

    input_direction =
        forward * input_params.move_direction.y + right * input_params.move_direction.x;

    // Direct acceleration (instant response, no ground/air distinction)
    acceleration = input_direction * accel;

    // PRINCIPLE TRADE-OFF: Coyote time and jump buffering
    //
    // These mechanics intentionally violate the Consistency principle (jumping only when grounded)
    // in service of the Prime Directive (Do No Harm to Gameplay - preserve player intent).
    //
    // Coyote time: Allows jumping for brief period after leaving ground edge.
    //   - Forgives near-miss timing when player presses jump just after walking off ledge
    //   - Player intent: "I wanted to jump at the edge" > strict physics: "must be grounded"
    //
    // Jump buffering: Remembers jump input pressed shortly before landing.
    //   - Forgives near-miss timing when player presses jump just before hitting ground
    //   - Executes jump on next grounded frame instead of discarding input
    //
    // Principle hierarchy: Prime Directive (player control) > Consistency (behavioral rules).
    // These forgiveness mechanics preserve player intent, which is the higher truth.
    //
    // Design reference: Super Mario Bros (1985) - "press jump button a few frames early
    // and Mario will automatically jump the moment he touches down"
    // (NOTES/DesigningGames/DG_Interface.md)
    //
    // See PRINCIPLES.md - Prime Directive: "Do No Harm to Gameplay"
    //
    bool jump_input = input_params.jump_pressed;
    bool can_jump = is_grounded || (coyote_timer < coyote_window);

    if (jump_input && can_jump) {
        velocity.y = jump_velocity;
        coyote_timer = coyote_window; // Exhaust coyote window
        jump_buffer_timer = 0.0f;     // Clear any buffered input
    } else if (jump_input && !can_jump) {
        // Store buffered input for next valid grounded frame
        jump_buffer_timer = jump_buffer_window;
    }
}

void controller::update(const collision_world* world, float dt) {
    FL_PRECONDITION(dt > 0.0f, "dt must be positive for frame-rate independence");
    FL_PRECONDITION(std::isfinite(dt), "dt must be finite");

    // Frame-rate independent physics integration
    //
    // Horizontal velocity: Exponential drag model (exact solution)
    //   Solves: dv/dt = a - k*v  where k = accel/max_speed
    //   Solution: v(t+dt) = v(t)*exp(-k*dt) + (a/k)*(1 - exp(-k*dt))
    //
    //   Guarantees:
    //   - Equilibrium at max_speed when a = accel (full input)
    //   - Frame-rate independent: identical behavior at any dt
    //   - Allows overspeed: dash mechanics can exceed max_speed, decay back naturally
    //   - Exponential convergence: smooth approach to equilibrium
    //
    // Vertical velocity: Standard Euler integration (gravity only)
    //   No drag on vertical component - jump/fall physics unchanged
    //
    // See PRINCIPLES.md: Time-Independence, Solid Mathematical Foundations

    // Apply gravity to vertical acceleration
    acceleration.y += gravity;

    // Calculate drag coefficient from equilibrium constraint
    // Derivation: At equilibrium dv/dt = 0, so a - k*v_eq = 0
    //             Therefore: v_eq = a/k
    //             Want: v_eq = max_speed when a = accel (full input)
    //             Therefore: k = accel / max_speed
    float k = accel / max_speed;

    FL_POSTCONDITION(k > 0.0f && std::isfinite(k), "drag coefficient must be positive and finite");

    // Extract horizontal components
    glm::vec3 horizontal_accel = math::project_to_horizontal(acceleration);
    glm::vec3 horizontal_velocity = math::project_to_horizontal(velocity);

    // Store pre-update horizontal speed for assertion
    float speed_before = glm::length(horizontal_velocity);

    // Apply frame-rate independent horizontal update
    if (k < 1e-6f) {
        // Fallback for degenerate case (near-zero accel or very large max_speed)
        // Standard Euler integration when drag is negligible
        horizontal_velocity += horizontal_accel * dt;
    } else {
        // Exact exponential solution
        float decay = std::exp(-k * dt);
        horizontal_velocity = horizontal_velocity * decay + (horizontal_accel / k) * (1.0f - decay);
    }

    // Integrate vertical velocity (standard Euler - no drag)
    velocity.y += acceleration.y * dt;

    // Reconstruct full velocity (horizontal + vertical)
    velocity.x = horizontal_velocity.x;
    velocity.z = horizontal_velocity.z;

    // Zero-velocity tolerance: exponential decay never fully stops
    // When horizontal speed drops below perceptible threshold, snap to zero
    // Prevents residual drift from numerical precision limits
    constexpr float VELOCITY_EPSILON = 0.01f; // m/s (imperceptible at 60fps)
    float horizontal_speed = glm::length(horizontal_velocity);
    if (horizontal_speed < VELOCITY_EPSILON) {
        velocity.x = 0.0f;
        velocity.z = 0.0f;
        horizontal_speed = 0.0f;
    }

    // Validate frame-rate independent drag behavior
    // When no input applied, drag should decrease speed (or maintain zero)
    float accel_magnitude = glm::length(horizontal_accel);
    constexpr float ACCEL_EPSILON = 0.01f; // m/s² (negligible acceleration)
    if (accel_magnitude < ACCEL_EPSILON) {
        // No input: speed must decrease or stay at zero
        // Allow small epsilon for numerical precision (1% tolerance)
        FL_POSTCONDITION(horizontal_speed <= speed_before * 1.01f,
                         "horizontal speed must decrease when no input applied");
    }

    // Integrate position (accumulate - required for physics)
    position += velocity * dt;

    // Resolve collisions (pure physics - returns contact data)
    // Derive wall threshold from max_slope_angle (single source of truth)
    float wall_threshold = glm::cos(glm::radians(max_slope_angle));
    float pre_collision_vertical_velocity = velocity.y;
    sphere_collision contact =
        resolve_collisions(collision_sphere, *world, position, velocity, wall_threshold);

    // Store collision debug info
    collision_contact_debug.active = contact.hit;
    collision_contact_debug.normal = contact.normal;
    collision_contact_debug.penetration = contact.penetration;
    collision_contact_debug.is_wall = contact.is_wall;

    // Interpret contact to determine grounded state (controller logic)
    // Use contacted_floor flag to handle simultaneous floor+wall contacts
    is_grounded = false;
    if (contact.contacted_floor) {
        is_grounded = true;
    }

    // Landing detection (after collision resolution, using pre-collision velocity)
    just_landed = !was_grounded && is_grounded;
    if (just_landed) {
        vertical_velocity_on_land = pre_collision_vertical_velocity;
    }
    was_grounded = is_grounded;

    // Update jump timing forgiveness timers
    if (is_grounded) {
        coyote_timer = 0.0f; // Reset when grounded
    } else {
        coyote_timer += dt; // Accumulate time since leaving ground
    }
    jump_buffer_timer = std::max(0.0f, jump_buffer_timer - dt); // Decay toward zero

    // Update locomotion state (speed classification + phase calculation)
    // Phase is an OUTPUT computed from movement, never drives physics
    float speed = glm::length(math::project_to_horizontal(velocity));
    FL_POSTCONDITION(speed >= 0.0f, "speed must be non-negative (magnitude property)");
    FL_POSTCONDITION(std::isfinite(speed), "speed must be finite");

    // Classify speed into discrete locomotion states
    if (speed < walk_threshold) {
        locomotion.state = locomotion_speed_state::WALK;
    } else if (speed < run_threshold) {
        locomotion.state = locomotion_speed_state::RUN;
    } else {
        locomotion.state = locomotion_speed_state::SPRINT;
    }

    // Accumulate distance traveled (frame-rate independent)
    // NOTE: distance_traveled is internal state, NOT part of locomotion_state output
    distance_traveled += speed * dt;
    FL_POSTCONDITION(std::isfinite(distance_traveled), "distance_traveled must remain finite");

    // Calculate phase (0-1 normalized position within cycle)
    // IMPORTANT: Phase is derived from distance_traveled, which is the source of truth
    // When state changes → cycle_length changes → phase recalculates from same distance
    // This causes phase value to jump, but preserves physical correctness
    // (the surveyor wheel re-scales, distance/rotation is preserved)
    locomotion.cycle_length = get_cycle_length(locomotion.state);
    FL_PRECONDITION(locomotion.cycle_length > 0.0f, "cycle_length must be positive");
    locomotion.phase =
        std::fmod(distance_traveled, locomotion.cycle_length) / locomotion.cycle_length;
    FL_POSTCONDITION(locomotion.phase >= 0.0f && locomotion.phase < 1.0f,
                     "phase must be in [0, 1) range");

    // Save acceleration for animation system (before reset)
    last_acceleration = acceleration;

    // Reset acceleration for next frame
    acceleration = glm::vec3(0, 0, 0);
}

float controller::get_cycle_length(locomotion_speed_state state) const {
    switch (state) {
    case locomotion_speed_state::WALK:
        return walk_cycle_length;
    case locomotion_speed_state::RUN:
        return run_cycle_length;
    case locomotion_speed_state::SPRINT:
        return sprint_cycle_length;
    }
    // Should never reach here - all enum values handled
    FL_ASSERT(false, "invalid locomotion_speed_state in get_cycle_length");
    return walk_cycle_length; // Unreachable, but satisfies compiler
}
