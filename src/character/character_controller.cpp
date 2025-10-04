#include "character/character_controller.h"
#include "rendering/scene.h"
#include "camera/camera.h"
#include "input/input.h"
#include "foundation/collision.h"
#include "sokol_app.h"
#include <glm/gtc/constants.hpp>
#include <algorithm>

character_controller::character_controller()
    : position(0.0f, 0.4f, 0.0f)  // Adjusted so weightlifter touches ground (accounts for 0.3m spring offset)
    , velocity(0.0f)
    , acceleration(0.0f)
    , mass(70.0f)
    , friction(0.9f)
    , air_control(0.3f)
    , is_grounded(false)
    , ground_normal(0.0f, 1.0f, 0.0f)
{
    // Initialize bumper sphere
    bumper.center = position;
    bumper.radius = 0.5f;

    // Initialize weightlifter sphere (offset below bumper)
    weightlifter.center = position + glm::vec3(0.0f, -0.4f, 0.0f);
    weightlifter.radius = 0.3f;
}

void character_controller::apply_input(const camera& cam, float dt) {
    // Read WASD input
    glm::vec2 move_direction(0.0f, 0.0f);

    if (input::is_key_down(SAPP_KEYCODE_W)) move_direction.y += 1.0f;
    if (input::is_key_down(SAPP_KEYCODE_S)) move_direction.y -= 1.0f;
    if (input::is_key_down(SAPP_KEYCODE_A)) move_direction.x -= 1.0f;
    if (input::is_key_down(SAPP_KEYCODE_D)) move_direction.x += 1.0f;

    if (glm::length(move_direction) > 0.0f) {
        move_direction = glm::normalize(move_direction);
    }

    // Convert 2D input to 3D acceleration (camera-relative)
    glm::vec3 forward = cam.get_forward_horizontal();
    glm::vec3 right = cam.get_right();

    glm::vec3 input_direction = forward * move_direction.y + right * move_direction.x;

    // Direct acceleration (instant response)
    float accel_magnitude = is_grounded ? ground_accel : air_accel;
    acceleration = input_direction * accel_magnitude;

    // Jump input (space bar)
    if (input::is_key_pressed(SAPP_KEYCODE_SPACE) && is_grounded) {
        velocity.y = jump_velocity;
    }
}

void character_controller::update(const scene* scn, float dt) {
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

    // Capture velocity before collision resolution zeroes it
    float pre_collision_velocity_y = velocity.y;

    // Reset grounded state - will be set by collision resolution
    is_grounded = false;

    // Resolve collisions (boxes first, then ground plane)
    resolve_box_collisions(scn);
    resolve_ground_collision();

    // Detect landing (airborne -> grounded transition with downward velocity)
    bool just_landed = is_grounded && !was_grounded_last_frame;
    if (just_landed && pre_collision_velocity_y < 0.0f) {
        landing_impact_velocity = -pre_collision_velocity_y;
    } else {
        landing_impact_velocity = 0.0f;
    }

    // Update state for next frame
    was_grounded_last_frame = is_grounded;

    // Update collision volumes
    bumper.center = position;
    weightlifter.center = position + glm::vec3(0.0f, -0.4f, 0.0f);

    // Reset acceleration for next frame
    acceleration = glm::vec3(0, 0, 0);
}

void character_controller::resolve_ground_collision() {
    using namespace glm;

    // Only check ground plane if not already grounded by box collision
    if (is_grounded) {
        return;
    }

    // Update weightlifter position before collision check
    weightlifter.center = position + vec3(0.0f, -0.4f, 0.0f);

    // Simple ground plane at y=0
    float ground_y = 0.0f;
    float distance_to_ground = weightlifter.center.y - ground_y;
    float penetration = weightlifter.radius - distance_to_ground;

    if (penetration > 0.0f) {
        // Push weightlifter out of ground
        weightlifter.center.y += penetration;

        // Update position to maintain offset
        position = weightlifter.center + vec3(0.0f, 0.4f, 0.0f);

        // Remove velocity into ground
        if (velocity.y < 0.0f) {
            velocity.y = 0.0f;
        }

        is_grounded = true;
        ground_normal = vec3(0, 1, 0);
        ground_height = ground_y;
    } else {
        is_grounded = false;
    }
}

void character_controller::resolve_box_collisions(const scene* scn) {
    if (!scn) return;

    using namespace glm;

    for (const auto& box : scn->collision_boxes()) {
        sphere weightlifter_copy = weightlifter;
        weightlifter_copy.center = position + vec3(0.0f, -0.4f, 0.0f);

        sphere_collision weightlifter_hit = resolve_sphere_aabb(weightlifter_copy, box);
        if (weightlifter_hit.hit) {
            vec3 collision_normal = weightlifter_hit.normal;

            vec3 weightlifter_displacement = weightlifter_copy.center - weightlifter.center;
            position += weightlifter_displacement;
            weightlifter.center = weightlifter_copy.center;

            float velocity_into_surface = dot(velocity, collision_normal);
            if (velocity_into_surface < 0.0f) {
                velocity -= collision_normal * velocity_into_surface;
            }

            if (collision_normal.y > 0.7f) {
                is_grounded = true;
                ground_normal = collision_normal;
                ground_height = box.center.y + box.half_extents.y;
            }
        }

        sphere bumper_copy = bumper;
        bumper_copy.center = position;

        sphere_collision bumper_hit = resolve_sphere_aabb(bumper_copy, box);
        if (bumper_hit.hit) {
            vec3 collision_normal = bumper_hit.normal;

            position = bumper_copy.center;

            float velocity_into_surface = dot(velocity, collision_normal);
            if (velocity_into_surface < 0.0f) {
                velocity -= collision_normal * velocity_into_surface;
            }
        }
    }
}

void character_controller::detect_landing() {
    // Deprecated - landing detection now inline in update()
}

float character_controller::get_landing_impact() const {
    return landing_impact_velocity;
}
