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

} // namespace easing
