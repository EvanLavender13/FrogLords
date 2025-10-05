#pragma once
#include <glm/glm.hpp>
#include "foundation/collision_primitives.h"

class camera;
class scene;

struct controller {
    // Collision volumes
    sphere bumper;       // Primary collision volume
    sphere weightlifter; // Step-up detection (smaller, offset below)

    // State (source of truth)
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;

    // Input intent (for animation/orientation when velocity is blocked)
    glm::vec3 input_direction;

    // Properties
    float mass = 70.0f;
    float friction = 0.9f;
    float air_control = 0.3f;

    // Ground state
    bool is_grounded = false;
    glm::vec3 ground_normal;
    float ground_height = 0.0f;

    // Tunable parameters
    float ground_accel = 20.0f;    // m/s^2
    float air_accel = 10.0f;       // m/s^2
    float max_speed = 8.0f;        // m/s
    float gravity = -9.8f;         // m/s^2
    float max_slope_angle = 45.0f; // degrees
    float step_height = 0.4f;      // meters
    float jump_velocity = 5.0f;    // m/s

    controller();

    void apply_input(const camera& cam, float dt);
    void update(const scene* scn, float dt);

  private:
    void resolve_ground_collision();
};
