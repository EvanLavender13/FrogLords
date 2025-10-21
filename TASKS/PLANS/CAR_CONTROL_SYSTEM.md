# System: Car-Like Control Scheme

Transform WASD input from world-space strafe to character-relative forward/back and turn.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-20
**Layer:** 2 (Primitives)
**Complexity:** Small (2-3 pts)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/REFERENCES -->
## References

**Technical documentation:**
- Tank Controls: Up/down = forward/backward relative to character, left/right = rotate character orientation (not strafe)
- Arcade physics: Instant direction changes, no inertia, snappy responsive feel
- Turn rate: Tunable rotation speed parameter (radians/second)
- Speed-dependent turning: Common arcade pattern - can only turn while moving (like a car)
- Input smoothing: Digital keyboard input → analog signal for natural feel (50-200ms acceleration scale)

**Visual references:**
- None needed (concept clear from Camera Lock graybox discovery)

**Inspiration/Prior art:**
- Classic Resident Evil / Tomb Raider tank controls
- Arcade racing games (speed-dependent steering)
- Modern action games with orientation-locked cameras

**Research findings:**
- Web search: Arcade controls prioritize "snappy, energetic" feel over realism
- Tank controls maintain orientation consistency across camera changes
- Speed-dependent turning prevents spinning in place (more realistic car feel)
- Input acceleration curves convert digital → analog (100-200ms typical)

**Constraints/Requirements:**
- Must support toggle/transition between world-space (current) and character-relative (new) control schemes
- Consider: Can only turn while moving (car-like behavior)
- Must enable Camera Lock system (deferred prerequisite)
- Arcade feel: responsive, snappy, not sluggish

**Local research notes:**
- Check `NOTES/DesigningGames/` for control design philosophy
- Check `NOTES/GDC/` for technical control implementation patterns
<!-- END: SELECT/REFERENCES -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required:**
- Character orientation system (exists in Layer 3 - `src/character/orientation.{h,cpp}`)
- Input system (exists in Layer 1 - `src/input/input.{h,cpp}`)
- Character controller update (exists in Layer 3 - `src/character/controller.{h,cpp}`)

**Enables (will become buildable):**
- Orientation-Locked Camera Mode (Layer 4 - currently deferred)
- Drift movement mechanics (Layer 4 - emergent behavior)
<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
Input transformation primitive. WASD world-space vector becomes character-relative: forward/back magnitude + turn rate. Character orientation drives subsequent input interpretation, not input drives orientation.

**Why a system, not a feature:**
Enables orthogonal camera behaviors. Free-orbit camera needs world-space input (current). Locked camera needs character-relative input (this primitive). One input source, multiple interpretations. Composability requires the primitive exists.
<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Needs proof:**
- Turn rate integration: `new_yaw = old_yaw + turn_rate * dt` (time-independent rotation)
- Speed-dependent turn scaling: `effective_turn_rate = base_turn_rate * speed_factor` (0 at rest, 1 at speed threshold)
- Input vector transformation: world-space WASD → character-local space projection

**Known uncertainties:**
- Turn rate magnitude: What radians/second feels "arcade-y" vs "sluggish"? (tuning parameter)
- Speed threshold for turning: Linear scale 0→1 over what speed range? (tuning parameter)
- Transition behavior: Instant switch vs blend between control schemes? (user specified constraint)
<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: SELECT/GRAYBOX -->
## Graybox

**Simplest implementation:**
- Add `control_scheme` enum to character controller or game world (FREE_STRAFE, CAR_LIKE)
- Add `turn_rate` tuning parameter (radians/second)
- Transform input:
  - FREE_STRAFE: Current behavior (WASD → world-space acceleration)
  - CAR_LIKE: W/S → forward/back magnitude, A/D → turn rate, apply turn to orientation, project forward/back along orientation
- GUI: Radio button to toggle control scheme + slider for turn rate
- Debug viz: Show turn rate as angular velocity arrow at character yaw ring

**Validation:**
- Press W: Character moves forward in facing direction (not world +Z)
- Press A/D while stationary: Character rotates in place OR no rotation (if speed-gated)
- Press A/D while moving: Character turns smoothly at turn_rate
- Switch control scheme mid-motion: Immediate response change (or blended transition if implemented)
<!-- END: SELECT/GRAYBOX -->

---

<!-- BEGIN: GRAYBOX/SCOPE -->
## Graybox Scope

**What will be built:**
Input transformation primitive that converts WASD digital input into car-like control: W/S controls forward/backward motion relative to character facing, A/D controls turn rate that updates character yaw. The character orientation system will be overridden when car control is active to directly track the turn-rate-driven yaw instead of velocity-driven yaw.

**Complexity:** Small
<!-- END: GRAYBOX/SCOPE -->

---

<!-- BEGIN: GRAYBOX/IMPLEMENTATION_PLAN -->
## Implementation Plan

**Files to modify:**

1. `src/character/controller.h`
   - Add `float heading_yaw = 0.0f` (physics state for car-like heading)
   - Add `float turn_rate = 3.0f` (radians/second, tunable)
   - No signature changes to `apply_input()`

2. `src/character/controller.cpp`
   - Modify `apply_input()`:
     - Add turn integration: `heading_yaw += input_params.move_direction.x * turn_rate * dt`
     - Wrap angle: `heading_yaw = math::wrap_angle_radians(heading_yaw)`
     - Keep existing transformation unchanged (lines 63-71)

3. `src/app/game_world.h`
   - Add `enum class control_scheme { FREE_STRAFE, CAR_LIKE }`
   - Add `control_scheme current_control_scheme = control_scheme::FREE_STRAFE`

4. `src/app/game_world.cpp`
   - Modify input construction (before line 40):
     - Switch on `current_control_scheme`:
       - FREE_STRAFE: `cam_params.forward = cam.get_forward_horizontal()`, `cam_params.right = cam.get_right()`
       - CAR_LIKE: `cam_params.forward = {sin(character.heading_yaw), 0, cos(character.heading_yaw)}`, `cam_params.right = {cos(character.heading_yaw), 0, -sin(character.heading_yaw)}`
   - Add toggle: `if (input::is_key_pressed(SAPP_KEYCODE_T))` flip scheme

**Call structure:**
```
game_world::update()
  → [NEW] select basis vectors based on control_scheme:
      FREE_STRAFE: cam_params from camera
      CAR_LIKE: cam_params from character.heading_yaw
  → controller::apply_input(input_params, cam_params, dt)
      → heading_yaw += turn_input * turn_rate * dt (always)
      → acceleration = (forward * y_input + right * x_input) * accel (unchanged)
  → controller::update(world, dt)
      → velocity integration (unchanged)
  → character_reactive_systems::update(controller, dt)
      → orientation.update(velocity, dt) (unchanged)
```

**Debug data flow:**
- Visualize `character.heading_yaw` as yaw ring when CAR_LIKE active
- Show control scheme indicator text

**Integration points:**
- `controller.h`: Add heading_yaw state
- `controller::apply_input()` line 63: Add heading integration before existing transform
- `game_world.cpp` before line 36: Add basis selection switch
- Controller stays orthogonal - no mode awareness
<!-- END: GRAYBOX/IMPLEMENTATION_PLAN -->

---

<!-- BEGIN: GRAYBOX/REVIEW -->
## Implementation Review

**Tool:** Codex CLI
**Date:** 2025-10-20

**Question asked:**
Does this implementation plan follow the principles? Are there any violations of Radical Simplicity, orthogonality, or accumulated state? Does the control scheme switch introduce unnecessary complexity or special cases?

**Feedback received:**
- Cross-layer coupling: Passing visual orientation yaw into physics violates "Physics Drives Animation"
- Duplicate authority over orientation: Visual system derives yaw AND physics would compute yaw
- Mode branch in primitive: Switch inside controller violates composability
- Suggested: Move mode selection to composition layer, controller owns heading as physics state

**Impact on implementation:**
- Moved control_scheme enum from controller to game_world (composition layer)
- Controller now owns heading_yaw as physics state, always updates from turn input
- Game_world selects which basis vectors (camera vs heading) to pass to controller
- Controller transformation stays mode-agnostic - no switch statement
- Preserves orthogonality: controller is primitive, game_world composes behavior
<!-- END: GRAYBOX/REVIEW -->

---

<!-- BEGIN: GRAYBOX/NOTES -->
## Notes

**Reactive system incompatibility:**

Current `character_reactive_systems::orientation` derives yaw from velocity (correct for free-strafe). This creates problems for car-like control:

- **Reverse motion:** Character rotates to face backward (velocity direction) instead of maintaining forward facing (heading direction)
- **Drift/sliding:** Character rotates toward slide direction instead of maintaining steering angle

**Not a bug** - current reactive system is correct for its design (velocity-driven orientation). Car-like control needs different reactive behavior (heading-driven orientation).

**Deferred:** Control system graybox validates input transformation only. Visual orientation systems need separate iteration when car control moves beyond graybox.

**Future work:** Create car-specific reactive system that uses `controller.heading_yaw` instead of velocity for visual orientation.
<!-- END: GRAYBOX/NOTES -->

---

<!-- BEGIN: GRAYBOX/RESULTS -->
## Results

**Status:**
- [x] Core functional
- [x] Build successful
- [x] Debug viz working
- [x] Ready for iteration

**Works:**
- T key toggles between FREE_STRAFE and CAR_LIKE modes
- CAR_LIKE mode: W/S moves forward/backward along heading direction
- CAR_LIKE mode: A/D rotates heading (no lateral strafing)
- Yellow heading arrow visualizes heading_yaw in CAR_LIKE mode
- Heading integration is time-independent (turn_rate * dt)
- Smooth turning while moving forward (no lateral acceleration artifacts)
- Toggle between modes works mid-motion

**Doesn't work (known limitations):**
- Visual orientation still velocity-driven (character faces backward when reversing)
- This is expected - reactive systems need separate work (see GRAYBOX/NOTES)

**Surprises:**
- Initial implementation had cross-layer coupling (visual yaw → physics)
- Codex review caught this before it shipped
- Moving heading integration to composition layer (game_world) kept controller orthogonal
- Turn direction required negation (coordinate system handedness)
- Lateral input initially created diagonal acceleration (fixed by zeroing x before normalization)

**Next:**
- ITERATE phase (add assertions, handle edge cases)
- Future: Car-specific reactive system for heading-driven visual orientation
<!-- END: GRAYBOX/RESULTS -->

---

## Iterations

- [ITERATION_1.md](CAR_CONTROL_ITERATION_1.md) - In Progress

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Core functional in graybox
- [ ] Mathematical correctness proven (time-independent turn rate)
- [ ] Debug visualization shows turn rate
- [ ] Control scheme toggle works instantly
- [ ] Principles upheld (orthogonal primitive, composable with camera modes)
<!-- END: SELECT/SUCCESS -->
