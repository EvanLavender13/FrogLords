# Handbrake System Refactor

**Move handbrake from friction_model composition to controller-level state**

---

<!-- BEGIN: SELECT/REFERENCES -->

## Internal References

**Existing Implementation:**
- `src/vehicle/handbrake_system.{h,cpp}` - Current system (Lines 13-32)
- `src/vehicle/friction_model.{h,cpp}` - Current owner (Line 22)
- `src/vehicle/controller.{h,cpp}` - Future owner (Line 99)

**Architecture Documentation:**
- `NOTES/CarMechanics/FrontRearWheelArchitecture.md` - Target architecture (Lines 82-102)
- `NOTES/CarMechanics/PhysicsFirst_Research.md` - Physics integration patterns

**Current Behavior:**
- Handbrake adds `brake_rate` to total drag coefficient
- Integrated via exponential drag: `v *= exp(-k_total*dt)`
- Updated from `controller_input_params` via `friction_model::update()`

**Integration Points:**
- `controller::update_physics()` - Calls `friction.compute_total_drag()` (Line 179)
- `friction_model::compute_total_drag()` - Queries handbrake state
- `friction_model::update()` - Updates handbrake from input

## External References

**Input State Management Patterns (GameDev.net, Stack Exchange):**
- Pure input state holders as separate systems
- Decoupling input from physics/behavior systems
- Input contexts where consumers query state

**Composition Architecture (Game Programming Patterns):**
- Component-based design promotes runtime flexibility
- Systems query components rather than own them
- "Composition over inheritance takes you a long way with games"

**Handbrake Physics Implementations:**
- Simple: Reduce tire friction when handbrake applied
- Torque-based: Apply brake torque to rear axle
- Parametric: Configure axle distribution (0=rear, 1=front)
- Our model: Drag coefficient contribution (longitudinal braking)

**Racing Game Architecture:**
- Front/rear axle separation is standard
- Handbrake typically affects rear axle grip reduction
- Brake bias parameters for distribution control

<!-- END: SELECT/REFERENCES -->

---

<!-- BEGIN: SELECT/SELECTED -->

## System Decision

**Handbrake System Refactor** - Move handbrake from `friction_model` composition to `controller`-level state, enabling multi-system consumption for future front/rear axle physics.

## Complexity Assessment

**Size:** Small
- Single ownership migration
- 2-3 call site updates
- Behavior preservation verification

**Integration:** Minimal
- `controller` owns handbrake_system
- `friction_model::compute_total_drag()` accepts handbrake state as parameter
- Future: `rear_axle_system` will query same state

**Mathematical Certainty:** High
- No physics changes
- Pure refactor: behavior must be identical
- Existing exponential drag integration unchanged

**Validation:** Straightforward
- Manual testing: handbrake braking force unchanged
- Visual: velocity trail shows identical deceleration
- GUI: base_drag_rate display unchanged

## Dependencies

**Layer 1 (CORE):**
- Runtime loop for update cycle
- Input system for handbrake button state

**Layer 2 (PRIMITIVES):**
- None (no new math required)

**Layer 3 (SYSTEMS):**
- Vehicle movement system (controller)
- Friction model (consumer of handbrake state)

## Core Mechanics

**Before (Current):**
```
friction_model owns handbrake_system
friction_model::update() → handbrake.update(input.handbrake)
friction_model::compute_total_drag() → handbrake.is_active()
```

**After (Refactored):**
```
controller owns handbrake_system
controller::apply_input() → handbrake.update(input.handbrake)
friction_model::compute_total_drag(handbrake_active) → accept as parameter
```

**Future Enablement:**
```
rear_axle_system::compute_lateral_force(handbrake_active)
  → reduces cornering_stiffness when true
```

## Mathematical Foundation

**No physics changes.** Handbrake continues to contribute brake_rate to total drag coefficient:

```
k_total = k_base + (handbrake_active ? brake_rate : 0)
v_new = v_old * exp(-k_total * dt) + (a/k_total) * (1 - exp(-k_total * dt))
```

**Refactor only changes ownership, not integration.**

## Graybox Approach

**Phase 1: Move Ownership**
- Add `handbrake_system handbrake` to `controller`
- Update handbrake in `controller::apply_input()`
- Remove handbrake from `friction_model` composition

**Phase 2: Update Call Sites**
- Change `friction_model::compute_total_drag()` signature
- Accept `bool handbrake_active` parameter
- Pass `controller.handbrake.is_active()` from call site

**Phase 3: Verify Behavior**
- Test handbrake braking force identical
- Check velocity trail deceleration matches
- Confirm GUI base_drag_rate display unchanged

**Phase 4: Update Metadata (if needed)**
- Ensure `brake_rate` parameter still visible in GUI
- Tuning interface should remain unchanged

## Verification Criteria

**Behavioral Preservation:**
- Handbrake braking force unchanged
- Same deceleration rate when engaged
- No change to non-handbrake movement

**Interface Consistency:**
- GUI shows brake_rate parameter
- Tuning sliders work identically
- Debug visualization unchanged

**Architectural Cleanliness:**
- Controller owns handbrake state (single source)
- Friction model accepts state as parameter
- Future rear_axle can query same state

<!-- END: SELECT/SELECTED -->

---

## Next Steps

1. Create branch `system/handbrake-refactor`
2. Proceed to GRAYBOX: implement minimal viable refactor
3. Verify behavior preservation through manual testing
4. Update DEPENDENCY_STACK.md if needed (handbrake ownership change)

---

**Build from certainty. Refactor for clarity. Enable emergence.**
