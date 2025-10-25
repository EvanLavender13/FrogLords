# Iteration 1: Lateral G-Force Calculator

**Started:** 2025-10-24
**Status:** In Progress

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
- [ ] **Proportionality**: G-force scales linearly with speed at constant turn rate
  - Requires manual testing: hold turn input constant, vary acceleration
- [ ] **Proportionality**: G-force scales linearly with turn rate at constant speed
  - Requires manual testing: vary turn input, observe g-force arrow length
<!-- END: ITERATE/CONTRACT -->

---
