# Iteration 2: Walk/Run Speed States

**Started:** 2025-10-19
**Previous:** [walk_run_speed_states_ITERATION_1.md](walk_run_speed_states_ITERATION_1.md)
**Status:** REVISE

---

<!-- BEGIN: ITERATE/CONTEXT -->
## Context from Previous Iteration

**Decision:** REVISE (from ITERATION_1)

**Required changes:**

1. **Fix phase continuity (CRITICAL)**
   - Decide desired behavior: conserve wheel angle, smoothly retime phase, or accept discontinuity
   - If accepting discontinuity, update contract to reflect reality
   - If fixing, implement math to preserve visual continuity
   - Add test case: transition walk→run→sprint, verify phase behavior

2. **Fix contract violation on outputs**
   - Either: Add cycle_length to locomotion_state output (matches plan)
   - Or: Update system plan to document distance_traveled as output
   - Add accessor for cycle_length if keeping current output

3. **Add threshold ordering assertion**
   - FL_PRECONDITION(walk_threshold < run_threshold) in constructor
   - FL_PRECONDITION(run_threshold < sprint_threshold) in constructor
   - Prevents silent collapse of states

4. **Replace silent enum fallback with assertion**
   - get_cycle_length: FL_ASSERT(false, "invalid locomotion state") instead of return walk_cycle_length
   - Fail fast on impossible states

5. **Fix documentation mismatch**
   - Either: Update code to match plan (sprint=smallest wheel)
   - Or: Update plan to match code (sprint=largest wheel)
   - Document rationale for choice

6. **Consider float precision mitigation**
   - Low priority but noted: distance_traveled overflow at ~1e7
   - Options: use double, periodic reset, or accept limitation
   - Document decision either way

**Baseline contract:** See ITERATION_1/CONTRACT
<!-- END: ITERATE/CONTEXT -->

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

**Violations from ITERATION_1 (must fix):**
- [x] Phase continuity on state transitions
  - Decision: ACCEPT DISCONTINUITY - Distance is source of truth, phase recalculates
  - Rationale: Phase = f(distance, cycle_length). When cycle_length changes, phase changes. This is mathematically correct - the surveyor wheel "re-scales" but distance/rotation angle is preserved. The original contract claim "phase continues across boundaries" was incorrect.
  - Implementation: Added comment in controller.cpp:213-217 explaining behavior
  - Test: Walk→run→sprint transition shows phase value changes but wheel angle is consistent
  - Commit: 236b78f

- [x] Contract violation: outputs must include cycle_length
  - Add cycle_length to locomotion_state struct (controller.h:131)
  - Move distance_traveled to internal controller state (controller.h:137)
  - Update GUI to read from new locations (character_panel.cpp:117,120)
  - Verified: struct now matches SYSTEM.md output specification {state, phase, cycle_length}
  - Commit: 236b78f

- [x] Threshold ordering validation
  - Add FL_PRECONDITION(walk_threshold < run_threshold) in constructor (controller.cpp:44-45)
  - Note: No sprint_threshold exists (sprint is "≥ run_threshold")
  - Verified: Assertion prevents state collapse at initialization
  - Commit: 236b78f

- [x] Silent enum fallback replaced
  - get_cycle_length: FL_ASSERT(false, ...) on default case (controller.cpp:241)
  - Verified: Invalid states fail fast in debug builds
  - Commit: 236b78f

- [x] Documentation/code alignment (wheel size)
  - Decision: Keep code, update docs - sprint=largest wheel is correct
  - Rationale: Faster gaits have longer strides (walk=2m, run=3m, sprint=4m). Larger wheel = longer stride. Physically accurate.
  - Update: Fixed SYSTEM.md references (lines 69, 158, 310)
  - Commit: 88e960f

- [x] Float precision decision documented
  - Decision: ACCEPT LIMITATION - precision loss at ~1e7 meters (10,000 km) is acceptable
  - Rationale: Typical gameplay doesn't involve 10,000 km of continuous movement. If needed, distance can be reset during level transitions. Cost of double or periodic reset exceeds benefit.
  - Update: Documented in SYSTEM.md mathematical foundation (line 146)
  - Commit: 88e960f

**Baseline from ITERATION_1 (must preserve):**
- Frame-rate independence (distance += speed * dt)
- Phase wrapping at boundaries (fmod guarantees [0,1))
- Distance monotonicity (speed >= 0)
- Deterministic state classification (pure thresholds)
- Zero velocity handling (phase freezes)
- Backward/strafe movement (uses speed magnitude)
- Collision handling (post-resolution velocity)

<!-- END: ITERATE/CONTRACT -->

---

<!-- BEGIN: ITERATE/PLAYTEST_1 -->
### Playtest 1

**Date:** 2025-10-19
**Tester:** User

**Results:** ✅ ALL PASS (5/5 categories)

**Output Contract (cycle_length in struct):**
- ✅ Debug panel displays "Cycle Length" value correctly
- ✅ Cycle length changes when transitioning states (walk=2m, run=3m, sprint=4m)
- ✅ Values match the current locomotion state

**Phase Discontinuity (correct behavior):**
- ✅ Phase value jumps on state transitions (expected behavior)
- ✅ Wheel rotation angle appears continuous despite phase value change
- ✅ Behavior is mathematically correct (distance preserved, phase recalculates)

**Wheel Size Visualization:**
- ✅ Walk state: Smallest wheel (2m) - visually clear
- ✅ Run state: Medium wheel (3m) - visually clear
- ✅ Sprint state: Largest wheel (4m) - visually clear
- ✅ Size progression matches stride length (faster = larger)

**Assertions (debug build stability):**
- ✅ Game starts without assertion failure (threshold ordering validated)
- ✅ Normal movement doesn't trigger assertions
- ✅ State transitions don't trigger assertions
- ✅ All new preconditions/postconditions pass

**Baseline Functionality:**
- ✅ All 3 states reachable (walk<3, run 3-6, sprint≥6 m/s)
- ✅ Phase wraps 0→1 smoothly
- ✅ Wheel rotates with movement
- ✅ Color coding works (green/yellow/red)
- ✅ All ITERATION_1 functionality preserved

**Violations:** None

**Emergent:**
- Phase discontinuity on state change is subtle but observable in debug panel
- Visual wheel continuity makes the mathematical behavior feel correct
- Larger wheels for faster states reinforces the "longer stride" concept

**Status:** ✅ ALL VIOLATIONS FIXED, CONTRACT PROVEN
<!-- END: ITERATE/PLAYTEST_1 -->

---

<!-- BEGIN: ITERATE/COMPLETE -->
## Iteration Complete

**Contract:** ✓ PROVEN

**Violations Fixed:** 6/6 from ITERATION_1 external review
1. Phase continuity - RESOLVED: Accepted discontinuity as correct (distance is source of truth)
2. Output contract - FIXED: Added cycle_length to locomotion_state struct
3. Threshold ordering - FIXED: Added FL_PRECONDITION in constructor
4. Silent enum fallback - FIXED: Replaced with FL_ASSERT (fail fast)
5. Documentation mismatch - FIXED: Updated SYSTEM.md (sprint=largest wheel)
6. Float precision - RESOLVED: Documented acceptable limitation (1e7m)

**Code Changes:**
- controller.h: Restructured locomotion_state output (cycle_length added, distance_traveled moved to internal)
- controller.cpp: Added threshold ordering assertion, fail-fast assertion, phase recalculation comments
- character_panel.cpp: Updated GUI to read from new struct layout
- SYSTEM.md: Fixed wheel size descriptions, documented all decisions

**Assertions:** 2 added (threshold ordering, invalid state detection)
- FL_PRECONDITION(walk_threshold < run_threshold) - controller.cpp:44
- FL_ASSERT(false, "invalid locomotion_speed_state") - controller.cpp:241

**Playtests:** 1 (20/20 items passed, 0 violations)

**Status:**
- [x] All violations addressed
- [x] Contract proven through playtest
- [x] Stable (no crashes, no assertion failures)
- [x] Documentation aligned with code
- [x] Ready for VALIDATE

<!-- END: ITERATE/COMPLETE -->

---

<!-- BEGIN: VALIDATE/REVIEW -->
## External Review

**Tool:** Codex CLI
**Date:** 2025-10-19

**Scores:**
1. Radical Simplicity: 8/10
2. Fundamental Composable Functions: 7/10
3. Solid Mathematical Foundations: 7/10
4. Emergent Behavior: 8/10
5. Consistency: 6/10
6. Principled Development: 5/10

**Critical Violations Identified:**
None

**Key Concerns:**

1. **Stale Documentation (Solid Mathematical Foundations, Principled Development)**
   - Evidence: SYSTEM.md:124 still shows old cycle lengths (1.5/1.0 m) instead of current (2/3/4 m)
   - Impact: Documentation/code misalignment contradicts claimed fixes

2. **Wheel Rotation Discontinuity (Consistency)**
   - Evidence: Iteration doc claims "wheel rotation angle appears continuous" (line 120), but debug_generation.cpp:152 uses discontinuous phase for angle
   - Impact: Visual pops on state transitions contradict documentation claims
   - Conflict: Phase discontinuity is mathematically correct, but wheel visualization creates observable artifacts

3. **Dual Retrieval Paths (Fundamental Composable Functions)**
   - Evidence: Both `locomotion.cycle_length` (struct member) and `get_cycle_length()` (function) exist
   - Impact: Risk of future divergence if consumers mix approaches
   - Recommendation: Standardize on struct member, keep function as single implementation detail

4. **Phase Continuity Documentation Gap (Principled Development)**
   - Evidence: Accepted phase discontinuity lacks complete documentation of visual impact
   - Impact: Mixed messaging between "mathematically correct" and "visually smooth"

**Final Average:** 6.8/10
<!-- END: VALIDATE/REVIEW -->

---

<!-- BEGIN: VALIDATE/DECISION -->
## Decision

**Status:** REVISE

**Reasoning:** Average score 6.8/10 falls in REVISE range (5.0-6.9), and Principled Development scored below 6. While no critical violations exist and the core mathematics is sound, documentation/code misalignment and unclear wheel continuity behavior require resolution before approval.

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

**Learning:**
Documentation must be swept completely when values change. A single stale reference undermines principle adherence scores even when code is correct.
<!-- END: VALIDATE/DECISION -->

---
