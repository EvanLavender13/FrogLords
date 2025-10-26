# System: Lateral G-Force Calculator

Pure math primitive for measuring centripetal acceleration in vehicle dynamics.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-24
**Layer:** 2
**Complexity:** Small
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/INTERNAL_REFERENCES -->
## Internal References

**Research notes:**
- NOTES/CarMechanics/PhysicsFirst_Research.md:149-158 - Vehicle tilt formula using lateral g-force
- NOTES/CarMechanics/PhysicsFirst_Research.md:21 - Dynamic FOV formula using lateral g-force

**Source files:**
- src/foundation/math_utils.h:87-115 - Existing slip angle calculator (pattern to follow)
- src/vehicle/controller.h:106 - heading_yaw physics state
- src/vehicle/controller.cpp:82-99 - heading integration

**Existing patterns:**
- Pure math primitives in `math` namespace (src/foundation/math_utils.h)
- Inline functions with debug assertions for contract validation
- Physics state tracked in controller struct

**Integration points:**
- Vehicle Tilt System (Layer 4) will consume this for visual lean
- Physics-Based Drift System (Layer 4) will use for grip threshold detection
- Dynamic FOV System (Layer 4) will use for camera effects
<!-- END: SELECT/INTERNAL_REFERENCES -->

---

<!-- BEGIN: SELECT/EXTERNAL_REFERENCES -->
## External References

**Technical documentation:**
- Standard centripetal acceleration formula: `a = v²/r = v * ω`
- Conversion to g-force: divide by 9.8 m/s²

**Visual references:**
- Debug visualization: Vector arrow showing lateral force magnitude/direction

**Constraints/Requirements:**
- Must work through existing physics state (no new state accumulation)
- Time-independent measurement of instantaneous physics
<!-- END: SELECT/EXTERNAL_REFERENCES -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required:**
- Vehicle angular velocity tracking (need to add to controller)
- Math utilities (exists in Layer 2)

**Enables (will become buildable):**
- Vehicle Tilt System (Layer 4)
- Physics-Based Drift System completion (Layer 4)
- Dynamic FOV System (Layer 4)
<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
Measure centripetal acceleration from speed and angular velocity, express as g-force multiplier.

**Why a system, not a feature:**
Enables multiple emergent behaviors: drift detection triggers drift state, g-force drives visual lean angle, camera responds to cornering intensity. Single measurement, multiple emergent uses.
<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Needs proof:**
- Angular velocity integration from heading_yaw changes is time-independent
- Lateral g-force calculation maintains sign convention consistency
- Zero-velocity edge cases return zero g-force

**Known uncertainties:**
- Does controller need angular_velocity as accumulated state or can it be derived per-frame?
- Sign convention for lateral g-force (left vs right turn)?
<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: SELECT/GRAYBOX -->
## Graybox

**Simplest implementation:**
- Add angular_velocity tracking to controller (derived from heading_yaw delta)
- Pure math function: `calculate_lateral_g_force(speed, angular_velocity)`
- Debug arrow showing lateral force direction and magnitude

**Validation:**
- Zero when moving straight
- Proportional to speed at constant turn rate
- Proportional to turn rate at constant speed
- Correct sign (left vs right turns)
<!-- END: SELECT/GRAYBOX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [x] Core functional in graybox
- [x] Mathematical correctness proven
- [x] Debug visualization shows behavior
- [ ] Emergent behaviors appear
- [x] Principles upheld
<!-- END: SELECT/SUCCESS -->

---

<!-- BEGIN: GRAYBOX/SCOPE -->
## Graybox Scope

**What will be built:**
Pure math primitive `calculate_lateral_g_force(speed, angular_velocity)` that derives centripetal acceleration from existing physics state. Add angular velocity tracking to controller (derived per-frame from heading_yaw delta). Debug visualization showing lateral g-force magnitude and direction as colored arrow.

**Validation:**
- Returns zero when moving straight (no turning)
- Proportional to speed at constant turn rate
- Proportional to turn rate at constant speed
- Sign convention matches coordinate system (positive = rightward force)
- Zero-velocity edge case returns zero

**Complexity:** Small
<!-- END: GRAYBOX/SCOPE -->

---

<!-- BEGIN: GRAYBOX/REVIEW -->
## Implementation Review

**Tool:** Codex CLI
**Date:** 2025-10-24

**Question asked:**
Validate implementation plan against principles: 1) Angular velocity derivation follows "Derive, Don't Accumulate", 2) Sign convention correct for coordinate system, 3) Avoids anti-patterns, 4) Matches existing math_utils.h patterns

**Feedback received:**
- HIGH: Raw angle subtraction will spike at wrap boundaries (±π → ∓π yields ±2π delta) - must use `angle_difference_radians()`
- HIGH: Sign convention was inverted - positive yaw is clockwise/right turn per `yaw_to_forward()` implementation

**Impact on implementation:**
- Changed angular velocity derivation to use `angle_difference_radians(heading_yaw, previous_heading_yaw) / dt`
- Corrected sign convention: positive angular_velocity = right turn, centripetal points right (into turn)
- Formula `lateral_accel = speed * angular_velocity` naturally produces correct direction
<!-- END: GRAYBOX/REVIEW -->

---

<!-- BEGIN: GRAYBOX/IMPLEMENTATION_PLAN -->
## Implementation Plan

**Files to modify:**

1. `src/foundation/math_utils.h` - Add `calculate_lateral_g_force()` pure math primitive
2. `src/vehicle/controller.h` - Add `angular_velocity` member (derived state, not accumulated)
3. `src/vehicle/controller.cpp` - Derive angular_velocity from heading_yaw delta in `apply_input()`
4. `src/vehicle/controller.h` - Add `calculate_lateral_g_force()` member function (convenience wrapper)
5. `src/app/debug_generation.cpp` - Add lateral g-force arrow visualization

**Call structure:**

```
controller::apply_input()
  └─> Derive angular_velocity from heading_yaw delta (current - previous) / dt

controller::calculate_lateral_g_force()
  └─> math::calculate_lateral_g_force(horizontal_speed, angular_velocity)
      └─> Returns lateral_accel / 9.8 (g-force multiplier)

generate_character_state_primitives()
  └─> float g_force = character.calculate_lateral_g_force()
  └─> Draw arrow perpendicular to velocity, scaled by g_force magnitude
```

**Debug data flow:**

1. Controller tracks `previous_heading_yaw` and current `heading_yaw`
2. In `apply_input()`: derive `angular_velocity = angle_difference_radians(heading_yaw, previous_heading_yaw) / dt`
   - CRITICAL: Must use `angle_difference_radians()` to handle wrap-around at ±π boundaries
   - Raw subtraction would spike to ±2π when wrapping, violating frame-independence
3. Store `previous_heading_yaw = heading_yaw` at end of frame
4. Math primitive: `lateral_accel = speed * angular_velocity`, return `lateral_accel / 9.8`
5. Debug visualization: Draw arrow from vehicle position, direction = right vector, length = g_force magnitude

**Integration points:**

- `controller::apply_input()` - Add angular_velocity derivation after heading integration
- `math` namespace in `math_utils.h` - Add inline function following `calculate_slip_angle()` pattern
- `generate_character_state_primitives()` - Add arrow visualization similar to existing orientation arrow
- Color convention: Purple/magenta for g-force (distinct from green orientation, cyan intent)

**Sign convention:**

Centripetal acceleration points toward the center of the circular path (into the turn).

- Positive yaw rotation = clockwise from above (toward +X) = right turn
- Positive angular_velocity = right turn → centripetal points right (toward center)
- Negative angular_velocity = left turn → centripetal points left (toward center)
- Formula `lateral_accel = speed * angular_velocity` naturally produces correct sign
- Visualization: `arrow_direction = math::yaw_to_right(heading_yaw) * g_force`
<!-- END: GRAYBOX/IMPLEMENTATION_PLAN -->

---

<!-- BEGIN: GRAYBOX/RESULTS -->
## Results

**Status:**
- [x] Core functional
- [x] Build successful
- [x] Debug viz working
- [x] Ready for iteration

**Works:**
- Zero g-force when moving straight (no arrow appears)
- G-force proportional to speed at constant turn rate (arrow grows with speed)
- G-force proportional to turn rate at constant speed (harder turns = longer arrow)
- Sign convention correct: right turn → right arrow, left turn → left arrow (centripetal points into turn)
- Zero-velocity edge case: no arrow when stationary
- Wrap boundary handling: smooth behavior across ±π boundaries (no spikes)

**Doesn't:**
- None identified

**Surprises:**
- Arrow scaling needed 3x multiplier for good visibility (tuned: 3.0 meters per g)
- Hard to test constant-speed scenarios due to binary input (0 or 100%), but proportionality validated at extremes

**Next:**
- ITERATE phase - system ready for higher-layer consumers (drift detection, camera FOV, vehicle tilt)
<!-- END: GRAYBOX/RESULTS -->

---

## Iterations

- [ITERATION_1.md](lateral_g_force_ITERATION_1.md) - APPROVED
