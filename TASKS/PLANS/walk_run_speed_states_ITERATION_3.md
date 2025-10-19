# Iteration 3: Walk/Run Speed States

**Started:** 2025-10-19
**Previous:** [walk_run_speed_states_ITERATION_2.md](walk_run_speed_states_ITERATION_2.md)
**Status:** In Progress

---

<!-- BEGIN: ITERATE/CONTEXT -->
## Context from Previous Iteration

**Decision:** REVISE (from ITERATION_2)

**Required changes:**

1. **Fix stale documentation (CRITICAL)**
   - Update SYSTEM.md:124 with correct cycle lengths (walk=2m, run=3m, sprint=4m)
   - Verify no other obsolete cycle length references exist in SYSTEM.md

2. **Clarify wheel rotation behavior**
   - Option A: Document that wheel angle DOES pop on state changes (matches current code)
   - Option B: Change wheel rotation to use `distance_traveled / wheel_radius` for continuous rotation
   - Choose one approach and align docs/code/iteration notes

3. **Document phase discontinuity completely**
   - State explicitly: "Phase value jumps on state change, wheel angle also pops"
   - Or if fixing: "Phase value jumps but wheel rotation remains continuous via distance-based angle"
   - Remove contradictory claims of visual smoothness if pops are accepted

4. **Standardize cycle_length access (LOW PRIORITY)**
   - Document that consumers should use `locomotion.cycle_length` from struct
   - Note that `get_cycle_length()` is internal implementation only

**Baseline contract:** See ITERATION_1/CONTRACT + ITERATION_2 fixes
<!-- END: ITERATE/CONTEXT -->

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

**Violations from ITERATION_2 (must fix):**
- [x] Stale cycle length documentation in SYSTEM.md
  - Found stale reference at SYSTEM.md:125 (walk=2m, run=1.5m, sprint=1.0m)
  - Replaced with correct values (walk=2m, run=3m, sprint=4m)
  - Verified no other stale references exist (grepped for "1.5" and "1.0.*m")
  - Commit: d9b6f4e

- [x] Wheel rotation continuity behavior unclear
  - Investigated: Old code used `phase` for rotation → pops on state change
  - Chosen approach: Implement continuous rotation via distance (Option B)
  - Implementation: Changed rotation_angle = -(distance_traveled / wheel_radius)
  - Rationale: Phase is OUTPUT (recalculates), distance is SOURCE OF TRUTH (continuous)
  - Result: Phase jumps on state change, but wheel angle remains smooth (correct!)
  - File: debug_generation.cpp:150-154
  - Commit: 8837e4f

- [x] Phase discontinuity documentation incomplete
  - Complete behavior documented:
    * Phase VALUE: Discontinuous (recalculates when cycle_length changes)
    * Wheel ANGLE: Continuous (uses distance_traveled, not phase)
    * Rationale: Phase = f(distance, cycle_length) is OUTPUT. Distance is SOURCE OF TRUTH.
  - ITERATION_2:120 claim "wheel rotation angle appears continuous" was TRUE in observation but FALSE in code
  - Now code matches observation: wheel uses distance_traveled (ITERATION_3 fix 8837e4f)
  - Updated SYSTEM.md:148 to document wheel visualization uses distance_traveled
  - Commit: 64e5988

- [x] cycle_length access pattern unstandardized
  - Fixed debug_generation.cpp:147 to use `locomotion.cycle_length` (not get_cycle_length())
  - Made get_cycle_length() private in controller.h:155-158
  - Added comment: "INTERNAL USE ONLY - External consumers should use locomotion.cycle_length"
  - Verified: Only internal controller.cpp:218 uses get_cycle_length() now
  - Pattern standardized: Struct member = public API, helper function = internal implementation
  - Commit: [pending]

**Baseline from ITERATION_1+2 (must preserve):**
- Frame-rate independence (distance += speed * dt)
- Phase wrapping at boundaries (fmod guarantees [0,1))
- Distance monotonicity (speed >= 0)
- Deterministic state classification (pure thresholds)
- Zero velocity handling (phase freezes)
- Backward/strafe movement (uses speed magnitude)
- Collision handling (post-resolution velocity)
- Output contract: {state, phase, cycle_length}
- Threshold ordering validation (FL_PRECONDITION)
- Fail-fast on invalid states (FL_ASSERT)

<!-- END: ITERATE/CONTRACT -->

---
