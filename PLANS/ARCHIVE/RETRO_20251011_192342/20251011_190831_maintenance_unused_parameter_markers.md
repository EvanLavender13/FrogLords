# Unused Parameter Markers

**Severity:** Low

**Current Issue:** Several functions silence unused parameters by casting them to `void`, which is harder to scan than applying the C++17 `[[maybe_unused]]` attribute that the codebase prefers.

**Files Affected:** src/main.cpp, src/rendering/debug_draw.cpp

**Suggested Fix:** Replace the `(void) param;` patterns with `[[maybe_unused]]` annotations on the relevant parameters so the intent is explicit.

**Workflow Path:** Path B (Standard)

**Estimated Complexity:** 2 points

---

## Implementation

**Date:** 2025-10-11

**Changes Made:**
- src/main.cpp: Annotated `sokol_main` parameters with `[[maybe_unused]]` and removed the void casts.
- src/rendering/debug_draw.cpp: Applied `[[maybe_unused]]` to locomotion parameters and dropped the `(void)` suppressors.

**Verification:**
- Build check: Not run (attribute-only change).
- Standards check: Confirmed formatting and attribute placement follow project style.

**Issues Encountered:** None

**Next Step:** REVIEW_FIX

---

## Code Review

**Date:** 2025-10-11
**Status:** APPROVED

**Fix Summary:** Replaced `(void)` casts with the `[[maybe_unused]]` attribute for unused function parameters.

**Verification Results:**
- Issue Resolution: ✓
- Code Standards: ✓
- Dependency Flow: ✓
- Scope Discipline: ✓

**Issues Found:** None

**Recommendation:** Ready to finalize.

---

## Finalization

**Date:** 2025-10-11
**Status:** COMPLETED

**Backlog Update:**
- Moved to Completed in MAINTENANCE_BACKLOG.md
- Learnings documented in backlog entry

**Related Items:**
- None identified

**Next Steps:**
- Ready for commit
