# Reference Frames

Tags: math, architecture, rendering

Purpose
- Clarify local-space vs world-space transformations and composition order.

Core Rules
- Transformations compose from local to world: object → parent → grandparent → world.
- Rotation axes after prior rotations use local-space vectors, not world-derived.
- Matrix multiplication order matters: translate → rotate_yaw → rotate_roll → rotate_pitch.

Local-Space
- Axes relative to object's current orientation.
- Forward = (0,0,1), Right = (1,0,0), Up = (0,1,0) in object coordinates.
- Used for: tilt after heading, pitch after yaw, relative rotations.

World-Space
- Fixed global axes regardless of object orientation.
- +X right, +Y up, +Z forward in this codebase.
- Used for: position, velocity, absolute heading angles.

Transform Composition
- Build transforms inside-out: local rotations before parent rotations.
- `translate(position) * rotate(yaw, UP) * rotate(roll, LOCAL_FORWARD) * rotate(pitch, LOCAL_RIGHT)`
- Each rotation happens in the space created by previous transforms.

Apply
- Use explicit local axes (0,0,1) not derived world axes for sequential rotations.
- Test transforms at various orientations to catch reference frame bugs.
- Verify tilt doesn't cause spurious rotation when moving straight.

Avoid
- Using yaw_to_forward() for rotation axis after yaw is applied.
- Mixing local and world axes in single transform chain.
- Assuming rotation order is commutative.

Signals
- Spurious roll when driving straight after adding tilt system.
- Rotations that work at origin but break when oriented differently.
- "Gimbal lock" symptoms from incorrect reference frames.

Default
- Use local-space axes for rotations applied after other rotations. Derive world-space vectors only for world-aligned operations.