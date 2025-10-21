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

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Core functional in graybox
- [ ] Mathematical correctness proven (time-independent turn rate)
- [ ] Debug visualization shows turn rate
- [ ] Control scheme toggle works instantly
- [ ] Principles upheld (orthogonal primitive, composable with camera modes)
<!-- END: SELECT/SUCCESS -->
