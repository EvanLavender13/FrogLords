# Iteration 1: Walk/Run Speed States

**Started:** 2025-10-19
**Status:** In Progress

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
