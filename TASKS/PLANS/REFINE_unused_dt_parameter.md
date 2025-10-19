# Refinement: Unused Delta-Time Parameter

Remove dead parameter from camera follow target setter

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-19
**Complexity:** Trivial
**Path:** A (trivial)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/camera/camera.cpp:78-86`
**Principle:** Radical Simplicity | Score: Before fix
**Severity:** Low
**Type:** Dead parameter, Unclear intent

**Current state:**
```cpp
void camera::set_follow_target(const glm::vec3* target, float dt) {
    // dt parameter is never used
    follow_target = target;
    // Instantaneous mode change - no smoothing
}
```

**Why violation:**
- Dead parameter obscures function intent
- Unclear whether instantaneous transition is intentional or missing implementation
- Violates "Radical Simplicity" - every element must justify existence
- Creates cognitive load: "Why is dt here if unused?"

**Impact:**
- Blocks: None (low severity)
- Cascades to: Code clarity throughout codebase
- Foundation: Camera at 98%, minimal impact but clean delete opportunity
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Delete

**DELETE:**
- Remove: `float dt` parameter from `set_follow_target()` signature
- Impact:
  - Function signature becomes `void set_follow_target(const glm::vec3* target)`
  - All call sites must be updated (likely 1-3 locations)
  - Intent becomes clear: "This is an instantaneous mode change"
- Tests: No test updates needed (behavior unchanged, signature simplified)

**Rationale:**
Follow mode changes are intentionally instantaneous - there's no transition smoothing needed. The camera's follow behavior itself uses spring-damper smoothing, but enabling/disabling follow mode happens instantly. The `dt` parameter was likely copy-pasted from another function or added "just in case" but never used.

Deleting the parameter makes the intent explicit: mode changes are instantaneous, smoothing happens during the follow update loop.
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Violation resolved
- [ ] Principle score improved
- [ ] Tests passing
- [ ] No regressions

**Metrics:**
- Before: Parameter count 2, Intent unclear, Principle 7/10
- After: Parameter count 1 (-1), Intent explicit, Principle 10/10 (+3)
<!-- END: SELECT/SUCCESS -->
