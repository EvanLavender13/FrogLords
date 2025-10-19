# Iteration 3: Walk/Run Speed States

**Started:** 2025-10-19
**Previous:** [walk_run_speed_states_ITERATION_2.md](walk_run_speed_states_ITERATION_2.md)
**Status:** APPROVED

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
  - Commit: 7179f68

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

<!-- BEGIN: ITERATE/PLAYTEST_1 -->
### Playtest 1

**Date:** 2025-10-19
**Tester:** User

**Results:** ✅ ALL PASS (9/9 categories)

**Fixed Violations (4/4):**
- ✅ Stale cycle length documentation - SYSTEM.md shows correct values (walk=2m, run=3m, sprint=4m)
- ✅ Wheel rotation continuity - Wheel size changes but rotation angle stays smooth through state transitions
  - Note: Spokes spin quickly at sprint (physically accurate - faster speed = faster rotation)
- ✅ Phase discontinuity documentation - SYSTEM.md:148 correctly documents behavior
- ✅ cycle_length access standardization - Wheel size correctly reflects state (walk=smallest, sprint=largest)

**Baseline Preservation (5/5):**
- ✅ Frame-rate independence - Game runs smoothly
- ✅ Phase wrapping - Phase wraps 0→1 seamlessly, no visual snap
- ✅ State classification - All 3 states reachable (green=walk, yellow=run, red=sprint)
- ✅ Zero velocity handling - Phase freezes when stationary, no drift
- ✅ Debug assertions - Game starts without assertion failures, all checks pass

**Violations:** None

**Emergent:**
- Wheel spokes difficult to see at sprint speed due to fast rotation (expected - physically accurate)
- Visual continuity of wheel rotation validates the distance-based approach
- Phase value discontinuity not visually distracting (wheel angle continuity dominates perception)

**Status:** ✅ ALL CONTRACT ITEMS VERIFIED
<!-- END: ITERATE/PLAYTEST_1 -->

---

<!-- BEGIN: ITERATE/COMPLETE -->
## Iteration Complete

**Contract:** ✓ PROVEN

**Violations Fixed:** 4/4 from ITERATION_2 external review
1. Stale cycle length documentation - FIXED: Updated SYSTEM.md:125 (d9b6f4e)
2. Wheel rotation continuity - FIXED: Changed to distance-based rotation (8837e4f)
3. Phase discontinuity documentation - FIXED: Clarified in SYSTEM.md:148 (64e5988)
4. cycle_length access standardization - FIXED: Made get_cycle_length() private, standardized on struct (7179f68)

**Code Changes:**
- SYSTEM.md: Fixed stale cycle lengths, added wheel visualization documentation
- debug_generation.cpp: Changed wheel rotation to use distance_traveled (continuous), added comments
- controller.h: Made get_cycle_length() private with usage documentation

**Key Insight:**
Phase is OUTPUT (discontinuous, recalculates). Distance is SOURCE OF TRUTH (continuous). Wheel rotation must use distance, not phase, to maintain visual continuity through state transitions.

**Assertions:** 0 added (ITERATION_2 already had necessary assertions)

**Playtests:** 1 (9/9 items passed, 0 violations)

**Status:**
- [x] All violations from ITERATION_2 addressed
- [x] Contract proven through playtest
- [x] Stable (no crashes, no assertion failures)
- [x] Documentation aligned with code and behavior
- [x] Ready for VALIDATE

<!-- END: ITERATE/COMPLETE -->

---

<!-- BEGIN: VALIDATE/REVIEW -->
## External Review

**Tool:** Codex
**Date:** 2025-10-19

**Scores:**
1. Radical Simplicity: 9/10
2. Fundamental Composable Functions: 8/10
3. Solid Mathematical Foundations: 9/10
4. Emergent Behavior: 8/10
5. Consistency: 9/10
6. Principled Development: 9/10

**Critical Violations Identified:**
None. One suggestion for future improvement:
- Debug visualization accesses `distance_traveled` (tagged internal-only) to maintain continuity. Consider exposing distance or rotation primitive on public surface if multiple consumers need continuous behavior.

**Evidence Summary:**
- Minimal locomotion surface keeps system lean (controller.h:125, SYSTEM.md:120)
- Pure cycle-length helper preserves clean composition (controller.cpp:199)
- Frame-rate independent distance accumulation (controller.cpp:207)
- Deterministic thresholds with runtime checks (controller.cpp:194)
- Iteration report shows violations closed with traceable commits (ITERATION_3.md:42)

**Final Average:** 8.7/10
<!-- END: VALIDATE/REVIEW -->

---

<!-- BEGIN: VALIDATE/DECISION -->
## Decision

**Status:** APPROVED

**Reasoning:**
Average score of 8.7/10 exceeds threshold of 7.0, and all individual principle scores (minimum 8/10) exceed threshold of 6.0. No critical violations identified. The single suggestion regarding distance_traveled exposure is a future enhancement opportunity, not a blocking issue—current internal access by debug visualization is acceptable for this iteration.

System demonstrates strong adherence to all six principles:
- Radical simplicity through minimal state surface
- Clean composability with pure functions
- Solid mathematical foundations (frame-rate independence, deterministic behavior)
- Emergent potential documented and demonstrated
- Consistent, predictable behavior verified through playtesting
- Principled development process with traceable violation fixes

**Required changes:** None
<!-- END: VALIDATE/DECISION -->

---

<!-- BEGIN: VALIDATE/EMERGENCE -->
## Emergence

**Surprising behaviors:**
- Distance-based wheel rotation creates perceptually smooth transitions despite phase discontinuities—visual continuity emerges from correct physics modeling, not special-case smoothing
- Fast spoke rotation at sprint speed provides intuitive speed feedback without explicit design—physical accuracy generates visual communication for free
- Phase discontinuity (internal recalculation) becomes invisible to player due to wheel angle continuity—single source of truth (distance) creates coherent multi-representation system

**Enables (future):**
- Animation blending can use phase to drive walk/run/sprint cycles while state changes naturally
- Audio footstep timing can sync to phase value without custom event systems
- Particle effects (dust, footprints) can trigger at phase thresholds
- Stamina/energy systems can integrate with state classification (sprint = drain)
- Movement abilities can key off locomotion state (sprint-only moves, walk-only precision)
- AI can read player state for behavior adjustments without velocity threshold duplication

**Learned:**
- Source of truth must be continuous primitive (distance), outputs can be discontinuous views (phase, state)
- When multiple representations exist, they must all derive from same source—never maintain parallel state
- Phase as OUTPUT (not INPUT) eliminates entire class of synchronization bugs
- Debug visualization accessing internal state revealed composability gap—future consideration for distance exposure
- Physical correctness (wheel rotation from distance) produces better player experience than hand-tuned smoothing would
<!-- END: VALIDATE/EMERGENCE -->

---
