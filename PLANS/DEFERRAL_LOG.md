# Deferral Log

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