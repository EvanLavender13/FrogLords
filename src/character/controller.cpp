#include "character/controller.h"
#include "foundation/collision.h"
#include "foundation/math_utils.h"
#include "input/input.h"
#include "input/keycodes.h"
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

void controller::apply_input(const camera_input_params& cam_params, float dt) {
    // Execute buffered jump on next grounded frame
    if (is_grounded && jump_buffer_timer > 0.0f) {
        velocity.y = jump_velocity;
        coyote_timer = coyote_window; // Exhaust coyote window
        jump_buffer_timer = 0.0f;     // Clear buffer
    }

    // Read WASD input
    glm::vec2 move_direction(0.0f, 0.0f);
    move_direction.y += input::is_key_down(SAPP_KEYCODE_W) ? 1.0f : 0.0f;
    move_direction.y -= input::is_key_down(SAPP_KEYCODE_S) ? 1.0f : 0.0f;
    move_direction.x -= input::is_key_down(SAPP_KEYCODE_A) ? 1.0f : 0.0f;
    move_direction.x += input::is_key_down(SAPP_KEYCODE_D) ? 1.0f : 0.0f;

    if (glm::length(move_direction) > 0.0f) {
        move_direction = glm::normalize(move_direction);
    }

    // Convert 2D input to 3D acceleration (camera-relative)
    glm::vec3 forward = cam_params.forward;
    glm::vec3 right = cam_params.right;

    input_direction = forward * move_direction.y + right * move_direction.x;

    // Direct acceleration (instant response)
    float accel_magnitude = is_grounded ? ground_accel : air_accel;
    acceleration = input_direction * accel_magnitude;

    // Jump input (space bar with coyote time and jump buffer)
    bool jump_input = input::is_key_pressed(SAPP_KEYCODE_SPACE);
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
    // Apply gravity
    acceleration.y += gravity;

    // Integrate velocity
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

    // Integrate position
    position += velocity * dt;

    // Resolve collisions and capture pre-collision state
    float pre_collision_vertical_velocity = velocity.y;
    resolve_collisions(collision_sphere, *world, position, velocity, is_grounded, ground_normal,
                       ground_height, max_slope_angle);

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
