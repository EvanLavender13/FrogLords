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

<!-- BEGIN: SELECT/GRAYBOX -->
## Graybox

**Simplest implementation:**
- Pure helper function (no class state)
- GUI toggle: Free Orbit / Lock to Orientation
- Same zoom/distance sliders as orbit
- Debug arrows show active lock direction

**Validation:**
- Toggle modes while moving
- Verify camera snaps instantly to character forward
- Smooth following (orientation already smoothed by yaw spring)
- Test with car-like controls (W/S forward/back, A/D turn)
<!-- END: SELECT/GRAYBOX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Core functional in graybox
- [ ] Mathematical correctness proven
- [ ] Debug visualization shows behavior
- [ ] Emergent behaviors appear
- [ ] Principles upheld
<!-- END: SELECT/SUCCESS -->
