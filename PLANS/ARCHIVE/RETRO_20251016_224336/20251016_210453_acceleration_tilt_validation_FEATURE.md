# Acceleration Tilt Math Validation

**Core Mechanic:** Isolated test suite validating acceleration vector to tilt quaternion conversion. Pure mathematical function with objective correctness—extend quaternion validation pattern to acceleration tilt math.

**Pattern Reuse:** Extends proven mathematical primitive validation pattern (quaternion suite: 24/24 tests passing). Uses existing acceleration tilt implementation (animation.cpp:24-77).

**Graybox Scope:**
- Test acceleration → tilt angles conversion (world to local space transformation)
- Edge cases: zero acceleration, vertical acceleration, extreme values
- Numerical stability: denormalized vectors, floating point precision
- Velocity scaling: tilt magnitude scales with speed (0.5x to 1.5x range)
- Contact weight scaling: tilt zeroes when airborne
- Estimated lines: 100-150 (test file + runner)

**Minimal Implementation:** Create `src/foundation/tests/test_acceleration_tilt.cpp` following quaternion validation pattern. Manually executable (not CI/CD).

**Problem Evidence:** Skeletal animation removal (2025-10-15) proved visual validation misses subtle mathematical errors. Quaternion validation suite caught bugs that "looked okay" but were fundamentally wrong. Acceleration tilt is pure math (acceleration vector → tilt quaternion)—bugs would be subtle angle errors hard to spot visually.

**Unlocks:**
- Immediate: Confident refactoring of acceleration tilt math, regression protection
- Future: Validation pattern for spring-damper, collision math, orientation math (foundation primitives)

**Test Loop:** Write test → run executable → verify output → iterate. Seconds per iteration. Run manually when refactoring tilt math.

**Certainty:** 100% (80% base + 20% debug tool boost)
- Pure math testing with objective correctness
- Pattern already validated (quaternion suite successful)
- Foundation layer primitive (stable code, low churn)
- Zero gameplay coupling
- Clear integration points (existing tilt code in animation.cpp)

---

## Test Coverage

**Core Transformation Tests:**
1. Zero acceleration → zero tilt
2. Forward acceleration → positive pitch (local space)
3. Right acceleration → positive roll (local space)
4. Backward acceleration → negative pitch
5. Left acceleration → negative roll
6. Diagonal acceleration → pitch + roll combination

**Edge Cases:**
7. Vertical acceleration (Y-axis) → ignored (horizontal projection)
8. Denormalized acceleration vector → normalized before tilt calculation
9. Very small acceleration (< 0.01f threshold) → decay to neutral
10. Extreme acceleration magnitude → normalized direction, clamped scaling

**Velocity Scaling:**
11. Zero velocity → 0.5x tilt magnitude (minimum scale)
12. Reference speed velocity → 1.0x tilt magnitude
13. High velocity → 1.5x tilt magnitude (clamped max)
14. Velocity scale clamped to [0.0, 1.0] range

**Contact Weight Scaling:**
15. Grounded (weight = 1.0) → full tilt
16. Airborne (weight = 0.0) → zero tilt
17. Transitioning (weight = 0.5) → half tilt

**Coordinate Space Transformation:**
18. World-space acceleration at 0° yaw → matches local space
19. World-space acceleration at 90° yaw → rotated correctly to local space
20. World-space acceleration at 180° yaw → reversed correctly
21. World-space acceleration at arbitrary yaw → correct local transformation

**Numerical Stability:**
22. Floating point precision near zero
23. Very large acceleration values
24. Denormalized input vectors

---

## Implementation Notes

**Validation Scope Boundaries:**
- ✅ Pure math with objectively correct outputs (acceleration → tilt angles)
- ✅ Foundation layer primitive (stable code)
- ❌ Game feel/mechanics requiring playtesting (tilt magnitude tuning)
- ❌ Reactive systems where "correct" is subjective (smoothing behavior)

**Pattern Guidelines:**
- Not violating "no automated tests" principle—this is the **exception** for mathematical primitives
- Tests live in isolation (`src/foundation/tests/` or similar)
- Run manually when refactoring math (not CI/CD overhead)
- Focus on edge cases and numerical stability, not exhaustive coverage

**Integration Points:**
- `animation_state::update_acceleration_tilt()` (animation.cpp:24-77)
- World-to-local rotation matrix (animation.cpp:37-39)
- Velocity scaling logic (animation.cpp:51-55)
- Contact weight scaling (animation.cpp:64-66)

---

## Success Criteria

- 20+ tests passing (covering core transformations, edge cases, scaling, numerical stability)
- Zero false positives (tests fail when math is wrong, pass when correct)
- Clear failure messages (expected vs. actual with context)
- Fast execution (< 1 second for full suite)
- Reproducible results (deterministic, no flaky tests)
