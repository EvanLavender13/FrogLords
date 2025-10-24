# Iteration 2: Speed-Dependent Steering

**Started:** 2025-10-24
**Previous:** [speed_dependent_steering_ITERATION_1.md](speed_dependent_steering_ITERATION_1.md)
**Status:** APPROVED

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
- [x] vehicle::tuning_params struct created with all vehicle parameters (vehicle/tuning.h)
- [x] param_meta defined for each tunable parameter (vehicle/tuning.h:47-63)
- [x] apply_to() method validates and copies to controller (vehicle/tuning.cpp:8-41)
- [x] Parameters moved from controller to tuning system: turn_rate, wheelbase, max_steering_angle, grip_coefficient, steering_reduction_factor

**Vehicle GUI Panel:**
- [x] vehicle_panel_state struct created (gui/vehicle_panel.h:10-12)
- [x] draw_vehicle_panel() function implemented (gui/vehicle_panel.cpp:68-84)
- [x] parameter_type enum extended with vehicle parameter types (gui/parameter_command.h:26-30)
- [x] Panel integrated into runtime (app/runtime.cpp:111-116, runtime.h:9,34)

**Integration:**
- [x] vehicle_params added to game_world (app/game_world.h:26)
- [x] apply_parameter_commands() extended to handle vehicle parameter changes (app/runtime.cpp:178-197)
- [x] vehicle_params.apply_to() called in game_world::init() (app/game_world.cpp:16)
- [x] Build successful with no assertion failures
- [x] Manual test: vehicle parameters tunable via GUI, changes reflected in gameplay
<!-- END: ITERATE/CONTRACT -->

---

<!-- BEGIN: ITERATE/PLAYTEST -->
### Playtest 1

**Date:** 2025-10-24
**Tester:** User

**Verified:**
- [x] Vehicle Tuning panel appears in Debug Panel with all 5 parameters
- [x] Turn Rate slider (1.0-10.0 rad/s): adjustable, changes turning speed immediately in CAR_LIKE mode
- [x] Steering Reduction slider (0.0-1.0): adjustable, steering cone visibly changes width
- [x] Wheelbase slider (1.5-4.0 m): adjustable (currently unused, ready for future systems)
- [x] Max Steering Angle slider (10-45 deg): adjustable (currently unused, ready for future systems)
- [x] Grip Coefficient slider (0.5-2.0): adjustable (currently unused, ready for future systems)
- [x] Vehicle State section displays 3 read-only values
- [x] Heading Yaw: updates correctly during turning
- [x] Steering Multiplier (derived): displays with formula "1 - (v/v_max)·r", updates with speed
- [x] Effective Turn Rate (derived): displays with formula "w_base · m", updates correctly
- [x] No assertion failures during normal operation
- [x] Mathematical formulas display correctly (no Unicode issues)

**Result:** ✓ All contract items validated. System functional and complete.
<!-- END: ITERATE/PLAYTEST -->

---

<!-- BEGIN: ITERATE/COMPLETE -->
## Iteration Complete

**Contract:** ✓ PROVEN

**Properties validated:**
- Precondition fix: steering_reduction_factor validated in constructor (controller.cpp:47-50)
- Vehicle tuning system: metadata-driven architecture following character::tuning_params pattern
- Vehicle GUI panel: tunable sliders + derived state display with mathematical formulas
- Integration: unidirectional data flow (GUI → commands → tuning_params → controller)
- All parameters tunable via GUI with immediate gameplay feedback

**Architecture created:**
- vehicle::tuning_params with param_meta for 5 parameters
- vehicle::apply_to() with precondition validation
- gui::vehicle_panel with metadata-driven widgets
- parameter_command extended with 5 vehicle types
- Mathematical formula display using derived_param widget

**Playtests:** 1 (all contract items verified)
**Commits:** 2 (precondition fix, vehicle architecture)

**Status:**
- [x] Contract proven
- [x] Stable
- [x] Ready for VALIDATE
<!-- END: ITERATE/COMPLETE -->

---

<!-- BEGIN: VALIDATE/REVIEW -->
## External Review

**Tool:** Codex
**Date:** 2025-10-24

**Principle Violations:**
- **Radical Simplicity / Horizon of Certainty**: Iteration wires GUI sliders, metadata, and command plumbing for `wheelbase`, `max_steering_angle`, and `grip_coefficient` even though no runtime code reads these values yet. Command emission in vehicle_panel.cpp:31, dispatch in runtime.cpp:187, copies in tuning.cpp:16. With no consumer system, this is premature complexity for hypothetical future work, violating "build the irreducible core first."

**Strengths:**
- `compute_steering_multiplier` enforces mathematical contract (clamped ratio, bounded output) and is reused by both simulation and instrumentation, honoring Single Source of Truth (vehicle/controller.cpp:57, gui/vehicle_panel.cpp:60)
- Constructor preconditions guard reduction factor domain, completing monotonic proof from prior iteration (vehicle/controller.cpp:47)
- Heading integration remains time-independent and physics-driven: velocity-derived speed feeds multiplier, integration scaled by dt with strong assertions (vehicle/controller.cpp:88)

**Assessment:** Core steering interpolation is mathematically sound and physics-first. Work behaves like a system (inputs flow through controller physics; GUI surfaces tuning). Primary concern is premature GUI/tuning scaffolding for unused parameters. Can be trimmed or deferred; aside from that, iteration aligns well with principles.
<!-- END: VALIDATE/REVIEW -->

---

<!-- BEGIN: VALIDATE/DECISION -->
## Decision

**Status:** APPROVED

**Reasoning:** Core speed-dependent steering system is mathematically sound and principle-aligned. Mathematical correctness proven, time-independence maintained, Single Source of Truth upheld. Identified "premature" infrastructure for wheelbase/max_steering_angle/grip_coefficient is for immediate next backlog task—within horizon of certainty. Minor organizational choice to include soon-to-be-used parameters does not violate Radical Simplicity when consumer systems are imminent.
<!-- END: VALIDATE/DECISION -->

---

<!-- BEGIN: VALIDATE/EMERGENCE -->
## Emergence

**Surprising behaviors:**
- Speed-dependent steering naturally creates arcade-style handling: responsive maneuvering at low speeds, stable high-speed cornering without manual intervention
- Metadata-driven architecture emerged as reusable pattern applicable to all tuning systems (character, vehicle, future systems)
- Derived parameter display with mathematical formulas provides immediate insight into system behavior without code inspection

**Enables (future):**
- Bicycle model physics using wheelbase parameter
- Slip angle and tire grip simulation using max_steering_angle and grip_coefficient
- Complex vehicle handling systems built on simple, composable physics parameters
- Unified tuning architecture across all game systems

**Learned:**
- Building GUI infrastructure slightly ahead of consumer systems acceptable when horizon is clear (immediate next task)
- Formula display pattern (derived_param widget) valuable for exposing mathematical relationships during tuning
- Separation between tuning_params (configuration) and controller (runtime physics) cleanly divides concerns
- Single Source of Truth for steering multiplier (compute_steering_multiplier) naturally emerged from need to display and simulate same value
<!-- END: VALIDATE/EMERGENCE -->

---
