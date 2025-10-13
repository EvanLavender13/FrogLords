#include "character/locomotion.h"
#include "foundation/easing.h"
#include <algorithm>
#include <cmath>

locomotion_system::locomotion_system() {
    // Walk keyframes (lower vertical bob)
    walk_state.pass_pose = {.root_offset = glm::vec3(0.0f, 0.0f, 0.0f), .leg_phase_offset = 0.0f};
    walk_state.reach_pose = {.root_offset = glm::vec3(0.0f, 0.08f, 0.0f), .leg_phase_offset = 0.5f};
    walk_state.stride_length = 1.2f;

    // Run keyframes (higher vertical bob)
    run_state.pass_pose = {.root_offset = glm::vec3(0.0f, 0.0f, 0.0f), .leg_phase_offset = 0.0f};
    run_state.reach_pose = {.root_offset = glm::vec3(0.0f, 0.15f, 0.0f), .leg_phase_offset = 0.5f};
    run_state.stride_length = 2.0f;
}

void locomotion_system::update(glm::vec3 ground_velocity, float dt) {
    // Calculate current speed from horizontal velocity (XZ plane)
    current_speed = glm::length(ground_velocity);

    // Smooth speed to prevent erratic behavior
    smoothed_speed = smoothed_speed + (current_speed - smoothed_speed) * speed_smoothing * dt;

    // Calculate blend factor using smoothed speed
    float blend = 0.0f;
    if (smoothed_speed <= walk_speed_threshold) {
        blend = 0.0f; // Pure walk
    } else if (smoothed_speed >= run_speed_threshold) {
        blend = 1.0f; // Pure run
    } else {
        blend =
            (smoothed_speed - walk_speed_threshold) / (run_speed_threshold - walk_speed_threshold);
    }

    // Blend stride length with eased blend weight
    float blended_stride = easing::smooth_mix(
        easing::scalar_span{walk_state.stride_length, run_state.stride_length}, blend);
    if (blended_stride <= 0.0f) {
        phase = 0.0f;
    } else {
        float drive_speed = std::max(0.0f, current_speed);

        float new_phase = phase + (drive_speed / blended_stride) * dt;

        phase = std::fmod(new_phase, 1.0f);
        if (phase < 0.0f) {
            phase += 1.0f;
        }

        // Accumulate distance continuously (surveyor-wheel odometer)
        distance_traveled += drive_speed * dt;
    }
}

simple_pose locomotion_system::get_current_pose() const {
    // Calculate blend factor using smoothed speed
    float blend = 0.0f;
    if (smoothed_speed <= walk_speed_threshold) {
        blend = 0.0f;
    } else if (smoothed_speed >= run_speed_threshold) {
        blend = 1.0f;
    } else {
        blend =
            (smoothed_speed - walk_speed_threshold) / (run_speed_threshold - walk_speed_threshold);
    }

    // Get walk and run poses at current phase
    simple_pose walk_pose, run_pose;
    if (phase < 0.5f) {
        float t = phase * 2.0f;
        walk_pose = cubic_interp(walk_state.pass_pose, walk_state.reach_pose, t);
        run_pose = cubic_interp(run_state.pass_pose, run_state.reach_pose, t);
    } else {
        float t = (phase - 0.5f) * 2.0f;
        walk_pose = cubic_interp(walk_state.reach_pose, walk_state.pass_pose, t);
        run_pose = cubic_interp(run_state.reach_pose, run_state.pass_pose, t);
    }

    // Blend between walk and run with eased weight
    simple_pose blended = lerp(walk_pose, run_pose, blend);

    return blended;
}

simple_pose locomotion_system::lerp(const simple_pose& a, const simple_pose& b, float t) const {
    return {.root_offset = easing::smooth_mix(a.root_offset, b.root_offset, t),
            .leg_phase_offset =
                easing::smooth_mix(easing::scalar_span{a.leg_phase_offset, b.leg_phase_offset}, t)};
}

simple_pose locomotion_system::cubic_interp(const simple_pose& a, const simple_pose& b,
                                            float t) const {
    // Cubic Hermite with zero tangents ensures velocity continuity between keyframes
    return {.root_offset = easing::cubic_hermite(a.root_offset, b.root_offset, t),
            .leg_phase_offset = easing::cubic_hermite(a.leg_phase_offset, b.leg_phase_offset, t)};
}
