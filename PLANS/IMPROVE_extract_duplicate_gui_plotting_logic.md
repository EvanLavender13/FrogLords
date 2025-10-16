# Extract Duplicate GUI Plotting Logic

**Severity/Priority:** High

**Current Issue:** The functions `gui::plot_value` and `gui::plot_histogram` in `src/gui/gui.cpp` contain identical logic for managing and pruning a rolling data buffer. This is a direct violation of the "Don't Repeat Yourself" (DRY) principle.

**Files Affected:**
- `src/gui/gui.cpp`

**Proposed Fix:** Extract the duplicated buffer management logic into a private static helper function (e.g., `update_plot_buffer`). Both `plot_value` and `plot_histogram` will then call this helper.

**Rationale:** Consolidates duplicated code, which improves maintainability and reduces the risk of inconsistent changes. This aligns with the core principle of "simplicity over sophistication."

**Workflow Path:** Path A (Trivial)

**Estimated Complexity:** 2 points

**Risk Level:** Low - This is a simple, behavior-preserving refactoring in a single file.

**Augmentations:** pattern_extraction

---

## EXECUTION (Trivial Path)

**Executed:**

### Changes Made

**File:** `src/gui/gui.cpp`
- Added new static helper function `update_plot_buffer` to encapsulate buffer update logic.
- Replaced duplicated code in `plot_value` with a call to `update_plot_buffer`.
- Replaced duplicated code in `plot_histogram` with a call to `update_plot_buffer`.
- Reordered helper functions to fix a compilation error.

### Verification

- [x] Build succeeds
- [ ] clang-format passed
- [ ] clang-tidy clean
- [ ] No new warnings

### Notes
- Initially introduced a build error by defining a function before its dependency. The issue was resolved by reordering the static helper functions.
- User confirmed the final build was successful.
- Formatting and linting were not run by me due to execution constraints.

---

## FINALIZATION

**Finalized:**

### Changes Summary

**Files Modified:**
- `src/gui/gui.cpp` - Refactored to remove duplicated code by extracting a helper function.

**Impact:** Improved code maintainability and simplicity by adhering to the DRY principle.

### New Items Discovered

**Added to Backlog:** None

### Learnings

**What Worked Well:**
- The trivial workflow is effective for small, focused refactoring tasks.
- Identifying the duplicated code and the fix was straightforward.

**What Was Harder Than Expected:**
- A simple mistake in function ordering led to a build failure, highlighting the importance of considering declaration order in C++ even for small changes.

**Insights for Future Work:**
- When extracting a helper function, ensure it is declared or defined before it is called to avoid compilation errors.

**Time Estimate Accuracy:**
- Estimated: 2 points
- Actual: 2 points
- Variance: Accurate
