#pragma once

// Camera Command Pattern for Unidirectional Data Flow
//
// Camera GUI panel emits commands, game loop applies them.
// This maintains architectural separation: GUI reads camera state (const refs),
// emits commands when user interacts, game logic applies commands explicitly.
//
// Pattern mirrors parameter_command.h for consistency.

namespace gui {

enum class camera_parameter_type {
    DISTANCE,
    HEIGHT_OFFSET,
    MIN_DISTANCE,
    MAX_DISTANCE,
};

struct camera_command {
    camera_parameter_type type;
    float value;
};

} // namespace gui
