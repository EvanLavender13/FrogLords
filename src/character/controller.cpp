#include "character/controller.h"
#include "foundation/collision.h"
#include "rendering/scene.h"
#include "camera/camera.h"
#include "input/input.h"
#include "sokol_app.h"
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <algorithm>
#include <cstdio>

namespace {
// Single-sphere collision configuration (experiment branch)
// Primary sphere handles all collision resolution
constexpr float BUMPER_RADIUS = 0.50f;
constexpr float STANDING_HEIGHT = BUMPER_RADIUS; // Spawn with sphere resting on ground

// (Surface classification constants removed as part of single-sphere simplification)
} // namespace

controller::controller()
    : position(0.0f, STANDING_HEIGHT, 0.0f)
    , velocity(0.0f)
    , acceleration(0.0f)
    , ground_normal(0.0f, 1.0f, 0.0f) {
    // Initialize single collision sphere
    collision_sphere.center = position;
    collision_sphere.radius = BUMPER_RADIUS;
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

    // Reset grounded state
    is_grounded = false;
    collision_contact_debug = contact_debug_info{};

    // Update collision sphere position
    collision_sphere.center = position;

    // Capture vertical velocity BEFORE collision resolution (for landing spring)
    float pre_collision_vertical_velocity = velocity.y;

    // Box collision resolution
    if (scn != nullptr) {
        resolve_box_collisions(*scn);
    }

    // Ground plane (fallback if no box collision)
    resolve_ground_collision();

    // Landing detection (after collision resolution, using pre-collision velocity)
    just_landed = !was_grounded && is_grounded;
    if (just_landed) {
        vertical_velocity_on_land = pre_collision_vertical_velocity;
        printf("[LANDING] just_landed=true, velocity.y = %.3f\n", pre_collision_vertical_velocity);
    }
    was_grounded = is_grounded;

    // Save acceleration for animation system (before reset)
    last_acceleration = acceleration;

    // Reset acceleration for next frame
    acceleration = glm::vec3(0, 0, 0);
}

void controller::resolve_ground_collision() {
    using namespace glm;

    // Only check ground plane if not already grounded by box collision
    if (is_grounded) {
        return;
    }

    // Simple ground plane at y=0 - single sphere collision
    float ground_y = 0.0f;
    float distance_to_ground = collision_sphere.center.y - ground_y;
    float penetration = collision_sphere.radius - distance_to_ground;

    if (penetration > 0.0f) {
        // Push sphere up to rest on surface
        collision_sphere.center.y += penetration;
        position = collision_sphere.center;

        // Remove velocity into ground
        if (velocity.y < 0.0f) {
            velocity.y = 0.0f;
        }

        is_grounded = true;
        ground_normal = vec3(0, 1, 0);
        ground_height = ground_y;

        collision_contact_debug.active = true;
        collision_contact_debug.from_box = false;
        collision_contact_debug.normal = vec3(0, 1, 0);
        collision_contact_debug.penetration = 0.0f;
        collision_contact_debug.vertical_penetration = 0.0f;
    } else {
        is_grounded = false;
    }
}

void controller::resolve_box_collisions(const scene& scn) {
    // EXPERIMENT: Single-sphere collision system
    // One sphere handles ALL collision - no dual-sphere complexity
    // Goal: Test if simpler system feels more responsive and stable

    // Multi-pass collision resolution to handle corner cases
    for (int pass = 0; pass < 3; ++pass) {
        bool any_collision = false;

        for (const auto& box : scn.collision_boxes()) {
            sphere_collision col = resolve_sphere_aabb(collision_sphere, box);

            if (col.hit) {
                // Push out of collision
                position += col.normal * col.penetration;
                collision_sphere.center = position;

                // Check if this is a ground surface (upward-facing normal)
                if (col.normal.y >= glm::cos(glm::radians(max_slope_angle))) {
                    // Set grounded state
                    is_grounded = true;
                    ground_normal = col.normal;
                    ground_height = box.center.y + box.half_extents.y;
                }

                // Record all collisions for debug
                collision_contact_debug.active = true;
                collision_contact_debug.from_box = true;
                collision_contact_debug.normal = col.normal;
                collision_contact_debug.penetration = col.penetration;
                collision_contact_debug.vertical_penetration =
                    col.penetration * std::max(col.normal.y, 0.0f);

                // Remove velocity into surface
                float vel_into_surface = glm::dot(velocity, col.normal);
                if (vel_into_surface < 0.0f) {
                    velocity -= col.normal * vel_into_surface;
                }

                any_collision = true;
            }
        }

        if (!any_collision)
            break; // Early exit if no collisions in this pass
    }
}
