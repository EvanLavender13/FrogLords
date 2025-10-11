# Pose Blending (Slerp-Based Walk Cycle Smoothing)

**Core Mechanic:** Replace discrete pose switching with continuous quaternion interpolation (slerp) between keyframe poses. Eliminates visible "pops" at phase thresholds by smoothly blending joint rotations throughout the walk cycle.

**Pattern Reuse:** Extends proven distance-phased animation system ([animation.cpp:110-144](../src/character/animation.cpp#L110-L144)). Upgrades threshold logic from single-pose selection to source/target pose pairs with blend factors. Reuses existing `keyframe` structure and quaternion architecture. Secondary motion springs operate on blended output unchanged.

**Graybox Scope:**
- Extract `get_keyframe_data(pose_type) -> keyframe` function (returns raw quaternions from hardcoded poses)
- Replace threshold-based selection (lines 127-138) with segment-based blending:
  - Segments: 0.0-0.25 (STEP_LEFT→NEUTRAL), 0.25-0.5 (NEUTRAL→STEP_RIGHT), 0.5-0.75 (STEP_RIGHT→NEUTRAL), 0.75-1.0 (NEUTRAL→STEP_LEFT for seamless wrap)
  - Blend factor: `t = (phase - segment_start) / (segment_end - segment_start)`
  - Slerp all 8 joints: `glm::slerp(source.joint, target.joint, t)` per joint
- Apply blended quaternions directly to skeleton (bypass `apply_pose()` in automatic mode)
- ~40-60 lines (replaces 12 lines of discrete selection logic)

**Minimal Implementation:** Modify `animation_state::update_skeletal_animation()` ([animation.cpp:110-144](../src/character/animation.cpp#L110-L144)). Add `get_keyframe_data()` in `keyframe.cpp`. Use `glm::slerp()`. Phase calculation unchanged. Secondary motion update unchanged (lines 140-143).

**Unlocks:** 2D blend space (walk↔run speed blending), speed-based animation scaling, custom easing curves. Validates quaternion interpolation before adding run gait content.

**Test Loop:**
1. Visual: Smooth limb transitions? (No pops at 0.25, 0.5, 0.75 thresholds)
2. Wrap: Phase 0.99→0.01 smooth? (Last segment targets STEP_LEFT)
3. Springs: Natural lag vs wobble-and-settle? (Test current parameters, tune if needed)
4. Performance: 480 slerps/sec acceptable? (Negligible)

Visual inspection while moving. Instant feedback via animation UI.

**Certainty:** 85%—Validated quaternion architecture, standard GLM slerp. Last segment (0.75-1.0) targets STEP_LEFT for seamless loop. Secondary motion parameters tested unchanged first. GDC-validated approach (Overgrowth: linear slerp for locomotion, springs for follow-through).

**Problem Evidence:** Phase thresholds create instant pose snaps ([pose_blending_explained.md:23](../NOTES/pose_blending_explained.md#L23)). "At phase 0.249 limbs are STEP_LEFT, at 0.251 they instantly jump to NEUTRAL—visible pops." Discrete switching was intentional MVP; blending is natural next step. Springs currently compensate for pops; blended input produces natural follow-through ([pose_blending_explained.md:544-586](../NOTES/pose_blending_explained.md#L544-L586)).

**References:**
- [DESIGN_BACKLOG.md:45-54](../PLANS/DESIGN_BACKLOG.md#L45-L54) | [pose_blending_explained.md](../NOTES/pose_blending_explained.md)
- [animation.cpp:110-144](../src/character/animation.cpp#L110-L144) | [keyframe.h:16-25](../src/character/keyframe.h#L16-L25)
- [animation.cpp:146-209](../src/character/animation.cpp#L146-L209) (secondary motion)
- GLM: `glm::slerp(quat x, quat y, float a)` where a ∈ [0,1]

## Completion

**Date Completed:** October 11, 2025

**Final Certainty Score:** 100%

**Code Review:** [code_review_pose_blending.md](code_review_pose_blending.md)

**Outcome:** Feature successfully implemented with all goals met. Quaternion blending eliminates visible pops at phase boundaries (0.25, 0.5, 0.75). Limb motion is smooth throughout walk cycle with seamless phase wrap (1.0 → 0.0). Secondary motion springs exhibit natural follow-through on blended output. Hemisphere correction issue discovered and resolved through adaptive problem-solving. Implementation stayed within scope (46 lines vs. 40-60 estimate). Zero architectural changes; zero technical debt. Quaternion foundation proven ready for richer animation content (2D blend spaces, speed scaling, run cycles).

## Reflection

**What went well?**
- Quaternion architecture validation in Static Keyframe Preview prevented representation surprises—slerp "just worked" on proven foundation
- Hemisphere correction issue caught immediately via visual testing; root-cause analysis (dot product) prevented workaround hacks
- Certainty score (85%) accurately predicted smooth implementation; no deferrals, no major rework

**What caused friction?**
- Initial hemisphere flipping required investigation (1-hour detour), but GDC transcript research quickly identified glm::slerp shortest-path guarantee
- Brief uncertainty about whether to negate before or after slerp—resolved by testing both approaches and validating dot product logic

**What would you do differently?**
- Nothing significant. The Static Keyframe Preview iteration's quaternion validation was the critical preparatory step. Future similar features should follow the same pattern: validate data representation in isolation before building complex features on it.
- Could add hemisphere correction discussion to NOTES/pose_blending_explained.md for future reference, but not critical.

## Certainty Calibration

**Predicted Certainty:** 85% (from iteration plan Section 3)

**Actual Outcome:**
- [x] Completed on first attempt (no deferrals, no major rework)

**Variance Analysis:**
- Predicted certainty was 85% and feature completed smoothly with one minor investigation (hemisphere correction).
- The 15% uncertainty buffer accurately accounted for quaternion interpolation edge cases (hemisphere flipping).
- Certainty score was well-calibrated: high enough to justify confident planning, low enough to acknowledge interpolation subtleties.

**Calibration Adjustment:**
- For similar quaternion-based features: 85% certainty is appropriate when:
  - Data representation validated in isolation (quaternion keyframes proven)
  - Industry-standard operations used (GLM slerp)
  - GDC references available for edge case guidance (hemisphere correction)
- Pattern reuse justifies +10-15% certainty boost (this was the 4th quaternion iteration: static preview → primary animation → secondary motion → pose blending)
- Future slerp-based features (2D blend spaces, speed blending) can start at 90% certainty given this validation.
