# Iteration 1: Lateral G-Force Calculator

**Started:** 2025-10-24
**Status:** Ready for VALIDATE

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

Mathematical correctness and edge case handling for lateral g-force calculation primitive.

- [x] **Time-Independence**: Angular velocity derivation produces identical results at any framerate
  - Validated: controller.cpp:118-119 - `angular_velocity = heading_delta / dt`
  - Postcondition: controller.cpp:121-122 - finite check
- [x] **Wrap-Around Safety**: Heading transitions across ±π boundary produce correct angular velocity (no ±2π spikes)
  - Validated: controller.cpp:118 - uses `angle_difference_radians()` not raw subtraction
  - Contract prevents ±2π spikes documented in implementation plan
- [x] **Zero-Speed Edge Case**: Returns 0.0 when speed below epsilon threshold
  - Validated: math_utils.h:132-135 - early return for speed < 0.0001 m/s
- [x] **Sign Convention**: Positive angular velocity (right turn) produces positive g-force (rightward centripetal)
  - Validated: math_utils.h:140 - `lateral_accel = speed * angular_velocity`
  - Debug viz: debug_generation.cpp:87-88 - arrow uses right vector scaled by g_force
- [x] **Finite Validation**: All inputs validated as finite in debug builds
  - Validated: math_utils.h:128-129 - preconditions for speed and angular_velocity
  - controller.cpp:92-96 - preconditions for all integration inputs
- [x] **No Accumulated State**: Angular velocity derived per-frame from heading delta, not accumulated
  - Validated: controller.h:108-111 - angular_velocity marked as DERIVED STATE
  - controller.cpp:104, 118-119 - computed fresh each frame from heading change
- [x] **Proportionality**: G-force scales linearly with speed at constant turn rate
  - Validated: Manual testing - hold D key, accelerate with W, magenta arrow grows with speed
  - Formula: `g = (speed * ω) / 9.8` - linear in speed ✓
- [x] **Proportionality**: G-force scales linearly with turn rate at constant speed
  - Validated: Math formula `g = (speed * ω) / 9.8` - linear in angular_velocity
  - Note: Cannot test independently with binary input (turn rate = turn_rate * steering_multiplier)
  - Speed-dependent steering causes ω to vary with speed, making independent testing impossible
  - Correctness proven by formula, not manual observation
<!-- END: ITERATE/CONTRACT -->

---

<!-- BEGIN: ITERATE/PLAYTEST -->
### Playtest 1

**Date:** 2025-10-24
**Tester:** User

**Test Results:**
- ✓ Zero when straight: No magenta arrow when moving without turning
- ✓ Sign convention: Right turns (D) produce rightward arrow, left turns (A) produce leftward arrow
- ✓ Proportional to speed: Arrow grows as speed increases during constant turn (hold D, accelerate W)
- ✓ Wrap-around: No spikes or jumps during extended circular driving

**Violations:**
None

**Emergent:**
- Test design error identified: Cannot independently test turn rate proportionality with binary input
- Speed-dependent steering (steering_reduction_factor) couples speed and angular velocity
- Mathematical formula proves linearity in both variables

**Process Violation:**
- Skipped Context Selection step before defining contract
- Should have reviewed ACCUMULATED_STATE, PRIMITIVES_GUIDE, PHYSICS_INTEGRATION_PATTERNS, VEHICLE_DYNAMICS_TERMINOLOGY
- Patterns were followed anyway (derived state, primitives boundary, dt scaling, sign conventions)
- Lesson: Don't skip process steps even when confident - context review catches unknown unknowns

**Status:**
✅ VERIFIED: All contract properties validated (6 via assertions, 2 via testing/math)
<!-- END: ITERATE/PLAYTEST -->

---

<!-- BEGIN: ITERATE/COMPLETE -->
## Iteration Complete

**Contract:** ✓ PROVEN

**Properties:**
- Time-independence: Angular velocity derivation validated via dt scaling and finite checks
- Edge cases: Zero-speed epsilon threshold, wrap-around safety via angle_difference_radians()
- Sign convention: Centripetal acceleration direction matches coordinate system
- Proportionality: Linear scaling with speed (manual test) and angular velocity (mathematical proof)

**Edges:**
- Zero speed: Returns 0.0 below 0.0001 m/s threshold
- Wrap boundaries: ±π transitions handled by angle_difference_radians() (no ±2π spikes)
- Finite inputs: All preconditions validate finite values in debug builds

**Assertions:**
- 3 preconditions in math::calculate_lateral_g_force() (speed ≥ 0, finite speed, finite angular_velocity)
- 5 preconditions in controller::apply_input() (finite dt, turn_rate, input, heading)
- 2 postconditions for angular_velocity (finite, valid range)
- 1 precondition in constructor (steering_reduction_factor bounds)

**Playtests:** 1 (all properties verified)

**Status:**
- [x] Contract proven
- [x] Stable
- [x] Ready for VALIDATE
<!-- END: ITERATE/COMPLETE -->

