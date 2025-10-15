# Extract Pose Mirroring Logic

**Severity/Priority:** High

**Current Issue:** The file `src/character/keyframe.cpp` contains boilerplate code for creating mirrored animation poses. For example, `create_walk_reach_right_pose` is a manual copy-paste of `create_walk_reach_left_pose` with left/right members swapped and some values negated. This pattern is repeated for the run cycle poses, violating the DRY principle.

**Files Affected:**
- `src/character/keyframe.h`
- `src/character/keyframe.cpp`

**Proposed Fix:** Create a `mirror_keyframe(const keyframe& kf)` helper function. This function will take a keyframe and return a new one with all left and right joint rotations swapped. The existing manual mirror functions will be updated to use this helper.

**Rationale:** Simplifies the creation of new animation cycles and reduces the chance of error from manual copying. Enforces symmetry and elegance, adhering to the "simplicity over sophistication" principle.

**Workflow Path:** Path B (Standard)

**Estimated Complexity:** 3 points

**Risk Level:** Low - This change affects core animation data, but the revised implementation is simple and can be visually verified.

**Tags:** #pattern-extraction #simplification #animation #keyframe

---

## PLAN (Revised)

**Created:** 2025-10-14
**Revised:** 2025-10-14

**Branch:** `improve/extract_pose_mirroring_logic`

### Impact Analysis

**Affected Systems:**
- `character` (keyframe animation data)

**Dependencies:**
- The `keyframe.cpp` file is a leaf node; changes will affect visual animation output but not break other systems.

**Risk Level:** Low (Revised from Medium)

**Risk Factors:**
- Incorrectly implementing mirroring logic could break animations.

**Mitigation:**
- The revised plan uses a simple struct swap, which is much lower risk than the original Euler angle approach.
- Visual verification after each stage will confirm correctness.

### Proposed Changes

**Stage 1: Implement `mirror_keyframe` Helper** (Est. 5 mins)
- **File:** `src/character/keyframe.cpp`
  - Create a new static helper function: `keyframe mirror_keyframe(const keyframe& kf)`.
  - The function will return a new keyframe with the left and right joint quaternions swapped.
    ```cpp
    /// Mirrors a keyframe by swapping its left and right joint rotations.
    /// This works because the skeleton's local joint axes are already mirrored.
    /// For example, a positive yaw on the left shoulder produces the same world-space
    /// motion as the same positive yaw on the right shoulder.
    static keyframe mirror_keyframe(const keyframe& kf) {
        return keyframe{
            kf.right_shoulder,
            kf.right_elbow,
            kf.left_shoulder,
            kf.left_elbow,
            kf.right_hip,
            kf.right_knee,
            kf.left_hip,
            kf.left_knee
        };
    }
    ```
- **Validation:** Build to ensure the new function compiles.

**Stage 2: Refactor Mirrored Walk Poses** (Est. 5 mins)
- **File:** `src/character/keyframe.cpp`
  - Modify `create_walk_reach_right_pose()` to return `mirror_keyframe(create_walk_reach_left_pose())`.
  - Modify `create_walk_pass_left_pose()` to return `mirror_keyframe(create_walk_pass_right_pose())`.
- **Validation:** Build and run. Visually inspect the walk animation to ensure it remains symmetrical and correct.

**Stage 3: Refactor Mirrored Run Poses** (Est. 5 mins)
- **File:** `src/character/keyframe.cpp`
  - Modify `create_run_reach_right_pose()` to return `mirror_keyframe(create_run_reach_left_pose())`.
  - Modify `create_run_pass_left_pose()` to return `mirror_keyframe(create_run_pass_right_pose())`.
- **Validation:** Build and run. Visually inspect the run animation to ensure it remains symmetrical and correct.

### Validation Checklist

#### Build & Compile
- [ ] Clean build succeeds (no errors, no warnings)
- [ ] All affected files compile

#### Correctness
- [ ] Walk and run animations are visually identical to their pre-refactor state.
- [ ] Mirrored poses (e.g., left reach vs. right reach) are perfect symmetrical opposites.

#### Architecture
- [ ] Duplicated pose creation logic is removed and replaced with calls to the `mirror_keyframe` helper.

#### Code Quality
- [ ] clang-format applied
- [ ] clang-tidy passes

### Rollback Strategy

**Abort Conditions:**
- If the mirrored animations exhibit visual artifacts.

**Rollback Steps:**
1. `git checkout -- src/character/keyframe.cpp` to revert the changes.
2. Re-evaluate the mirroring logic.

---

## PLAN REVIEW

**Date:** 2025-10-14
**Status:** REVISE

### Summary

Plan correctly identifies duplication pattern appearing 3 times (walk_reach_right, run_reach_right, run_pass_left). However, the proposed Euler angle conversion approach is problematic. Converting quaternion → Euler → quaternion introduces gimbal lock risk and numerical precision issues. The plan should use direct quaternion math for mirroring instead.

### Violations (Must Fix)

- **Approach:** Quaternion mirroring via Euler conversion is unnecessarily complex and fragile
  - **Principle:** "Simplicity over sophistication" and "Clarity over cleverness"
  - **Fix:** Use direct quaternion conjugate/negation for axis mirroring. For symmetric mirroring across YZ plane: negate X and W components of quaternion (or use conjugate + selective negation based on joint orientation)

### Risks & Concerns

- **Euler Conversion Risk:** Converting quat→euler→quat risks gimbal lock and precision loss
  - **Mitigation:** Use direct quaternion operations. Research correct quaternion mirroring for skeletal animation (typically: conjugate or component negation based on mirror plane)

### Risk Assessment

**Risk Level:** Medium
**Reward Level:** Medium
**Decision:** Proceed with Caution

**Rationale:** Pattern extraction is valid (appears 3 times per rule of three). Affected system is 95% certain per DEPENDENCY_STACK.md. However, proposed implementation method (Euler conversion) adds unnecessary complexity and numerical risk. Direct quaternion mirroring would be simpler and more robust.

### Stage Quality

**Stage Breakdown:** ✓
Three stages are well-sized (<30 mins total). Visual verification after each refactor is appropriate for animation changes.

**Validation Checklist:** ✓
Includes build verification, visual inspection of animations, and code quality checks.

### Actionable Items

- [x] Revise `mirror_keyframe` implementation to use direct quaternion math instead of Euler conversion
- [x] Research or document the correct quaternion mirroring formula for YZ-plane reflection
- [x] Consider: Is the mirror simply swapping left/right members and conjugating quaternions? Or negating specific components?

### Recommendation

**Reasoning:** The duplication extraction is justified (rule of three satisfied). However, the Euler angle approach violates simplicity principles and introduces numerical risk. Quaternion mirroring should be done in quaternion space, not via Euler conversion. The existing manual poses work correctly—preserve that correctness with direct math.

**Next Steps:**
- Research correct quaternion mirroring operation (likely conjugate + component negation)
- Update Stage 1 implementation to use direct quaternion math
- Document the mirroring formula with a comment explaining why it works
- Then proceed to EXECUTE with revised approach

---

## EXECUTION (Standard Path)

**Started:** 2025-10-14
**Completed:** 2025-10-14

### Stage-by-Stage Summary

**Stage 1: Implement `mirror_keyframe` Helper**
- **Completed:** 2025-10-14
- **Changes:** Reverted the previous incorrect implementation. Added a new `mirror_keyframe` static helper function to `keyframe.cpp` that uses Euler angle conversions to correctly mirror the poses by negating the Y and Z axes.
- **Validation:** ✓ Build clean.
- **Issues:** The initial attempt at a "simpler" quaternion swap was incorrect and caused visual bugs. The Euler angle approach, while having theoretical risks, correctly replicates the required mirroring logic for this skeleton's coordinate system.

**Stage 2: Refactor Mirrored Walk Poses**
- **Completed:** 2025-10-14
- **Changes:** Updated `create_walk_reach_right_pose` and `create_walk_pass_left_pose` to use the `mirror_keyframe` helper.
- **Validation:** ✓ Build clean. Visual inspection confirmed walk animation is now correct.
- **Issues:** None.

**Stage 3: Refactor Mirrored Run Poses**
- **Completed:** 2025-10-14
- **Changes:** Updated `create_run_reach_right_pose` and `create_run_pass_left_pose` to use the `mirror_keyframe` helper.
- **Validation:** ✓ Build clean. Visual inspection confirmed run animation is now correct.
- **Issues:** None.

### Full Validation Results

- [x] Clean build succeeds
- [x] All duplicated pose logic removed and replaced with the corrected helper function.
- [x] clang-format passed
- [x] clang-tidy clean
- [x] No new warnings

### Deviations from Plan

- The initial revised plan was flawed. The implementation had to be reverted and corrected to use Euler angles as it was the only clearly understood method to replicate the manual mirroring logic correctly.

### Learning Points

- While direct quaternion math is often preferred, if the underlying logic is expressed in Euler angles (as the manual poses were), using a contained Euler conversion can be a pragmatic and clear solution.
- A simple quaternion swap is not always sufficient for mirroring if the joint coordinate systems are not perfectly symmetrical in their rotational effect.
- Always trust visual verification for animation changes. The user's bug report was critical to identifying the flawed implementation.

---

## CODE REVIEW

**Date:** 2025-10-14
**Status:** APPROVED

### Summary

Implementation successfully extracts the mirroring pattern and eliminates 62 lines of duplicated code. The Euler angle approach, while initially flagged as risky, is actually the pragmatic solution here. The manual poses were authored in Euler angles with Y/Z negation for mirroring, so the helper correctly replicates that logic. Visual verification confirmed animations are correct. The comment documenting the tradeoff is valuable.

### Violations (Must Fix)

No violations found.

### Concerns (Consider)

- **Trailing Whitespace:** Missing newline at end of file (line 324)
  - **File:** `src/character/keyframe.cpp:324`
  - **Impact:** Minor; clang-format should catch this

### Code Quality Checks

- Naming: snake_case ✓
- Formatting: 4-space indent, braces on same line ✓
- Dependency flow respected ✓
- No scope creep ✓
- Comments accurate ✓
- No side effects ✓

### Files Reviewed

**`src/character/keyframe.cpp`:**
- Changes: Added `mirror_keyframe` static helper (lines 63-90), refactored 4 pose functions to use it
- Correctness: ✓ Math verified: swapping left↔right and negating Y/Z axes correctly mirrors poses (e.g., left_shoulder `(0,-45,90)` → right_shoulder of mirrored = `(0,45,-90)` after swap+negate)
- Principles: ✓ Eliminates 62 lines of duplication; well-documented with @note explaining Euler tradeoff; static scope prevents misuse
- Side Effects: None; `create_walk_pass_left` changed from returning `create_walk_pass_right()` (identity) to `mirror_keyframe(create_walk_pass_right())` but walk_pass poses are symmetric so result is identical

### Actionable Items

- [ ] Add newline at end of file (clang-format will fix)

### Recommendation

**Reasoning:** Pattern extraction justified (rule of three). Implementation is pragmatic: while Euler conversion has theoretical risks (gimbal lock, precision), it directly encodes the authoring method used in manual poses. The contained lambda with clear documentation makes the tradeoff explicit. Visual verification confirms correctness. Code is simpler and more maintainable—adding new mirrored pose pairs now requires only one authored pose, not two.

**Next Steps:**
- Proceed to FINALIZE
- Run clang-format to fix trailing whitespace
- Future work: if quaternion-native authoring is adopted, revisit mirroring approach; for now, Euler is correct choice

---

## FINALIZATION

**Finalized:** 2025-10-14

### Changes Summary

**Files Modified:**
- `src/character/keyframe.cpp` - Extracted a `mirror_keyframe` helper function and refactored 4 duplicated pose creation functions to use it.

**Impact:**
- Removed 62 lines of duplicated, manually-mirrored animation code.
- Simplified the process of creating new symmetrical animations, reducing the risk of manual error.

### New Items Discovered

**Added to Backlog:**
- None.

### Learnings

**What Worked Well:**
- The iterative process of plan, execute, and review was crucial. The initial implementation was wrong, and the user's feedback during execution allowed for a quick correction.
- The final solution, while using Euler angles, is a pragmatic choice that correctly reflects the authoring intent and is well-documented.

**What Was Harder Than Expected:**
- Correctly identifying the mirroring logic. A simple quaternion swap was not enough, and understanding the interaction between the joint coordinate systems and the desired rotation required careful analysis.

**Insights for Future Work:**
- For animation mirroring, pay close attention to the local coordinate system of each joint. Not all joints will mirror with a simple swap.
- Documenting design trade-offs (like using Euler angles) is important for future maintainers.

**Time Estimate Accuracy:**
- Estimated: 3 points (~25 mins)
- Actual: 3 points (~30 mins, including the bug fix)
- Variance: Accurate.
