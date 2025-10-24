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
