# System: Camera Lock Behavior

**Camera positioning from direction vectors - enables orientation lock and velocity lock modes**

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-20
**Layer:** 3
**Complexity:** Small
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/REFERENCES -->
## References

**Technical documentation:**
- None required (vector math primitive)

**Visual references:**
- Racing/drifting game cameras (lock to velocity/course direction)
- Standard third-person lock cameras (lock to character forward)

**Inspiration/Prior art:**
- Racing games: instant snap to velocity direction, responsive to drift
- Action games: lock to character facing for combat
- World of Warcraft: right-click drag rotates character with camera (future exploration)

**Constraints/Requirements:**
- Instant snap (no spring on camera direction itself)
- Vectors already smooth from upstream systems (yaw spring, physics integration)
- Same distance/height controls as orbit mode
- Snap transitions between modes (test feel first, add smoothing later if needed)
- Toggle between: Free Orbit / Lock to Orientation / Lock to Velocity

**Research findings:**
- Character orientation already smoothed via yaw spring (Layer 5)
- Character velocity already smoothed via physics integration (Layer 3)
- Camera just reads these smooth vectors and positions instantly
- Drift mechanic (from friction refactor) creates visible difference between orientation and velocity - lock modes make this observable
<!-- END: SELECT/REFERENCES -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required:**
- Camera math (Layer 1: `src/camera/camera.h`) - view/projection from eye/target
- Character controller (Layer 3: `src/character/controller.h`) - provides velocity vector
- Character orientation (Layer 5: `src/character/orientation.h`) - provides forward direction
- GUI framework (Layer 1: `src/gui/gui.h`) - mode toggle buttons
- Camera panel (Layer 1: `src/gui/camera_panel.h`) - existing orbit controls

**Enables (will become buildable):**
- WoW-style camera (right-click drag rotates character with camera)
- More sophisticated directional lock modes (look-ahead, path prediction)
- Camera composition patterns (blend between behaviors)
<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
A function that computes camera eye/target from: target position, direction vector, distance, and height offset. No smoothing, no state - just pure position calculation from inputs.

**Why a system, not a feature:**
Creates composable primitive. The lock behavior doesn't know about "orientation" or "velocity" - it just positions from any direction vector. Modes are emergent from feeding different vectors (character forward, velocity, input direction, path tangent, etc.). One primitive enables infinite directional sources.
<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Needs proof:**
- Eye position calculation: `eye = target - normalize(direction) * distance + vec3(0, height, 0)`
- Zero vector handling: What if direction magnitude is zero? (character at rest, velocity = 0)
- Maintain valid eye/target in all cases (no NaN, no degenerate camera)

**Known uncertainties:**
- Fallback direction when velocity is zero (use last valid direction? use orientation? default forward?)
- Height offset in world space vs along direction vector (world space simpler, matches orbit)
- Transition behavior when switching modes (snap is hypothesis, may need smoothing)
<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: SELECT/GRAYBOX -->
## Graybox

**Simplest implementation:**
- Cube or sphere as character (already exists)
- Debug arrows showing:
  - Character orientation (forward vector)
  - Character velocity (course vector)
  - Active lock direction (which vector camera is using)
- Camera panel toggles:
  - ( ) Free Orbit (existing)
  - ( ) Lock to Orientation
  - ( ) Lock to Velocity
- Same zoom/distance sliders as orbit mode

**Validation:**
- Toggle between modes while character moves/rotates
- Observe camera snap instantly to new direction
- Verify smooth following (vectors already smooth from upstream)
- Test drift: velocity and orientation diverge, camera clearly shows which is locked
- Test zero velocity: camera maintains valid position
- Debug viz confirms active direction vector
<!-- END: SELECT/GRAYBOX -->

---

<!-- BEGIN: GRAYBOX/IMPLEMENTATION_PLAN -->
## Implementation Plan

**Files to modify:**

1. `src/camera/camera_follow.h` - Add camera mode enum and lock positioning method
   - Add `enum class camera_mode { FREE_ORBIT, LOCK_ORIENTATION, LOCK_VELOCITY }`
   - Add `camera_mode mode` member
   - Add `glm::vec3 compute_lock_eye_position(const glm::vec3& target_pos, const glm::vec3& direction)` method

2. `src/camera/camera_follow.cpp` - Implement lock positioning function
   - Implement `compute_lock_eye_position`: `eye = target - normalize(direction) * distance + vec3(0, height, 0)`
   - Handle zero-length direction vector (fallback to last valid or default forward)

3. `src/app/game_world.cpp` - Switch between orbit and lock modes in camera update
   - Lines 73-75: Replace unconditional orbit mode with switch on `cam_follow.mode`
   - Pass `character.velocity` for LOCK_VELOCITY mode
   - Pass character forward direction for LOCK_ORIENTATION mode (compute from `character_visuals.orientation.get_yaw()`)

4. `src/gui/camera_command.h` - Add CAMERA_MODE command type
   - Add `CAMERA_MODE` to `camera_parameter_type` enum

5. `src/gui/camera_panel.cpp` - Add radio buttons for mode selection
   - Add three radio buttons: "Free Orbit", "Lock to Orientation", "Lock to Velocity"
   - Emit `camera_command` when mode changes

6. `src/app/game_world.h` - Add `apply_camera_mode_change` method signature

7. `src/app/game_world.cpp` - Add command handler for mode changes
   - Implement `apply_camera_mode_change(camera_mode mode)` to set `cam_follow.mode`

**Call structure:**

```
game_world::update(dt)
  → character.update(dt)  [velocity already smoothed by physics]
  → character_visuals.update(character, dt)  [orientation already smoothed by spring]
  → switch (cam_follow.mode):
      FREE_ORBIT → cam_follow.compute_eye_position(target)  [existing]
      LOCK_ORIENTATION → cam_follow.compute_lock_eye_position(target, forward_from_yaw)
      LOCK_VELOCITY → cam_follow.compute_lock_eye_position(target, character.velocity)
  → cam.set_position(eye)
  → cam.set_target(look_target)
```

**Debug data flow:**

Character velocity and orientation are already computed and smoothed by existing systems:
- `character.velocity` (Layer 3) - smoothed by physics integration
- `character_visuals.orientation.get_yaw()` (Layer 5) - smoothed by yaw spring

Lock direction vectors:
- LOCK_VELOCITY: `glm::normalize(character.velocity)` if `length > epsilon`, else fallback
- LOCK_ORIENTATION: `vec3(sin(yaw), 0, cos(yaw))` from character yaw angle

Debug visualization will show:
- Green arrow: character orientation (forward from yaw)
- Blue arrow: character velocity
- Red arrow: active lock direction (whichever camera is using)
- Camera position/target as small spheres

**Integration points:**

- `game_world::update` (line 73-75): Add mode switch to select eye position computation
- `draw_camera_panel`: Add radio buttons after line 20 ("Mode: FOLLOW")
- GUI command processing in main loop: Add handling for CAMERA_MODE commands (call `apply_camera_mode_change`)

**Zero-vector handling:**

When direction vector has zero length (character at rest, velocity = 0):
- Store `last_valid_direction` in `camera_follow` state
- On zero input, use last valid direction
- Initialize to default forward `vec3(0, 0, -1)` at construction
<!-- END: GRAYBOX/IMPLEMENTATION_PLAN -->

---

<!-- BEGIN: GRAYBOX/REVIEW -->
## Implementation Review

**Tool:** Codex CLI
**Date:** 2025-10-20

**Question asked:**
Review the implementation plan for the camera lock system. Does it follow principles (especially Radical Simplicity, Single Source of Truth, and composability)? Are there any concerns with the architecture, the zero-vector handling strategy, or the integration points? Is this the simplest approach?

**Feedback received:**
- Core direction-lock formula is correct and minimal
- Avoid mixing paradigms: keep orbit and directional lock separate (don't add mode state to `camera_follow`)
- Store `camera_mode` in gameplay state (`game_world`), not in camera primitive
- Zero-vector handling: use stateless fallback to orientation forward (already exists) instead of storing `last_valid_direction`
- Use existing math helpers: `safe_normalize`, `yaw_to_forward`, `project_to_horizontal`
- Command typing: don't encode mode as float - use typed command payload
- Keep lock as pure function, not method that adds state

**Impact on implementation:**
- Changed: Create pure helper function `compute_eye_from_direction` instead of adding method to `camera_follow`
- Changed: Store `camera_mode` in `game_world`, not `camera_follow`
- Simplified: Remove `last_valid_direction` state - derive fallback from orientation system
- Improved: Use `math::yaw_to_forward` for orientation lock direction
- Improved: Use `math::project_to_horizontal` for velocity lock (avoid vertical swing during jumps)
- Improved: Use `math::safe_normalize` with orientation forward fallback
- Changed: Create typed `camera_mode_command` instead of encoding as float
<!-- END: GRAYBOX/REVIEW -->

---

<!-- BEGIN: GRAYBOX/REVISED_PLAN -->
## Revised Implementation Plan

**Files to modify:**

1. `src/camera/camera_follow.h` - Add pure helper function (free function or namespace)
   - Add free function: `glm::vec3 compute_eye_from_direction(const glm::vec3& target_pos, const glm::vec3& direction, float distance, float height)`
   - No state changes to `camera_follow` class

2. `src/camera/camera_follow.cpp` - Implement pure lock positioning
   - Implement free function: `eye = target - normalize(direction) * distance + vec3(0, height, 0)`
   - Use `math::safe_normalize` with default fallback

3. `src/app/game_world.h` - Add camera mode state and command handler
   - Add `enum class camera_mode { FREE_ORBIT, LOCK_ORIENTATION, LOCK_VELOCITY }` in namespace
   - Add `camera_mode current_camera_mode` member to `game_world`
   - Add `void set_camera_mode(camera_mode mode)` method

4. `src/app/game_world.cpp` - Implement mode switching in camera update
   - Initialize `current_camera_mode = camera_mode::FREE_ORBIT` in `init()`
   - Lines 73-75: Replace with switch on `current_camera_mode`:
     - `FREE_ORBIT`: use existing `cam_follow.compute_eye_position(target)`
     - `LOCK_ORIENTATION`: compute direction with `math::yaw_to_forward(character_visuals.orientation.get_yaw())`, call `compute_eye_from_direction`
     - `LOCK_VELOCITY`: compute direction with `math::project_to_horizontal(character.velocity)`, use `safe_normalize` with orientation forward fallback, call `compute_eye_from_direction`
   - Implement `set_camera_mode(camera_mode mode)` to update state

5. `src/gui/camera_command.h` - Add typed mode command
   - Add new `camera_mode_command` struct with `camera_mode mode` member
   - Keep existing `camera_command` for numeric parameters

6. `src/gui/camera_panel.h` - Update signature to return both command types
   - Change return type or add second vector output for mode commands

7. `src/gui/camera_panel.cpp` - Add radio buttons for mode selection
   - Replace line 20 text "Mode: FOLLOW" with radio button group
   - Three radio buttons: "Free Orbit", "Lock to Orientation", "Lock to Velocity"
   - Emit `camera_mode_command` when selection changes

8. `src/app/runtime.cpp` - Handle mode commands in main loop
   - Process `camera_mode_command` vector alongside existing `camera_command` processing
   - Call `game_world.set_camera_mode(cmd.mode)` for each mode command

**Simplified call structure:**

```
game_world::update(dt)
  → switch (current_camera_mode):
      FREE_ORBIT:
        eye = cam_follow.compute_eye_position(character.position)
      LOCK_ORIENTATION:
        forward = math::yaw_to_forward(character_visuals.orientation.get_yaw())
        eye = compute_eye_from_direction(character.position, forward, cam_follow.distance, cam_follow.height_offset)
      LOCK_VELOCITY:
        vel_horizontal = math::project_to_horizontal(character.velocity)
        forward_fallback = math::yaw_to_forward(character_visuals.orientation.get_yaw())
        direction = math::safe_normalize(vel_horizontal, forward_fallback)
        eye = compute_eye_from_direction(character.position, direction, cam_follow.distance, cam_follow.height_offset)
  → cam.set_position(eye)
  → cam.set_target(cam_follow.compute_look_target(character.position))
```

**Stateless zero-vector handling:**

No stored state needed. When velocity is zero or near-zero:
- `math::safe_normalize(vel_horizontal, forward_fallback)` returns `forward_fallback`
- Fallback is orientation forward (already smoothed by yaw spring)
- This matches existing behavior: character maintains orientation when stationary
<!-- END: GRAYBOX/REVISED_PLAN -->

---

<!-- BEGIN: GRAYBOX/RESULTS -->
## Results

**Status:** DEFER - Prerequisites missing

**Date:** 2025-10-20

### What Works

- [x] Core math is correct (orientation lock computes valid eye positions)
- [x] Build successful, no crashes, no NaN
- [x] Debug visualization clearly shows orientation vs velocity vs active lock direction
- [x] Zero-vector fallback works (stationary character maintains valid camera)
- [x] Stateless implementation (no accumulated state, derives from existing systems)

### What Doesn't Work

- Velocity lock: camera follows collision response, causes chaos when hitting walls
- Orientation lock with current controls: holding "S" causes camera shake (orientation flips because character orients to velocity)

### Surprises

**Critical insight: This is a control scheme problem, not a camera problem.**

Current controls (WASD world-space) conflict with orientation lock:
- Character orientation follows velocity direction
- Player input "S" = backward velocity = orientation flips 180°
- Camera locked to orientation = camera flips
- Result: camera shakes when backpedaling

**Root cause:** Trying to use car-like camera with non-car-like controls.

### What This Reveals

Orientation lock **requires** car-like controls:
- W/S = forward/backward relative to character
- A/D = turn character (change orientation directly)
- Character orientation = input intent (not velocity result)
- Camera follows orientation smoothly

Without car-like controls, FREE_ORBIT is the only sensible mode (manual camera positioning).

### Decision

**DEFER** until control scheme refactor (Layer 2).

**Why defer is success:**
- Graybox validated math works
- Discovered real dependency: control scheme must match camera behavior
- Avoided building complete system for wrong prerequisite
- Cheap to build, cheap to defer, expensive lesson learned early

### Next Steps

1. Defer this system (keep code for reference)
2. Design car-like control scheme (separate plan)
3. Revisit camera lock after controls support it

**Dependency chain:**
Layer 2: Car-like controls (W/S = forward/back, A/D = turn) → Layer 3: Camera orientation lock becomes useful
<!-- END: GRAYBOX/RESULTS -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [x] Core functional in graybox
- [x] Mathematical correctness proven (no NaN, valid positions, zero-vector handling)
- [x] Debug visualization shows active lock direction
- [x] Emergent behaviors appear (velocity lock fails, orientation lock reveals control mismatch)
- [x] Principles upheld (composable, simple, consistent)
- [x] **Graybox succeeded: discovered missing prerequisite before building complete system**
<!-- END: SELECT/SUCCESS -->
