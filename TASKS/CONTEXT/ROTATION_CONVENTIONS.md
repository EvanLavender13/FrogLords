# Rotation Conventions

Tags: physics, math, terminology

Purpose
- Distinguish heading-relative from world-absolute rotation semantics.

Core Rules
- "Turn right/left" describes rotation from current heading, not world axes.
- Angular velocity sign indicates rotation direction in world space.
- Input semantics differ from mathematical conventions by design.

Heading-Relative
- Turn right = clockwise from current facing.
- Turn left = counterclockwise from current facing.
- Used for: player input, steering commands, gameplay descriptions.

World-Absolute
- Positive yaw = rotation toward +X in right-handed system.
- Angular velocity = yaw delta / dt in world coordinates.
- Used for: physics integration, transform matrices, mathematical operations.

The Bridge
- `heading_yaw += -turn_input * turn_rate * dt`
- Negation maps intuitive input (right = positive) to math convention (right = decreasing yaw).
- Players think "turn right," not "decrease yaw angle."

Apply
- Test rotations at multiple world orientations.
- Verify D key turns right regardless of facing.
- Check angular velocity sign: negative = right turn, positive = left turn.

Avoid
- Assuming positive angular velocity means right turn.
- Conflating turn input sign with yaw delta sign.
- Using world-absolute language for player-facing behavior.

Signals
- "Right turn toward +X" mixes heading-relative action with world-absolute direction.
- Sign confusion in rotation calculations or debug visualization.

Default
- Describe player actions as heading-relative. Describe math as world-absolute. Bridge with explicit negation.