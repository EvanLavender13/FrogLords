#include "character/controller.h"
#include "foundation/collision.h"
#include "foundation/math_utils.h"
#include "foundation/debug_assert.h"
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <algorithm>

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

} // namespace

controller::controller()
    : position(0.0f, STANDING_HEIGHT, 0.0f)
    , velocity(0.0f)
    , acceleration(0.0f)
    , ground_normal(math::UP)
    , locomotion{locomotion_speed_state::walk, 0.0f, walk_cycle_length} {
    // Validate threshold ordering (prevents state collapse)
    FL_PRECONDITION(walk_threshold < run_threshold,
                    "walk_threshold must be less than run_threshold to define distinct states");

    // Validate dash parameters (prevent division by zero in debug viz)
    FL_PRECONDITION(dash_cooldown > 0.0f, "dash_cooldown must be positive");

    // Initialize single collision sphere
    collision_sphere.center = position;
    collision_sphere.radius = BUMPER_RADIUS;

    // Calculate drag coefficient for equilibrium at max_speed
    recalculate_drag_coefficient();
}

void controller::apply_input(const controller_input_params& input_params,
                             const camera_input_params& cam_params, float dt) {
    // Execute buffered jump on next grounded frame (jump buffer forgiveness)
    // PRINCIPLE TRADE-OFF: See "PRINCIPLE TRADE-OFF: Coyote time and jump buffering" below for full
    // rationale
    if (is_grounded && jump_buffer_timer > 0.0f) {
        velocity.y = jump_velocity;
        coyote_timer = coyote_window; // Exhaust coyote window
        jump_buffer_timer = 0.0f;     // Clear buffer
    }

    // Convert 2D input to 3D acceleration (camera-relative)
    glm::vec3 forward = cam_params.forward;
    glm::vec3 right = cam_params.right;

    input_direction =
        forward * input_params.move_direction.y + right * input_params.move_direction.x;

    // Direct acceleration (same in air and on ground)
    // Velocity-dependent friction creates equilibrium, prevents infinite acceleration
    acceleration = input_direction * ground_accel;

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

    // Dash: Apply instant impulse in input direction
    // Cooldown prevents spam
    // Velocity-dependent friction naturally decelerates from overspeed
    bool dash_input = input_params.dash_pressed;
    bool can_dash_now = is_grounded && dash_timer <= 0.0f;

    if (dash_input && can_dash_now) {
        velocity += input_direction * dash_impulse;
        dash_timer = dash_cooldown; // Start cooldown
    }
}

void controller::update(const collision_world* world, float dt) {
    FL_PRECONDITION(dt > 0.0f, "dt must be positive for frame-rate independence");
    FL_PRECONDITION(std::isfinite(dt), "dt must be finite");

    // Physics integration using semi-implicit Euler method (Symplectic Euler):
    //
    // Integration order:
    //   1. v(t+dt) = v(t) + a(t)·dt     (velocity first, using current acceleration)
    //   2. x(t+dt) = x(t) + v(t+dt)·dt  (position second, using NEW velocity)
    //
    // Properties:
    //   - Stable for damped systems (friction, collision resolution)
    //   - Better energy conservation than explicit Euler
    //   - Sufficient accuracy for platformer physics (no rigid body dynamics)
    //   - First-order accuracy: O(dt)
    //
    // Alternatives considered:
    //   - Verlet integration: Better energy conservation, but requires code refactor
    //   - RK4 (Runge-Kutta): Overkill for platformer (4x computational cost)
    //
    // Trade-off accepted: Semi-implicit Euler is fast, stable, and sufficient.
    // See PRINCIPLES.md for accumulated state exception (physics integration).

    // Apply gravity
    acceleration.y += gravity;

    // Integrate velocity (accumulate - required for physics)
    velocity += acceleration * dt;

    // Apply velocity-dependent friction (same everywhere - ground and air)
    // Total friction = base_friction·|g| (contact/air resistance) + drag·speed (velocity-dependent)
    // At max_speed: friction = ground_accel (equilibrium, net acceleration = 0)
    // Below max_speed: friction < accel (net positive acceleration)
    // Above max_speed: friction > accel (net negative acceleration, natural deceleration)
    {
        glm::vec3 horizontal_velocity = math::project_to_horizontal(velocity);
        float speed = glm::length(horizontal_velocity);
        if (speed > 0.0f) {
            // Apply full friction everywhere (consistent physics)
            float base_friction_decel = base_friction * std::abs(gravity);
            float drag_decel = drag_coefficient * speed;
            float total_friction_decel = (base_friction_decel + drag_decel) * dt;

            // Apply friction by reducing speed
            float new_speed = std::max(0.0f, speed - total_friction_decel);
            glm::vec3 direction = horizontal_velocity / speed; // Normalize
            velocity.x = direction.x * new_speed;
            velocity.z = direction.z * new_speed;
        }
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
        ground_normal = contact.floor_normal;
        // Query box geometry directly (no interpretation in collision system)
        ground_height = contact.contact_box->center.y + contact.contact_box->half_extents.y;
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

    // Update dash cooldown timer (frame-rate independent decay)
    dash_timer = std::max(0.0f, dash_timer - dt);

    // Update locomotion state (speed classification + phase calculation)
    // Phase is an OUTPUT computed from movement, never drives physics
    float speed = glm::length(math::project_to_horizontal(velocity));
    FL_POSTCONDITION(speed >= 0.0f, "speed must be non-negative (magnitude property)");
    FL_POSTCONDITION(std::isfinite(speed), "speed must be finite");

    // Classify speed into discrete locomotion states
    if (speed < walk_threshold) {
        locomotion.state = locomotion_speed_state::walk;
    } else if (speed < run_threshold) {
        locomotion.state = locomotion_speed_state::run;
    } else {
        locomotion.state = locomotion_speed_state::sprint;
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
    case locomotion_speed_state::walk:
        return walk_cycle_length;
    case locomotion_speed_state::run:
        return run_cycle_length;
    case locomotion_speed_state::sprint:
        return sprint_cycle_length;
    }
    // Should never reach here - all enum values handled
    FL_ASSERT(false, "invalid locomotion_speed_state in get_cycle_length");
    return walk_cycle_length; // Unreachable, but satisfies compiler
}
