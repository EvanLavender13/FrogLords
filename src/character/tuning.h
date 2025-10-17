#pragma once

#include "character/controller.h"

namespace character {

struct tuning_params {
    // TUNED: Time from rest to max_speed (responsiveness feel)
    // Platformer typical range: 300-500ms (fighting games: 100-200ms)
    // Current: 400ms provides responsive but not twitchy control
    // NOTE: Default (0.4s) inconsistent with controller defaults (~1.6s actual)
    // Used in: apply_to to calculate ground_accel and air_accel
    float time_to_max_speed = 0.4f; // seconds

    // TUNED: Target vertical jump height (world-space distance)
    // With BUMPER_RADIUS=0.5m: Character can reach platforms at Y = 0.5 + 1.3 = 1.8m
    // Used in: apply_to to calculate jump_velocity via v = √(2·|g|·h)
    // Reverse calculated: Current jump_velocity (5.0 m/s) implies h ≈ 1.276m (2% difference)
    float jump_height = 1.3f; // meters

    void apply_to(controller& c) const;
    void read_from(const controller& c);
};

} // namespace character
