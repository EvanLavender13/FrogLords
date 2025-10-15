# Secondary Motion Joint Limits

**Core Mechanic:** Anatomical bend constraints for elbow/knee joints to prevent unnatural hyperextension during spring-driven secondary motion. Clamps spring offsets within joint-specific ranges before applying rotation.

**Pattern Reuse:** Extends proven secondary motion spring system (100% stable). Adds per-joint min/max offset clamps—same architectural pattern as dual-reference smoothing (target + constrained state).

**Graybox Scope:**
- Add `min_offset`/`max_offset` fields to `secondary_motion_state` struct
- Clamp spring offset before rotation application: `offset = clamp(offset, min, max)`
- Initial ranges (tunable):
  - Elbows: [0°, 150°] (hinge joint, no backward bend)
  - Knees: [0°, 150°] (similar hinge constraint)
- Expose limits via debug UI for iteration
- Estimated: ~10-15 lines

**Minimal Implementation:**
- File: `src/character/animation.cpp` (or wherever `secondary_motion_state::update()` lives)
- Hook: After spring offset calculation, before `apply_rotation()`
- Add limit fields to struct definition
- Single clamp operation per joint update
- Initialize limits in setup (hardcoded first, then tuning parameters)

**Problem Evidence:** Observed 2025-10-13 during air locomotion weights validation, increased visibility 2025-10-14 during run gait testing. Rapid pose transitions (swing → T-pose mid-jump, run gait transitions) cause elbows/knees to hyperextend backward as springs respond to parent velocity reversals. Root cause: spring offsets clamped only by damping, not anatomical constraints.

**Unlocks:**
- **Immediate:** Graybox-acceptable limb behavior during high-speed transitions; validates spring constraint pattern
- **Future:** Foundation for full IK joint limit cones (if needed); reusable clamp pattern for other joint types (spine, shoulders)

**Test Loop:**
- Visual: Jump mid-stride with T-pose blending active
- Visual: Run gait rapid walk/run transitions
- Validation: Elbows/knees stay within anatomical range during spring catch-up
- Iteration: Tune min/max via debug UI in real-time
- Expected loop: Minutes (tune → observe → adjust)

**Certainty:** ~75% (boosted from 60-70% base)
- Base 60-70%: Polish item, graybox-acceptable without but increasingly visible
- +10-15% boost: Builds on 100% stable secondary motion infrastructure (debug tool pattern—zero gameplay risk, clear integration point)
- Minimal novelty: Standard clamp operation on proven spring system
- Risk: Tuning ranges to feel natural (iteration, not architecture)

---

## Deferral

**Date:** 2025-10-15

**Reason:** Implementation revealed fundamental gap in understanding skeleton local coordinate frames. Left/right joint asymmetry indicates mirrored bones don't use symmetric local axes. Multiple attempts to fix asymmetry (axis flipping, soft limits, absolute angle measurement) failed. Small visual effect (secondary motion wobble) not worth debugging complex quaternion/hierarchy issues.

**Missing Prerequisites:**
- Skeleton local coordinate frame visualization tooling
- Understanding of bone hierarchy transform chain (local → world → local)
- Validation that mirrored joints (left/right) use symmetric local axes
- Debug visualization of rotation axes per joint
- Clear mental model of how rotations compose through parent-child chain

**Reconsideration Criteria:** 
- After building skeleton debugging/visualization tools
- After implementing IK (which will force solving the same transform hierarchy problems)
- When ROI justifies time investment (currently small polish effect)

**Technical Insight:** 
- Quaternion keyframe blending works when treating skeleton as black box
- Per-bone axis-dependent rotations expose our incomplete understanding of local frames
- Right knee spring offset always positive, left knee oscillates positive/negative → axes not mirrored as expected
- Attempting to measure "absolute angle" for soft limits revealed we can't reliably decompose rotations around arbitrary axes in local bone space

### Implementation Attempts

1. **Hard clamp with symmetric limits (0°-150°):** Both knees hyperextended identically → confirmed limits were being applied to offset, not absolute angle. Heels kicked character's butt.

2. **Reduced offset range (±30°):** Reduced hyperextension but right knee still reached higher values than left.

3. **Soft limit resistance:** Attempted to measure absolute joint angle and apply exponentially stronger spring force near limits. Failed due to quaternion decomposition complexity—couldn't extract signed angle around hinge axis reliably in mirrored local frames.

4. **Axis flipping (right knee X- instead of X+):** Produced worse hyperextension, confirming axes aren't simply mirrored.

5. **Improved axis projection:** Simplified velocity injection to always use projected component (removed conditional logic). Didn't solve asymmetry.

6. **Absolute angle measurement via reference vector:** Created perpendicular reference vector, rotated by joint, measured angle via acos + cross product for sign. Complex, still failed to produce symmetric measurements.

**Key Insight:** The problem isn't the spring math or limit clamping—it's that we don't understand the skeleton's bone local coordinate systems well enough to apply rotations confidently. Every attempt to "fix" asymmetry revealed another layer of quaternion/hierarchy complexity we haven't mastered. For IK or targeted limb control (grabbing ledges), we'll need this understanding. For a small secondary motion polish effect, it's not worth the investment now.

**Code State:** Reverted to simple hard clamp on spring offset. Feature functional but asymmetric. Secondary motion toggle defaults to ON but can be disabled via GUI. No architectural damage—system is self-contained.

**Review:** See `PLANS/ARCHIVE/20251014_230307_secondary_motion_joint_limits_PLAN_REVIEW.md` for principle alignment check
