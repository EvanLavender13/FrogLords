# Animation System Cleanup

**Severity:** Medium

**Current Issue:** This is a batch of two related issues in the animation system:
1. `update_acceleration_tilt` performs a manual 2D rotation using `sin` and `cos` to transform world-space acceleration to local space.
2. `#define GLM_ENABLE_EXPERIMENTAL` is defined locally in `animation.cpp`, which can lead to inconsistent behavior and ODR violations.

**Files Affected:**
- `src/character/animation.cpp`
- `CMakeLists.txt`

**Proposed Fix:**
1. Use a 2x2 rotation matrix or a quaternion to perform the transformation in `update_acceleration_tilt`.
2. Move the `#define GLM_ENABLE_EXPERIMENTAL` to a global build flag in the `CMakeLists.txt` file.

**Rationale:** Improves code clarity, robustness, and ensures consistent compilation. This aligns with the principles of "simplicity over sophistication" and "clarity over cleverness".

**Workflow Path:** Path B (Standard)

**Complexity:** 3 points

**Risk:** Low - These are simple, behavior-preserving refactorings.

**Tags:** #animation #simplification #build #dependencies #batch

---

## PLAN

**Created:** 2025-10-16
**Branch:** `improve/animation_cleanup`

### Impact Analysis
**Affected:** `animation` system, build process
**Risk:** Low
**Mitigation:** Each stage will be followed by a build to ensure no regressions are introduced.

### Stages
**Stage 1: Add `GLM_ENABLE_EXPERIMENTAL` to `CMakeLists.txt`**
- **Changes:**
  - In `CMakeLists.txt`, add `add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)`.
- **Validation:** The project builds successfully.

**Stage 2: Remove local define from `animation.cpp`**
- **Changes:**
  - In `src/character/animation.cpp`, remove the line `#define GLM_ENABLE_EXPERIMENTAL`.
- **Validation:** The project builds successfully.

**Stage 3: Refactor `update_acceleration_tilt`**
- **Changes:**
  - In `src/character/animation.cpp`, in the `update_acceleration_tilt` function, replace the manual 2D rotation with a `glm::mat2` rotation.
- **Validation:** The project builds successfully.

### Validation Checklist
- [ ] Build succeeds (no warnings)
- [ ] Behavior unchanged
- [ ] Dependency flow correct
- [ ] No reverse dependencies
- [ ] Naming conventions followed
- [ ] No duplication remains
- [ ] clang-format/tidy passes

---

## PLAN REVIEW

**Date:** 2025-10-16
**Status:** APPROVED

### Summary
The plan to clean up the animation system is well-structured and addresses two separate but related issues in a logical, staged manner. The risk is low, and the reward is a cleaner, more maintainable codebase.

### Violations (Must Fix)
None.

### Risks & Concerns
None. The plan is straightforward and the staged approach minimizes risk.

### Risk Assessment
**Risk:** Low
**Reward:** Medium
**Decision:** Proceed

### Actionable Items
None.

### Recommendation
**Reasoning:** The plan is a high-quality, low-risk refactoring that improves code clarity and build consistency.
**Next Steps:** Proceed to EXECUTE

---

## EXECUTION (Path B)
**Duration:** [Total time]

**Stages Completed:**
- Stage 1: [Time] ✓
- Stage 2: [Time] ✓
- Stage 3: [Time] ✓

**Validation:** All checks passed

**Deviations:** None

---

## CODE REVIEW

**Date:** 2025-10-16
**Status:** APPROVED

### Summary
The changes correctly implement the planned animation system cleanup. The code is cleaner, more consistent, and adheres to project principles.

### Violations (Must Fix)
None.

### Quality Checks
- Naming: ✓ snake_case
- Dependencies: ✓ Proper flow
- Side effects: ✓ None
- Build: ✓ Clean

### Files Reviewed
- `CMakeLists.txt`: ✓
- `src/character/animation.cpp`: ✓

### Recommendation
**Next:** FINALIZE

---

## FINALIZATION

**Finalized:** 2025-10-16

### Changes Summary
**Files Modified:**
- `CMakeLists.txt` - Added global define for `GLM_ENABLE_EXPERIMENTAL`.
- `src/character/animation.cpp` - Removed local define and refactored `update_acceleration_tilt` to use `glm::mat2`.

**Impact:** Improved code clarity, robustness, and build consistency.

### Learnings
**What Worked:** Batching related small tasks into a single improvement was efficient.
**What Was Hard:** Nothing.
**Future Insights:** Look for more batching opportunities in the backlog.

**Time Accuracy:**
- Estimated: 3 points
- Actual: 3 points
- Variance: Accurate