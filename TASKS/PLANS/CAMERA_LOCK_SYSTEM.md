# System: Orientation-Locked Camera

**Camera locks behind character facing direction**

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-21
**Layer:** 4
**Complexity:** Small
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/REFERENCES -->
## References

**Technical documentation:**
- Previous graybox validation: @TASKS/PLANS/CAMERA_LOCK_SYSTEM_OLD.md
- Core math validated: `eye = target - normalize(direction) * distance + vec3(0, height, 0)`

**Visual references:**
- Racing game cameras (instant snap to direction)
- Action game third-person lock cameras (Resident Evil 4, Dark Souls)

**Inspiration/Prior art:**
- Instant snap (no spring on direction itself - vectors already smooth from upstream)
- Same distance/height controls as orbit mode
- Simple toggle between Free Orbit / Locked modes

**Research findings:**
- Math proven correct in previous graybox (2025-10-20)
- Prerequisite (car-like controls) now complete
- Stateless implementation: derive from character orientation, no accumulated state

**Constraints/Requirements:**
- Toggle: GUI radio button
- Locked mode uses character forward direction from orientation system
- Maintains existing zoom/distance controls
- Snap transitions (instant, not smoothed)
<!-- END: SELECT/REFERENCES -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required:**
- Camera Follow System (Layer 3) - provides distance/height parameters
- Character Orientation System (Layer 5) - provides smoothed forward direction
- Car-Like Control Scheme (Layer 2) - completed 2025-10-21
- GUI Framework (Layer 1) - mode toggle

**Enables (will become buildable):**
- WoW-style camera (right-click drag rotates character)
- More sophisticated lock modes (look-ahead, path prediction)
- Camera composition patterns
<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
Pure function: compute camera eye position from target position, direction vector, distance, and height. No state, no smoothing—just position calculation.

**Why a system, not a feature:**
Creates composable primitive. The lock doesn't know about "orientation"—it positions from any direction vector. This enables infinite directional sources (velocity, input, path tangent, etc.). One primitive, emergent modes.
<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Needs proof:**
- Eye position: `eye = target - normalize(direction) * distance + vec3(0, height, 0)`
- Already validated in previous graybox (no NaN, handles zero vectors)

**Known uncertainties:**
- None (math proven, prerequisites exist)
<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: GRAYBOX/SCOPE -->
## Graybox

**What will be built:**
Pure function that computes camera eye position from target, direction yaw, distance, and height. Camera mode enum added to game_world (FREE_ORBIT, LOCK_TO_ORIENTATION). GUI radio button toggles mode. In lock mode, controller's heading_yaw determines direction vector. Debug arrow shows lock direction when active.

**Complexity:** Small
<!-- END: GRAYBOX/SCOPE -->

<!-- BEGIN: GRAYBOX/IMPLEMENTATION_PLAN -->
## Implementation Plan

**Files modified:**
- `src/camera/camera_follow.h` - Added pure function `compute_locked_eye_position(target, forward_dir, distance, height)` accepting vec3; Added `camera_mode` enum and `mode` field to `camera_follow` struct (SSOT)
- `src/camera/camera_follow.cpp` - Implemented locked camera position calculation with vec3 direction input
- `src/gui/camera_panel.h` - Added radio button for camera mode toggle
- `src/gui/camera_panel.cpp` - Implemented mode toggle UI
- `src/app/game_world.cpp` - Updated camera position logic to branch on mode, derives direction from orientation system

**Actual implementation:**
`game_world::update()` reads `camera_mode` from `cam_follow.mode` (camera_follow.h:10). When LOCK_TO_ORIENTATION: computes forward direction vec3 from `character_visuals.orientation.get_yaw()` using `math::yaw_to_forward()` helper (game_world.cpp:119-120), passes to `camera_follow::compute_locked_eye_position(character.position, forward_dir, cam_follow.distance, cam_follow.height_offset)` (game_world.cpp:122-123).

**Debug visualization:**
Existing green orientation arrow sufficient—no additional debug arrow needed.

**Integration points:**
- `camera_follow::mode` stores camera_mode enum (SSOT) (camera_follow.h:10)
- `game_world::update()` at line 117 branches on mode from cam_follow component
- Direction derived once from `character_visuals.orientation.get_yaw()` via `math::yaw_to_forward()` and passed to camera function
<!-- END: GRAYBOX/IMPLEMENTATION_PLAN -->

<!-- BEGIN: GRAYBOX/REVIEW -->
## Implementation Review

**Tool:** Codex CLI
**Date:** 2025-10-21

**Question asked:**
Does this implementation plan follow the principles? Specifically: 1) Is the pure function approach correct (no accumulated state)? 2) Is the camera mode enum the right level of abstraction? 3) Should heading_yaw from controller be the source, or should we use velocity-derived direction? 4) Any violations of orthogonality or Single Source of Truth?

**Feedback received:**
- Pure function approach correct; prefer vec3 forward_dir input over yaw for composability
- Camera mode enum acceptable; keep it localized in one place (SSOT)
- Use orientation system's facing output, not raw controller heading_yaw or velocity
- Avoid duplicating camera_mode state; store once in camera_follow_component
- Compute direction vector once per frame and pass to camera function
- Keep camera math geometric and ignorant of "modes"

**Impact on implementation:**
- Changed function signature from `direction_yaw` to `forward_dir` (vec3)
- Removed camera_mode from game_world, kept only in camera_follow_component
- Use character_visuals.orientation.get_yaw() instead of character.heading_yaw
- Compute direction once and reuse for camera function
<!-- END: GRAYBOX/REVIEW -->

---

<!-- BEGIN: GRAYBOX/RESULTS -->
## Results

**Status:**
- [x] Core functional
- [x] Build successful
- [x] Debug viz working (existing orientation arrow sufficient)
- [x] Ready for iteration

**Works:**
- Camera locks instantly behind character orientation
- Smooth following from upstream orientation spring
- Mode toggle in GUI (Free Orbit / Lock to Orientation)
- Zoom/distance controls work in both modes
- Pure function approach (no accumulated state)

**Doesn't:**
- Nothing broken

**Surprises:**
- Debug arrow redundant (removed) - existing green orientation arrow already shows direction

**Next:**
- ITERATE phase
<!-- END: GRAYBOX/RESULTS -->

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [x] Core functional in graybox
- [x] Mathematical correctness proven
- [x] Debug visualization shows behavior
- [ ] Emergent behaviors appear
- [x] Principles upheld
<!-- END: SELECT/SUCCESS -->

---

## Iterations

- [CAMERA_LOCK_ITERATION_1.md](CAMERA_LOCK_ITERATION_1.md) - REVISE
- [CAMERA_LOCK_ITERATION_2.md](CAMERA_LOCK_ITERATION_2.md) - REVISE
- [CAMERA_LOCK_ITERATION_3.md](CAMERA_LOCK_ITERATION_3.md) - APPROVED
