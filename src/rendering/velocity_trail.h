#pragma once

#include <vector>
#include <glm/glm.hpp>

constexpr size_t MAX_TRAIL_SAMPLES = 25;

struct velocity_trail_state {
    std::vector<glm::vec3> positions;
    std::vector<float> timestamps;
    float sample_interval = 0.1f;
    float time_since_last_sample = 0.0f;
};
