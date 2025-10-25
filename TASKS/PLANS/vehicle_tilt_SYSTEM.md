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

**Architectural validation:**
Current implementation uses arcade conversion factors (multipliers) for immediate gameplay feedback. Future enhancements can coexist or replace:
- Realistic per-wheel suspension simulation (spring compression at contact points)
- Damage-based body tilt (persistent lean from collision damage)
- Terrain-reactive body roll (crossing slopes, bumps)
All remain visual-only, all derive from physics state, none affect collision or movement.

**Physics-first trade-off:**
Arcade tilt multipliers are non-physical conversion factors chosen for visual feedback over realism. Real vehicles don't rotate bodies proportional to g-forces (suspension compresses vertically instead). Trade-off documented per PHYSICS_FIRST_TUNING.md: gameplay demands exaggerated visual weight transfer for motion readability. Spring damping remains physically accurate even though the mechanism being damped is non-physical.
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

<!-- BEGIN: GRAYBOX/SCOPE -->
## Graybox

**What will be built:**
Minimal vehicle tilt system demonstrating visual-only weight transfer. New vehicle_visual_systems struct stores previous_velocity and computes lean and pitch angles from controller state (lateral g-force and derived longitudinal acceleration). Debug box changed to vehicle proportions and rendered with tilt transforms applied.

**Complexity:** Small
<!-- END: GRAYBOX/SCOPE -->

---

<!-- BEGIN: GRAYBOX/IMPLEMENTATION_PLAN -->
## Implementation Plan

**Files to modify:**

1. `src/vehicle/vehicle_visual_systems.h` (NEW - vehicle visual layer)
   - Struct with lean/pitch tuning parameters
   - Store `glm::vec3 previous_velocity` for acceleration derivation
   - `void update(const controller& ctrl, float dt)` - compute tilt angles
   - `glm::mat4 get_visual_transform(const controller& ctrl) const` - compose transform
   - Members: lean_angle, pitch_angle, previous_velocity, lean_multiplier, pitch_multiplier

2. `src/vehicle/vehicle_visual_systems.cpp` (NEW - implementation)
   - update:
     - Calculate lateral_g from controller.calculate_lateral_g_force()
     - Derive accel: (ctrl.velocity - previous_velocity) / dt
     - Project to forward direction for longitudinal component
     - Compute angles: lean_angle = lateral_g * lean_multiplier, pitch_angle = accel_forward * pitch_multiplier
     - Store previous_velocity = ctrl.velocity
   - get_visual_transform:
     - translate(position) → rotate(heading_yaw, UP) → rotate(lean, FORWARD) → rotate(pitch, RIGHT)

3. `src/app/game_world.h` (add vehicle visuals to state)
   - Add `vehicle_visual_systems vehicle_visuals` member

4. `src/app/game_world.cpp` (update vehicle visuals each frame)
   - Call `vehicle_visuals.update(character, dt)` after character.update

5. `src/app/debug_generation.h` (new function signature)
   - Add `void generate_vehicle_body_primitives(debug::debug_primitive_list&, const controller&, const vehicle_visual_systems&)`

6. `src/app/debug_generation.cpp` (vehicle debug viz)
   - Implement generate_vehicle_body_primitives using vehicle proportions (1.2, 0.4, 0.6)
   - Get transform from vehicle_visuals.get_visual_transform
   - Transform vertices and emit debug lines (same pattern as character body)

7. `src/app/runtime.cpp` (switch to vehicle viz)
   - Replace generate_character_body_primitives call with generate_vehicle_body_primitives
   - Pass vehicle_visuals from game_world

**Call structure:**
```
runtime::render_frame
  ├─ game_world.update(dt)
  │   ├─ character.update(world, dt)
  │   └─ vehicle_visuals.update(character, dt)  [NEW]
  │       ├─ character.calculate_lateral_g_force()
  │       ├─ derive accel = (velocity - previous_velocity) / dt
  │       └─ compute lean/pitch angles, store previous_velocity
  └─ debug generation
      └─ generate_vehicle_body_primitives(character, vehicle_visuals)  [NEW]
          └─ vehicle_visuals.get_visual_transform()
              ├─ translate(position)
              ├─ rotate(heading_yaw, UP)
              ├─ rotate(lean_angle, FORWARD)  [lean left/right]
              └─ rotate(pitch_angle, RIGHT)   [pitch forward/back]
```

**Debug data flow:**
1. controller.update: physics state changes (velocity, heading, etc.)
2. vehicle_visuals.update: read controller state → calculate tilt angles → store previous_velocity
3. debug_generation: call get_visual_transform → apply to box vertices → emit lines

**Integration points:**
- game_world::update: add vehicle_visuals.update call after character.update
- runtime::render_frame: replace character body viz with vehicle body viz
- Controller unchanged (zero physics modification)
- Character reactive systems unchanged (legacy code, not used for vehicle)
<!-- END: GRAYBOX/IMPLEMENTATION_PLAN -->

---

<!-- BEGIN: GRAYBOX/IMPLEMENTATION_LESSON -->
## Implementation Lesson

**Error caught during planning:**
Initial plan attempted to add `previous_velocity` to controller (physics layer) instead of vehicle_visual_systems (visual layer). This would have violated separation of concerns - physics state should not be polluted with visual-only tracking data.

**Correct approach:**
Visual systems own all visual-only state. Controller provides read-only state via const accessors. Visual systems derive what they need and store their own history.

**Pattern:**
- Physics layer: owns position, velocity, acceleration (source of truth)
- Visual layer: reads physics state, stores own tracking data (previous_velocity), derives visual transforms
- Zero data flow from visual → physics
<!-- END: GRAYBOX/IMPLEMENTATION_LESSON -->

---

<!-- BEGIN: GRAYBOX/REVIEW -->
## Implementation Review

**Tool:** Codex CLI
**Date:** 2025-10-25

**Question asked:**
Validate the implementation plan. Check: (1) separation of concerns between physics and visual layers, (2) unidirectional data flow, (3) violations of principles or conventions, (4) transform composition order for coordinate system.

**Feedback received:**
- Physics/visual separation maintained: vehicle_visual_systems only reads from controller (const), stores own history
- Unidirectional flow correct: physics → visual → debug, no back-pressure
- Aligns with principles (single source of truth, physics drives animation)
- Transform order correct: translate → yaw → roll(forward) → pitch(right) matches GLM column-major
- Minor note: Use math::UP constant, derive forward/right from yaw or use explicit glm::vec3

**Impact on implementation:**
- Enforce const controller& in update signature
- ~~Use yaw_to_forward/yaw_to_right helpers for rotation axes~~ **WRONG CHOICE**
- Follow character_reactive_systems pattern for consistency

**Critical lesson learned:**
Codex suggested "derive forward/right from yaw OR use explicit glm::vec3". Initial implementation chose derived axes, which caused tilt to apply in world space instead of local space. This created spurious roll when driving straight. **Correct solution:** Use explicit local axes (glm::vec3(0,0,1) for forward, glm::vec3(1,0,0) for right) to ensure tilt rotations happen in vehicle's local reference frame AFTER yaw rotation is applied.

**Pattern:** When applying rotations after a previous rotation, use local-space axes (unit vectors), not world-space derived axes.
<!-- END: GRAYBOX/REVIEW -->
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
- Vehicle box (0.6m wide × 0.4m tall × 1.2m long) renders correctly
- Smooth spring-damped lean during cornering
- Smooth spring-damped pitch during acceleration/deceleration
- Tilt in local reference frame (no spurious roll when straight)
- Orientation from velocity (not heading) - box doesn't rotate when stationary
- Zero physics modification (collision sphere unchanged)

**Doesn't:**
- N/A - core mechanics validated

**Surprises:**
- Initial implementation had THREE major bugs caught during testing:
  1. Used heading_yaw instead of velocity-based orientation (box rotated when stationary)
  2. Used input_direction instead of velocity for orientation (accumulated incorrectly)
  3. Used world-space derived axes instead of local-space axes (spurious roll)
- All bugs traced to misunderstanding reference frames and data flow
- Codex review warned about derived vs explicit axes - chose wrong path initially
- Spring damping essential - instant response felt "snappy" and jarring

**Next:**
- ITERATE phase - tune multipliers, explore visual enhancements
<!-- END: GRAYBOX/RESULTS -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [x] Debug box changed to vehicle proportions
- [x] Box leans correctly in corners (toward turn center)
- [x] Box pitches during acceleration/braking
- [x] Tilt magnitude tunable via parameters
- [x] Zero physics modification (collision sphere unchanged)
- [x] Visual feedback enhances motion readability
<!-- END: SELECT/SUCCESS -->

---

## Iterations

- [ITERATION_1.md](vehicle_tilt_ITERATION_1.md) - REVISE
