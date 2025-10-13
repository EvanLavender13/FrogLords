# Run Gait Blending (2D Pose Blending)

**Core Mechanic:** Bilinear interpolation between walk and run keyframe poses based on gait phase and character speed. Larger limb extension at sprint speeds; smaller strides at walk speeds. Visual feedback that matches movement velocity.

**Pattern Reuse:** Extends proven 1D phase-based pose blending (validated in prior iterations) to 2D blend space. Walk gait poses (4 keyframes) already integrated; run gait poses (4 keyframes) already authored (2025-10-12) but unused. Uses existing `walk_factor` (0.0 = run, 1.0 = walk) as vertical blend axis. Distance-phased surveyor wheel provides horizontal blend axis (gait phase).

**2D Blend Space:**
```
Speed (walk_factor)
  ^
  |
Walk|  WALK_REACH_L → WALK_PASS_R → WALK_REACH_R → WALK_PASS_L
  |      ↕              ↕               ↕              ↕
Run |  RUN_REACH_L  → RUN_PASS_R  → RUN_REACH_R  → RUN_PASS_L
  |
  +────────────────────────────────────────────────> Phase
     0.0          0.25           0.5          0.75        1.0
```

At any moment, character exists at point (phase, walk_factor) in this space. Bilinear slerp blends four corner poses simultaneously.

**Graybox Scope:**
- Add `run_gait_poses[4]` array (REACH_LEFT, PASS_RIGHT, REACH_RIGHT, PASS_LEFT) alongside existing `walk_gait_poses[4]`
- Implement bilinear slerp (two-pass blending):
  1. **Horizontal pass:** Blend walk poses by phase → `walk_blended`
  2. **Horizontal pass:** Blend run poses by same phase → `run_blended`
  3. **Vertical pass:** Blend results by walk_factor → `final_pose`
- Debug toggles: isolate walk-only, run-only, or 2D blended output
- Estimated line count: ~40-60 lines in [src/character/animation.cpp](../src/character/animation.cpp)

**Minimal Implementation:**
- Hook into `character::animation::update()` after gait phase calculation, before applying rotations to skeleton
- Files affected:
  - [src/character/animation.cpp](../src/character/animation.cpp): Add run pose array, implement bilinear blending
  - [src/character/animation.h](../src/character/animation.h): Potentially expose debug toggles
- Implementation approach:
  1. Declare `run_gait_poses[4]` with quaternion rotations from running_gait_keyframes iteration (2025-10-12)
  2. Use existing `get_blended_keyframe()` for both walk and run pose sets with same gait phase
  3. Implement vertical blend: `slerp_keyframes(walk_blended, run_blended, 1.0 - walk_factor)`
  4. Apply final blended pose to skeleton (secondary motion springs react on top, as designed)
  5. Optional: ImGui debug toggles for walk-only/run-only/2D-blended visualization

**Problem Evidence:**
Run keyframes authored on 2025-10-12 but never integrated into animation pipeline. Current implementation uses walk poses at all speeds, creating visual disconnect: sprinting character shows small walk strides instead of large run strides. Observed during playtesting after running gait keyframe completion—larger limb extension exists in authored poses but never visible in-game.

**Unlocks:**
- **Immediate:**
  - Completes walk/run visual distinction with smooth speed transitions
  - Validates authored run content (4-pose run cycle with larger limb extension)
  - Provides clear silhouette differentiation at different speeds (core GDC animation principle)
  - Proves 2D blend space architecture for future expansion
- **Future:**
  - Foundation for additional gait types (crouch walk, climb) as separate blend axes
  - Validates bilinear slerp pattern for complex multi-axis pose blending
  - Enables per-gait parameter tuning if needed (separate arm swing magnitudes for walk vs run)

**Test Loop:**
- Sprint at varying speeds, verify smooth visual transition from walk→run
- Debug UI confirms both walk and run pose contributions visible
- No pops or discontinuities during speed changes
- Limb extension magnitude clearly larger at high speeds
- Expected iteration time: Minutes—toggle between speeds, adjust walk_factor threshold if needed
- Self-test loop: Real-time parameter adjustment via existing animation debug panel pattern

**Certainty:** ~85%

**Justification:**
- **Low novelty:** Bilinear slerp is well-understood (standard game animation technique)
- **Proven infrastructure:** 1D phase-based blending validated; 2D is straightforward extension
- **Clear integration point:** Existing `get_blended_keyframe()` handles horizontal axis; add one vertical slerp pass
- **Content exists:** Run poses authored with correct joint rotations (2025-10-12)
- **Minimal risk:** Architecture proven, only tuning risk remains
- **Dependencies stable:** Builds on 100% certain skeletal system, 95% certain locomotion system
- **GDC validation:** Bilinear pose blending explicitly recommended in [NOTES/pose_blending_explained.md](../NOTES/pose_blending_explained.md)

**Primary Risk:**
Walk_factor transition feel may need tuning—existing computation (based on speed) may create abrupt or sluggish visual transitions. Secondary risk: run pose magnitudes may need tweaking once blended in-game (authored in isolation), but this is content adjustment, not architectural change. Tertiary risk: increased line count estimate (40-60 vs initial 30-50) due to bilinear approach.

**References:**
- [NOTES/pose_blending_explained.md](../NOTES/pose_blending_explained.md) - 2D blending theory and implementation guidance
- [NOTES/GDC/GDC_DesignPhilosophy.md](../NOTES/GDC/GDC_DesignPhilosophy.md) - Synchronized blending principles
- [NOTES/GDC/GDC_TechnicalWhitepaper.md](../NOTES/GDC/GDC_TechnicalWhitepaper.md) - Bilinear interpolation for locomotion

---

## Post-Implementation Note

**When this feature is complete:** Update [NOTES/pose_blending_explained.md](../NOTES/pose_blending_explained.md) with:
- Any deviations from predicted implementation approach
- Actual function signatures and code structure used
- Performance characteristics or unexpected behaviors discovered
- Tuning values that worked well (walk_factor threshold, blend smoothness)
- Secondary motion interaction observations (did springs behave as predicted?)
- Lessons learned or patterns validated for future multi-axis blending

This ensures the reference document remains accurate and reflects real implementation rather than theoretical design.

---

## Completion

**Date completed:** 2025-10-13
**Final certainty:** 100%
**Code Review:** [run_gait_blending_CODE_REVIEW.md](run_gait_blending_CODE_REVIEW.md)

**Outcome:** Feature was successfully implemented, validated through smoke-testing, and approved. The bilinear slerp with smoothed `walk_factor` provides a natural visual transition between walk and run gaits.

---

## Reflection

### What went well?

- The implementation plan was clear and accurate, leading to a smooth development process.
- Reusing the existing 1D pose blending infrastructure and `get_blended_keyframe` function made the extension to 2D straightforward.
- The optional refactoring to centralize the `get_run_blend()` logic in the locomotion system improved code quality and maintainability.

### What caused friction?

- The initial implementation without smoothing on the `walk_factor` resulted in a visually abrupt transition between walk and run gaits.
- The need for tuning the keyframe poses to get the desired visual result required an extra iteration.

### What would you do differently?

- I would incorporate the `walk_factor` smoothing into the initial implementation plan, as it was a predictable requirement for a smooth visual transition.
- I would have considered the keyframe tuning as part of the initial plan, rather than a separate step.

---

## Certainty Calibration

**Predicted Certainty:** 85%

**Actual Outcome:**
- [x] Completed on first attempt (no deferrals, no major rework)

**Variance Analysis:**
- The predicted certainty of 85% was a good estimate. The feature was completed smoothly, but the need for tuning the `walk_factor` transition and keyframe poses was a small, predictable risk that was accounted for in the initial estimate.

**Calibration Adjustment:**
- For similar features that involve blending animations, an 85-90% certainty is appropriate if the underlying systems are stable. The need for tuning should always be considered as a small risk.
