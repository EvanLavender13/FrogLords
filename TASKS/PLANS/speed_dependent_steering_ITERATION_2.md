# Iteration 2: Speed-Dependent Steering

**Started:** 2025-10-24
**Previous:** [speed_dependent_steering_ITERATION_1.md](speed_dependent_steering_ITERATION_1.md)
**Status:** In Progress

---

<!-- BEGIN: ITERATE/CONTEXT -->
## Context from Previous Iteration

**Decision:** REVISE (from ITERATION_1)

**Required changes from VALIDATE:**
- Add precondition in constructor: assert steering_reduction_factor ∈ [0,1] and finite
- Ensures mathematical contract is enforced at initialization, not deferred to postcondition
- Completes the stated proof that formula produces monotonic decrease

**Planned architectural work from ITERATION_1:**
- Vehicle tuning system architecture (metadata, apply_to pattern)
- Vehicle GUI panel (separate from character_panel)

**Baseline contract:** See ITERATION_1/CONTRACT
<!-- END: ITERATE/CONTEXT -->

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

**Precondition Fix:**
- [x] Controller constructor validates: steering_reduction_factor ∈ [0,1] and finite (controller.cpp:47-50)
- [x] Mathematical contract complete: monotonic decrease with enforced parameter bounds

**Vehicle Tuning Architecture:**
- [ ] vehicle::tuning_params struct created with all vehicle parameters
- [ ] param_meta defined for each tunable parameter
- [ ] apply_to() method validates and copies to controller
- [ ] Parameters moved from controller to tuning system: turn_rate, wheelbase, max_steering_angle, grip_coefficient, steering_reduction_factor

**Vehicle GUI Panel:**
- [ ] vehicle_panel_state struct created
- [ ] draw_vehicle_panel() function implemented
- [ ] parameter_type enum extended with vehicle parameter types
- [ ] Panel integrated into runtime

**Integration:**
- [ ] vehicle_params added to game_world
- [ ] apply_vehicle_commands() or extended apply_parameter_commands() handles vehicle parameter changes
- [ ] Build successful with no assertion failures
- [ ] Manual test: vehicle parameters tunable via GUI, changes reflected in gameplay
<!-- END: ITERATE/CONTRACT -->

---
