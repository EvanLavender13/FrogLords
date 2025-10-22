# Refinement: Variable Scope Reduction

Reduce scope of `pre_collision_vertical_velocity` to innermost block.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-21
**Complexity:** Trivial (1 pt)
**Path:** A (trivial)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/character/controller.cpp:215`
**Principle:** Radical Simplicity
**Severity:** Low
**Type:** Variable declared at wider scope than necessary

**Current state:**
```cpp
// Line 215 - declared before collision resolution
float pre_collision_vertical_velocity = velocity.y;
sphere_collision contact =
    resolve_collisions(collision_sphere, *world, position, velocity, wall_threshold);

// Line 235 - only used here, 20 lines later
if (just_landed) {
    vertical_velocity_on_land = pre_collision_vertical_velocity;
}
```

**Why violation:**
- Variable declared at function scope but only used within landing detection block
- Wider scope than necessary increases cognitive load
- Declaration distance from usage reduces clarity

**Impact:**
- Blocks: Nothing (isolated)
- Cascades to: Nothing (single variable)
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Document (False Positive)

**DISCOVERY:**
This is a cppcheck false positive. Variable MUST be declared before `resolve_collisions` because that function modifies `velocity` by reference. Moving declaration later would capture post-collision velocity, breaking landing spring calculation.

**ACTUAL FIX:**
Accept variable scope but improve clarity:
1. Add phase-based scoping comments (Collision Resolution / Landing Detection)
2. Add inline suppression comment explaining why scope is correct
3. Update analyze.sh to enable `--inline-suppr` for cppcheck

**RATIONALE:**
Variable scope is semantically minimal - captures pre-state before mutation. Phase structure makes temporal dependency obvious.

<!-- END: SELECT/FIX -->

---

<!-- BEGIN: REFINE/COMPLETED -->
## Completed

**Change:** Added phase-based scoping comments and cppcheck inline suppression
**Files Modified:**
- `src/character/controller.cpp:212-242` - Added phase comments and suppression
- `scripts/bash/analyze.sh:48` - Added `--inline-suppr` flag

**Tests:** All passing (build verified)
**Analysis:** cppcheck variableScope warning now suppressed

**Result:** ✓ False positive handled - code clarity improved, tool noise removed
<!-- END: REFINE/COMPLETED -->
