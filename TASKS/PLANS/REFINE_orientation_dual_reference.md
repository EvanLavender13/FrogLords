# Refinement: Orientation Dual-Reference Investigation

**Investigate and resolve potential dual-reference violation in orientation system**

---

## Selected

**Date:** 2025-10-17
**Complexity:** Standard (needs investigation)
**Path:** B (standard - requires thought and validation)

---

## Violation Details

**Location:**
- File: `src/character/orientation.{h,cpp}`
- Lines: orientation.h:5 (target_yaw member), orientation.cpp:14-24 (update logic)
- System: Orientation System (Layer 3)

**Principle Violated:**
- Primary: Consistency (The Dual Reference pattern)
- Score before: 8.5/10

**Severity:** High

**Type:** Potential circular reference / Complexity

**Note:** Requires investigation to confirm actual violation vs. backlog artifact

---

## Current State

**What exists now:**
```cpp
// orientation.h
struct orientation_system {
    float target_yaw = 0.0f;     // Member variable - persists across frames
    float current_yaw = 0.0f;    // Smoothed output
    float yaw_smoothing = 5.0f;  // Exponential decay rate

    void update(glm::vec3 velocity, float dt);
    float get_yaw() const { return current_yaw; }
};

// orientation.cpp
void orientation_system::update(glm::vec3 velocity, float dt) {
    float speed = glm::length(velocity);

    if (speed > 0.01f) {
        target_yaw = glm::atan(velocity.x, velocity.z);  // Recalculated from velocity
        float delta = math::angle_difference_radians(target_yaw, current_yaw);
        float smoothing_factor = 1.0f - std::exp(-yaw_smoothing * dt);
        current_yaw += delta * smoothing_factor;
        current_yaw = math::wrap_angle_radians(current_yaw);
    }
    // When not moving: target_yaw retains old value
}
```

**Backlog claims:**
- "target yaw references smoothed yaw"
- "Circular reference"
- ~50 lines of complexity

**Actual observation:**
- File is only 27 lines total
- `target_yaw` is calculated from `velocity` (pure input), not from `current_yaw`
- `target_yaw` persists as member variable (retains value when stopped)
- Uses exponential smoothing instead of spring-damper

**Possible violations:**
1. **State persistence:** `target_yaw` as member variable might be unnecessary
2. **Complexity:** Exponential smoothing reimplements what spring-damper should do
3. **Backlog outdated:** Violation may have been fixed in earlier refactors

**Why this might violate principles:**
- Target_yaw persisting as state could be considered "accumulated state"
- Exponential smoothing is less principled than spring-damper (Layer 2 primitive)
- Could be simpler: derive target locally, use spring-damper for smoothing

**Impact:**
- Blocks: Further orientation improvements
- Cascades to: Animation system that uses orientation
- Foundation impact: Layer 3 at 93%, needs 95%

---

## Fix Approach

**Primary approach:** Investigate → Simplify

**Investigation Phase:**
1. Determine if dual-reference actually exists
2. If not, determine actual violation (complexity vs. spring-damper)
3. Update backlog if violation is misdescribed

**If TRUE dual-reference found:**
- From: target_yaw persisting across frames
- To: target_yaw as local variable
- Impact: Behavior when stopped (needs consideration)

**If COMPLEXITY is the issue:**
- From: Custom exponential smoothing logic
- To: Use spring-damper primitive (Layer 2, 100% proven)
- Benefits:
  - Reduces LOC (~10-15 lines removed)
  - Uses proven primitive instead of custom logic
  - Consistent with landing animation approach
  - Better velocity continuity (spring tracks velocity too)

**If BACKLOG OUTDATED:**
- Document: Why current implementation is correct
- Remove: Backlog entry
- Update: Dependency stack if needed

---

## Success Criteria

**This refinement succeeds when:**
- [ ] Actual violation identified (or confirmed absent)
- [ ] If violation exists: resolved via spring-damper or simplification
- [ ] If no violation: backlog updated, implementation documented
- [ ] All tests passing
- [ ] No behavioral regressions
- [ ] Principle score improved (if violation found)
- [ ] Layer 3 certainty improved

---

## Estimated Metrics

**Before:**
- LOC: 27 (orientation.cpp) + 18 (orientation.h) = 45
- Principle score: 8.5/10 (claimed)
- Dependencies: GLM, math_utils
- Approach: Exponential smoothing

**After (if spring-damper):**
- LOC: ~30-35 total (-10 to -15)
- Principle score: 9.5/10 (+1.0)
- Dependencies: GLM, math_utils, spring_damper (Layer 2)
- Approach: Spring-damper (proven primitive)

**After (if no violation):**
- LOC: Same (45)
- Principle score: Unchanged or +0.5 for documentation
- Backlog entry: Removed/updated
- Understanding: +10% certainty

---

## Risk Assessment

**What could go wrong:**
- Changing to spring-damper alters feel of rotation
- Removing target_yaw persistence changes stopped behavior
- Misidentifying violation wastes time
- Spring-damper might be overkill for simple yaw smoothing

**Mitigation:**
- Compare behaviors side-by-side with debug visualization
- Tune spring-damper to match current feel
- Document investigation even if no change made
- Keep current implementation as reference

**Rollback plan:**
- Git revert to current commit
- Exponential smoothing is simple and working
- Low risk change overall

---

## Investigation Questions

**To answer during PLAN phase:**

1. **Is target_yaw actually a dual-reference?**
   - Does it reference current_yaw? (No, uses velocity)
   - Should it be local vs. member? (Affects stopped behavior)

2. **What's the actual violation?**
   - Dual-reference? (Unclear)
   - Complexity? (Uses custom smoothing vs. spring-damper)
   - Nothing? (Backlog outdated)

3. **Should we use spring-damper?**
   - Benefits: Proven primitive, velocity continuity
   - Costs: Might be heavier than needed for yaw
   - Comparison: Landing animation uses spring-damper successfully

4. **What's the correct fix?**
   - Simplify to spring-damper
   - Make target_yaw local
   - Document current approach as correct
   - Update backlog

---

## Next Step

**→ REFINE/PLAN** - Investigation and analysis phase

Determine true nature of violation before implementing fix.
