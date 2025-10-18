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

---

## Investigation Results (2025-10-17)

**Question 1: Is target_yaw actually a dual-reference?**
- ❌ **NO** - `target_yaw` is recalculated from `velocity` (pure input) each frame
- `current_yaw` never influences `target_yaw`
- Backlog description is misleading

**Question 2: What's the actual violation?**
- ✅ **Composability Violation** - Reimplements smoothing instead of using spring-damper primitive
- ✅ **Unnecessary State** - `target_yaw` member variable serves no purpose (derived locally each frame)
- ❌ **Not Circular Reference** - No feedback loop exists

**Question 3: Should we use spring-damper?**
- ✅ **YES** - Benefits:
  - Proven Layer 2 primitive (100% validated)
  - Velocity continuity (tracks angular velocity)
  - Consistent with landing animation pattern
  - More precise tuning (stiffness + damping vs. single rate)
  - Physically correct (second-order vs. first-order approximation)
- Costs:
  - Requires tuning to match current feel
  - Slightly more state (2 floats vs 1)

**Question 4: What's the correct fix?**
- **Replace exponential smoothing with spring-damper**
- Remove `target_yaw` member variable (make local)
- Add `spring_damper yaw_spring` member
- Simplify update logic to use primitive

**Verdict:**
- Principle violated: **Fundamental Composable Functions** (not dual-reference)
- Severity: Medium (works but duplicates logic)
- Fix approach: Refactor to spring-damper (proven primitive)
- Complexity: Standard (requires tuning)

---

## Refinement Plan

**Goal:** Replace custom exponential smoothing with spring-damper primitive

**Benefits:**
- Removes: ~8 lines of custom smoothing logic
- Removes: Unnecessary state (target_yaw member)
- Adds: Spring-damper primitive usage (proven, tested)
- Improves: Velocity continuity
- Improves: Consistency with other systems (landing animation)

**Steps:**

### Step 1: Add spring-damper to orientation_system

**Changes:**
- File: `src/character/orientation.h`
- From: `float target_yaw`, `float current_yaw`, `float yaw_smoothing`
- To: `spring_damper yaw_spring`

**Implementation:**
```cpp
#include "foundation/spring_damper.h"

struct orientation_system {
    spring_damper yaw_spring;

    // Constructor initializes spring parameters
    orientation_system();

    void update(glm::vec3 velocity, float dt);
    float get_yaw() const { return yaw_spring.get_position(); }
};
```

**Validation:**
- [ ] Compiles
- [ ] No missing includes

---

### Step 2: Tune spring parameters

**Goal:** Match current feel (exponential smoothing with rate=5.0)

**Current behavior:**
- Time constant: τ = 1/rate = 1/5 = 0.2s
- Reaches 63% in 0.2s
- No overshoot (first-order system)

**Spring-damper tuning:**
- Use critical damping (no overshoot)
- Tune stiffness to match response time
- Target: Similar feel to current implementation

**Initial guess:**
```cpp
// Response time ~0.2s → natural frequency ω ≈ 5 rad/s
// ω = √(k/m) → k = ω² = 25 (for m=1)
yaw_spring.stiffness = 25.0f;
yaw_spring.damping = critical_damping(25.0f); // ≈ 10.0
```

**Validation:**
- [ ] Test in-game
- [ ] Compare rotation feel
- [ ] Adjust if needed

---

### Step 3: Simplify update logic

**Changes:**
- File: `src/character/orientation.cpp`
- From: Custom exponential smoothing
- To: Spring-damper update

**Implementation:**
```cpp
void orientation_system::update(glm::vec3 velocity, float dt) {
    float speed = glm::length(velocity);

    if (speed > 0.01f) {
        float target_yaw = glm::atan(velocity.x, velocity.z);

        // Handle angle wrapping for spring target
        float current = yaw_spring.get_position();
        float delta = math::angle_difference_radians(target_yaw, current);
        float wrapped_target = current + delta;

        yaw_spring.update({wrapped_target, dt});

        // Normalize output
        yaw_spring.position = math::wrap_angle_radians(yaw_spring.position);
    }
    // When stopped: spring continues toward last target
}
```

**Tests to update:**
- (None exist yet - this is a gap)

**Validation:**
- [ ] Compiles
- [ ] Tests pass (when they exist)
- [ ] Rotation feels similar
- [ ] No NaN/explosion
- [ ] Handles angle wrapping correctly

---

### Step 4: Add constructor for initialization

**Changes:**
- File: `src/character/orientation.cpp`
- Add: Constructor to initialize spring parameters

**Implementation:**
```cpp
orientation_system::orientation_system() {
    yaw_spring.stiffness = 25.0f;
    yaw_spring.damping = critical_damping(25.0f);
    yaw_spring.position = 0.0f;
    yaw_spring.velocity = 0.0f;
}
```

**Validation:**
- [ ] Compiles
- [ ] Spring initialized correctly

---

## Rollback Plan

**If this breaks things:**
1. `git reset --hard HEAD` (revert all changes)
2. Or: `git revert <commit>` (create inverse commit)

**Safe because:**
- On feature branch `refine/orientation-dual-reference`
- Small, focused changes
- Can revert to exponential smoothing easily
- Low risk (orientation is self-contained)

---

## Expected Metrics

**Before:**
- LOC: 27 (orientation.cpp) + 18 (orientation.h) = 45
- Members: 3 floats (target_yaw, current_yaw, yaw_smoothing)
- Smoothing: Custom exponential
- Principle score: 7/10 (composability violation)

**After:**
- LOC: ~35 total (-10)
- Members: 1 spring_damper (2 floats internally, but abstracted)
- Smoothing: Spring-damper primitive (Layer 2)
- Principle score: 9.5/10 (+2.5)

**Improvements:**
- ✅ Uses proven primitive
- ✅ Removes unnecessary state
- ✅ Removes custom smoothing logic
- ✅ Velocity continuity
- ✅ Consistent with landing animation pattern

---

## Risks and Mitigation

**Risk 1: Feel changes**
- Mitigation: Tune spring parameters to match
- Fallback: Adjust stiffness/damping iteratively

**Risk 2: Angle wrapping issues**
- Mitigation: Wrap target before spring, wrap output after
- Validation: Test 180° transitions

**Risk 3: Performance**
- Mitigation: Spring-damper is well-optimized
- Reality: Negligible (same order of complexity)

---

## Next Step

**→ REFINE/REFINE** - Execute the plan

Implement spring-damper replacement step by step.
