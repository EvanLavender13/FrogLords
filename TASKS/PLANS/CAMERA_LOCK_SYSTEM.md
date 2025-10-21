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

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Core functional in graybox
- [ ] Mathematical correctness proven (no NaN, valid positions, zero-vector handling)
- [ ] Debug visualization shows active lock direction
- [ ] Emergent behaviors appear (drift visible in velocity lock, responsive in orientation lock)
- [ ] Principles upheld (composable, simple, consistent)
<!-- END: SELECT/SUCCESS -->
