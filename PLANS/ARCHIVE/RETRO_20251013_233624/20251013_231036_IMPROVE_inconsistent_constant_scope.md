# Inconsistent Constant Naming Scope

**Severity/Priority:** Low

**Current Issue:** Constants in the character subsystem use inconsistent scoping patterns. File-scope constants use anonymous namespaces (e.g., `FRICTION_RATIO`, `BUMPER_RADIUS`), while some derived constants are repeated at function scope (e.g., `NET_FRACTION` appears twice in `tuning.cpp`).

**Files Affected:**
- `src/character/tuning.cpp`

**Proposed Fix:** Extract the repeated function-scope `NET_FRACTION` constant to the anonymous namespace at file scope, eliminating duplication and aligning with the existing pattern used for `FRICTION_RATIO` and other file-scope constants.

**Rationale:** Consistency in constant placement improves code clarity and follows the "simplicity over sophistication" principle. File-scope constants in anonymous namespaces establish single sources of truth and prevent duplication across functions.

**Workflow Path:** Path A (Trivial)

**Estimated Complexity:** 2 points

**Risk Level:** Low - Mechanical refactor within a single file; no behavior change; simply moves a duplicated constant to file scope.

**Tags:** #conventions #polish #cleanup

---

## EXECUTION (Trivial Path)

**Executed:** 2025-10-14 02:58 UTC

### Changes Made

**File:** `src/character/tuning.cpp`
- Line 7: Added `NET_FRACTION` constant to anonymous namespace (calculated as `1.0f - FRICTION_RATIO`)
- Line 17: Removed function-scope `constexpr float NET_FRACTION` declaration from `apply_to()`
- Line 30: Removed function-scope `constexpr float NET_FRACTION` declaration from `read_from()`

### Verification

- [x] Build succeeds
- [x] clang-format passed
- [x] clang-tidy clean
- [x] No new warnings

### Notes

Simple extraction that eliminates duplication and aligns with existing file-scope constant pattern. The `NET_FRACTION` constant is now defined once at file scope alongside `FRICTION_RATIO`, matching the same pattern used in `controller.cpp` for its constants. Both functions now reference the shared constant instead of redefining it locally.

---

## FINALIZATION

**Finalized:** 2025-10-14 03:06 UTC

### Changes Summary

**Files Modified:**
- `src/character/tuning.cpp` - Added `NET_FRACTION` constant to anonymous namespace (line 7); removed duplicate function-scope declarations from `apply_to()` and `read_from()` functions

**Impact:**
Established consistent constant scoping pattern across character subsystem. Eliminated duplication of the `NET_FRACTION` constant definition, creating a single source of truth at file scope that matches the existing pattern used for `FRICTION_RATIO` and constants in other character files.

### Backlog Updates

**Item Status:** Moved to Completed in IMPROVE_BACKLOG.md

**New Items Discovered:**
- None

**Batch Opportunities Noted:**
- None at this time

### Learnings

**What Worked Well:**
- Trivial path execution was appropriate for this localized cleanup
- Anonymous namespace pattern provides clear file-scope visibility
- Single-file change minimized risk and validation effort

**What Was Harder Than Expected:**
- Nothing - straightforward refactor as planned

**Insights for Future Work:**
- File-scope constants work well for derived values (like `NET_FRACTION = 1.0f - FRICTION_RATIO`)
- Anonymous namespace is the right scope for constants shared across multiple functions in the same file
- This pattern could be documented in CONVENTIONS.md if more examples emerge

**Time Estimate Accuracy:**
- Estimated: 2 points
- Actual: ~2 points (including documentation)
- Variance: Accurate
