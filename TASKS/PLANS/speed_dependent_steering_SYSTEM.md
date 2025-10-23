# System: Speed-Dependent Steering

Reduce maximum steering angle at high speeds to prevent unrealistic tight turns.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-23
**Layer:** 3 (Systems - vehicle movement modifier)
**Complexity:** Small (1 pt)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/REFERENCES -->
## References

**Technical documentation:**
- Formula: `max_angle = base_angle * (1.0 - (speed/maxSpeed) * reduction_factor)`
- Industry standard across arcade racers (Research.md section 2)
- Low-speed: full 10° steering for parking maneuvers
- High-speed: restricted to 2-3° for stability

**Research findings:**
- "Speed-dependent steering limits prove universally critical across successful arcade racers"
- "This single technique solves countless tuning problems and appears in virtually every polished racer"
- Prevents unrealistic tire slip from tight turns at highway speeds

**Constraints/Requirements:**
- Must work with existing heading integration in `apply_input()`
- Should scale `turn_rate` by `(steering_limit / max_steering_angle)`
- Default reduction factor: 0.7 (70% reduction at max speed)
<!-- END: SELECT/REFERENCES -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required:**
- Vehicle controller (exists in Layer 3)
- Heading integration (exists in controller::apply_input)
- Max speed parameter (exists)

**Enables (will become buildable):**
- Circle-based position calculation (step 2.9)
- Drift detection (step 2.10)
<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
Calculate horizontal speed, compute speed-dependent steering multiplier using the formula, scale turn_rate by multiplier before heading integration.

**Why a system, not a feature:**
Enables physically plausible vehicle behavior at all speeds. Foundation for drift mechanics (step 2.10) which rely on lateral acceleration limits. Without this, high-speed tight turns would violate physics constraints that drift detection depends on.
<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Needs proof:**
- Formula produces values in [0, 1] range across all valid speeds
- Steering reduction is monotonic (never increases with speed)

**Known uncertainties:**
- Optimal reduction_factor value (0.7 from backlog, may need tuning)
- Whether to use instantaneous speed or smoothed speed
<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: SELECT/GRAYBOX -->
## Graybox

**Simplest implementation:**
Direct formula application in `apply_input()`. Calculate `speed = length(horizontal_velocity)`, compute `steering_limit = max_steering_angle * (1.0f - (speed/max_speed) * reduction_factor)`, scale turn_rate integration by `(steering_limit / max_steering_angle)`.

**Validation:**
- At zero speed: full turn_rate applied
- At max speed: 30% of turn_rate applied (with default 0.7 reduction)
- Turning radius increases with speed
- Debug visualization shows tighter heading changes at low speed
<!-- END: SELECT/GRAYBOX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [x] Core functional in graybox
- [x] Mathematical correctness proven
- [x] Debug visualization shows behavior
- [x] Emergent behaviors appear
- [x] Principles upheld
<!-- END: SELECT/SUCCESS -->
