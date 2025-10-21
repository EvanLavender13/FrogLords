# Refinement: Const Naming Conventions

Rename local constant arrays and values to UPPER_CASE per conventions.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-20
**Complexity:** Trivial
**Path:** A (trivial)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/app/debug_generation.cpp:54,60,162`, `src/foundation/collision.cpp:16`
**Principle:** Consistency
**Severity:** Medium
**Type:** Naming convention violation

**Current state:**
```cpp
// debug_generation.cpp:54,60
constexpr gradient_stop gradient[] = {...};
constexpr int num_stops = sizeof(gradient) / sizeof(gradient[0]);

// debug_generation.cpp:162
constexpr int circle_segments = 16;

// collision.cpp:16
constexpr float epsilon = 0.0001f;
```

**Why violation:**
- CONVENTIONS.md states: "Exception: Enum constants are UPPER_CASE"
- .clang-tidy enforces UPPER_CASE for constant values
- Inconsistent with other constants in codebase
- clang-tidy flagged these locations

**Impact:**
- Blocks: Code style consistency
- Cascades to: Developer expectations, tooling alignment
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify

**CRITICAL:** Rename constants to UPPER_CASE convention. No behavioral changes.

**SIMPLIFY:**
- From: snake_case constant names
- To: UPPER_CASE constant names
- Rename targets:
  - `gradient` → `GRADIENT`
  - `num_stops` → `NUM_STOPS`
  - `circle_segments` → `CIRCLE_SEGMENTS`
  - `epsilon` → `EPSILON`
- Check duplicates: Each constant is single-use, scoped locally
- Update all references within same file

**Validation:**
- Build succeeds
- clang-tidy no longer flags these locations
- No runtime behavior changes
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Violation resolved
- [ ] Principle upheld
- [ ] Tests passing
- [ ] No regressions

**Metrics:**
- Before: LOC 4 violations
- After: LOC 0 violations (-4)
<!-- END: SELECT/SUCCESS -->
