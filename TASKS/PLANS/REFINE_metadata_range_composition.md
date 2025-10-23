# Refinement: Metadata Range Composition

Make metadata primitives composable by deriving distance range from min/max distance metadata.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-23
**Complexity:** Standard
**Path:** B (standard)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/camera/camera_follow.h:28,36,40`
**Principle:** Fundamental Composable Functions, Single Source of Truth
**Severity:** Low
**Type:** Non-composable metadata primitives

**Current state:**
```cpp
static constexpr param_meta distance_meta = {
    "Distance", "m", 1.5f, 15.0f  // Static range
};

static constexpr param_meta min_distance_meta = {
    "Min Distance", "m", 0.5f, 10.0f
};

static constexpr param_meta max_distance_meta = {
    "Max Distance", "m", 5.0f, 30.0f
};
```

**Why violation:**
- distance_meta range [1.5, 15.0] is independent of min_distance_meta and max_distance_meta ranges
- Designer can set min_distance=8.0 and max_distance=12.0, but distance slider still shows [1.5, 15.0]
- Distance slider can display values the system will clamp, creating false affordance
- Metadata primitives cannot inform each other, violating composability

**Impact:**
- Blocks: Derived metadata patterns, composable parameter systems
- Cascades to: Any system using metadata-driven UI with interdependent parameters
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify

**FROM:** Static independent metadata ranges
**TO:** Derive distance_meta range from min/max metadata during GUI presentation

**Derive/Generalize:**
Metadata-about-metadata pattern: When parameter A is constrained by parameters B and C, derive A's metadata range from B and C's metadata ranges at presentation time, not definition time.

**Check duplicates:**
Audit all param_meta definitions for similar interdependencies (parameter ranges that should derive from other parameters' ranges).
<!-- END: SELECT/FIX -->
