# Iteration 1: Speed-Dependent Steering

**Started:** 2025-10-23
**Status:** Ready for VALIDATE

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
