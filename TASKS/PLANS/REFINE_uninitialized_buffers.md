# Refinement: Uninitialized Buffers

Initialize renderer buffer members to match sokol handle pattern.

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

**Location:** `src/rendering/renderer.cpp:6`
**Principle:** Consistency
**Severity:** Medium
**Type:** Uninitialized members

**Current state:**
```cpp
wireframe_renderer::wireframe_renderer()
    : pipeline({0})
    , shader({0}) {}
```

Members `dynamic_vertex_buffer` and `dynamic_index_buffer` not in initializer list.

**Why violation:**
- Inconsistent with `pipeline` and `shader` initialization pattern
- Potential UB if accessed before `init()` call
- cppcheck flags as uninitMemberVar

**Impact:**
- Blocks: Safe construction guarantees
- Cascades to: None (isolated to renderer)
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify

**SIMPLIFY:**
- From: Partial member initialization (2 of 4 sokol handles)
- To: Complete member initialization (all sokol handles zero-initialized)
- Derive/Generalize: Match existing pattern for `pipeline` and `shader`
- Check duplicates: None - single initialization site
<!-- END: SELECT/FIX -->
