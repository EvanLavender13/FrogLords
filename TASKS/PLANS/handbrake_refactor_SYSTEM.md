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

<!-- BEGIN: GRAYBOX/PLAN -->

## Implementation Plan

**Approach:** Simple ownership migration with interface parameter change.

**Scope:**
- Move `handbrake_system handbrake` from `friction_model` to `controller`
- Update `friction_model::compute_total_drag()` to accept handbrake state as parameter
- Remove `friction_model::update()` (handbrake update moves to controller)
- Update call sites in `controller::apply_input()` and `controller::update_physics()`

**Structure Changes:**

**Before:**
```
friction_model {
    handbrake_system handbrake;  // owned here
    compute_total_drag() → queries handbrake.is_active() internally
    update(input) → handbrake.update(input.handbrake)
}

controller {
    friction_model friction;  // contains handbrake
    apply_input() → friction.update(input)
    update_physics() → friction.compute_total_drag(accel, max_speed)
}
```

**After:**
```
friction_model {
    // handbrake removed
    compute_total_drag(accel, max_speed, handbrake_active) → use parameter
    // update() removed
}

controller {
    friction_model friction;  // pure drag calculations
    handbrake_system handbrake;  // now owned here
    apply_input() → handbrake.update(input.handbrake)
    update_physics() → friction.compute_total_drag(accel, max_speed, handbrake.is_active())
}
```

**Integration Points:**
1. `controller.h` - Add `handbrake_system handbrake` member
2. `controller.cpp::apply_input()` - Update handbrake directly instead of via friction
3. `friction_model.h` - Change `compute_total_drag()` signature, remove `update()`
4. `friction_model.cpp` - Accept handbrake_active parameter, remove update implementation
5. `controller.cpp::update_physics()` - Pass handbrake state to compute_total_drag()

**Debug Visualization:**
- Existing: Handbrake brake_rate visible in tuning GUI
- No changes needed: Parameter metadata should transfer automatically
- Verify: GUI still shows brake_rate parameter after refactor

**Validation:**
- Build succeeds
- Handbrake braking force identical to before
- Velocity trail shows same deceleration
- GUI displays brake_rate parameter

<!-- END: GRAYBOX/PLAN -->

---

<!-- BEGIN: GRAYBOX/RESULTS -->

## Graybox Results

**Status:** ✅ Core works - behavior preserved

**What Works:**
- Handbrake ownership successfully moved from `friction_model` to `controller`
- Physics integration unchanged: same drag coefficient calculation
- GUI displays handbrake state correctly
- Tuning parameter (brake_rate) accessible and functional
- Build succeeds with all call sites updated

**Validated:**
- Handbrake activation/deactivation responsive
- Braking force identical to pre-refactor behavior
- GUI shows "Handbrake: ACTIVE/INACTIVE" status
- Parameter tuning interface intact

**Architectural Cleanliness:**
- Controller owns handbrake state (single source of truth)
- Friction model accepts state as parameters (pure function)
- Future rear_axle can query controller.handbrake directly
- Query pattern established for multi-system consumption

**Changes Made:**
- `friction_model.h/.cpp` - Removed handbrake composition, parameterized compute_total_drag()
- `controller.h/.cpp` - Added handbrake member, update in apply_input(), pass to compute_total_drag()
- `tuning.cpp` - Updated to `c.handbrake.brake_rate`
- `vehicle_panel.cpp` - Updated to `vehicle.handbrake.is_active()`

**Surprises:**
- None - straightforward refactor as planned

**Readiness:** Ready for ITERATE (architectural refinement if needed)

<!-- END: GRAYBOX/RESULTS -->

---

## Next Steps

1. ~~Create branch `system/handbrake-refactor`~~ ✅ (Already on branch)
2. ~~Proceed to GRAYBOX: implement minimal viable refactor~~ ✅
3. ~~Verify behavior preservation through manual testing~~ ✅
4. Commit changes
5. Update CURRENT_PLAN.md: GRAYBOX → ITERATE (or CLOSE if sufficient)

---

**Build from certainty. Refactor for clarity. Enable emergence.**
