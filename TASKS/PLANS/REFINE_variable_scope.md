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

**Approach:** Simplify

**FROM:**
Variable declared at function scope, far from single usage point.

**TO:**
Variable declared at innermost block scope, adjacent to usage.

**DERIVE/GENERALIZE:**
Move declaration from line 215 to line 234 (inside the `if (just_landed)` block). The value captured is from the pre-collision state, which is already captured before `resolve_collisions` modifies `velocity`.

**CHECK DUPLICATES:**
Single occurrence. No similar patterns in function.
<!-- END: SELECT/FIX -->
