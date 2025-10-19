# Iteration 2: Walk/Run Speed States

**Started:** 2025-10-19
**Previous:** [walk_run_speed_states_ITERATION_1.md](walk_run_speed_states_ITERATION_1.md)
**Status:** In Progress

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

- [ ] Documentation/code alignment (wheel size)
  - Decision: Keep code, update docs - sprint=largest wheel is correct
  - Rationale: Faster gaits have longer strides (walk=2m, run=3m, sprint=4m). Larger wheel = longer stride. Physically accurate.
  - Update: SYSTEM.md needs correction (currently says sprint=smallest)
  - Commit: [Pending - docs update]

- [ ] Float precision decision documented
  - Decision: ACCEPT LIMITATION - precision loss at ~1e7 meters (10,000 km) is acceptable
  - Rationale: Typical gameplay doesn't involve 10,000 km of continuous movement. If needed, distance can be reset during level transitions. Cost of double or periodic reset exceeds benefit.
  - Update: Document in SYSTEM.md mathematical foundation
  - Commit: [Pending - docs update]

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
