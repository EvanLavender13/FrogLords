# Iteration 1: Speed-Dependent Steering

**Started:** 2025-10-23
**Status:** REVISE

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

**Mathematical Correctness:**
- [x] Formula produces values in [0, 1] at speed=0, speed=max_speed, speed>max_speed
- [x] Steering multiplier decreases monotonically with speed (never increases)
- [x] Preconditions validate: speed>=0, max_speed>0, finite values (controller.cpp:53-55)
- [x] Postcondition validates: multiplier in [0, 1] (controller.cpp:64-65)

**Edge Cases:**
- [x] Zero speed: full steering authority (multiplier=1.0) - formula verified
- [x] Max speed: reduced steering (multiplier=0.3 with default 0.7 factor) - formula verified
- [x] Overspeed: multiplier clamped to valid range (speed_ratio clamped to [0,1], line 59)
- [x] Zero max_speed: precondition assertion fires (line 55)

**Derived State (ACCUMULATED_STATE):**
- [x] Steering multiplier computed on-demand (no caching) - computed in apply_input:84-86
- [x] Helper function enables clean derivation for simulation and debug (compute_steering_multiplier)
- [x] No accumulated steering state across frames - no member variable

**Integration:**
- [x] Heading integration correctly scaled by steering multiplier (controller.cpp:88)
- [x] Debug visualization shows steering authority (cone scaling) - validated in graybox
- [x] Time-independent behavior maintained (dt scaling present)

**Metadata Status (documented, not blocking):**
- No param_meta for: steering_reduction_factor, turn_rate, wheelbase, max_steering_angle, grip_coefficient
- These parameters not exposed in GUI tuning system
- Deferred: parameter metadata task in backlog

**NOTE: Architectural Debt**
- Vehicle parameters (steering_reduction_factor, turn_rate, etc.) currently live in `controller` struct
- No dedicated vehicle tuning system exists (character::tuning_params handles character movement, not vehicle)
- No dedicated vehicle GUI panel exists (character_panel would be wrong location)
- **Decision:** Next iteration (ITERATION_2) will create proper vehicle tuning/panel architecture
- **Current iteration:** Test with default steering_reduction_factor=0.7, no GUI integration
- VALIDATE will review current state; iteration 2 proceeds regardless of VALIDATE decision
<!-- END: ITERATE/CONTRACT -->

---

<!-- BEGIN: ITERATE/PLAYTEST -->
### Playtest 1

**Date:** 2025-10-23
**Tester:** User

**Verified:**
- [x] Low speed (0-2 m/s): Orange steering cone is wide
- [x] Mid speed (3-6 m/s): Cone width decreases smoothly
- [x] Max speed (7-8 m/s): Cone is narrow (30% authority)
- [x] Acceleration: Cone gradually shrinks when speeding up
- [x] Deceleration: Cone gradually expands when slowing down
- [x] High-speed steering: Tighter turning (can't make sharp turns)
- [x] Low-speed steering: Looser turning (can make tight turns)
- [x] No assertion failures during normal movement
- [x] Frame-rate independent behavior (smooth)

**Clarification:**
- At zero speed, steering multiplier is mathematically 1.0 (full authority)
- However, turning only occurs with forward motion (CAR_LIKE control)
- Yaw arrow and heading changes require forward velocity
- This is correct behavior - cars don't turn in place

**Result:** ✓ All contract items validated. No violations found.
<!-- END: ITERATE/PLAYTEST -->

---

<!-- BEGIN: ITERATE/COMPLETE -->
## Iteration Complete

**Contract:** ✓ PROVEN

**Properties validated:**
- Formula correctness: produces [0,1] range at all speeds (preconditions/postconditions)
- Monotonic decrease: multiplier never increases with speed
- Edge cases: zero speed (multiplier=1.0), max speed (multiplier=0.3), overspeed (clamped)
- Derived state: on-demand computation via helper, no caching
- Integration: heading scaled by multiplier, time-independent

**Edges handled:**
- Zero max_speed: precondition assertion fires
- Overspeed: speed_ratio clamped to [0,1]
- CAR_LIKE control: turning requires forward motion (clarified, not a bug)

**Assertions:** 4 added (preconditions: 3, postcondition: 1)
**Playtests:** 1 (all contract items verified)

**Status:**
- [x] Contract proven
- [x] Stable
- [x] Ready for VALIDATE

**Known debt for ITERATION_2:**
- Vehicle tuning system architecture (metadata, apply_to pattern)
- Vehicle GUI panel (separate from character_panel)
<!-- END: ITERATE/COMPLETE -->

---

<!-- BEGIN: VALIDATE/REVIEW -->
## External Review

**Tool:** Codex
**Date:** 2025-10-23

**Principle Violations:**
- **Mathematical Foundation**: `compute_steering_multiplier` assumes `steering_reduction_factor ∈ [0,1]` but lacks precondition enforcement. Misconfigured values (>1 or <0) break the monotonic guarantee listed as "proven" in iteration contract. Violation only caught after deriving invalid multiplier via postcondition (controller.cpp:52-66). Contract incomplete.

**Strengths:**
- **Single Source of Truth**: Steering authority properly derived; helper reused by both simulation and debug with no cached multiplier (controller.cpp:84-88, character_panel.cpp:105-113)
- **Radical Simplicity / Orthogonal Systems**: Heading integration gained only scalar multiplier, no extra state or branching (controller.cpp:69-106, controller.h:133-136)
- **Emergent Design**: Framed as reusable infrastructure for future vehicle behaviors rather than bespoke tweak (system plan)

**Assessment:** Quality strong overall. Behavior derived cleanly, instrumentation pulls from same logic, system fits within existing control flow without coupling. Mathematical gap remains: need explicit precondition asserting steering_reduction_factor range and finiteness to uphold stated proofs.
<!-- END: VALIDATE/REVIEW -->

---

<!-- BEGIN: VALIDATE/DECISION -->
## Decision

**Status:** REVISE

**Reasoning:** Core implementation upholds principles (Single Source of Truth, Radical Simplicity, Emergent Design) but has fixable mathematical gap. Missing precondition on steering_reduction_factor allows contract violation. Violation is minor and easily correctable without architectural change.

**Required changes:**
- Add precondition in constructor: assert steering_reduction_factor ∈ [0,1] and finite
- Ensures mathematical contract is enforced at initialization, not deferred to postcondition
- Completes the stated proof that formula produces monotonic decrease
<!-- END: VALIDATE/DECISION -->

---
