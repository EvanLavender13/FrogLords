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
