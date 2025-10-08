# Deferral Log

## Primary Skeletal Animation (Locomotion-Driven) (2025-10-07)

**Reason:** Fundamental data structure limitation - single-float-per-joint cannot represent 3D rotation

**Key Learning:** The original plan used `struct run_keyframe { float left_shoulder; float right_shoulder; ... }` storing one angle per joint. This is architecturally insufficient for skeletal animation:
- Real 3D joint rotation requires 3+ degrees of freedom (pitch/yaw/roll or quaternion)
- Single float cannot encode arbitrary 3D rotation
- Transform composition with T-pose baselines produces incorrect results when axis-of-rotation varies by joint orientation
- Multiple implementation attempts (custom swing vectors, direct axis rotation, baseline-ignoring transforms) all failed due to fundamental data limitation

Attempted to shortcut proper keyframe representation; "simplicity" became "impossibility."

**Missing Prerequisites:** 
- Quaternion-based (or Euler-angle-based) keyframe data structure
- Clear separation of keyframe authoring from animation playback
- Understanding of per-joint rotation axes and composition rules

**Principle Reinforced:** 
- "Clarity over cleverness" - attempting to encode 3D rotation in 1D was too clever by half
- "Work bottom-up. Stabilize core gameplay before adding layers" - should have validated keyframe representation works before adding locomotion integration
- "Short plans → build → test → repeat" - plan attempted too much in single iteration (keyframes + locomotion + GUI + phase computation)

**Next Steps:** 
Implement reduced-scope feature: **Static Keyframe Preview**
1. Upgrade keyframe struct to quaternions
2. Store 4 hardcoded poses (no authoring UI)
3. GUI manual selection only (no locomotion phase)
4. Validates quaternion storage/application before adding animation

**Review Document:** N/A - architectural issue discovered during implementation (Step 4 GUI review)

**Implementation Notes:** Steps 1-4 completed but non-functional due to data structure issue. Code preserved on branch `iteration/primary_skeletal_animation` for reference. See `PLANS/implementation_primary_skeletal_animation.md` header for detailed technical analysis.

---

## Secondary Motion (2025-10-06)

**Reason:** Missing critical prerequisite: primary skeletal animation system

**Key Learning:** Dependency analysis failed to identify that "reactive animation layer" requires a stable "primary animation layer" beneath it. Secondary motion needs base skeletal motion to react to—a static T-pose has no motion to add wobble on top of. Three implementation attempts (parent rotation lag, acceleration-driven wobble, slerp-based lag) all failed due to the same architectural blocker: no base animation exists for springs to offset from. Static data structure ≠ animated data structure. Reactive layers need motion sources, not just data structures.

**Missing Prerequisites:** Locomotion-driven skeletal animation (walk/run cycles, arm swing synchronized to gait phase)

**Principle Reinforced:** "Work bottom-up. Stabilize core gameplay before adding layers" and "Dependency = if A changes, B must change. Uncertainty multiplies up the stack." Also reinforces procedural animation layering: Core Logic (primary animation) → Reactive Systems (secondary motion) → Polish.

**Review Document:** Initial plan review approved (`PLANS/ARCHIVE/20251006_220556_plan_review_secondary_motion.md`) but missed architectural prerequisite

**Implementation Notes:** All code reverted cleanly. See `PLANS/ARCHIVE/20251006_220556_implementation_secondary_motion.md` for detailed technical analysis of three failed implementation approaches and key insights about reactive animation layer dependencies.

---

## Speed-Based Animation Scaling (2025-10-06)

**Reason:** Premature feature without evidence of need

**Key Learning:** Test-first principle applies to features, not just bugs. "May not be necessary" intuitions deserve investigation before planning entire iterations. Current system working well is evidence to preserve, not improve speculatively.

**Principle Reinforced:** "Paradox of quality: accept temporary roughness early to maximize iteration count" and "Test before building"

**Review Document:** `PLANS/ARCHIVE/20251006_120000_plan_review_speed_animation_scaling.md`