#include "test_terrain.h"
#include <cmath>

using namespace glm;

float test_terrain::get_height(vec3 position, float character_y) const {
    float ground_height = 0.0f;

    // Ramp region (sloped surface from x=0 to x=5)
    if (position.x >= ramp_start.x && position.x <= ramp_end.x &&
        fabs(position.z) <= ramp_width * 0.5f) {
        float t = (position.x - ramp_start.x) / (ramp_end.x - ramp_start.x);
        ground_height = ramp_start.y + t * (ramp_end.y - ramp_start.y);
    }

    // Low platform - only if character is above it (prevents walking through sides)
    vec3 low_half_size = low_platform_size * 0.5f;
    if (fabs(position.x - low_platform_pos.x) <= low_half_size.x &&
        fabs(position.z - low_platform_pos.z) <= low_half_size.y) {
        float platform_top = low_platform_size.z;
        // Only use platform if character is above it (with small threshold)
        if (character_y > platform_top - 0.2f) {
            ground_height = fmaxf(ground_height, platform_top);
        }
    }

    // High platform - only if character is above it
    vec3 high_half_size = high_platform_size * 0.5f;
    if (fabs(position.x - high_platform_pos.x) <= high_half_size.x &&
        fabs(position.z - high_platform_pos.z) <= high_half_size.y) {
        float platform_top = high_platform_size.z;
        // Only use platform if character is above it (with small threshold)
        if (character_y > platform_top - 0.2f) {
            ground_height = fmaxf(ground_height, platform_top);
        }
    }

    return ground_height;
}

vec3 test_terrain::get_normal(vec3 position, float character_y) const {
    // Ramp has tilted normal perpendicular to slope
    if (position.x >= ramp_start.x && position.x <= ramp_end.x &&
        fabs(position.z) <= ramp_width * 0.5f) {
        vec3 ramp_direction = ramp_end - ramp_start;
        vec3 tangent = normalize(vec3(ramp_direction.x, ramp_direction.y, 0.0f));
        vec3 normal = vec3(-tangent.y, tangent.x, 0.0f);  // Perpendicular in XY plane
        return normalize(normal);
    }

    // Platforms and ground have vertical normal
    return vec3(0.0f, 1.0f, 0.0f);
}
