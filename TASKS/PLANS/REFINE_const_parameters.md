# Refinement: Const Parameter References

Add const to read-only GUI panel parameters

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-21
**Complexity:** Trivial
**Path:** A (trivial)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/gui/character_panel.cpp:9`, `src/gui/camera_panel.cpp:8`
**Principle:** Consistency
**Severity:** Low
**Type:** Missing const-correctness on read-only parameters

**Why violation:**
- Five parameters are read but never modified
- Missing const violates API contract clarity
- Cppcheck flagged as constParameterReference

**Impact:**
- Blocks: Const-correctness, clear API contracts
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Document (const-correctness)

Add const qualifier to state parameters in headers and implementations:
- `character_panel.{h,cpp}`: state parameter
- `camera_panel.{h,cpp}`: state parameter
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: MEASURE/METRICS -->
## Metrics

**Files:**
- `src/gui/camera_panel.h`: 18 → 18 (0 lines, +5 chars)
- `src/gui/camera_panel.cpp`: 74 → 74 (0 lines, +6 chars)
- `src/gui/character_panel.h`: 23 → 23 (0 lines, +6 chars)
- `src/gui/character_panel.cpp`: 130 → 130 (0 lines, +6 chars)

**Total:** 0 lines changed, +23 characters (4 const qualifiers added)

**Violations removed:**
- constParameterReference warnings: 2 → 0 (-2)
<!-- END: MEASURE/METRICS -->

---

<!-- BEGIN: MEASURE/LEARNING -->
## Learning

**Root cause:** Panel functions only read state.show flag but accepted mutable reference (copy-paste from earlier iteration where state was modified).

**Prevention:** Run cppcheck regularly. Mark all parameters const by default, only remove when mutation is required.

**Remaining work:** None for this pattern - all GUI panel const-correctness violations resolved.
<!-- END: MEASURE/LEARNING -->
