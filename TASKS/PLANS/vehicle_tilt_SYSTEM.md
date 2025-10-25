# System: Vehicle Tilt

Visual weight transfer through model tilting based on lateral acceleration and forward/backward pitch.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-24
**Layer:** 4 (Variation)
**Complexity:** Small
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/INTERNAL_REFERENCES -->
## Internal References

**Research notes:**
- NOTES/CarMechanics/PhysicsFirst_Research.md:170-184 (Visual-Only Vehicle Dynamics: Tilt System)

**Source files:**
- src/foundation/math_utils.h:117-145 (calculate_lateral_g_force primitive)
- src/vehicle/controller.h:80-83 (calculate_lateral_g_force wrapper)
- src/vehicle/controller.cpp:80-83 (implementation)
- src/app/debug_generation.cpp:124-150 (character body visualization, transform application)
- src/character/character_reactive_systems.h (visual transform pattern)
- src/character/character_reactive_systems.cpp (get_visual_transform implementation)

**Existing patterns:**
- character_reactive_systems demonstrates visual-only transform pattern (reference only, legacy code)
- Visual transforms compose position, rotation, offset without modifying physics state
- Debug visualization applies transforms to wireframe meshes (debug_generation.cpp:130)

**Integration points:**
- NEW: vehicle visual system (separate from legacy character code)
- controller provides lateral g-force and acceleration data
- Debug visualization consumes vehicle visual transform
<!-- END: SELECT/INTERNAL_REFERENCES -->

---

<!-- BEGIN: SELECT/EXTERNAL_REFERENCES -->
## External References

**Constraints/Requirements:**
- Change debug box from tall (0.4, 0.8, 0.3) to long vehicle shape
- Box orientation must follow heading (already exists via transform)
- Tilt system applied on top of existing heading orientation
<!-- END: SELECT/EXTERNAL_REFERENCES -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required:**
- Lateral G-Force Calculator (Layer 2 - complete)
- Vehicle Movement System (Layer 3 - complete)
- Reactive visual transform pattern (Layer 3 - complete)

**Enables (will become buildable):**
- Enhanced visual feedback for drift system
- More readable vehicle dynamics
- Foundation for suspension animation
<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
Visual model rotation based on lateral g-force (lean in corners) and longitudinal acceleration (pitch during speed changes). Applied to visual transform only, zero physics modification.

**Why a system, not a feature:**
Enables category of behavior: weight transfer visualization extends to suspension animation, damage model tilting, terrain-based lean. Generic "derive visual rotation from acceleration" pattern.
<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Needs proof:**
- Lean angle proportional to lateral g-force produces visually accurate cornering
- Pitch angle from acceleration/deceleration doesn't conflict with ground contact visualization
- Tilt composition with heading rotation produces expected visual (no gimbal lock)

**Known uncertainties:**
- Optimal lean multiplier (how many degrees per g)
- Pitch multiplier for acceleration
- Whether to use spring-damping for smooth transitions or direct mapping
<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: SELECT/GRAYBOX -->
## Graybox

**Simplest implementation:**
- Change debug box dimensions to long vehicle shape (e.g., 1.2, 0.4, 0.6)
- Lateral tilt: lean_angle = lateral_g_force * lean_multiplier
- Longitudinal pitch: derive effective_accel = (velocity - prev_velocity) / dt, pitch_angle = forward_component * pitch_multiplier
- NEW vehicle visual system stores previous_velocity, calculates tilt transform from controller state
- Apply transform in debug_generation (separate from character system)
- No smoothing initially (validate instant response first)

**Validation:**
- Drive in circles: box leans toward turn center
- Apply forward input: box pitches back (acceleration)
- Release input: box pitches forward (drag deceleration)
- Verify tilt magnitude feels natural (iterate multipliers)
- Confirm no physics behavior changes
<!-- END: SELECT/GRAYBOX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Debug box changed to vehicle proportions
- [ ] Box leans correctly in corners (toward turn center)
- [ ] Box pitches during acceleration/braking
- [ ] Tilt magnitude tunable via parameters
- [ ] Zero physics modification (collision sphere unchanged)
- [ ] Visual feedback enhances motion readability
<!-- END: SELECT/SUCCESS -->
