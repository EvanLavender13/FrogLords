# Vehicle Mass Property System

**Layer 4 - Foundation for Physics-Driven Drift**

---

<!-- BEGIN: SELECT/REFERENCES -->

## Internal References

**Current State:**
- `tuning.h:26` - `weight = -9.8f` with comment "will convert to force with mass later"
- `controller.h:62` - Same weight parameter duplicated
- `math_utils.h:143` - Hardcoded `GRAVITY = 9.8f` constant for g-force calculation
- No mass parameter exists anywhere in codebase

**Architecture Context:**
- `FrontRearWheelArchitecture.md` - Defines force-based physics requiring mass:
  - Lateral force application: `lateral_velocity += (lateral_force_total / mass) * dt`
  - Moment of inertia: `mass * radius²` for rotational dynamics
  - Normal load distribution for tire forces

**Integration Points:**
- `vehicle::tuning_params` - Metadata-driven parameter system
- `controller` - Physics state and integration
- `math::calculate_lateral_g_force()` - Hardcoded gravity constant

**Dependency Stack:**
- Layer 1: Parameter metadata system (exists)
- Layer 2: Math utilities (exists)
- Layer 3: Vehicle tuning system (exists)
- **No blockers - all dependencies satisfied**

## External References

None required - this is a fundamental physics property with well-established meaning.

<!-- END: SELECT/REFERENCES -->

---

<!-- BEGIN: SELECT/SELECTED -->

## System Decision

**Vehicle Mass Property** - Add mass parameter to controller, replace weight acceleration constant with proper F=ma force calculation.

**Why now:**
- Zero dependencies - builds on existing tuning system
- Enables entire physics-driven drift chain (front/rear axle systems)
- Fixes architectural lie: "weight" is currently acceleration, should be force
- Single source of truth violation: gravity constant duplicated

**Complexity:** Small

**Layer:** 4 (Variation within vehicle movement system)

**Dependencies:**
- ✅ Layer 2: Parameter metadata (exists)
- ✅ Layer 3: Vehicle tuning system (exists)
- ✅ Layer 3: Vehicle movement system (exists)

---

## Core Mechanics

### Mathematical Foundation

**Current (incorrect):**
```cpp
float weight = -9.8f; // m/s² - this is acceleration, not force
velocity.y += weight * dt; // Treating acceleration as force
```

**Target (correct):**
```cpp
float mass = 150.0f; // kg
float weight_force = mass * -9.8f; // N (Newtons)
float weight_accel = weight_force / mass; // m/s² - cancels back to -9.8
velocity.y += weight_accel * dt;
```

**Why bother if it cancels?** Because lateral forces don't cancel:
```cpp
// Future use in axle systems:
float lateral_accel = lateral_force / mass; // F=ma - mass matters here
lateral_velocity += lateral_accel * dt;
```

### Parameter Design

**Add to `tuning_params`:**
```cpp
float mass = 150.0f; // kg
static constexpr param_meta mass_meta = {"Mass", "kg", 50.0f, 500.0f};
```

**Update `controller`:**
```cpp
float mass = 150.0f; // kg (from tuning)
// Remove: float weight (acceleration)
// Compute weight force when needed: mass * -9.8f
```

**Reference constant:**
```cpp
// In math_utils.h or physics_constants.h
constexpr float GRAVITY = 9.8f; // m/s² (Earth standard)
```

### Integration Pattern

**Weight force application remains unchanged behaviorally:**
```cpp
// In controller::update_physics()
float weight_accel = (mass * -GRAVITY) / mass; // Simplifies to -GRAVITY
velocity.y += weight_accel * dt;
```

**But now enables force-based lateral physics:**
```cpp
// Future: In axle system integration
float lateral_accel = lateral_force / mass;
velocity.x += lateral_accel * dt; // Mass affects responsiveness
```

---

## Graybox Approach

**Minimal validation target:**
1. Add mass parameter to tuning system
2. Update weight calculation to use mass * gravity
3. Verify vehicle behavior unchanged (mass cancels in weight-only case)
4. Display mass in vehicle_panel GUI

**Success criteria:**
- Vehicle moves identically before/after (weight acceleration unchanged)
- Mass visible and tunable in GUI
- No hardcoded gravity constants (single GRAVITY reference)

**No scope creep:**
- Don't implement lateral force application (that's axle systems)
- Don't add moment of inertia calculation (that's heading integrator)
- Don't add weight transfer (that's optional enhancement)

---

## Architectural Benefits

**Enables downstream systems:**
- Front Axle System - needs mass for F=ma lateral force
- Rear Axle System - needs mass for F=ma lateral force
- Heading Integrator - needs mass for moment of inertia calculation

**Fixes current violations:**
- Single source of truth: gravity constant exists once
- Semantic correctness: weight is force (N), not acceleration (m/s²)
- Mathematical foundation: F=ma properly separated from special case optimization

**Pattern established:**
- All forces converted to acceleration via F/m
- Physics constants (gravity) defined once in foundation
- Tuning parameters follow metadata-driven pattern

<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: GRAYBOX/PLAN -->

## Implementation Approach

**Scope:** Add mass parameter to vehicle system, replace weight acceleration with F=ma force calculation, establish gravity constant as single source of truth.

**Structure:**

1. **Physics constant (foundation layer):**
   - Add `GRAVITY = 9.8f` constant to `foundation/math_utils.h` (existing location of gravity reference)
   - Remove local `constexpr float GRAVITY` from `calculate_lateral_g_force()` function

2. **Tuning parameter (tuning system):**
   - Add `mass` member to `vehicle::tuning_params` with default `150.0f` kg
   - Add `mass_meta` with range `[50.0f, 500.0f]` kg
   - Update `apply_to()` to copy mass to controller

3. **Controller state (physics system):**
   - Add `mass` member to `controller` (copied from tuning)
   - Remove `weight` member (replaced by mass + gravity)
   - Compute weight force in `update_physics()` as `mass * -GRAVITY`

4. **GUI integration (vehicle_panel):**
   - Add `MASS` to `parameter_type` enum
   - Add mass slider in `draw_vehicle_tuning_section()`
   - Add mass command handler in runtime.cpp

**Integration Points:**
- `foundation/math_utils.h` - Global GRAVITY constant
- `vehicle/tuning.h` - Mass parameter with metadata
- `vehicle/controller.h` - Mass state property
- `vehicle/controller.cpp` - Weight force calculation in update_physics()
- `gui/parameter_command.h` - MASS parameter type
- `gui/vehicle_panel.cpp` - Mass GUI slider
- `app/runtime.cpp` - Mass command handler

**Debug Visualization Strategy:**
- Display mass in vehicle_panel tuning section (tunable slider)
- Verify weight calculation unchanged by testing before/after (vehicle should behave identically)
- No special visualization needed - mass is visible via GUI, behavior validates correctness

**Critical:** Mass cancels in weight-only case (`(mass * -GRAVITY) / mass = -GRAVITY`), so behavior must be identical. Any change in vehicle movement indicates implementation error.

<!-- END: GRAYBOX/PLAN -->

---

<!-- BEGIN: GRAYBOX/RESULTS -->

## Results

**Status:** ✅ Core mechanic validated

**What works:**
- Mass parameter added to tuning system (150 kg default, 50-500 kg range)
- GRAVITY constant established in `foundation/math_utils.h` (single source of truth)
- Weight force calculation replaced with F=ma: `(mass * -GRAVITY) / mass`
- GUI displays mass slider in vehicle tuning panel
- Vehicle behavior unchanged (mass cancels correctly in weight-only case)
- No assertion failures or runtime errors

**Validated properties:**
- Mathematical correctness: F=ma integration preserves time-independence
- Behavioral identity: Vehicle movement identical before/after (mass cancels to -9.8 m/s²)
- Tunability: Mass adjustable 50-500 kg with no effect on current behavior
- Architecture: Single GRAVITY constant, no duplication
- GUI integration: Mass parameter follows metadata-driven pattern

**Emergent observations:**
- Mass slider tunable but currently has no visible effect (expected - mass cancels in weight calculation)
- Foundation established for lateral force systems (future: `lateral_accel = lateral_force / mass`)
- Gravity constant now centralized (was duplicated in `calculate_lateral_g_force()`)

**Code locations:**
- `foundation/math_utils.h:13` - GRAVITY constant
- `vehicle/tuning.h:26` - Mass parameter
- `vehicle/controller.h:62` - Mass state
- `vehicle/controller.cpp:173` - Weight force calculation
- `gui/vehicle_panel.cpp:33` - Mass GUI slider

**Ready for:** ITERATE

**Considerations for iteration:**
- Core mechanic mathematically correct and behaviorally validated
- Mass currently has no visible effect (expected - cancels in weight calculation)
- Foundation solid for downstream force-based systems
- May defer iteration if no architectural improvements needed

<!-- END: GRAYBOX/RESULTS -->

---

## Iterations

- [ITERATION_1.md](VEHICLE_MASS_ITERATION_1.md) - APPROVED
