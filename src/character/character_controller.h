#pragma once
#include <glm/glm.hpp>

class camera;
class scene;

struct sphere {
    glm::vec3 center;
    float radius;
};

struct character_controller {
    // Collision volumes
    sphere bumper;           // Primary collision volume
    sphere weightlifter;     // Step-up detection (smaller, offset below)

    // State (source of truth)
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;

    // Properties
    float mass;
    float friction;
    float air_control;

    // Ground state
    bool is_grounded;
    bool was_grounded_last_frame = false;
    glm::vec3 ground_normal;
    float ground_height = 0.0f;
    float landing_impact_velocity = 0.0f;

    // Tunable parameters
    float ground_accel = 20.0f;    // m/s²
    float air_accel = 10.0f;       // m/s²
    float max_speed = 8.0f;        // m/s
    float gravity = -9.8f;         // m/s²
    float max_slope_angle = 45.0f; // degrees
    float step_height = 0.4f;      // meters
    float jump_velocity = 5.0f;    // m/s

    character_controller();

    void apply_input(const camera& cam, float dt);
    void update(const scene* scn, float dt);
    float get_landing_impact() const;

private:
    void resolve_ground_collision();
    void resolve_box_collisions(const scene* scn);
    void detect_landing();
};
