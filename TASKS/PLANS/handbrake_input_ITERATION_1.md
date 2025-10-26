<!-- BEGIN: ITERATE/INTERNAL_REVIEW -->
## Internal Review

**Date:** 2025-10-26

**Findings:**

**Positive:**
- Clean composition pattern: handbrake_system composed into controller
- Semantic clarity: `handbrake.apply()` is self-documenting
- Input flow clean: Space → input_params.handbrake → handbrake_system
- Active vs reactive terminology established consistently
- Debug assertions validate preconditions
- No handbrake-specific dead state

**Issues identified:**

1. **dt parameter unused**: handbrake_system::apply() receives dt but doesn't use it
2. **Brake strength units unclear**: Comment says "dimensionless multiplier" but formula `brake_force = -velocity * brake_strength` implies units of [1/time]
3. **Tuning parameter not metadata-driven**: brake_strength is raw float, not using metadata-driven pattern from vehicle_tuning_params
4. **Assertion incomplete**: Precondition checks `>= 0.0f` but doesn't validate finiteness
<!-- END: ITERATE/INTERNAL_REVIEW -->

---

<!-- BEGIN: ITERATE/EXTERNAL_REVIEW -->
## External Review

**Tool:** Codex CLI (second run with corrected model)
**Date:** 2025-10-26

**Findings:**

**Composition Pattern:**
- Doc/implementation divergence: "Implementation Review" section states active systems should live in game_world, but "Results" and actual code kept it in controller
- Both patterns valid, but document and code must align

**Physics Integration (Critical):**
- **Time-independence violation**: Current approach adds `-velocity * brake_strength` to acceleration, making acceleration velocity-dependent. This breaks the exact exponential integrator in update_physics() which assumes constant a, k over dt
- **Unit mismatch**: velocity (m/s) * brake_strength requires brake_strength units of [1/s] to produce acceleration (m/s²), not "dimensionless"
- **Vertical contamination**: Brake force includes Y component, unintentionally affecting vertical physics (gravity/ramps)
- **Correct approach**: Treat handbrake as additional horizontal drag coefficient. Use `k_total = accel/max_speed + (handbrake_active ? brake_rate : 0)` in existing exact solution

**Dead State:**
- `contact_debug_info::from_box` - never read or written
- `contact_debug_info::vertical_penetration` - never read or written
- `controller::soft_threshold` - validated but never used
- `controller::medium_threshold` - validated but never used
- `clamp_horizontal_speed()` function - defined but never called
- `handbrake_system::apply()` dt parameter - received but unused

**Configuration:**
- brake_strength not in vehicle_tuning_params metadata system
- Cannot tune via GUI (not centralized)

**Violations:**
- Mathematical Foundation: Breaks time-independence guarantee
- Physics-First Tuning: Unit inconsistency
- Single Source of Configuration: Missing from tuning metadata
- Plan Section Divergence: Document contradicts itself on ownership
<!-- END: ITERATE/EXTERNAL_REVIEW -->

---

<!-- BEGIN: ITERATE/SCOPE -->
## Refinement Scope

**Architectural principle:** handbrake_system must handle braking entirely. No modifications to controller::update_physics().

**Priority 1: Physics Correctness (Blocking)**

Fix time-independence violation using exponential decay within handbrake_system:

1. Rename `brake_strength` → `brake_rate` (units: 1/s, drag coefficient)
2. Change from adding force to acceleration → direct velocity modification using exact exponential decay
3. Apply `horizontal_velocity *= exp(-brake_rate * dt)` within handbrake_system::apply()
4. Restrict to horizontal motion only (project to horizontal, preserve Y)
5. Update comments to reflect drag-based physics

**Priority 2: Dead State Cleanup**

Delete unused fields and functions:
1. Remove `contact_debug_info::from_box`
2. Remove `contact_debug_info::vertical_penetration`
3. Remove `controller::soft_threshold`
4. Remove `controller::medium_threshold`
5. Remove `clamp_horizontal_speed()` function

**Priority 3: Configuration Centralization**

Migrate brake_rate to metadata-driven tuning:
1. Add brake_rate to vehicle_tuning_params with metadata
2. Apply through tuning system (not raw member)
3. Surface in GUI vehicle panel

**Priority 4: Documentation Alignment**

Fix plan document divergence:
1. Update GRAYBOX/IMPLEMENTATION_PLAN section to match actual physics approach
2. Clarify that handbrake owns complete braking behavior

**Out of scope (deferred):**
- Creating vehicle_active_systems struct (no urgency, single active system)
- GUI tuning interface (polish layer)
<!-- END: ITERATE/SCOPE -->

---

<!-- BEGIN: ITERATE/REFINEMENT -->
## Refinement

**Priority 1: Physics Correctness** (COMPLETE)

Fixed time-independence violation by changing handbrake from force-based to drag-based physics:

1. **Renamed parameter**: `brake_strength` → `brake_rate` (units: 1/s)
   - Updated src/vehicle/handbrake_system.h:24
   - Clarified units and physical meaning in comments

2. **Changed physics approach**: Force addition → direct velocity modification
   - src/vehicle/handbrake_system.cpp:15-31
   - Now applies: `horizontal_velocity *= exp(-brake_rate * dt)`
   - Exact solution to drag equation: dv/dt = -k*v
   - Time-independent: identical behavior at any framerate

3. **Horizontal-only application**: Projects to horizontal, preserves vertical velocity
   - src/vehicle/handbrake_system.cpp:18 uses `math::project_to_horizontal()`
   - Writes back only X, Z components (line 26-27)
   - No contamination of vertical physics (gravity/ramps)

4. **Updated documentation**: Comments reflect exponential decay physics
   - src/vehicle/handbrake_system.h:6-15 - Header comment updated
   - src/vehicle/handbrake_system.cpp:16-17 - Implementation comment shows exact solution
   - src/vehicle/handbrake_system.h:27-32 - Function comment updated

5. **Enhanced validation**: Added finiteness check for brake_rate
   - src/vehicle/handbrake_system.cpp:10 - Precondition checks finite

**Priority 2: Dead State Cleanup** (COMPLETE)

Removed unused fields and functions:

1. **contact_debug_info cleanup**:
   - Removed `from_box` field (never read or written)
   - Removed `vertical_penetration` field (never read or written)
   - src/vehicle/controller.h:36-40

2. **Speed threshold cleanup**:
   - Removed `soft_threshold` member (validated but never used)
   - Removed `medium_threshold` member (validated but never used)
   - src/vehicle/controller.h:107 (removed lines)
   - src/vehicle/controller.cpp:42-44 (removed validation)

3. **Function cleanup**:
   - Removed `clamp_horizontal_speed()` function (defined but never called)
   - src/vehicle/controller.cpp:17-24 (removed anonymous namespace function)

**Priority 3: Configuration Centralization** (COMPLETE)

Migrated brake_rate to metadata-driven tuning system:

1. **Added to tuning_params**:
   - src/vehicle/tuning.h:42-47 - Parameter with documentation
   - src/vehicle/tuning.h:75 - Metadata (range: 0.5-10.0, units: 1/s)

2. **Validation and application**:
   - src/vehicle/tuning.cpp:23-24 - Range precondition check
   - src/vehicle/tuning.cpp:31 - Finiteness assertion
   - src/vehicle/tuning.cpp:58 - Apply to controller.handbrake.brake_rate

3. **GUI integration**:
   - src/gui/parameter_command.h:25 - Added BRAKE_RATE enum
   - src/gui/vehicle_panel.cpp:24 - Local copy for widget
   - src/gui/vehicle_panel.cpp:43-45 - Slider widget emits command
   - src/app/runtime.cpp:175-178 - Command handler updates params and applies

**Build Status**: ✓ Success (all files compiled, no errors)
<!-- END: ITERATE/REFINEMENT -->

---

<!-- BEGIN: ITERATE/PLAYTEST -->
## Playtest

**Date:** 2025-10-26

**Scenarios tested:**
1. Basic braking functionality - Space key applies smooth exponential deceleration
2. Brake rate tuning - GUI slider adjusts braking strength in real-time
3. Horizontal-only braking - Vertical physics unaffected by handbrake
4. No regressions - Movement, camera, tilt, control scheme toggle all functional

**Status:** ✓ Stable through testing
**Violations found:** None
<!-- END: ITERATE/PLAYTEST -->

---
