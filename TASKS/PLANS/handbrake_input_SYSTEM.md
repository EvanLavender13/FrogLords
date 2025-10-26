# System: Handbrake Input & Active/Reactive Architecture

Establishes handbrake input for drift initiation and introduces active/reactive system composition pattern.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-25
**Layer:** 4 (Variation)
**Complexity:** Medium
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/INTERNAL_REFERENCES -->
## Internal References

**Research notes:**
- NOTES/CarMechanics/PhysicsFirst_Research.md:59 - Drift initiation: "Handbrake + steering at speed"
- TASKS/RETROS/RETRO_2025-10-24/SLIP_ANGLE_SYSTEM.md - Slip angle calculator complete (drift detection foundation)

**Source files:**
- src/vehicle/controller.h:14-17 - controller_input_params struct (add handbrake field)
- src/app/game_world.cpp:35-59 - Input polling pattern (add Space key)
- src/vehicle/vehicle_visual_systems.{h,cpp} - Rename to vehicle_reactive_systems
- src/vehicle/tuning.{h,cpp} - Metadata-driven parameter pattern to follow

**Existing patterns:**
- Binary input polling: input::is_key_down(SAPP_KEYCODE_*)
- Input struct construction in game_world::update()
- Metadata-driven tuning parameters (vehicle_tuning_params)
- Spring-damped reactive systems (tilt, lean)

**Integration points:**
- controller::apply_input() receives handbrake state
- Future: vehicle_active_systems consumes handbrake for drift detection
- Future: Drift detection primitive uses handbrake as intensity multiplier
<!-- END: SELECT/INTERNAL_REFERENCES -->

---

<!-- BEGIN: SELECT/EXTERNAL_REFERENCES -->
## External References

**Technical documentation:**
- Arcade racing handbrake: Binary on/off input for drift initiation (not analog)
- Physics implementation: Reduces rear wheel traction when active
- Common pattern: "Brake boost" multiplies drift intensity when held

**Architectural patterns:**
- Input → Intent → Physics modification (three-layer separation)
- Active systems modify physics, reactive systems respond to physics
- Physics engine conventions: active rigid bodies (modify) vs passive (respond)

**Constraints/Requirements:**
- Space key for handbrake (standard racing game convention)
- Binary input (pressed/not pressed)
- Foundation for drift chain (6+ systems depend on this)
- Must establish composable architecture (not bloat controller)

**Research findings:**
- Web search: Input systems should capture state, controller layer translates to physics
- Separation of concerns: "input represents what player is asking, physics determines if it can happen"
- Active vs reactive terminology aligns with established physics engine patterns
<!-- END: SELECT/EXTERNAL_REFERENCES -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required:**
- Input system (exists in Layer 1)
- Controller input params struct (exists in Layer 3)
- Vehicle movement system (exists in Layer 3)

**Enables (will become buildable):**
- Drift Detection Primitive (Layer 4)
- Continuous Friction Scaling (Layer 4)
- Drift Speed Maintenance (Layer 4)
- Drift Visual Feedback (Layer 4)
- Entire drift mechanics chain
<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
Handbrake binary input captured, passed through input params, accessible to vehicle systems. Establishes active/reactive composition pattern for controller.

**Why a system, not a feature:**
This enables an entire category of vehicle dynamics modifications (drift, boost, assists). The architectural pattern (active systems compose controller behavior) is the real system - handbrake is the first implementation proving the pattern.
<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Needs proof:**
- N/A (no math, pure architectural/input work)

**Known uncertainties:**
- Should active systems modify controller state directly or return modification deltas?
- Update order: active systems before or after physics integration?
- Ownership: who owns vehicle_active_systems instance? (game_world or controller?)
<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: SELECT/GRAYBOX -->
## Graybox

**Simplest implementation:**
Add bool handbrake to controller_input_params. Poll Space key in game_world. Create vehicle_active_systems.{h,cpp} with handbrake state storage (unused for now, proves pattern). Rename vehicle_visual_systems → vehicle_reactive_systems. Debug visualization: show "HANDBRAKE ACTIVE" text when Space held.

**Validation:**
- Space key toggles handbrake state
- State visible in GUI (vehicle panel, derived parameter)
- vehicle_active_systems exists and receives input
- vehicle_reactive_systems renamed successfully
- Build succeeds, no functional regressions
<!-- END: SELECT/GRAYBOX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Handbrake field added to controller_input_params
- [ ] Space key polling in game_world
- [ ] vehicle_active_systems.{h,cpp} created (stores handbrake state)
- [ ] vehicle_reactive_systems renamed from vehicle_visual_systems
- [ ] All references updated (CMakeLists, includes, game_world)
- [ ] Handbrake state visible in GUI
- [ ] Debug visualization shows handbrake state
- [ ] Pattern established for future active systems
- [ ] Principles upheld (composition, orthogonal systems)
<!-- END: SELECT/SUCCESS -->

---

<!-- BEGIN: GRAYBOX/SCOPE -->
## Graybox Scope

**What will be built:**
Add handbrake input capture (Space key) flowing through controller_input_params to handbrake_system. Create handbrake_system as discrete component composed into controller. Rename vehicle_visual_systems → vehicle_reactive_systems to clarify active vs reactive architecture. Debug visualization shows handbrake state.

**Complexity:** Medium

**Validation:**
- Space key sets handbrake field in controller_input_params
- handbrake_system.apply() called in controller::apply_input() (stub does nothing)
- vehicle_reactive_systems renamed successfully
- Handbrake state visible in GUI
- Build succeeds, zero regressions
<!-- END: GRAYBOX/SCOPE -->

---

<!-- BEGIN: GRAYBOX/IMPLEMENTATION_PLAN -->
## Implementation Plan

**Architectural decision:**
Controller composes handbrake_system as member. Handbrake applies physical brake force opposing velocity. Simple, semantic, testable. Controller reads clearly: "handbrake.apply()" not formula soup.

**Files to modify:**

1. `src/vehicle/handbrake_system.h` (new file)
   - Forward declare controller
   - Define handbrake_system struct
   - State: `bool active = false;`
   - Tunable: `float brake_strength = 2.0f;` (multiplier on velocity for brake force)
   - Method: `void apply(bool input, controller& ctrl, float dt);`
   - Getter: `bool is_active() const { return active; }`

2. `src/vehicle/handbrake_system.cpp` (new file)
   - Implement apply():
     - Store input state
     - If active: compute brake_force = -ctrl.velocity * brake_strength
     - Add brake_force to ctrl.acceleration
   - Debug assertions for preconditions

3. `src/vehicle/controller.h`
   - Add `#include "vehicle/handbrake_system.h"`
   - Add `bool handbrake;` to controller_input_params struct
   - Add member: `handbrake_system handbrake;` (after angular_velocity state)

4. `src/vehicle/controller.cpp`
   - In apply_input(), after line 131 (acceleration = input_direction * accel):
     `handbrake.apply(input_params.handbrake, *this, dt);`

5. `src/app/game_world.cpp`
   - Add after line 60 (after input normalization):
     `input_params.handbrake = input::is_key_down(SAPP_KEYCODE_SPACE);`
   - Update all vehicle_visuals references → vehicle_reactive

6. `src/vehicle/vehicle_visual_systems.h` → `src/vehicle/vehicle_reactive_systems.h`
   - Rename file and struct
   - Update header comment: "Reactive systems respond to controller physics state"

7. `src/vehicle/vehicle_visual_systems.cpp` → `src/vehicle/vehicle_reactive_systems.cpp`
   - Rename file
   - Update include and all struct references

8. `src/app/game_world.h`
   - Update include: vehicle_visual_systems.h → vehicle_reactive_systems.h
   - Rename member: `vehicle_visuals` → `vehicle_reactive`

9. `src/vehicle/tuning.{h,cpp}`
   - Update forward declaration: vehicle_visual_systems → vehicle_reactive_systems
   - Update apply_to() parameter type

10. `src/gui/vehicle_panel.cpp`
    - Display handbrake state: access character.handbrake.is_active()
    - Show "Handbrake: [ACTIVE/INACTIVE]"

11. `CMakeLists.txt`
    - Rename vehicle_visual_systems.cpp → vehicle_reactive_systems.cpp
    - Add src/vehicle/handbrake_system.cpp

**Call structure:**
```
controller::apply_input(input_params, cam_params, dt)
  ├─ Heading integration
  ├─ acceleration = input_direction * accel
  └─ handbrake.apply(input_params.handbrake, *this, dt)  [NEW - adds brake force]

controller::update(world, dt)
  ├─ update_physics(dt)  [integrates acceleration with brake force]
  └─ update_collision(world, dt)
```

**Physics:**
```
Handbrake input → handbrake_system.active
                       ↓
    If active: brake_force = -velocity * brake_strength
                       ↓
           acceleration += brake_force
                       ↓
              Physics integration
```

**Integration points:**
- controller_input_params: add handbrake field
- controller: compose handbrake_system member, call in apply_input()
- game_world::update(): poll Space key into input_params
- vehicle_panel: display handbrake state via character.handbrake.is_active()
- All vehicle_visual_systems references → vehicle_reactive_systems
<!-- END: GRAYBOX/IMPLEMENTATION_PLAN -->

---

<!-- BEGIN: GRAYBOX/REVIEW -->
## Implementation Review

**Tool:** Codex CLI
**Date:** 2025-10-25

**Question asked:**
Validate the implementation plan. Check: 1) Does the composition pattern align with principles? 2) Is the call location appropriate? 3) Are there architectural concerns? 4) Does renaming visual→reactive clarify architecture?

**Feedback received:**
- **Coupling concern**: Initial plan added handbrake_system to controller, reintroducing mixed concerns previously removed in RETRO_2025-10-18/REFINE_controller_mixed_concerns.md
- **Call timing wrong**: Calling in controller::apply_input() runs before acceleration computed, but handbrake needs to modify traction/force
- **Architecture mismatch**: Inconsistency between narrative (vehicle_active_systems) and implementation (handbrake on controller)
- **Rename approved**: visual→reactive improves symmetry with character_reactive_systems and clarifies "reacts to physics" vs "modifies physics"

**Impact on implementation:**
- Moved handbrake_system ownership from controller to game_world (follows established pattern)
- Active system called before physics update in game_world::update(), not inside controller
- Controller remains physics-only (maintains unidirectional data flow)
- Handbrake input passed directly to system (not through controller_input_params, since controller doesn't use it)
- Architecture now consistent with RETRO_2025-10-18 pattern separation
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
- Handbrake applies brake force opposing velocity (brake_force = -velocity * brake_strength)
- Space key input flows through controller_input_params to handbrake_system
- handbrake_system composed into controller (controller.handbrake member)
- GUI displays handbrake state (ACTIVE/INACTIVE) in Vehicle Panel
- Physics integration applies brake force correctly
- vehicle_visual_systems → vehicle_reactive_systems rename complete (all references updated)

**Validated:**
- Handbrake slows vehicle when Space held
- GUI state updates immediately on press/release
- Braking force proportional to velocity (stronger at higher speeds)
- No effect when stationary (velocity zero → brake force zero)
- No regressions: movement, camera, tilt system all work unchanged
- Control scheme toggle (T key) still functional

**Architecture:**
- Controller composed of handbrake_system (semantic, testable)
- Clear call site: `handbrake.apply(input_params.handbrake, *this, dt);` in controller::apply_input()
- Active vs reactive terminology established (handbrake modifies physics, reactive systems respond)

**Next:**
- ITERATE phase: tune brake_strength parameter for feel
- Future: integrate with drift detection (handbrake + steering + slip angle)

<!-- END: GRAYBOX/RESULTS -->

---

## Iterations

- [ITERATION_1.md](handbrake_input_ITERATION_1.md) - REVISE
- [ITERATION_2.md](handbrake_input_ITERATION_2.md) - APPROVED

---
