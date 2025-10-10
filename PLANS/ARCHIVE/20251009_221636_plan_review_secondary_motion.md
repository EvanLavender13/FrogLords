# Principle Review: Secondary Motion

### 1. Summary

The secondary motion plans are **well-aligned** with core principles. The feature demonstrates excellent pattern reuse (spring-damper validated twice), appropriate scoping (elbows/knees only), and clean reactive layering. The iteration plan correctly identifies the "stupid simple core" (4 joints, 2 global parameters) and maintains tight feedback loops (1-2 day horizon). The implementation plan provides atomic, ordered steps with clear integration points. Architecture ownership is correctly identified with no system migration issues. This is a low-risk, high-certainty iteration building on stable foundations.

**Review Status:** All identified issues have been resolved. The implementation pseudocode has been corrected to clarify spring state persistence, rotation composition order has been documented, integration point ambiguity has been removed, and dependency verification has been completed. Plans are ready for implementation.

### 2. Violations & Contradictions

- **Violation:** Incorrect Spring Target in Pseudocode (moved from Misalignments—this is a technical error, not just ambiguity)
  - **Source:** `PLANS/ARCHIVE/20251009_221636_implementation_secondary_motion.md:60-78`
  - **Principle:** "Clarity over cleverness"
  - **Details:** The pseudocode shows `target_offset = 0.0f`, which is technically correct BUT the explanation is missing. The intended behavior is: when `apply_pose()` switches from NEUTRAL to STEP_LEFT, the shoulder snaps instantly but the elbow springs toward the new pose over time. The spring tracks the offset between current lagged rotation and target rotation from pose. The key is that **spring state persists across pose switches**—this non-zero state creates the lag effect.
  - **Corrected Pseudocode:**
    ```cpp
    // The pose has already been applied by apply_pose() - it sets target rotations
    // Spring state (offset + velocity) PERSISTS across pose switches
    // When pose changes, the non-zero spring state creates visible lag as it catches up

    float target_offset = 0.0f;  // We want to reach the pose target (zero offset = at target)
    float current_offset = secondary_motion.left_elbow_offset;  // Current lag behind target

    // Spring physics drives offset toward zero (catching up to pose target)
    float spring_force = secondary_motion.stiffness * (target_offset - current_offset);
    float damping = critical_damping(secondary_motion.stiffness) * secondary_motion.damping_ratio;
    float damping_force = damping * secondary_motion.left_elbow_velocity;
    float acceleration = spring_force - damping_force;

    // Integrate (spring state carries over between frames AND between pose switches)
    secondary_motion.left_elbow_velocity += acceleration * dt;
    secondary_motion.left_elbow_offset += secondary_motion.left_elbow_velocity * dt;

    // Apply lagged offset to joint (negative because offset represents "behind target")
    skel.joints[LEFT_ELBOW].local_transform =
        glm::rotate(skel.joints[LEFT_ELBOW].local_transform,
                    -secondary_motion.left_elbow_offset,  // Lag applied as negative offset
                    glm::vec3(0, 1, 0));
    ```
  - **Critical Addition to Implementation Plan:** Add explicit note: "Spring state (offset + velocity) must NOT reset when pose changes. State persistence across pose switches is what creates the lag effect."

### 3. Misalignments & Risks

- **Misalignment:** Axis Application Order ✅ RESOLVED
  - **Source:** `PLANS/ARCHIVE/20251009_221636_implementation_secondary_motion.md:58`, `PLANS/ARCHIVE/20251009_221636_feature_secondary_motion.md:83`
  - **Principle:** "Clarity over cleverness"
  - **Details:** The pseudocode uses `glm::rotate(existing_local_transform, offset, axis)` to inject spring offsets. The order of rotation composition matters: `glm::rotate` post-multiplies the rotation, applying it AFTER the pose rotation (in parent space). Given that `apply_pose()` sets `local_transform = translate(t_pose_pos) * mat4_cast(pose_quat)`, the result is `translate * pose_quat * rotate(axis, offset)`. This applies the offset in the joint's local space (which is parent space after the pose rotation).
  - **Resolution:** Added detailed comments to implementation pseudocode explaining composition order. Clarified that offset is applied in parent space, which is correct for the intended effect. For elbows, Y-axis in parent space = forward/back swing axis (the axis we want to lag). For knees, X-axis in parent space = forward/back swing axis. Added verification step to visual validation checklist to confirm correct axis behavior during Phase 5 testing.

- **Risk:** Integration Point Ambiguity ✅ RESOLVED
  - **Source:** `PLANS/ARCHIVE/20251009_221636_implementation_secondary_motion.md:83-88`, `PLANS/ARCHIVE/20251009_221636_iteration_secondary_motion.md:98`
  - **Principle:** "Single source of truth"
  - **Details:** The plans originally provided three different integration points: (1) inside `update_skeletal_animation()` after `apply_pose()`, (2) in game_world.cpp after `update_skeletal_animation()`, (3) inside `update_skeletal_animation()` as encapsulated logic (which is the same as option 1). This created decision ambiguity during implementation.
  - **Resolution:** Chose single integration point: integrate `update_secondary_motion()` inside `update_skeletal_animation()` after `apply_pose()` for encapsulation. Updated both implementation plan and iteration plan to remove all "alternatively" clauses. This maintains clean separation while keeping related animation logic together.

- **Risk:** Missing Critical Damping Function Dependency ✅ RESOLVED
  - **Source:** `PLANS/ARCHIVE/20251009_221636_implementation_secondary_motion.md:166-169`
  - **Principle:** "Bottom-up dependencies"
  - **Details:** The implementation notes reference `critical_damping()` function from `foundation/spring_damper.h`, but this function was marked as "(100%, unused)" in DEPENDENCY_STACK.md:40, creating uncertainty about whether it exists and is correct.
  - **Resolution:** Verified that `critical_damping()` function exists at [src/foundation/spring_damper.cpp:24-26](src/foundation/spring_damper.cpp#L24-L26) with correct implementation: `return 2.0f * sqrtf(stiffness * mass)`. The function is already used by landing spring ([src/character/animation.cpp:9](src/character/animation.cpp#L9)), so "unused" in dependency stack was outdated. Added verification note to Phase 2 checklist in implementation plan.

### 4. Actionable Items

- [x] **Update implementation plan pseudocode:** ✅ COMPLETED - Replaced pseudocode in `20251009_221636_implementation_secondary_motion.md` with corrected version. Added critical note about state persistence.

- [x] **Remove integration point ambiguity:** ✅ COMPLETED - Chose single integration point (inside `update_skeletal_animation()` after `apply_pose()`). Removed "alternatively" clauses from both iteration and implementation plans.

- [x] **Document rotation composition order:** ✅ COMPLETED - Added detailed comments in implementation pseudocode explaining `glm::rotate` composition order. Clarified that offset is applied in parent space (post-pose rotation). Added verification step to Phase 5 checklist to confirm correct visual behavior (forward/back lag, not wrong-axis rotation).

- [x] **Verify critical_damping() dependency:** ✅ COMPLETED - Verified `critical_damping()` function exists at [src/foundation/spring_damper.cpp:24-26](src/foundation/spring_damper.cpp#L24-L26) with correct implementation: `return 2.0f * sqrtf(stiffness * mass)`. Added verification note to Phase 2 checklist in implementation plan.

- [x] **Optional Enhancement:** ✅ COMPLETED - Added comprehensive ASCII diagram to implementation plan showing:
  - Skeleton hierarchy (front view with joint indices)
  - Spring behavior diagram (parent-child relationships with lag arrows)
  - Axis orientation diagrams for both arms (Y-axis) and legs (X-axis)
  - Clear mapping of which springs follow which parent rotations
  - See [implementation_secondary_motion.md:105-161](PLANS/ARCHIVE/20251009_221636_implementation_secondary_motion.md#L105-L161)
