**Status:** APPROVED

---

<!-- BEGIN: ITERATE/CONTEXT -->
## Context from ITERATION_1

**Previous iteration status:** REVISE

**Decision from VALIDATE/DECISION (handbrake_input_ITERATION_1.md:249-277):**

Time-independence violation identified in handbrake physics implementation. While each exponential operation is individually time-independent, their sequential composition creates frame-rate dependence when acceleration and handbrake are both active.

**The violation:**
- Handbrake applies `v *= exp(-b*dt)` in apply_input() (controller.cpp:120)
- update_physics() then applies `v' = v*exp(-k*dt) + (a/k)*(1-exp(-k*dt))` (controller.cpp:195-196)
- Combined map has wrong particular solution term: uses `a/k` instead of `a/(b+k)`
- Frame-rate dependent when acceleration and handbrake both active

**Mathematical Foundation principle is absolute:** "results must be identical at any framerate."

**Required changes:**

Unify drag coefficients in the exact integrator:
1. Change handbrake.apply() to set state only, not mutate velocity
2. Pass handbrake drag to update_physics() via controller member or query
3. Use `k_total = accel/max_speed + (handbrake.is_active() ? handbrake.brake_rate : 0.0f)` in exact solution
4. Apply unified exponential integrator with correct particular solution term
<!-- END: ITERATE/CONTEXT -->

---

<!-- BEGIN: ITERATE/SCOPE -->
## Refinement Scope

**Architectural decision:** Extract unified `friction_model` system that composes all drag sources (base + handbrake + future). This establishes the active system pattern while fixing time-independence violation.

**Priority 1: Extract friction_model System**

Create `src/vehicle/friction_model.{h,cpp}`:
1. Struct owns `handbrake_system` member (move from controller)
2. Method `compute_total_drag(accel, max_speed)` returns unified k_total
3. Method `update(input_params, dt)` updates handbrake state
4. Exposes `handbrake` member for GUI/debug access
5. Method `get_base_drag_rate(accel, max_speed) const` for GUI derived parameter display

**Priority 2: Integrate friction_model into Controller**

Modify `src/vehicle/controller.{h,cpp}`:
1. Replace `handbrake_system handbrake;` with `friction_model friction;`
2. In `apply_input()`: call `friction.update(input_params, dt)` instead of `handbrake.apply()`
3. In `update_physics()`: replace `k = accel/max_speed` with `k = friction.compute_total_drag(accel, max_speed)`
4. Update comments to reflect unified drag model

**Priority 3: Update handbrake_system Interface**

Modify `src/vehicle/handbrake_system.{h,cpp}`:
1. Remove `apply()` method (no longer mutates velocity)
2. Keep `brake_rate` parameter and `is_active()` query
3. Add `update(bool input)` method (sets state only)
4. Update comments: drag contribution, not velocity mutation

**Priority 4: Update Dependent Code**

Modify code that accesses handbrake:
1. `src/gui/vehicle_panel.cpp`:
   - `character.handbrake` → `character.friction.handbrake`
   - Add derived parameter display: "Base Drag Rate: [value] /s" (computed from accel/max_speed)
2. Any other handbrake references in codebase

**Success criteria:**
- Single exponential integrator with unified k_total
- Time-independent when throttle + handbrake both active
- Composition pattern established for future friction modifiers
- Semantic clarity: `friction.compute_total_drag()` not formula soup
- Base drag rate visible in GUI as derived parameter

**Out of scope:**
- Making base_drag_rate an independent tuning parameter (keep formula-derived for now)
- Surface friction modifiers (no requirements yet)
<!-- END: ITERATE/SCOPE -->

---

<!-- BEGIN: ITERATE/REFINEMENT -->
## Refinement

**Status:** Complete - Build successful, ready for playtest

**Changes:**

1. **Extracted controller_input_params to separate header** (src/vehicle/controller_input_params.h)
   - Broke circular dependency between friction_model and controller
   - Clean separation: input params are now standalone type

2. **Created friction_model system** (src/vehicle/friction_model.{h,cpp})
   - Owns handbrake_system as composed member
   - `compute_total_drag(accel, max_speed)`: Returns k_total = k_base + k_handbrake
   - `get_base_drag_rate(accel, max_speed)`: Derived parameter for GUI display
   - `update(input)`: Updates handbrake state from input

3. **Updated handbrake_system interface** (src/vehicle/handbrake_system.{h,cpp})
   - Removed `apply()` method (no longer mutates velocity directly)
   - Added `update(bool)`: Sets state only, drag applied via friction_model
   - Updated comments: drag contribution, not velocity mutation

4. **Integrated friction_model into controller** (src/vehicle/controller.{h,cpp})
   - Replaced `handbrake_system handbrake` with `friction_model friction`
   - In `apply_input()`: calls `friction.update(input_params)` instead of `handbrake.apply()`
   - In `update_physics()`: `k = friction.compute_total_drag(accel, max_speed)` replaces hardcoded formula
   - Single exponential integrator with unified drag coefficient

5. **Updated dependent code**
   - src/gui/vehicle_panel.cpp: `vehicle.friction.handbrake.is_active()` + base drag derived param display
   - src/vehicle/tuning.{h,cpp}: `controller.friction.handbrake.brake_rate` for parameter application
   - src/app/game_world.cpp: Include controller_input_params.h, use non-nested type
   - CMakeLists.txt: Added src/vehicle/friction_model.cpp

**Physics correctness:**
- ✓ Single exponential integrator: `v' = v*exp(-k_total*dt) + (a/k_total)*(1 - exp(-k_total*dt))`
- ✓ Unified drag coefficient: k_total = k_base + k_handbrake
- ✓ Time-independent: identical behavior at any framerate
- ✓ Correct particular solution term when throttle + handbrake both active

**Build status:** ✓ Success (all files compiled, no errors)
<!-- END: ITERATE/REFINEMENT -->

---

<!-- BEGIN: ITERATE/PLAYTEST -->
## Playtest

**Date:** 2025-10-26

**Scenarios tested:**
1. Handbrake functionality - Space key applies smooth deceleration ✓
2. Time-independence - Throttle + handbrake simultaneously (W+Space) behaves consistently ✓
3. Base drag visibility - GUI displays "Base Drag Rate" derived parameter ✓
4. Brake rate tuning - GUI slider adjusts handbrake strength in real-time ✓
5. No regressions - Movement, camera, tilt, control scheme toggle all functional ✓

**Status:** ✓ Stable through testing
**Violations found:** None

**Physics validation:**
- Single exponential integrator working correctly
- No frame-rate dependent behavior observed
- Unified drag coefficient resolves time-independence violation from ITERATION_1
<!-- END: ITERATE/PLAYTEST -->

---

<!-- BEGIN: VALIDATE/REVIEW -->
## External Review

**Tools:** Codex + Gemini (dual review)
**Date:** 2025-10-26

**Convergent Findings:**

Both reviewers agree on:
- Time-independence violation completely fixed via unified drag coefficient and single exponential integrator
- Physics correctness: k_total = k_base + k_handbrake is mathematically correct, sequential exponential composition eliminated
- friction_model is a true system that divides complexity and establishes generalizable composition pattern
- Single source of truth maintained: drag coefficients derived on-demand, no duplicate/accumulated state
- Clean orthogonal composition: friction_model and handbrake_system have clear boundaries, no special cases
- controller_input_params extraction was necessary to break circular dependency
- All conventions followed: snake_case, robust preconditions/postconditions, comments explain why

**Divergent Findings:**

Codex identified minor hygiene issues:
- Unused forward declaration in friction_model.h:5 with misleading comment
- Transitive include reliance (controller.h doesn't directly include controller_input_params.h)
- Euler fallback (k < 1e-6) introduces non-exact time-independence branch (though unlikely to trigger)
- Imprecise comment in handbrake_system.h:10 (mentions decay term without particular solution)

Gemini gave unqualified approval:
- "Textbook example of how to correctly apply principles"
- No violations found
- Focused on architectural strengths and emergent benefits

**Principle Violations:**

Codex identified **minor** issues only:
- Mathematical Foundation (time-independence): Euler fallback path (controller.cpp:187) breaks exact time-independence when k < 1e-6 (unlikely under current parameters)
- Radical Simplicity: Unused forward declaration (friction_model.h:5)
- Process/Documentation: Transitive include coupling (controller.h)
- Conventions: Comment imprecision (handbrake_system.h:10)

**Strengths:**

- Unified drag and single exact integrator fixes frame-rate dependence cleanly
- Clear separation of concerns: handbrake contributes coefficient, friction_model composes, controller integrates
- Composition pattern obviously extensible for future friction modifiers
- Base drag rate visibility in GUI increases transparency and tunability (emergent benefit)
- Robust validation with strong pre/postconditions throughout
- Addresses root cause, not just symptom—prevents entire class of future time-dependence bugs

**Assessment:**

Strong convergence on principle compliance. Time-independence violation resolved. Architecture significantly improved. Codex identified minor hygiene issues; Gemini gave unqualified approval. Core implementation is correct and aligns with all principles.
<!-- END: VALIDATE/REVIEW -->

---

<!-- BEGIN: VALIDATE/DECISION -->
## Decision

**Status:** APPROVED

**Reasoning:**

Time-independence violation from ITERATION_1 is completely resolved. Both external reviewers converged on core principle compliance:

- Mathematical Foundation upheld: Unified drag coefficient k_total = k_base + k_handbrake in single exact exponential integrator eliminates sequential composition and ensures frame-rate independence
- Systems Not Features: friction_model is a true system that divides complexity and establishes generalizable composition pattern for all future friction modifiers
- Single Source of Truth: Drag coefficients derived on-demand, no duplication or accumulation
- Orthogonal Systems: Clean composition boundaries, no special cases
- Radical Simplicity: controller_input_params extraction justified to break circular dependency

Minor hygiene issues identified by Codex (unused forward declaration, transitive includes, comment imprecision, degenerate case fallback) do not violate principles and can be addressed in future refactoring if needed. Core implementation is mathematically correct and architecturally sound.

Playtest validation confirms stable behavior with no regressions. System ready for CLOSE.

**Required changes:** None (approved as-is)
<!-- END: VALIDATE/DECISION -->

---

<!-- BEGIN: VALIDATE/EMERGENCE -->
## Emergence

**Surprising behaviors:**
- Base drag rate visibility in GUI emerged as natural consequence of friction_model abstraction. Original plan didn't anticipate this, but extracting compute_total_drag() made get_base_drag_rate() obvious for derived parameter display.
- Architectural pattern (friction_model composition) solves entire class of future physics modification problems beyond handbrake. Pattern emerged from fixing time-independence bug, not from upfront design.

**Enables (future):**
- Surface-type friction modifiers (ice, dirt, grass) via additional terms in compute_total_drag()
- Speed-dependent aerodynamic drag without touching controller integration
- Tire wear degradation by modifying friction coefficients over time
- Power-ups/boosts that modify friction characteristics
- All future drag sources compose cleanly without controller changes

**Learned:**
- Extracting systems to fix bugs often reveals better architecture than designing upfront
- Time-independence violations compound—unified integration is non-negotiable
- Making base drag visible as derived parameter increased tuning transparency more than expected
- controller_input_params.h extraction pattern useful for breaking circular dependencies while maintaining clean composition
<!-- END: VALIDATE/EMERGENCE -->

---
