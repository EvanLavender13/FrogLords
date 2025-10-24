# System: Slip Angle Calculator

Pure mathematical primitive measuring angle between velocity vector and forward direction. Enables drift detection, tire force modeling, and vehicle dynamics analysis.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-24
**Layer:** 2 (Primitives)
**Complexity:** Small
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/INTERNAL_REFERENCES -->
## Internal References

**Research notes:**
- NOTES/CarMechanics/PhysicsFirst_Research.md:75-83 - Drift detection via slip angle formula

**Source files:**
- src/vehicle/controller.h:28 - Vehicle velocity vector (source data)
- src/vehicle/controller.h:106 - Vehicle heading_yaw (orientation)
- src/foundation/math_utils.h - Established patterns for pure math primitives

**Existing patterns:**
- Pure mathematical functions (project_to_horizontal, yaw_to_forward)
- Vector decomposition and angle calculations
- Namespace math for utility functions

**Integration points:**
- Vehicle controller provides velocity and heading
- Future: Individual tire positions and orientations
- Future: Drift detection system consumes slip angle
<!-- END: SELECT/INTERNAL_REFERENCES -->

---

<!-- BEGIN: SELECT/EXTERNAL_REFERENCES -->
## External References

**Technical documentation:**
- PhysicsFirst_Research.md documents standard formula: `slip_angle = atan2(lateral_velocity, forward_velocity)`
- Criterion Games (Burnout) and Need for Speed use this for drift detection

**Constraints/Requirements:**
- Must work for whole-vehicle calculations (current)
- Must support future individual tire simulation (per-wheel slip angles)
- Pure function: no dependencies on vehicle state structure
<!-- END: SELECT/EXTERNAL_REFERENCES -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required:**
- GLM (vec3, atan2)
- math::project_to_horizontal (exists in Layer 2)

**Enables (will become buildable):**
- Physics-Based Drift System (Layer 4)
- Drift Detection (Layer 4)
- Slip Angle Visualization (Layer 5)
- Individual Tire Simulation (future)
<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
Calculate angle between movement direction and facing direction using velocity vector and forward vector as inputs. Returns signed angle in radians.

**Why a system, not a feature:**
This is a foundational measurement primitive. Like "distance" or "dot product", it's a mathematical building block that enables entire categories of vehicle dynamics systems - drift mechanics, tire force models, stability control, and behavioral analysis.
<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Needs proof:**
- Slip angle sign convention matches physics literature (positive = sliding right, negative = sliding left)
- Handles zero-velocity edge case without division by zero
- Result stays bounded in [-π/2, π/2] for typical vehicle motion

**Known uncertainties:**
- Optimal coordinate frame: vehicle-local or world-space calculation?
- Should result be wrapped to [-π, π] or returned raw from atan2?
<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: SELECT/GRAYBOX -->
## Graybox

**Simplest implementation:**
Function in math_utils.h taking velocity vector and forward direction vector, returns slip angle. Debug visualization: draw arc on speed ring showing angle offset between velocity arrow and heading arrow.

**Validation:**
- Zero velocity returns zero slip angle
- Moving straight ahead (velocity parallel to forward) returns zero
- Moving 90° sideways returns ±π/2
- Sign convention matches expected lateral direction
<!-- END: SELECT/GRAYBOX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Pure function implemented in foundation layer
- [ ] Zero-velocity edge case handled
- [ ] Sign convention validated against test cases
- [ ] Debug visualization shows slip angle arc
- [ ] Works for vehicle controller integration
- [ ] Design supports future per-tire usage
<!-- END: SELECT/SUCCESS -->

---

<!-- BEGIN: GRAYBOX/SCOPE -->
## Graybox

**What will be built:**
Pure mathematical function `math::calculate_slip_angle(velocity, forward)` in `math_utils.h`. Takes velocity vector and forward direction vector, returns signed angle in radians. Handles zero-velocity edge case. Debug visualization adds slip angle arc to existing speed ring, showing offset between velocity arrow and heading arrow.

**Validation:**
- Moving straight ahead: slip angle ≈ 0°
- Sliding right (velocity right of heading): positive angle
- Sliding left (velocity left of heading): negative angle
- Stationary: returns 0.0 (no slip)
- 90° sideways motion right: returns ≈ +π/2
- 90° sideways motion left: returns ≈ -π/2
- Backing up: returns ≈ ±π

**Complexity:** Small
<!-- END: GRAYBOX/SCOPE -->

---

<!-- BEGIN: GRAYBOX/IMPLEMENTATION_PLAN -->
## Implementation Plan

**Files to modify:**
- `src/foundation/math_utils.h` - Add `calculate_slip_angle()` function
- `src/app/debug_generation.cpp` - Add slip angle visualization to `generate_car_control_primitives()`

**Call structure:**
```
generate_car_control_primitives()
  └─> math::calculate_slip_angle(horizontal_velocity, forward)
      └─> Returns signed angle (radians)
  └─> Draw velocity direction arrow (cyan) showing actual movement
      └─> Visual comparison with heading arrow (yellow) shows slip
```

**Debug data flow:**
1. Extract horizontal velocity from `character.velocity` (project to XZ plane)
2. Compute forward direction from `character.heading_yaw` (using `math::yaw_to_forward`)
3. Call `math::calculate_slip_angle(horizontal_velocity, forward_direction)`
4. Visualize with velocity direction arrow:
   - Start: character position
   - End: position + normalized(horizontal_velocity) * current_speed
   - Color: Cyan (distinct from yellow heading, green orientation)
   - Shows actual movement direction vs yellow heading arrow
   - Slip angle = visual angle between yellow and cyan arrows
   - Only draw when speed > 0.05 m/s

**Integration points:**
- `generate_car_control_primitives()` in `debug_generation.cpp:246` - Add velocity direction arrow after steering cone
- New function `math::calculate_slip_angle()` in `math_utils.h:75` - Pure mathematical primitive following existing patterns

**Mathematical approach:**
```cpp
// Inputs: horizontal_velocity (already projected to XZ), forward (unit vector)
// Compute right vector: cross(UP, forward), normalized
glm::vec3 right = glm::normalize(glm::cross(math::UP, forward));
float forward_speed = glm::dot(horizontal_velocity, forward);
float lateral_speed = glm::dot(horizontal_velocity, right);
slip_angle = atan2(lateral_speed, forward_speed);
```

**Edge cases:**
- Zero velocity: returns 0.0 (no slip)
- Backing up: atan2 returns values in [-π, π]; reversing produces ±π angles
- Non-unit forward: asserted in debug builds

**Range:** [-π, π] from atan2, not wrapped. Caller interprets based on context.

**Sign convention:**
- Positive: velocity points right of forward (right = cross(UP, forward))
- Negative: velocity points left of forward
<!-- END: GRAYBOX/IMPLEMENTATION_PLAN -->

---

<!-- BEGIN: GRAYBOX/REVIEW -->
## Implementation Review

**Tool:** Codex CLI
**Date:** 2025-10-24

**Question asked:**
Review the implementation plan for the slip angle calculator system. Validate: (1) Does the mathematical approach follow physics-first principles? (2) Are there any edge cases not handled? (3) Does the coordinate frame choice make sense? (4) Is the debug visualization appropriate? (5) Are there any violations of coding conventions or anti-patterns? (6) Should the result be wrapped to a specific range?

**Feedback received:**
- Sign convention mismatch: Original validation stated "drifting right → positive" but contradicted the math formula
- Horizontal projection inconsistency: Example code used unprojected velocity despite claiming to project
- Backing up edge case: atan2 returns ±π when reversing, contradicting claim of [-π/2, π/2] bounds
- Missing validation: Foundation primitives should assert preconditions (unit-length forward, epsilon checks)
- Arc primitive doesn't exist: Debug visualization assumed unavailable primitive
- Range handling: Need explicit decision on [-π, π] vs clamping

**Impact on implementation:**
- Fixed sign convention: positive = velocity right of heading, negative = velocity left
- Clarified inputs must be horizontal (already projected)
- Documented full [-π, π] range including backing up case
- Changed visualization from arc to velocity direction arrow (cyan) vs heading arrow (yellow)
- Added edge case documentation: zero velocity, backing up, non-unit forward
- Will add debug assertions for validation in implementation
<!-- END: GRAYBOX/REVIEW -->

---

<!-- BEGIN: GRAYBOX/RESULTS -->
## Results

**Status:**
- [x] Core functional
- [x] Build successful
- [x] GUI integration working
- [x] Ready for iteration

**Works:**
- Pure math primitive `math::calculate_slip_angle()` implemented with precondition assertions
- Vehicle method `controller::calculate_slip_angle()` provides clean interface
- GUI displays slip angle as derived parameter in Vehicle State panel
- Zero-velocity edge case handled (returns 0.0)
- Sign convention validated: positive = right slide, negative = left slide
- Full [-π, π] range supported

**Doesn't:**
- N/A - all planned functionality working

**Surprises:**
- Initial visualization attempt duplicated existing velocity arrow (corrected)
- Metadata-driven GUI pattern cleaner than world-space text overlay
- Three-layer architecture emerged: primitive → system method → GUI (clean separation)

**Next:**
- ITERATE phase: Add drift detection threshold system
- Future: Individual tire slip angles for advanced vehicle dynamics
<!-- END: GRAYBOX/RESULTS -->

---

## Iterations

- [SLIP_ANGLE_ITERATION_1.md](SLIP_ANGLE_ITERATION_1.md) - REVISE
- [SLIP_ANGLE_ITERATION_2.md](SLIP_ANGLE_ITERATION_2.md) - Ready for VALIDATE
