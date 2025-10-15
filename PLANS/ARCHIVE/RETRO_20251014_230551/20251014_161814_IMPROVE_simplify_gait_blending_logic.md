# Simplify Gait Blending Logic

**Severity/Priority:** Medium

**Current Issue:** `gui/character_panel.cpp` has a `compute_base_walk_factor` function that re-implements the logic from `locomotion_system::get_run_blend`. The GUI should not be re-implementing core game logic.

**Files Affected:**
- `src/gui/character_panel.cpp`
- `src/character/locomotion.cpp`

**Proposed Fix:** Remove `compute_base_walk_factor` from the GUI and have it call `locomotion.get_run_blend()` directly (and compute `1.0f - run_blend` if needed).

**Rationale:** Consolidates game logic within the responsible system (`locomotion`), making the GUI purely a view/controller. Reduces duplication and improves maintainability. Adheres to the "Clarity over cleverness; simplicity over sophistication" and "Layered Architecture" principles.

**Workflow Path:** Path A (Trivial)

**Estimated Complexity:** 1 point

**Risk Level:** Low - The change is a simple replacement of duplicated logic with a direct call to the authoritative source.

**Tags:** #simplification #cleanup #gui #locomotion

---

## EXECUTION (Trivial Path)

**Executed:** 2025-10-14

### Changes Made

**File:** `src/gui/character_panel.cpp`
- Removed duplicated `compute_base_walk_factor` function.
- Updated call site to use `1.0f - locomotion.get_run_blend()` to derive the walk factor from the authoritative source in the locomotion system.

### Verification

- [x] Build succeeds
- [x] clang-format passed
- [x] clang-tidy clean
- [x] No new warnings

### Notes

The change was straightforward. The `replace` tool was used to remove the function and update the call site in a single atomic operation. This ensures the GUI is no longer re-implementing core game logic, adhering to the planned architectural improvement.

---

## FINALIZATION

**Finalized:** 2025-10-14

### Changes Summary

**Files Modified:**
- `src/gui/character_panel.cpp` - Removed duplicated gait blending logic.

**Impact:**
- Consolidated game logic within the `locomotion` system, removing it from the `gui` layer.
- Improved maintainability and adherence to the project's layered architecture.

### New Items Discovered

**Added to Backlog:**
- None.

### Learnings

**What Worked Well:**
- The Trivial Path workflow was efficient for this small, focused change.
- Using the `replace` tool to perform the deletion and modification in one step was clean and effective.

**What Was Harder Than Expected:**
- Nothing. The task was as simple as estimated.

**Insights for Future Work:**
- This type of architectural cleanup (moving logic to the correct layer) is high-value and low-risk. It's good to prioritize these when they appear in the backlog.

**Time Estimate Accuracy:**
- Estimated: 1 point
- Actual: 1 point
- Variance: Accurate
