#include "locomotion.h"
#include <cmath>

locomotion_system::locomotion_system() {
    run_state.pass_pose = {
        .root_offset = glm::vec3(0.0f, 0.0f, 0.0f),
        .leg_phase_offset = 0.0f
    };
    run_state.reach_pose = {
        .root_offset = glm::vec3(0.0f, 0.15f, 0.0f),
        .leg_phase_offset = 0.5f
    };
    run_state.stride_length = 2.0f;
}

void locomotion_system::update(glm::vec3 ground_velocity, float dt, bool is_grounded) {
    if (is_grounded) {
        float speed = glm::length(ground_velocity);
        distance_traveled += speed * dt;
        phase = fmodf(distance_traveled / run_state.stride_length, 1.0f);
    }
}

simple_pose locomotion_system::get_current_pose() const {
    if (phase < 0.5f) {
        float t = phase * 2.0f;
        return lerp(run_state.pass_pose, run_state.reach_pose, t);
    } else {
        float t = (phase - 0.5f) * 2.0f;
        return lerp(run_state.reach_pose, run_state.pass_pose, t);
    }
}

simple_pose locomotion_system::lerp(const simple_pose& a, const simple_pose& b, float t) const {
    return {
        .root_offset = glm::mix(a.root_offset, b.root_offset, t),
        .leg_phase_offset = a.leg_phase_offset + (b.leg_phase_offset - a.leg_phase_offset) * t
    };
}
