#include "character/controller.h"
#include "foundation/collision.h"
#include "foundation/math_utils.h"
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
    , ground_normal(math::UP) {
    // Initialize single collision sphere
    collision_sphere.center = position;
    collision_sphere.radius = BUMPER_RADIUS;
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

    // Direct acceleration (instant response)
    float accel_magnitude = is_grounded ? ground_accel : air_accel;
    acceleration = input_direction * accel_magnitude;

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

    // Apply friction (if grounded)
    if (is_grounded) {
        float speed = glm::length(math::project_to_horizontal(velocity));
        if (speed > 0.0f) {
            float friction_decel = friction * std::abs(gravity) * dt;
            float new_speed = std::max(0.0f, speed - friction_decel);
            clamp_horizontal_speed(velocity, new_speed);
        }
    }

    // Apply speed cap
    clamp_horizontal_speed(velocity, max_speed);

    // Integrate position (accumulate - required for physics)
    position += velocity * dt;

    // Resolve collisions (pure physics - returns contact data)
    float pre_collision_vertical_velocity = velocity.y;
    sphere_collision contact = resolve_collisions(collision_sphere, *world, position, velocity);

    // Interpret contact to determine grounded state (controller logic)
    is_grounded = false;
    if (contact.hit && contact.normal.y >= glm::cos(glm::radians(max_slope_angle))) {
        is_grounded = true;
        ground_normal = contact.normal;
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

    // Save acceleration for animation system (before reset)
    last_acceleration = acceleration;

    // Reset acceleration for next frame
    acceleration = glm::vec3(0, 0, 0);
}
