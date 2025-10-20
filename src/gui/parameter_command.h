#pragma once

// Parameter Command Pattern for Unidirectional Data Flow
//
// GUI panels emit commands, game loop applies them.
// This maintains architectural separation: GUI reads state (const refs),
// emits commands when user interacts, game logic applies commands explicitly.
//
// Pattern: Observer + Commander (not Executor)
// - GUI observes game state (read-only)
// - GUI commands changes (via parameter_command)
// - Game loop executes changes (explicit application)
//
// Future: Consider registry pattern if parameter count grows significantly.
// Current: 9 parameters, centralized dispatch acceptable for this scale.

namespace gui {

enum class parameter_type {
    max_speed,
    ground_accel,
    base_friction,
    dash_impulse,
    jump_height,
    gravity,
    coyote_window,
    jump_buffer_window,
    landing_stiffness,
    landing_damping,
    landing_impulse_scale,
};

struct parameter_command {
    parameter_type type;
    float value;
};

} // namespace gui
