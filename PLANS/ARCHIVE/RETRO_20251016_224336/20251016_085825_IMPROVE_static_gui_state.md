# [State Management] Static variables in gui/camera_panel.cpp

**Severity:** Medium

**Current Issue:** The `draw_camera_panel` function uses `static` variables for `min_dist` and `max_dist`. This makes the function non-reentrant and can lead to unexpected behavior if the panel is used in multiple places.

**Files Affected:**
- `src/gui/camera_panel.cpp`
- `src/gui/camera_panel.h`

**Proposed Fix:** Move the state into the `camera_panel_state` struct.

**Rationale:** Improves state management and makes the GUI more robust and predictable. This aligns with the principle of "simplicity over sophistication".

**Workflow Path:** Path A (Trivial)

**Complexity:** 1 point

**Risk:** Low - This is a simple, behavior-preserving refactoring in a single file.

**Tags:** #gui #state-management

---

## EXECUTION (Path A)
**Executed:** 2025-10-16

**Changes:**
- `src/gui/camera_panel.h`: Moved `min_dist` and `max_dist` to `camera_panel_state` struct.
- `src/gui/camera_panel.cpp`: Changed `draw_camera_panel` to take a non-const state and use the state variables.

**Verified:** Build ✓ Format ✓ Lint ✓

---

## FINALIZATION

**Finalized:** 2025-10-16

### Changes Summary
**Files Modified:**
- `src/gui/camera_panel.h` - Moved `min_dist` and `max_dist` to `camera_panel_state` struct.
- `src/gui/camera_panel.cpp` - Changed `draw_camera_panel` to take a non-const state and use the state variables.

**Impact:** Improved state management and made the GUI more robust and predictable.

### Learnings
**What Worked:** The trivial workflow is efficient for simple changes.
**What Was Hard:** Nothing.
**Future Insights:** Moving state out of functions and into state structs is a good pattern to follow for GUI code.

**Time Accuracy:**
- Estimated: 1 point
- Actual: 1 point
- Variance: Accurate
