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

- [ ] Core functional in graybox
- [ ] Mathematical correctness proven
- [ ] Debug visualization shows behavior
- [ ] Emergent behaviors appear
- [ ] Principles upheld
<!-- END: SELECT/SUCCESS -->
