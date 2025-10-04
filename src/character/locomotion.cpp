#include "character/locomotion.h"
#include "foundation/easing.h"
#include <algorithm>
#include <cmath>

namespace {
float shortest_unit_delta(float from, float to) {
    float delta = to - from;
    delta -= std::floor(delta + 0.5f);
    return delta;
}

bool crossed_phase_threshold(float previous, float delta, float threshold) {
    if (delta <= 0.0f) {
        return false;
    }

    float end = previous + delta;
    if (end >= 1.0f) {
        float wrapped_end = end - 1.0f;
        if (previous < threshold) {
            return true;
        }
        return threshold <= wrapped_end;
    }

    return previous < threshold && threshold <= end;
}
}

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

    // Slightly overdamped vertical spring (~2.2 Hz) to match cadence
    constexpr float natural_frequency = 2.2f;
    constexpr float damping_ratio = 0.9f;
    constexpr float two_pi = 6.28318530718f;
    float omega = two_pi * natural_frequency;
    vertical_spring.stiffness = omega * omega;
    vertical_spring.damping = damping_ratio * 2.0f * omega;

    // Initialize spring at target height
    vertical_spring.reset(0.3f);
}

void locomotion_system::update(glm::vec3 ground_velocity, float dt, bool is_grounded, float ground_height) {
    if (is_grounded) {
        current_speed = glm::length(ground_velocity);

        // Smooth speed to prevent erratic behavior
        smoothed_speed = smoothed_speed + (current_speed - smoothed_speed) * speed_smoothing * dt;

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
            is_relaxing = false;
        } else if (smoothed_speed < idle_speed_threshold) {
            if (!is_relaxing) {
                is_relaxing = true;
                relax_stride = blended_stride;
            }

            float relax_t = 1.0f - std::exp(-idle_relax_rate * dt);
            float delta = shortest_unit_delta(phase, 0.0f);
            phase += delta * relax_t;
            phase = std::fmod(phase, 1.0f);
            if (phase < 0.0f) {
                phase += 1.0f;
            }
            if (std::abs(delta) < 1e-4f || phase > 1.0f - 1e-4f || phase < 1e-4f) {
                phase = 0.0f;
            }

            distance_traveled = phase * relax_stride;
            time_since_last_step += dt;
        } else {
            is_relaxing = false;

            if (smoothed_speed > 0.1f) {
                step_period = blended_stride / smoothed_speed;
            }

            float prev_phase = phase;

            distance_traveled += smoothed_speed * dt;
            distance_traveled = std::fmod(distance_traveled, blended_stride);
            if (distance_traveled < 0.0f) {
                distance_traveled += blended_stride;
            }

            phase = distance_traveled / blended_stride;

            time_since_last_step += dt;

            float phase_delta = shortest_unit_delta(prev_phase, phase);

            int steps_crossed = 0;
            if (crossed_phase_threshold(prev_phase, phase_delta, 0.5f)) {
                steps_crossed += 1;
            }
            if (crossed_phase_threshold(prev_phase, phase_delta, 0.0f)) {
                steps_crossed += 1;
            }

            if (steps_crossed > 0) {
                // Apply subtle downward impulse when foot plants (much smaller than landing impacts)
                // Footsteps should be gentle bounce, landings should be pronounced
                float impulse = -smoothed_speed * bounce_impulse_scale * 0.15f * static_cast<float>(steps_crossed);
                vertical_spring.add_impulse(impulse);
                time_since_last_step = 0.0f;
            }
        }
    } else {
        is_relaxing = false;
    }

    // Spring seeks standing offset above ground (0.3m offset from surface)
    // Spring position is RELATIVE offset, not absolute world position
    float target_offset = 0.3f;
    vertical_spring.update(target_offset, dt);
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

    // Apply spring-damper vertical offset (relative to ground)
    // Spring oscillates around 0.3m, compresses below that on landings/steps
    blended.root_offset.y = vertical_spring.get_position();

    return blended;
}

simple_pose locomotion_system::lerp(const simple_pose& a, const simple_pose& b, float t) const {
    return {
        .root_offset = easing::smooth_mix(a.root_offset, b.root_offset, t),
        .leg_phase_offset = easing::smooth_mix(a.leg_phase_offset, b.leg_phase_offset, t)
    };
}

simple_pose locomotion_system::cubic_interp(const simple_pose& a, const simple_pose& b, float t) const {
    // cubic smooth easing maintains velocity continuity
    return lerp(a, b, t);
}




