#pragma once

#include <glm/glm.hpp>

namespace easing {

float cubic_smooth(float t);

float smooth_mix(float a, float b, float t);

glm::vec3 smooth_mix(const glm::vec3& a, const glm::vec3& b, float t);

} // namespace easing
