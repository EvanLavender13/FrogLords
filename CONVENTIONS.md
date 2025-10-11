# FrogLords Coordinate System and Conventions

This document captures the implicit coordinate system conventions used throughout the codebase. Making these explicit helps contributors understand geometric operations and prevents confusion during implementation.

---

## World Coordinate System

**Right-Handed Coordinate System**

- **Y-up:** Vertical axis points upward (gravity acts along -Y)
- **Z-forward:** Character forward direction along positive Z axis
- **X-right:** Horizontal axis points right (cross product of Y-up × Z-forward)

**Constant Reference:**
```cpp
// src/foundation/math_utils.h
constexpr glm::vec3 UP(0.0f, 1.0f, 0.0f);  // World up vector
```

**Implications:**
- Ground plane: Y = 0 (XZ plane)
- Gravity vector: `glm::vec3(0.0f, -9.8f, 0.0f)`
- Character height measured along +Y axis
- Collision normals for ground contact point upward: `glm::vec3(0, 1, 0)`

---

## Rotation Conventions

### Yaw Angle (Y-axis Rotation)

**Definition:** Rotation around the world Y-axis (horizontal turning)

**Direction Convention:**
- Positive yaw: Counter-clockwise rotation when viewed from above
- 0 radians: Facing along positive Z axis
- π/2 radians: Facing along positive X axis

**Yaw-to-Direction Utilities:**
```cpp
// src/foundation/math_utils.h
inline glm::vec3 yaw_to_forward(float yaw) {
    return glm::vec3(std::sin(yaw), 0.0f, std::cos(yaw));
}

inline glm::vec3 yaw_to_right(float yaw) {
    return glm::vec3(-std::cos(yaw), 0.0f, std::sin(yaw));
}
```

**Usage Pattern:**
- Character orientation: `orientation.get_yaw()` returns current facing angle
- Movement direction: `yaw_to_forward(yaw)` converts yaw to world-space forward vector
- Strafing: `yaw_to_right(yaw)` for perpendicular movement

### Quaternion Rotations

**Identity Quaternion:**
```cpp
glm::quat identity(1.0f, 0.0f, 0.0f, 0.0f);  // (w, x, y, z)
```
Represents no rotation (T-pose baseline for skeletal animation).

**Interpolation:**
- Use `glm::slerp(quat1, quat2, t)` for spherical linear interpolation
- GLM guarantees shortest-path interpolation (handles hemisphere correction automatically)
- For opposite hemisphere quaternions (dot product < 0), negate one quaternion before slerp

**Application Order:**
- Local-to-world: Translate → Rotate (position first, then orientation)
- Joint hierarchy: Parent transform → Local rotation → Child transform

---

## Camera Conventions

### Camera Modes

**ORBIT Mode:**
- Spherical coordinates around center point
- Immediate eye position update on parameter changes
- Zoom modifies `distance` (2-30m range)

**FOLLOW Mode:**
- Follows character with height offset
- Uses `follow_distance` (1.5-15m range) and `follow_height_offset`
- Updates eye position each frame based on character position

**Spherical Coordinate Behavior:**
- Camera orbits around point above character (not character feet)
- Height offset creates "shift" effect during zoom (intentional for better framing)
- Both latitude and longitude affect eye position calculation

---

## Animation Conventions

### Distance-Phased Animation

**Surveyor-Wheel Pattern:**
- Animation driven by `locomotion.distance_traveled` (not time)
- Phase wraps at 1.0 (seamless loop)
- Ensures visual coherence: limbs match actual body movement

**Pose Blending:**
- Segment-based blending (0.0-0.25, 0.25-0.5, 0.5-0.75, 0.75-1.0)
- Quaternion slerp for smooth joint interpolation
- Last segment (0.75-1.0) targets first pose for seamless wrap

### Joint Angles

**Euler Angle Ranges:**
- GUI sliders: -180° to 180° for X/Y/Z axes
- Internally converted to quaternions for interpolation
- Hemisphere correction applied automatically during slerp

---

## Debug Visualization

### Color Conventions

**Speed Gradient:**
- Blue → Green → Yellow → Red (slow to fast)
- Ring radius = max speed (distance traveled in 1 second)

**Debug Spheres:**
- White: Velocity trail samples
- Green: Yaw indicator (character facing direction)
- Opacity decay: Older samples more transparent

### Coordinate Space

**World Space:**
- All debug visuals rendered in world coordinates
- Character position is world origin for relative measurements

**Screen Space:**
- GUI panels positioned in normalized screen coordinates (0.0-1.0)
- ImGui uses pixel coordinates for widget layout

---

## References

**Implementation Examples:**
- Yaw utilities: [src/foundation/math_utils.h](src/foundation/math_utils.h)
- Y-axis constant: [src/foundation/math_utils.h](src/foundation/math_utils.h) `math::UP`
- Camera spherical orbit: [src/camera/camera.cpp](src/camera/camera.cpp)
- Quaternion animation: [src/character/animation.cpp](src/character/animation.cpp)
- Identity pose: [src/character/keyframe.cpp](src/character/keyframe.cpp) `create_identity_pose()`

**Retrospective Context:**
- Coordinate conventions documented during Retro 2 (Oct 2025)
- Yaw utilities refactor captured Y-up, Z-forward convention
- Y-axis constant refactor made world orientation explicit
- Camera zoom feature revealed spherical pivot geometry

---

## Notes for Contributors

**When adding geometric operations:**
1. Use `math::UP` constant instead of `glm::vec3(0, 1, 0)` literals
2. Use `yaw_to_forward()` / `yaw_to_right()` utilities for direction conversions
3. Verify right-handed coordinate system assumptions (cross products, rotation directions)
4. Document any new coordinate space transformations

**When debugging rotation issues:**
1. Check quaternion initialization (`glm::quat(1,0,0,0)` for identity)
2. Verify hemisphere correction for slerp (dot product check)
3. Confirm rotation order (yaw around Y-axis, not arbitrary axis)
4. Use debug visualizations to verify direction vectors match intent
