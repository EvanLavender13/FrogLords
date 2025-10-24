# System: Circle-Based Turning

Calculate position along circular arc from speed and steering angle. Foundation for predictable arcade handling.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-24
**Layer:** 2
**Complexity:** Small
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/REFERENCES -->
## References

**Technical documentation:**
- "Implementing Racing Games" - Circle-based turning algorithm (Research.md:217-236)
- Marco Monster's "Car Physics for Games" - Vehicle dynamics foundation
- Bicycle model geometry: `turnRadius = wheelbase / sin(steeringAngle)`
- Angular velocity: `omega = speed / turnRadius`
- Lateral acceleration threshold: `lateralAccel = speed * omega` vs `maxLateralAccel = gripCoeff * gravity`

**Visual references:**
- Debug visualization showing circular arc, circle center, desired position vs actual
- Turning radius visualization decreasing with speed (speed-dependent steering)

**Inspiration/Prior art:**
- Ridge Racer - rail-based drift system using circular arcs
- Mario Kart - predictable turning with clear feedback
- Burnout - "calculate exact positions applying forces to reach them" philosophy

**Research findings:**
- Circle-based turning "creates predictable, satisfying control that's trivial to tune compared to realistic slip-angle calculations" (Research.md:18-20)
- System naturally detects drift conditions when lateral acceleration exceeds grip threshold
- Enables brake-to-drift, rail-based cornering, and speed-dependent steering

**Constraints/Requirements:**
- Pure mathematical primitive - no side effects
- Time-independent calculation
- Returns data only - systems decide how to apply
- Must handle edge cases (zero speed, zero steering, singularities)
<!-- END: SELECT/REFERENCES -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required:**
- GLM math library (exists in Layer 1)
- Math utilities: safe normalization, angle operations (exists in Layer 2)

**Enables (will become buildable):**
- Arcade Drift System (Layer 4)
- Rail-Based Cornering (Layer 4)
- Speed-Dependent Steering improvements
- Drift angle visualization
<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
Given speed, steering angle, heading, wheelbase, and dt, calculate position along circular arc and detect if lateral acceleration exceeds grip threshold.

**Why a system, not a feature:**
Provides foundation for entire category of arcade handling behaviors. Drift detection, predictable cornering, rail-based systems all emerge from this primitive. Not tied to any specific mechanic.
<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Needs proof:**
- Turn radius calculation handles singularity at zero steering angle
- Angular velocity integration is time-independent (same result at any dt)
- Lateral acceleration threshold correctly identifies grip vs slide conditions
- Circle center calculation remains stable across frame boundaries

**Known uncertainties:**
- Ackermann geometry: should inner/outer wheels have different angles? (Research suggests arcade games skip this)
- Speed floor: minimum speed threshold to prevent divide-by-zero or unrealistic behavior
- Steering angle limits: how to clamp/smooth steering input before calculation
<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: GRAYBOX/REPURPOSE -->
## Graybox - Repurposed as Debug Visualization

**Original intent:** Circle-based turning as physics system
**Problem discovered:** Doesn't fit acceleration→velocity→position integration model. Speed-dependent steering already exists and works correctly.

**Root cause of planning failure:**
1. Proposed system without understanding integration with existing physics
2. Research.md describes direct position-setting arcade model, but this project uses force integration
3. Added vehicle parameters (wheelbase, grip_coefficient, max_steering_angle) prematurely without validating system fit
4. Failed multiple architectural checks during planning (coupling violations, direct position-setting)

**Repurpose decision:** Convert to diagnostic visualization tool
- Shows theoretical turning circle geometry from current physics state
- Makes speed-dependent steering math visible
- Tuning aid for understanding parameter effects
- Zero impact on actual physics

**Cleanup required:**
Remove unused parameters from controller.h and vehicle/tuning.h:
- wheelbase
- max_steering_angle
- grip_coefficient

These were added in anticipation of this system but serve no purpose.

**New scope:**

Pure debug visualization primitive calculating turning geometry from current state:

**Input:** speed, heading, turn_input scaled by speed-dependent steering multiplier, turn_rate, dt
**Output:** Visualization geometry (arc points, center, radius, lateral accel)

**Visualization shows:**
- Circular arc vehicle is implicitly tracing (15 segments)
- Circle center point (cyan sphere)
- Turn radius line (gray)
- Lateral acceleration vector (purple, scaled by magnitude)
- Numeric overlay: speed, effective steering angle, turn radius, lateral accel

**Value:**
- Understand what speed-dependent steering actually does geometrically
- Tune turn_rate and steering_reduction_factor by seeing results
- Educational tool showing relationship between speed and turn radius
- Replaces weak cone direction visual with actual path geometry

**Complexity:** Small
<!-- END: GRAYBOX/REPURPOSE -->

---

<!-- BEGIN: GRAYBOX/REVIEW -->
## Implementation Review

**Tool:** Codex CLI
**Date:** 2025-10-24

**Question asked:**
Review the implementation plan for circle turning debug visualization. Check for architectural violations, anti-pattern violations, mathematical correctness, integration approach, and separation of concerns.

**Feedback received:**
- **High:** Wrong integration location - plan couples debug rendering into `game_world::update`, bypassing existing `debug_generation.cpp` pipeline. Violates orthogonal systems and separation of concerns.
- **High:** Wrong sign in angular velocity - plan uses `turn_input` but controller actually uses `-turn_input` when integrating heading. Would mirror all geometry incorrectly.
- **Medium:** Time-independence violation - `dt` parameter makes visualization frame-rate dependent, contradicts mathematical correctness requirement.
- **Medium:** Per-frame allocation - `std::vector<glm::vec3>` causes heap allocation every frame. Should use `std::array<glm::vec3, 15>` instead.

**Impact on implementation:**
- Extend existing `generate_car_control_primitives()` in `debug_generation.cpp` instead of adding to `game_world::update()`
- Use `-turn_input` to match controller's actual heading integration
- Remove `dt` parameter, use fixed lookahead angle/distance for time-independent visualization
- Change `std::vector<glm::vec3> arc_points` to `std::array<glm::vec3, 15> arc_points` to avoid allocations
<!-- END: GRAYBOX/REVIEW -->

---

<!-- BEGIN: GRAYBOX/IMPLEMENTATION_PLAN -->
## Implementation Plan (Revised after Codex review)

**Files to create:**

`src/vehicle/circle_turning_viz.h` and `.cpp`
- Pure geometry calculation primitive
- No side effects, no state, no allocations
- Time-independent

**Files to modify:**

`src/app/debug_generation.cpp`
- Extend existing `generate_car_control_primitives()` function
- Add circle turning visualization after cone visualization
- Uses existing debug primitive infrastructure

**Function signature:**

```cpp
struct turning_geometry {
    glm::vec3 circle_center;          // Center point of turning circle
    float turn_radius;                 // Radius in meters
    float angular_velocity;            // omega in rad/s
    float lateral_acceleration;        // m/s²
    std::array<glm::vec3, 15> arc_points; // Fixed-size arc points (no allocation)
};

turning_geometry compute_turning_geometry(
    glm::vec3 position,
    float heading_yaw,             // Current heading
    float horizontal_speed,        // Speed in m/s
    float turn_input,              // Current turn input [-1, 1]
    float turn_rate,               // Base turn rate in rad/s
    float steering_multiplier      // Speed-dependent multiplier [0, 1]
);
```

**Call structure:**

```
app::generate_debug_primitives()
  └─> generate_car_control_primitives(list, character, scheme)
      └─> if (scheme == CAR_LIKE && horizontal_speed > epsilon)
          ├─> geometry = compute_turning_geometry(...)
          └─> populate debug_list from geometry
              ├─> Arc: loop arc_points, add debug_line between consecutive points
              ├─> Center: debug_sphere at circle_center (cyan, 0.1m)
              ├─> Radius line: debug_line from center to position (gray)
              └─> Lateral accel: debug_arrow perpendicular to heading
```

**Implementation details:**

compute_turning_geometry() logic:
1. Calculate effective angular velocity: `omega = -turn_input * turn_rate * steering_multiplier` (negative matches controller)
2. Handle zero-turning case: if abs(omega) < epsilon, return straight-line geometry
3. Calculate turn radius: `radius = horizontal_speed / abs(omega)` (derived from circular motion)
4. Calculate circle center direction: perpendicular to heading, sign from omega
5. Circle center: `center = position + perpendicular_direction * radius`
6. Generate 15 arc points: fixed lookahead angle (e.g. PI/2 radians total arc)
   - For each point i: `angle = start_angle + (i / 14.0) * arc_span`
   - Rotate around center: `point = center + radius * (cos(angle), sin(angle))`
7. Calculate lateral acceleration: `lat_accel = horizontal_speed * abs(omega)`
8. Return geometry struct

Debug primitives in generate_car_control_primitives():
- Arc: cyan lines connecting arc_points
- Center: small cyan sphere (0.1m radius)
- Radius line: gray line from center to position
- Lateral accel: purple arrow, length = lat_accel * 0.5 (visual scaling)
- Optional: text overlay showing radius and lat_accel values

**Assertions:**

Preconditions:
- turn_rate >= 0 and finite
- steering_multiplier in [0, 1] and finite
- heading_yaw finite
- horizontal_speed >= 0 and finite

Postconditions:
- turn_radius >= 0 and finite (can be very large for near-zero omega)
- angular_velocity finite
- lateral_acceleration >= 0 and finite
- arc_points.size() == 15 (compile-time guaranteed by std::array)

**Edge cases:**

- Zero speed: skip visualization (no meaningful circle)
- Zero turn input: straight line visualization (very large radius, near-zero omega)
- Near-zero omega: epsilon check (1e-4 rad/s), treat as straight line
- High speed + low steering multiplier: very large radius, shallow arc

**Integration:**

- Visualization extends existing `generate_car_control_primitives()` in `debug_generation.cpp`
- No changes to controller or game_world
- Primitive in src/vehicle/ (Layer 2), only depends on GLM
- Zero runtime cost when debug visualization disabled
- No per-frame allocations (std::array is stack-allocated)
<!-- END: GRAYBOX/IMPLEMENTATION_PLAN -->

---

<!-- BEGIN: GRAYBOX/RESULTS -->
## Results

**Status:** DEFERRED - System does not fit project architecture

**Outcome:** Visualization deleted, only parameter cleanup committed

**What happened:**
1. Initial proposal: Circle-based turning as physics system
   - Problem: Doesn't integrate with acceleration→velocity→position model
   - Root cause: Research.md describes direct position-setting, but project uses force integration

2. Repurpose attempt: Circle-based turning as debug visualization
   - Implementation: Pure function computing theoretical turning circle geometry
   - Problem: Visualization showed path that vehicle never actually follows
   - Root cause: Assumes constant speed and turn input, but both constantly change

3. User testing feedback: "Impossible to tell if it correctly predicts turning path"
   - Theoretical circle assumes constants that don't exist in dynamic system
   - Speed changes due to drag/input changes
   - Turn input constantly adjusting
   - No drift/slip mechanics to validate against

**What was deleted:**
- `src/vehicle/circle_turning_viz.h/cpp` - Pure geometry calculation primitive
- Debug visualization integration in `debug_generation.cpp`
- `last_turn_input` field from controller (was only for visualization)

**What was kept:**
- Removal of unused parameters: `wheelbase`, `max_steering_angle`, `grip_coefficient`
- These were added prematurely without validating system fit
- Cleanup reduces surface area and removes dead state

**Lessons learned:**
1. **Validate integration before implementation** - "How does this fit?" must be answered in SELECT, not discovered in GRAYBOX
2. **Visualization of theoretical physics isn't useful when theory doesn't match reality** - Dynamic systems with changing inputs make constant-assumption visualizations meaningless
3. **Addition by subtraction works** - Deleting the failed experiment leaves the codebase cleaner than before
4. **Premature parametrization is technical debt** - Adding fields "for future systems" creates cleanup work when systems fail validation

**Branch disposition:** Merge parameter cleanup only

**Next:** Update CURRENT_PLAN.md to reflect deferred status
<!-- END: GRAYBOX/RESULTS -->
