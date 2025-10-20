# Refinement: Input Scaling Outside Camera System

**Move magic input scaling from runtime into camera system as tunable parameters**

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-20
**Complexity:** Trivial
**Path:** A (trivial)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/app/runtime.cpp:76-77,83`
**Principle:** Consistency (Input is Intent), Composable Functions
**Severity:** Medium
**Type:** Lost responsiveness, poor separation of concerns

**Current state:**
```cpp
// runtime.cpp:76-77
float delta_x = (input::mouse_x() - last_mouse_x) * 0.5f;
float delta_y = (input::mouse_y() - last_mouse_y) * 0.5f;

// runtime.cpp:83
world.apply_camera_zoom(-scroll_delta * 0.5f);
```

**Why violation:**
- Input layer applies magic `0.5f` scaling before passing to camera system
- Violates "Input is Intent" - input should be pure player intent, not pre-processed
- Makes camera feel less responsive (halves all input)
- Scaling belongs in camera system where it can be tuned and exposed in GUI

**Impact:**
- Blocks: Camera responsiveness tuning (parameters hidden in wrong layer)
- Cascades to: User perception of camera control (feels sluggish)
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Delete + Simplify

**DELETE:**
- Remove: `* 0.5f` scaling from mouse delta and scroll in runtime.cpp
- Impact: Camera input now passes unscaled values to camera system
- Tests: Manual - verify camera still responds to input

**SIMPLIFY:**
- From: Input layer scales → camera consumes
- To: Input layer passes raw → camera scales with tunable parameters
- Add to `camera_follow`: sensitivity fields for orbit and zoom
- Pseudocode:
  ```
  struct camera_follow {
      float orbit_sensitivity = 0.5f;  // degrees per pixel
      float zoom_sensitivity = 0.5f;   // distance per scroll unit

      void orbit(float delta_x, float delta_y) {
          longitude += delta_x * orbit_sensitivity;
          latitude += delta_y * orbit_sensitivity;
          // ... clamp and wrap
      }

      void zoom(float delta) {
          distance += delta * zoom_sensitivity;
          // ... clamp
      }
  };
  ```
- Check duplicates: None - this is the only place input scaling occurs

<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [x] Violation resolved
- [x] Principle upheld
- [x] Tests passing
- [x] No regressions

**Metrics:**
- Before: LOC 80 (runtime.cpp)
- After: LOC 80 (runtime.cpp, +0)
<!-- END: SELECT/SUCCESS -->

---

<!-- BEGIN: MEASURE/METRICS -->
## Metrics

**Files:**
- `src/app/runtime.cpp`: 80 → 80 (+0 lines, -3 violations)
- `src/camera/camera_follow.h`: 26 → 30 (+4 lines)
- `src/camera/camera_follow.cpp`: 54 → 54 (+0 lines)

**Total:** +4 lines (net addition of tunable parameters)

**Violations removed:**
- Magic constants in input layer: 3 → 0 (-3)
<!-- END: MEASURE/METRICS -->

---

<!-- BEGIN: MEASURE/LEARNING -->
## Learning

**Root cause:** Input scaling applied at wrong layer - runtime owns intent, camera owns interpretation

**Prevention:** Input functions should pass raw values; systems that consume input own their own scaling/sensitivity

**Remaining work:** None - this was the only location where input layer modified intent before passing to systems
<!-- END: MEASURE/LEARNING -->
