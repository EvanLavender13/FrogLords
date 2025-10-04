#pragma once
#include <glm/glm.hpp>

/// Simple heightfield test terrain for Phase 3.5 validation
/// Provides ground height and normal queries for ramps and platforms
struct test_terrain {
    // Ramp configuration (0-5m on X, rising 2m)
    glm::vec3 ramp_start = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 ramp_end = glm::vec3(5.0f, 2.0f, 0.0f);
    float ramp_width = 3.0f;

    // Low platform (easy jump test)
    glm::vec3 low_platform_pos = glm::vec3(8.0f, 0.0f, 0.0f);
    glm::vec3 low_platform_size = glm::vec3(3.0f, 3.0f, 1.5f); // width, depth, height

    // High platform (heavy landing test)
    glm::vec3 high_platform_pos = glm::vec3(12.0f, 0.0f, 0.0f);
    glm::vec3 high_platform_size = glm::vec3(3.0f, 3.0f, 3.0f);

    /// Query ground height at XZ position
    /// character_y: current character Y position for context-aware surface selection
    float get_height(glm::vec3 position, float character_y) const;

    /// Query ground normal at XZ position
    glm::vec3 get_normal(glm::vec3 position, float character_y) const;
};
