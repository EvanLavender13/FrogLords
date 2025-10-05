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

float smooth_mix(scalar_span span, float t) {
    float weight = cubic_smooth(t);
    return span.start + (span.end - span.start) * weight;
}

glm::vec3 smooth_mix(const glm::vec3& a, const glm::vec3& b, float t) {
    float weight = cubic_smooth(t);
    return glm::mix(a, b, weight);
}

float cubic_hermite(float p0, float p1, float t) { // NOLINT(bugprone-easily-swappable-parameters)
    t = clamp01(t);
    // Hermite basis functions with zero tangents at endpoints
    // This ensures smooth velocity continuity between keyframes
    float t2 = t * t;
    float t3 = t2 * t;
    float h00 = 2.0f * t3 - 3.0f * t2 + 1.0f; // p0 weight
    float h01 = -2.0f * t3 + 3.0f * t2;       // p1 weight

    // Zero tangents (m0 = m1 = 0) for natural continuity
    return h00 * p0 + h01 * p1;
}

glm::vec3 cubic_hermite(const glm::vec3& p0, const glm::vec3& p1, float t) {
    t = clamp01(t);
    float t2 = t * t;
    float t3 = t2 * t;
    float h00 = 2.0f * t3 - 3.0f * t2 + 1.0f;
    float h01 = -2.0f * t3 + 3.0f * t2;

    return h00 * p0 + h01 * p1;
}

} // namespace easing
