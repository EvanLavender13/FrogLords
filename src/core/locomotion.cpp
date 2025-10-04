#include "locomotion.h"
#include "easing.h"
#include <cmath>

locomotion_system::locomotion_system() {
    // Walk keyframes (no vertical offset - spring-damper will handle vertical motion in Phase 3)
    walk_state.pass_pose = {
        .root_offset = glm::vec3(0.0f, 0.0f, 0.0f),
        .leg_phase_offset = 0.0f
    };
    walk_state.reach_pose = {
        .root_offset = glm::vec3(0.0f, 0.0f, 0.0f),
        .leg_phase_offset = 0.5f
    };
    walk_state.stride_length = 1.2f;

    // Run keyframes (no vertical offset - spring-damper will handle vertical motion in Phase 3)
    run_state.pass_pose = {
        .root_offset = glm::vec3(0.0f, 0.0f, 0.0f),
        .leg_phase_offset = 0.0f
    };
    run_state.reach_pose = {
        .root_offset = glm::vec3(0.0f, 0.0f, 0.0f),
        .leg_phase_offset = 0.5f
    };
    run_state.stride_length = 2.0f;
}

void locomotion_system::update(glm::vec3 ground_velocity, float dt, bool is_grounded) {
    if (is_grounded) {
        current_speed = glm::length(ground_velocity);

        // Smooth speed to prevent erratic behavior
        smoothed_speed = smoothed_speed + (current_speed - smoothed_speed) * speed_smoothing * dt;

        // Reset to idle state when stationary
        if (smoothed_speed < 0.01f) {
            phase = 0.0f;
            distance_traveled = 0.0f;
            return;
        }

        // Calculate blend factor using smoothed speed
        float blend = 0.0f;
        if (smoothed_speed <= walk_speed_threshold) {
            blend = 0.0f;  // Pure walk
        } else if (smoothed_speed >= run_speed_threshold) {
            blend = 1.0f;  // Pure run
        } else {
            blend = (smoothed_speed - walk_speed_threshold) / (run_speed_threshold - walk_speed_threshold);
        }

        // Blend stride length with eased blend weight
        float blended_stride = easing::smooth_mix(walk_state.stride_length, run_state.stride_length, blend);
        if (blended_stride <= 0.0f) {
            phase = 0.0f;
            distance_traveled = 0.0f;
            return;
        }

        // Accumulate distance and keep it bounded using modulo
        distance_traveled += smoothed_speed * dt;
        distance_traveled = fmodf(distance_traveled, blended_stride);

        // Phase is just normalized distance
        phase = distance_traveled / blended_stride;
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
        blend = (smoothed_speed - walk_speed_threshold) / (run_speed_threshold - walk_speed_threshold);
    }

    float blend_weight = easing::cubic_smooth(blend);

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
    return lerp(walk_pose, run_pose, blend_weight);
}

simple_pose locomotion_system::lerp(const simple_pose& a, const simple_pose& b, float t) const {
    return {
        .root_offset = glm::mix(a.root_offset, b.root_offset, t),
        .leg_phase_offset = a.leg_phase_offset + (b.leg_phase_offset - a.leg_phase_offset) * t
    };
}

simple_pose locomotion_system::cubic_interp(const simple_pose& a, const simple_pose& b, float t) const {
    // Smoothstep (cubic Hermite) for velocity continuity
    float smoothed_t = t * t * (3.0f - 2.0f * t);
    return lerp(a, b, smoothed_t);
}
