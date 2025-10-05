#pragma once
#include <glm/glm.hpp>
#include "foundation/collision_primitives.h"
#include "character/animation.h"

class camera;
class scene;

struct controller {
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

    // Tunable parameters
    float ground_accel = 20.0f;    // m/s^2
    float air_accel = 10.0f;       // m/s^2
    float max_speed = 8.0f;        // m/s
    float gravity = -9.8f;         // m/s^2
    float max_slope_angle = 45.0f; // degrees
    float jump_velocity = 5.0f;    // m/s

    // Reactive animation layer
    character::animation_state animation;

    controller();

    void apply_input(const camera& cam, float dt);
    void update(const scene* scn, float dt);

  private:
    void resolve_ground_collision();
    void resolve_box_collisions(const scene& scn);
};
