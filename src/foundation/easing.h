#pragma once

#include <glm/glm.hpp>

namespace easing {

struct scalar_span {
    float start = 0.0f;
    float end = 0.0f;
};

float cubic_smooth(float t);

float smooth_mix(scalar_span span, float t);

glm::vec3 smooth_mix(const glm::vec3& a, const glm::vec3& b, float t);

// Cubic Hermite interpolation with zero tangents (ensures velocity continuity)
// p0: start position, p1: end position, t: interpolation parameter [0,1]
float cubic_hermite(float p0, float p1, float t);

glm::vec3 cubic_hermite(const glm::vec3& p0, const glm::vec3& p1, float t);

} // namespace easing
