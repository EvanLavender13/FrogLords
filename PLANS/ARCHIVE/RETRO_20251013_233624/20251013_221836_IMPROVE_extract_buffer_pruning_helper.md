# Extract Buffer Pruning Helper

**Severity/Priority:** Medium

**Current Issue:** The `plot_value()` and `plot_histogram()` functions in `src/gui/gui.cpp` contain identical logic for pruning old data from their plot buffers. This duplication makes the code harder to maintain.

**Files Affected:**
- `src/gui/gui.cpp`

**Proposed Fix:** Create a static helper function `prune_plot_buffer` that encapsulates the common pruning logic and call it from both `plot_value()` and `plot_histogram()`.

**Rationale:** This change improves code clarity and maintainability by removing duplicated code, adhering to the "simplicity over sophistication" principle. It establishes a single source of truth for plot buffer management.

**Workflow Path:** Path A (Trivial)

**Estimated Complexity:** 2 points

**Risk Level:** Low - The change is localized to a single file and refactors existing logic without altering behavior.

**Tags:** #pattern-extraction #gui #cleanup

---

## EXECUTION (Trivial Path)

**Executed:** 2025-10-14 02:13 UTC

### Changes Made

**File:** `src/gui/gui.cpp`
- Lines 99-111: Created static helper function `prune_plot_buffer()` to encapsulate buffer pruning logic
- Line 124: Replaced inline pruning code in `plot_value()` with call to `prune_plot_buffer()`
- Line 163: Replaced inline pruning code in `plot_histogram()` with call to `prune_plot_buffer()`

### Verification

- [x] Build succeeds
- [x] clang-format passed
- [x] clang-tidy clean
- [x] No new warnings

### Notes

Refactoring completed successfully. The helper function extracts 8 lines of duplicated pruning logic (size capping and time-window pruning) into a single reusable function. Both plot functions now call the helper, eliminating code duplication and establishing a single source of truth for buffer management. No behavior changes - pure extraction refactor.

---

## FINALIZATION

**Finalized:** 2025-10-14 02:17 UTC

### Changes Summary

**Files Modified:**
- `src/gui/gui.cpp` - Created `prune_plot_buffer()` helper function (lines 99-111); replaced inline pruning code in `plot_value()` and `plot_histogram()` with calls to helper

**Impact:**
Eliminated code duplication by extracting identical buffer pruning logic into a single reusable helper function. Reduced line count by ~14 lines and established single source of truth for plot buffer management.

### Backlog Updates

**Item Status:** Moved to Completed in IMPROVE_BACKLOG.md

**New Items Discovered:**
- None

**Batch Opportunities Noted:**
- None at this time

### Learnings

**What Worked Well:**
- Trivial path execution was appropriate for this localized refactor
- Static helper function pattern works cleanly for internal utilities
- Code diff was clear and easy to verify

**What Was Harder Than Expected:**
- Nothing - straightforward extraction as planned

**Insights for Future Work:**
- Pattern extraction of duplicated logic blocks is low-risk and high-value
- Internal static helpers are the right abstraction level for single-file utilities
- Similar opportunities likely exist in other GUI functions

**Time Estimate Accuracy:**
- Estimated: 2 points
- Actual: ~2 points (including documentation)
- Variance: Accurate
