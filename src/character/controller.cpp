#include "character/controller.h"
#include "rendering/scene.h"
#include "camera/camera.h"
#include "input/input.h"
#include "sokol_app.h"
#include <glm/gtc/constants.hpp>
#include <algorithm>

namespace {
// Overgrowth-style configuration: Similar-sized spheres, lifter slightly buried
constexpr float BUMPER_RADIUS = 0.50f;
constexpr float WEIGHTLIFTER_RADIUS = 0.45f; // Nearly same size as bumper
constexpr float WEIGHTLIFTER_DROP = 0.10f;   // Small offset - lifter just below bumper
constexpr float STANDING_HEIGHT = WEIGHTLIFTER_RADIUS - 0.10f; // Lifter buried 0.10m in ground
const glm::vec3 WEIGHTLIFTER_OFFSET(0.0f, -WEIGHTLIFTER_DROP, 0.0f);
} // namespace

controller::controller()
    : position(0.0f, STANDING_HEIGHT,
               0.0f) // Start at standing height so weightlifter rests on ground
    , velocity(0.0f)
    , acceleration(0.0f)
    , ground_normal(0.0f, 1.0f, 0.0f) {
    // Initialize bumper sphere
    bumper.center = position;
    bumper.radius = BUMPER_RADIUS;

    // Initialize weightlifter sphere (offset below bumper)
    weightlifter.center = position + WEIGHTLIFTER_OFFSET;
    weightlifter.radius = WEIGHTLIFTER_RADIUS;
}

void controller::apply_input(const camera& cam, float dt) {
    // Read WASD input
    glm::vec2 move_direction(0.0f, 0.0f);

    if (input::is_key_down(SAPP_KEYCODE_W))
        move_direction.y += 1.0f;
    if (input::is_key_down(SAPP_KEYCODE_S))
        move_direction.y -= 1.0f;
    if (input::is_key_down(SAPP_KEYCODE_A))
        move_direction.x -= 1.0f;
    if (input::is_key_down(SAPP_KEYCODE_D))
        move_direction.x += 1.0f;

    if (glm::length(move_direction) > 0.0f) {
        move_direction = glm::normalize(move_direction);
    }

    // Convert 2D input to 3D acceleration (camera-relative)
    glm::vec3 forward = cam.get_forward_horizontal();
    glm::vec3 right = cam.get_right();

    input_direction = forward * move_direction.y + right * move_direction.x;

    // Direct acceleration (instant response)
    float accel_magnitude = is_grounded ? ground_accel : air_accel;
    acceleration = input_direction * accel_magnitude;

    // Jump input (space bar)
    if (input::is_key_pressed(SAPP_KEYCODE_SPACE) && is_grounded) {
        velocity.y = jump_velocity;
    }
}

void controller::update(const scene* scn, float dt) {
    (void) scn;
    // Apply gravity
    acceleration.y += gravity;

    // Integrate velocity
    velocity += acceleration * dt;

    // Apply friction (if grounded)
    if (is_grounded) {
        glm::vec3 horizontal_velocity = velocity;
        horizontal_velocity.y = 0.0f;

        float speed = glm::length(horizontal_velocity);
        if (speed > 0.0f) {
            float friction_decel = friction * std::abs(gravity) * dt;
            float new_speed = std::max(0.0f, speed - friction_decel);
            horizontal_velocity = horizontal_velocity * (new_speed / speed);

            velocity.x = horizontal_velocity.x;
            velocity.z = horizontal_velocity.z;
        }
    }

    // Apply max speed cap
    glm::vec3 horizontal_velocity = glm::vec3(velocity.x, 0, velocity.z);
    float speed = glm::length(horizontal_velocity);
    if (speed > max_speed) {
        horizontal_velocity = horizontal_velocity * (max_speed / speed);
        velocity.x = horizontal_velocity.x;
        velocity.z = horizontal_velocity.z;
    }

    // Integrate position
    position += velocity * dt;

    // Reset grounded state - will be set by collision resolution
    is_grounded = false;

    // Resolve collision against ground plane
    resolve_ground_collision();

    // Update collision volumes
    bumper.center = position;
    weightlifter.center = position + WEIGHTLIFTER_OFFSET;

    // Reset acceleration for next frame
    acceleration = glm::vec3(0, 0, 0);
}

void controller::resolve_ground_collision() {
    using namespace glm;

    // Only check ground plane if not already grounded by box collision
    if (is_grounded) {
        return;
    }

    // Update weightlifter position before collision check
    weightlifter.center = position + WEIGHTLIFTER_OFFSET;

    // Simple ground plane at y=0
    // Overgrowth-style: Allow lifter to remain buried ~0.10m in ground for stable contact
    constexpr float INTENDED_BURIAL_DEPTH = 0.10f;
    float ground_y = 0.0f;
    float distance_to_ground = weightlifter.center.y - ground_y;
    float penetration = weightlifter.radius - distance_to_ground;

    // Only resolve if penetration exceeds the intentional burial depth
    float excess_penetration = penetration - INTENDED_BURIAL_DEPTH;

    if (excess_penetration > 0.0f) {
        // Push weightlifter up only by the excess penetration
        weightlifter.center.y += excess_penetration;

        // Update position to maintain offset
        position = weightlifter.center - WEIGHTLIFTER_OFFSET;

        // Remove velocity into ground
        if (velocity.y < 0.0f) {
            velocity.y = 0.0f;
        }

        is_grounded = true;
        ground_normal = vec3(0, 1, 0);
        ground_height = ground_y;
    } else if (penetration > 0.0f) {
        // Within intended burial range - grounded but don't adjust position
        is_grounded = true;
        ground_normal = vec3(0, 1, 0);
        ground_height = ground_y;

        // Still remove downward velocity to prevent falling through
        if (velocity.y < 0.0f) {
            velocity.y = 0.0f;
        }
    } else {
        is_grounded = false;
    }
}
