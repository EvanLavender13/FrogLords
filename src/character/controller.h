#pragma once
#include <glm/glm.hpp>
#include "foundation/collision_primitives.h"
#include "character/animation.h"
#include "character/orientation.h"

struct collision_world;

struct controller {
    struct camera_input_params {
        glm::vec3 forward;
        glm::vec3 right;
    };

    // Collision volumes
    sphere collision_sphere; // Single sphere used for all collision

    // State (source of truth)
    glm::vec3 position;
    glm::vec3 velocity;
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
    };

    contact_debug_info collision_contact_debug;

    // Properties
    float friction = 0.9f;

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

    // Tunable parameters
    float ground_accel = 20.0f;       // m/s^2
    float air_accel = 10.0f;          // m/s^2
    float max_speed = 8.0f;           // m/s (movement speed cap)
    float gravity = -9.8f;            // m/s^2
    float max_slope_angle = 45.0f;    // degrees
    float jump_velocity = 5.0f;       // m/s
    float coyote_window = 0.15f;      // seconds (150ms default - window after leaving ground)
    float jump_buffer_window = 0.15f; // seconds (150ms default - window for buffered jump input)

    // Reactive animation layer
    character::animation_state animation;

    // Orientation system
    orientation_system orientation;

    controller();

    void apply_input(const camera_input_params& cam_params, float dt);
    void update(const collision_world* world, float dt);
    void update_reactive_systems(float dt);
    glm::mat4 get_world_transform() const;
};
