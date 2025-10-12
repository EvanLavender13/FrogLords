#pragma once
#include <glm/glm.hpp>
#include "foundation/collision_primitives.h"
#include "character/animation.h"
#include "character/orientation.h"

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
    float ground_accel = 20.0f; // m/s^2
    float air_accel = 10.0f;    // m/s^2
    float run_speed = 8.0f;     // m/s (reference run speed - tunable parameter)
    float max_speed = 8.0f;     // m/s (smoothed current speed limit - modified each frame)
    float target_max_speed =
        8.0f;                // m/s (target speed based on walk/run input, smoothly interpolated)
    float walk_speed = 2.0f; // m/s (shift key speed limit)
    float walk_transition_rate =
        10.0f;             // How quickly max_speed transitions between walk/run (larger = faster)
    float gravity = -9.8f; // m/s^2
    float max_slope_angle = 45.0f; // degrees
    float jump_velocity = 5.0f;    // m/s

    // Input modifiers
    bool is_walking = false; // True when shift key held

    // Reactive animation layer
    character::animation_state animation;

    // Orientation system
    orientation_system orientation;

    controller();

    void apply_input(const camera& cam, float dt);
    void update(const scene* scn, float dt);
    glm::mat4 get_world_transform() const;

  private:
    void resolve_collisions(const scene* scn, float& out_pre_collision_vertical_velocity);
    void resolve_ground_collision();
    void resolve_box_collisions(const scene& scn);
};
