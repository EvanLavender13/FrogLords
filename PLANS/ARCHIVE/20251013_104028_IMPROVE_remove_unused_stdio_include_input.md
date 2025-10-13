# Cleanup: Remove Unused stdio Include (Input)

**Severity/Priority:** Medium

**Current Issue:** `src/input/input.cpp` includes `<cstdio>` but does not use any symbols from it.

**Files Affected:**
- `src/input/input.cpp`

**Proposed Fix:** Remove the unused `#include <cstdio>` from `src/input/input.cpp`.

**Rationale:** Simplifies includes and reduces compile surface area, aligning with AGENTS.md principles of clarity over cleverness and avoiding unnecessary dependencies. This is a mechanical cleanup that preserves behavior.

**Workflow Path:** Path A (Trivial)

**Estimated Complexity:** 1 point

**Risk Level:** Low — purely mechanical include removal; no functional changes.

**Tags:** #cleanup #includes #input

---

## EXECUTION (Trivial Path)

**Executed:** 2025-10-13

### Changes Made

**File:** `src/input/input.cpp`
- Line 4: Removed `#include <cstdio>` (unused)

### Verification

- [x] Build succeeds
- [x] clang-format passed
- [x] clang-tidy clean
- [x] No new warnings

### Notes

Purely mechanical include removal. Confirmed `gui.cpp` still requires `<cstdio>` for `snprintf`, so no batching beyond this single file.

---

## FINALIZATION

**Finalized:** 2025-10-13

### Changes Summary

**Files Modified:**
- `src/input/input.cpp` - Removed unused `#include <cstdio>`
- `PLANS/IMPROVE_BACKLOG.md` - Moved backlog item to Completed

**Impact:** Simplified input module includes and captured the cleanup in backlog documentation for future traceability.

### Backlog Updates

**Item Status:** Moved to Completed in `IMPROVE_BACKLOG.md`

**New Items Discovered:** None

**Batch Opportunities Noted:** None

### Learnings

- GUI overlay helpers still rely on `snprintf`; wait for a shared formatting helper before removing `<cstdio>` elsewhere.

**Time Estimate Accuracy:**
- Estimated: 1 point
- Actual: 1 point
- Variance: Accurate

### Final Validation

- [x] Clean build passes
- [x] clang-format clean
- [x] clang-tidy clean
- [x] No new warnings
- [x] Git diff reviewed
- [x] Backlog updated
- [x] Learnings captured

### Commit Message

```
[Improve] Remove unused stdio include from input

Sweep input module to drop leftover <cstdio> include and record the cleanup.

Files:
- src/input/input.cpp
- PLANS/IMPROVE_BACKLOG.md
- PLANS/IMPROVE_remove_unused_stdio_include_input.md

Doc: PLANS/IMPROVE_remove_unused_stdio_include_input.md
```
