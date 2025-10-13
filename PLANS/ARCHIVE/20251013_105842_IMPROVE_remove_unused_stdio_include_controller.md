# Cleanup: Remove Unused stdio Include (Character Controller)

**Severity/Priority:** Medium

**Current Issue:** `src/character/controller.cpp` includes `<cstdio>` but does not use any symbols from it.

**Files Affected:**
- `src/character/controller.cpp`

**Proposed Fix:** Remove the unused `#include <cstdio>` from `src/character/controller.cpp`.

**Rationale:** Keep dependencies minimal and improve clarity, aligning with AGENTS.md: clarity over cleverness, and avoiding unnecessary includes at higher layers.

**Workflow Path:** Path A (Trivial)

**Estimated Complexity:** 1 point

**Risk Level:** Low — purely mechanical include removal; no functional changes.

**Tags:** #cleanup #includes #controller

---

## EXECUTION (Trivial Path)

**Executed:** 2025-10-13

### Changes Made

**File:** `src/character/controller.cpp`
- Line 11: Removed `#include <cstdio>` (unused)

### Verification

- [x] Build succeeds
- [x] clang-format passed
- [x] clang-tidy clean
- [x] No new warnings

### Notes

Controller module no longer pulls in stdio. Rendering layer still needs `<cstdio>` for debug formatting utilities.

---

## FINALIZATION

**Finalized:** 2025-10-13

### Changes Summary

**Files Modified:**
- `src/character/controller.cpp` - Removed unused `#include <cstdio>`
- `PLANS/IMPROVE_BACKLOG.md` - Moved backlog item to Completed

**Impact:** Reduced unnecessary includes in the character controller, keeping dependencies lean per layered architecture and clarity principles.

### Backlog Updates

**Item Status:** Moved to Completed in `IMPROVE_BACKLOG.md`

**New Items Discovered:** None

**Batch Opportunities Noted:** None (GUI still needs `<cstdio>` for `snprintf`)

### Learnings

- Keep cleanup surgical: only remove headers where proven unused; debug/GUI formatting still legitimately uses `<cstdio>`.

**Time Estimate Accuracy:**
- Estimated: 1 point
- Actual: 1 point
- Variance: Accurate

### Final Validation

- [x] Clean build passes
- [x] clang-format clean
- [x] clang-tidy clean
- [x] No new warnings
- [x] Git diff reviewed (note: `TASKS/IMPROVE/SELECT.md` modified outside this change)
- [x] Backlog updated
- [x] Learnings captured

### Commit Message

```
[Improve] Remove unused stdio include from controller

Drop leftover <cstdio> include in character controller and document the cleanup.

Files:
- src/character/controller.cpp
- PLANS/IMPROVE_BACKLOG.md
- PLANS/IMPROVE_remove_unused_stdio_include_controller.md

Doc: PLANS/IMPROVE_remove_unused_stdio_include_controller.md
```
