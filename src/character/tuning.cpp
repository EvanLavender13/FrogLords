#include "character/tuning.h"
#include <cmath>

namespace character {

void tuning_params::apply_to(controller& c) const {
    if (time_to_max_speed <= 0.0f) {
        return;
    }

    float desired_accel = c.max_speed / time_to_max_speed;
    constexpr float FRICTION_RATIO = 0.75f;
    constexpr float NET_FRACTION = 1.0f - FRICTION_RATIO;
    if (NET_FRACTION <= 0.0f) {
        return;
    }

    c.ground_accel = desired_accel / NET_FRACTION;
    c.air_accel = desired_accel;

    float gravity_mag = std::abs(c.gravity);
    if (gravity_mag > 0.0f) {
        c.friction = (c.ground_accel * 0.75f) / gravity_mag;
        c.jump_velocity = std::sqrt(2.0f * gravity_mag * jump_height);
    }
}

void tuning_params::read_from(const controller& c) {
    constexpr float FRICTION_RATIO = 0.75f;
    constexpr float NET_FRACTION = 1.0f - FRICTION_RATIO;
    if (c.ground_accel > 0.0f && NET_FRACTION > 0.0f) {
        float net_accel = c.ground_accel * NET_FRACTION;
        time_to_max_speed = c.max_speed / net_accel;
    }

    float gravity_mag = std::abs(c.gravity);
    if (gravity_mag > 0.0f) {
        jump_height = (c.jump_velocity * c.jump_velocity) / (2.0f * gravity_mag);
    }
}

void sync_locomotion_targets(controller& c, locomotion_system& loco) {
    float run_speed = c.max_speed;
    loco.run_speed_threshold = run_speed;
    loco.walk_speed_threshold = run_speed * 0.2f;
    loco.walk_state.stride_length = loco.walk_speed_threshold * 1.0f;
    loco.run_state.stride_length = run_speed * 0.8f;
}

} // namespace character
