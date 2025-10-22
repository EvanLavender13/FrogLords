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

Add const qualifier to read-only reference parameters in headers and implementations:
- `character_panel.{h,cpp}`: character, visuals, params
- `camera_panel.{h,cpp}`: cam, cam_follow

State parameters remain mutable.
<!-- END: SELECT/FIX -->
