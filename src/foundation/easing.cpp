#include "foundation/easing.h"
#include <algorithm>

namespace easing {
namespace {
float clamp01(float t) {
    return std::clamp(t, 0.0f, 1.0f);
}
} // namespace

float cubic_smooth(float t) {
    t = clamp01(t);
    return t * t * (3.0f - 2.0f * t);
}

glm::vec3 smooth_mix(const glm::vec3& a, const glm::vec3& b, float t) {
    float weight = cubic_smooth(t);
    return glm::mix(a, b, weight);
}

} // namespace easing
