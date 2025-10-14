#include "character/tuning.h"
#include <algorithm>
#include <cmath>

namespace {
constexpr float FRICTION_RATIO = 0.75f;
constexpr float NET_FRACTION = 1.0f - FRICTION_RATIO;
} // namespace

namespace character {

void tuning_params::apply_to(controller& c) const {
    if (time_to_max_speed <= 0.0f) {
        return;
    }

    float desired_accel = c.max_speed / time_to_max_speed;

    c.ground_accel = desired_accel / NET_FRACTION;
    c.air_accel = desired_accel;

    float gravity_mag = std::abs(c.gravity);
    if (gravity_mag > 0.0f) {
        c.friction = (c.ground_accel * FRICTION_RATIO) / gravity_mag;
        c.jump_velocity = std::sqrt(2.0f * gravity_mag * jump_height);
    }
}

void tuning_params::read_from(const controller& c) {
    if (c.ground_accel > 0.0f) {
        float net_accel = c.ground_accel * NET_FRACTION;
        time_to_max_speed = c.max_speed / net_accel;
    }

    float gravity_mag = std::abs(c.gravity);
    if (gravity_mag > 0.0f) {
        jump_height = (c.jump_velocity * c.jump_velocity) / (2.0f * gravity_mag);
    }
}

void sync_locomotion_targets(const controller& c, locomotion_system& loco) {
    float run_speed = std::max(c.run_speed, 0.0f);
    float walk_speed = std::clamp(c.walk_speed, 0.0f, run_speed);

    loco.run_speed_threshold = run_speed;
    loco.walk_speed_threshold = walk_speed;

    float walk_stride = std::max(walk_speed, 0.1f);
    float run_stride = std::max(run_speed * 0.8f, walk_stride);
    loco.walk_state.stride_length = walk_stride;
    loco.run_state.stride_length = run_stride;
}

} // namespace character