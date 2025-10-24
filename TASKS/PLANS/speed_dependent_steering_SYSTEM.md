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

---

<!-- BEGIN: GRAYBOX/SCOPE -->
## Graybox

**What will be built:**
Speed-dependent steering multiplier that scales `turn_rate` based on current horizontal speed. At zero speed, full turning authority applies. At max speed, steering is reduced by the reduction factor (default 0.7). This prevents unrealistic tight turns at high speeds while preserving maneuverability at low speeds.

**Validation:**
- Low-speed (0-2 m/s): near-full turn_rate produces tight heading changes
- Mid-speed (3-6 m/s): partial reduction visible in wider turning arcs
- High-speed (7-8 m/s): maximum reduction creates stable, gentle turns
- Debug visualization displays current steering multiplier and effective turn rate

**Complexity:** Small
<!-- END: GRAYBOX/SCOPE -->

---

<!-- BEGIN: GRAYBOX/IMPLEMENTATION_PLAN -->
## Implementation Plan

**Files to modify:**

1. `src/vehicle/controller.h` - Add steering reduction factor parameter
2. `src/vehicle/controller.cpp` - Modify `apply_input()` to apply speed-dependent steering multiplier
3. `src/gui/character_panel.cpp` - Add debug display showing steering multiplier and effective turn rate

**Call structure:**

`controller::apply_input()` (src/vehicle/controller.cpp:52)
  1. Calculate horizontal speed: `speed = length(project_to_horizontal(velocity))`
  2. Compute steering multiplier via helper: `steering_multiplier = compute_steering_multiplier(speed)`
  3. Scale turn rate integration: `heading_yaw += -input * turn_rate * steering_multiplier * dt`
  4. Continue with existing heading integration

`controller::compute_steering_multiplier(float speed)` (new helper - private or public)
  1. Compute speed ratio: `speed_ratio = speed / max_speed`
  2. Clamp ratio to [0, 1]: `speed_ratio = clamp(speed_ratio, 0.0f, 1.0f)`
  3. Calculate multiplier: `1.0f - (speed_ratio * steering_reduction_factor)`
  4. Return clamped result in [0, 1]

**Debug data flow:**

- GUI derives horizontal speed from `controller.velocity` (existing pattern)
- GUI calls `controller.compute_steering_multiplier(speed)` to get current multiplier
- Display both speed and multiplier in debug panel
- No cached/stored state - all values derived on-demand

**Integration points:**

- Modify existing heading integration in `controller::apply_input()` at line 66
- Add new parameter `steering_reduction_factor` to controller struct (default 0.7)
- Add debug display to existing "Locomotion State" section in `character_panel.cpp`
- No changes to physics integration or collision systems
<!-- END: GRAYBOX/IMPLEMENTATION_PLAN -->

---

<!-- BEGIN: GRAYBOX/REVIEW -->
## Implementation Review

**Tool:** Codex CLI
**Date:** 2025-10-23

**Question asked:**
Review the implementation plan in speed_dependent_steering_SYSTEM.md. Does this approach follow principles? Are there any anti-patterns? Is the integration clean? Any concerns about storing derived state (speed, steering_multiplier) as members vs computing on-demand?

**Feedback received:**
- **Unbounded multiplier**: Original formula could produce negative values when speed exceeds max_speed (overspeed is intentionally allowed). Violates "Mathematical Foundation" requirement that formula stays in [0,1].
- **Derived state caching**: Storing speed and steering_multiplier as members violates "Derive, Don't Accumulate" and "Single Source of Truth" principles. These are pure derivatives of velocity and parameters.
- **Debug coupling/staleness**: Cached values updated in apply_input() would be stale after update() mutates velocity, causing misleading instrumentation.

**Impact on implementation:**
- Added explicit clamping of speed_ratio to [0, 1] to guarantee multiplier bounds
- Removed cached state - compute steering multiplier on-demand via helper function
- Helper function provides deterministic computation for both simulation and debug GUI
- GUI derives speed from velocity (existing pattern) and calls helper for multiplier
<!-- END: GRAYBOX/REVIEW -->

---

<!-- BEGIN: GRAYBOX/RESULTS -->
## Results

**Status:**
- [x] Core functional
- [x] Build successful
- [x] Debug viz working
- [x] Ready for iteration

**Works:**
- Speed-dependent steering multiplier correctly scales turn_rate integration
- Formula produces expected range: 1.0 at zero speed → 0.3 at max speed (with reduction_factor=0.7)
- Debug panel displays real-time steering multiplier and effective turn rate
- No assertion failures, mathematically correct with clamping for overspeed
- **Visual validation: Orange steering authority cone visibly shrinks at high speeds, expands at low speeds**
- Cone width directly represents turn_rate * steering_multiplier - proves math is correct, not just numbers
- Behavior "looks right" across speed ranges (user feedback)

**Doesn't:**
- N/A - core mechanic functional

**Surprises:**
- Keyboard binary input (0%/100%) makes smooth speed testing challenging but doesn't prevent validation
- Turning at zero speed: heading integration occurs but requires forward input (W) to produce visible movement in CAR_LIKE mode (expected behavior)
- Turning causes velocity slowdown: velocity/heading mismatch creates drag effect - deferred to later phase (circle-based position calculation or drift mechanics)

**Observations:**
- Codex review prevented three principle violations: unbounded multiplier, derived state caching, debug coupling
- Helper function pattern (compute_steering_multiplier) enables clean on-demand computation for both simulation and debug
- Integration required minimal changes: one parameter, one helper function, heading integration scaling, debug display
- **Visual validation mandatory**: Initial numeric-only debug display was insufficient - added steering authority cone for undeniable proof
- Orange cone makes steering reduction immediately obvious: wide at low speed, narrow at high speed

**Next:**
- ITERATE phase: tune steering_reduction_factor if needed
- Consider: circle-based position calculation (step 2.9) to address velocity/heading mismatch
- Consider: drift detection (step 2.10) building on lateral acceleration foundation
<!-- END: GRAYBOX/RESULTS -->

---

## Iterations

- [speed_dependent_steering_ITERATION_1.md](speed_dependent_steering_ITERATION_1.md) - REVISE
- [speed_dependent_steering_ITERATION_2.md](speed_dependent_steering_ITERATION_2.md) - APPROVED
