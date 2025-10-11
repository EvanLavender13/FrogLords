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
