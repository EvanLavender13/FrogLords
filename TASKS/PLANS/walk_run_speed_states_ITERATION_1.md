# Iteration 1: Walk/Run Speed States

**Started:** 2025-10-19
**Status:** REVISE

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

**Mathematical properties:**
- [x] Phase calculation is frame-rate independent (same distance → same phase at any FPS)
  - Validated: distance += speed * dt (controller.cpp:204) - scales with dt
  - Assertion: FL_PRECONDITION(dt > 0.0f) (controller.cpp:106)
- [x] Phase wraps cleanly at cycle boundaries (no discontinuities at 0.99→0.0)
  - Validated: std::fmod guarantees [0, 1) range mathematically
  - Assertion: FL_POSTCONDITION(phase >= 0.0f && phase < 1.0f) (controller.cpp:210-211)
- [x] Distance accumulation is continuous and monotonic (never decreases, never resets unexpectedly)
  - Validated: distance += speed * dt where speed = length(velocity) >= 0
  - Assertion: FL_POSTCONDITION(speed >= 0.0f) (controller.cpp:191)
- [x] Cycle length is always positive (divide-by-zero impossible)
  - Validated: FL_PRECONDITION(cycle_length > 0.0f) (controller.cpp:208) - existing
- [x] State classification is deterministic (same speed → same state every time)
  - Validated: Pure threshold checks, no randomness (controller.cpp:195-200)

**Edge cases:**
- [x] Zero velocity (stationary): phase freezes, no distance accumulated
  - Validated: distance += speed * dt where speed=0 → no change (controller.cpp:204)
- [x] State transitions: phase continues across state boundaries (no reset)
  - Validated: distance_traveled never resets, only cycle_length changes (controller.cpp:207-211)
- [x] Backward movement: phase accumulates (uses speed magnitude, not signed velocity)
  - Validated: speed = length(velocity) always positive (controller.cpp:190)
- [x] Strafe movement: phase accumulates based on horizontal speed magnitude
  - Validated: uses project_to_horizontal then length (controller.cpp:190)
- [x] Collision during movement: distance only accumulates when actually moving
  - Validated: uses velocity AFTER collision resolution (controller.cpp:190)
- [x] Very high speeds: phase calculation remains stable (no overflow/precision loss)
  - Validated: Added isfinite assertion (controller.cpp:205-206)
- [x] Rapid state oscillation: state changes instant, no hysteresis artifacts
  - Validated: Stateless threshold checks, no smoothing (controller.cpp:195-200)

**Consistency:**
- [x] Same input → same output (deterministic state classification)
  - Validated: Pure functions, no randomness (see mathematical properties)
- [x] Frame-rate independent (verified through variable dt testing)
  - Validated: All calculations scale with dt (see mathematical properties)
- [x] No special cases (all movement types use same phase calculation)
  - Validated: Code review shows no conditional logic or special paths (controller.cpp:188-211)

<!-- END: ITERATE/CONTRACT -->

---

<!-- BEGIN: ITERATE/PLAYTEST_1 -->
### Playtest 1

**Date:** 2025-10-19
**Tester:** User

**Results:** ✅ ALL PASS (18/18 items)

**Mathematical Properties:**
- ✅ Frame-rate independence: Confirmed stable
- ✅ Phase wrapping: Smooth 0→1 wrap, no discontinuities
- ✅ Distance accumulation: Continuous while moving, correctly freezes when airborne
- ✅ Positive cycle length: No crashes or errors
- ✅ Deterministic state: Consistent state classification at same speeds

**Edge Cases:**
- ✅ Stationary: Phase freezes correctly
- ✅ State transitions: Phase continues smoothly across walk→run→sprint
- ✅ Backward movement: Character/wheel orientation responds correctly
- ✅ Strafe movement: Phase accumulates based on speed magnitude
- ✅ Collision: Wheel stops rotating when blocked (distance stops)
- ✅ Very high speeds: Stable, no glitches
- ✅ Rapid oscillation: No flickering or artifacts observed

**Consistency:**
- ✅ Deterministic: Same movement → same results
- ✅ No special cases: Uniform behavior across all directions

**Visual:**
- ✅ Color coding: Walk=green, Run=yellow, Sprint=red, instant transitions
- ✅ Surveyor wheel: Rotates in movement direction
- ✅ Wheel size: Matches state (walk=2m, run=3m, sprint=4m)
- ✅ Debug panel: All values display correctly

**Violations:** None

**Emergent:**
- Distance correctly doesn't accumulate during jumps (horizontal speed determines phase)
- Character orientation and wheel respond correctly to input direction
- Wheel size differences clearly visible (walk smallest, sprint largest)

**Status:** ✅ CONTRACT PROVEN
<!-- END: ITERATE/PLAYTEST_1 -->

---

<!-- BEGIN: ITERATE/COMPLETE -->
## Iteration Complete

**Contract:** ✓ PROVEN

**Properties:** Validated through debug assertions and mathematical analysis
- Frame-rate independence: dt > 0 precondition + dt-scaled distance accumulation
- Phase wrapping: fmod properties proven mathematically + [0,1) postcondition
- Distance monotonicity: speed >= 0 postcondition ensures never decreases
- Positive cycle length: cycle_length > 0 precondition prevents divide-by-zero
- Deterministic classification: Pure threshold functions, no randomness

**Edges:** All 7 cases handled correctly through design + validation
- Zero velocity: Natural consequence of speed=0 in distance += speed * dt
- State transitions: distance_traveled never resets, continuous across states
- Backward/strafe: Uses horizontal speed magnitude, direction-agnostic
- Collision: Uses post-resolution velocity, naturally stops when blocked
- High speeds: isfinite assertion catches overflow
- Rapid oscillation: Stateless classification, no artifacts

**Assertions:** 6 added (controller.cpp)
- FL_PRECONDITION(dt > 0.0f && isfinite(dt)) - line 106-107
- FL_POSTCONDITION(speed >= 0.0f && isfinite(speed)) - line 191-192
- FL_POSTCONDITION(isfinite(distance_traveled)) - line 205-206
- FL_PRECONDITION(cycle_length > 0.0f) - line 210 (existing)
- FL_POSTCONDITION(phase >= 0.0f && phase < 1.0f) - line 211-212

**Playtests:** 1 (18/18 items passed, 0 violations)

**Status:**
- [x] Contract proven (assertions + playtest)
- [x] Stable (no crashes, no violations)
- [x] Ready for VALIDATE

<!-- END: ITERATE/COMPLETE -->

---

<!-- BEGIN: VALIDATE/SCORES -->
## Principle Scores

**1. Radical Simplicity:** 9/10
- Does every element justify its existence?
- Evidence: Single state enum, pure threshold checks, no special cases. Distance accumulation is one line. Phase calculation is fmod(distance/cycle_length). Three thresholds define all states. Only weakness: Could cycle_length be derived rather than stored per-state?

**2. Fundamental Composable Functions:** 9/10
- Are systems orthogonal and composable without special cases?
- Evidence: Phase calculation is completely independent of state classification. State determines cycle_length, nothing else. Can swap speed thresholds without touching phase logic. No cross-dependencies. Composes cleanly with existing controller.

**3. Solid Mathematical Foundations:** 10/10
- Is every behavior mathematically correct?
- Evidence: Frame-rate independence proven via dt scaling (controller.cpp:204). Phase wrapping uses std::fmod mathematical properties. Speed magnitude always non-negative (controller.cpp:190-191). Assertions verify all invariants. No accumulated error, no drift.

**4. Emergent Behavior:** 8/10
- Does the system generate unexpected behaviors from simple rules?
- Evidence: Jump behavior emerges naturally (no distance while airborne). Direction response emerges from velocity-based distance. Wheel size variation emerges from state-based cycle_length. Limited emergence due to simplicity—system does exactly what designed for, no surprising interactions yet.

**5. Consistency:** 10/10
- Does the system behave predictably with same input?
- Evidence: Pure threshold functions (controller.cpp:195-200). Deterministic state classification proven in playtest. No randomness, no time-dependent state. Frame-rate independent. Zero special cases. Contract proven through assertions.

**6. Principled Development:** 9/10
- Do all decisions trace back to principles?
- Evidence: Speed magnitude over signed velocity → Consistency. Distance accumulation → Time-Independence. Threshold-based states → Radical Simplicity. Assertions → Validation Before Integration. Only weakness: Initial design didn't question if cycle_length storage is necessary.

**Average:** 9.2/10
<!-- END: VALIDATE/SCORES -->

---

<!-- BEGIN: VALIDATE/REVIEW -->
## External Review

**Tool:** Codex CLI
**Date:** 2025-10-19

**Codex Scores:**
1. Radical Simplicity: 7/10 (self: 9/10)
2. Fundamental Composable Functions: 6/10 (self: 9/10)
3. Solid Mathematical Foundations: 7/10 (self: 10/10)
4. Emergent Behavior: 5/10 (self: 8/10)
5. Consistency: 6/10 (self: 10/10)
6. Principled Development: 4/10 (self: 9/10)
**Average: 5.8/10** (self: 9.2/10)

**Critical Violations Identified:**

1. **Phase continuity BROKEN** (Mathematical Foundation + Consistency)
   - When cycle_length changes, phase jumps discontinuously
   - Example: 5.5m traveled → run phase = fmod(5.5,3)/3 = 0.833
   - After sprint transition → phase = fmod(5.5,4)/4 = 0.375
   - **This violates the contract claim "phase continues across state boundaries"**
   - controller.cpp:208-213

2. **Contract violation on outputs** (Composability)
   - System plan: output {state, phase, cycle_length}
   - Actual code: output {state, phase, distance_traveled}
   - Downstream systems must recompute cycle_length, violating orthogonality
   - controller.h:124-134 vs SYSTEM.md:120-133

3. **Documentation mismatch** (Principled Development)
   - Plan says sprint uses smallest wheel for visual clarity
   - Code has walk=2m, run=3m, sprint=4m (opposite)
   - Either visualization wrong or plan outdated
   - SYSTEM.md:156 vs controller.h:124-135

4. **No threshold ordering validation** (Validation Before Integration)
   - Can set run_threshold <= walk_threshold with no assertion
   - Would collapse middle state silently
   - controller.cpp:195-201

5. **Float precision drift** (Mathematical Foundations)
   - distance_traveled accumulates indefinitely as float
   - Above ~1e7, fmod loses fractional precision → phase jitter
   - No mitigation or assertion to catch
   - controller.cpp:203-206

6. **Silent enum fallback** (Consistency)
   - Returns walk_cycle_length for invalid states
   - Masks bugs instead of failing fast
   - controller.cpp:222-231

**Adjusted Scores (accepting Codex evidence):**
1. Radical Simplicity: 7/10 (silent fallback hides complexity)
2. Fundamental Composable Functions: 6/10 (contract violation on outputs)
3. Solid Mathematical Foundations: 6/10 (phase discontinuity is critical)
4. Emergent Behavior: 5/10 (limited, hardcoded behavior)
5. Consistency: 5/10 (phase jumps break predictability)
6. Principled Development: 4/10 (multiple plan/code mismatches)

**Final Average: 5.5/10**
<!-- END: VALIDATE/REVIEW -->

---

<!-- BEGIN: VALIDATE/DECISION -->
## Decision

**Status:** REVISE

**Reasoning:**
Average 5.5/10 is below the 7.0 threshold for approval. Multiple principles scored below 6:
- Consistency: 5/10 (critical - phase jumps break predictability)
- Mathematical Foundations: 6/10 (critical - discontinuities violate contract)
- Principled Development: 4/10 (critical - plan/code mismatches)

Most critical: **Phase continuity is broken**. The contract claimed "phase continues across state boundaries" but Codex proved this false with concrete math. When cycle_length changes, phase jumps discontinuously (e.g., 0.833 → 0.375). This is a fundamental mathematical violation.

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
<!-- END: VALIDATE/DECISION -->

---
