#pragma once
#include <glm/glm.hpp>

// Input parameters for vehicle controller
// Separated into own header to avoid circular dependencies with friction_model
struct controller_input_params {
    glm::vec2 move_direction; // Normalized WASD-equivalent [-1,1] per axis
    float turn_input;         // Turn input for heading integration [-1,1]
    bool handbrake;           // Handbrake input (Space key)
};
