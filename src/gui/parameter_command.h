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
// Current: 5 parameters, centralized dispatch acceptable for this scale.

namespace gui {

enum class parameter_type {
    MAX_SPEED,
    ACCEL,
    WEIGHT,
    TURN_RATE,
    STEERING_REDUCTION_FACTOR,
    // Visual tilt parameters
    LEAN_MULTIPLIER,
    PITCH_MULTIPLIER,
    TILT_STIFFNESS,
    // Orientation parameters
    ORIENTATION_STIFFNESS,
};

struct parameter_command {
    parameter_type type;
    float value;
};

} // namespace gui
