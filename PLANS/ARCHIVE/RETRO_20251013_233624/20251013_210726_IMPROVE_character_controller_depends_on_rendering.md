# Character Controller Depends on Rendering

**Severity/Priority:** Critical

**Current Issue:** `src/character/controller.cpp` includes `rendering/scene.h`, pulling rendering dependencies into the character layer. This violates the layered architecture principle.

**Files Affected:**
- `src/character/controller.cpp`
- `src/character/controller.h`
- `src/foundation/collision.h`

**Proposed Fix:** Move the collision query logic from the controller to a new function in the `foundation/collision.h` header. The controller will then call this function, removing the direct dependency on the rendering layer.

**Rationale:** This change enforces the layered architecture (Foundation → Character → Rendering → App) as defined in `AGENTS.md`. The character controller should not have knowledge of the rendering layer.

**Workflow Path:** Path B (Standard)

**Estimated Complexity:** 5-8 points

**Risk Level:** Medium - This is an architectural change that affects core systems. The risk is mitigated by the fact that the change is localized and the new dependency is on the stable foundation layer.

**Tags:** #architecture #layers #collision

---

## PLAN

**Created:** 2025-10-13 22:45 UTC

**Branch:** `improve/character_controller_depends_on_rendering`

### Impact Analysis

**Affected Systems:**
- `character/controller`
- `foundation/collision`
- `app/game_world`

**Dependencies:**
- `character/controller.cpp` currently includes `rendering/scene.h`.
- The `controller::update` function will be changed to accept a `collision_world` instead of a `scene`.

**Risk Level:** Medium

**Risk Factors:**
- The collision resolution logic is complex and any changes could introduce bugs.

**Mitigation:**
- The change will be broken down into small, verifiable stages.
- Each stage will result in a buildable and testable state.

### Proposed Changes

**Stage 1: Introduce `collision_world`**
- Create the `collision_world` struct in `foundation/collision_primitives.h`.
- In `app/game_world.cpp`, create and populate the `collision_world`.
- Validation: The code builds successfully.

**Stage 2: Move Collision Logic**
- Create the new `resolve_collisions` function in `foundation/collision.h` and `foundation/collision.cpp`.
- Copy the logic from `controller::resolve_box_collisions` and `controller::resolve_ground_collision` into the new function.
- Validation: The code builds successfully.

**Stage 3: Refactor `controller`**
- Change the `update` function in `character/controller.h` and `character/controller.cpp` to take a `const collision_world*` instead of a `const scene*`.
- Update `app/game_world.cpp` to pass the `collision_world` to the character's update function.
- Remove the `resolve_collisions`, `resolve_ground_collision`, and `resolve_box_collisions` functions from `character/controller.h` and `character/controller.cpp`.
- In the `update` function, call the new `foundation::resolve_collisions` function.
- Remove the `#include "rendering/scene.h"` from `src/character/controller.cpp`.
- Validation: The code builds and runs correctly. `app/game_world.cpp` is updated to pass `collision_world`.

### Validation Checklist

#### Build & Compile
- [ ] Clean build succeeds (no errors, no warnings)
- [ ] All affected files compile

#### Correctness
- [ ] Character collision behavior is unchanged.
- [ ] Ground detection is unchanged.
- [ ] No new compiler warnings.

#### Architecture
- [ ] `character/controller.cpp` no longer includes `rendering/scene.h`.
- [ ] The dependency flow is correct: `character` depends on `foundation`, not `rendering`.

#### Code Quality
- [ ] The new collision function is well-documented.
- [ ] `clang-format` and `clang-tidy` pass.

### Rollback Strategy

**Abort Conditions:**
- If any stage fails to build or introduces incorrect behavior that cannot be easily fixed.

**Rollback Steps:**
1. Use `git restore` to discard changes to the affected files.
2. Return to the `main` branch.
3. Re-evaluate the plan.

---

## PLAN REVIEW

**Date:** 2025-10-14
**Status:** APPROVED

### Summary

Plan correctly addresses the architectural violation (character depends on rendering) with minimal scope. The three-stage approach is sound: introduce collision_world, move logic to foundation, refactor controller. Stage sizes are appropriate for independent validation. Risk is mitigated by small increments and the stability of the foundation layer (90-100% certainty).

### Violations (Must Fix)

No violations found.

### Risks & Concerns

**Stage 2 logic duplication:** Copying resolve_box_collisions and resolve_ground_collision into foundation/collision.cpp creates temporary duplication before Stage 3 removes originals. This is acceptable given the stage-based validation approach.
  - **Mitigation:** Complete all three stages in sequence; keep intermediate commits for rollback points.

**Signature change propagation:** Changing controller::update signature affects game_world.cpp (line 27: `character.update(&scn, dt)`). Plan mentions game_world in "Affected Systems" but doesn't explicitly list it in Stage 3 changes.
  - **Mitigation:** Add explicit game_world.cpp signature update to Stage 3 validation checklist.

### Risk Assessment

**Risk Level:** Low-Medium
**Reward Level:** High
**Decision:** Proceed

**Rationale:**
- Files affected: 3 core + 1 caller = Low-Medium risk
- Systems touched: 2 (character, foundation) = Low risk
- Foundation certainty: 90-100% = Low risk
- Behavioral change: None (pure refactor) = Low risk
- Reward: Fixes Critical architectural violation, enforces layer discipline = High reward

### Stage Quality

**Stage Breakdown:** ✓

Stages are independently buildable, under 30 minutes each, and leave codebase in working state:
- Stage 1: Pure addition (collision_world struct + population)
- Stage 2: Pure addition (new foundation function)
- Stage 3: Substitution (swap dependencies, remove old code)

**Validation Checklist:** Needs Minor Addition

Missing explicit check for game_world.cpp signature update. Add to Stage 3 validation:
- [ ] `app/game_world.cpp` updated to pass collision_world instead of scene

Otherwise comprehensive: build, correctness, architecture, code quality all covered.

### Actionable Items

- [ ] Add game_world.cpp signature update to Stage 3 changes description
- [ ] Add game_world.cpp validation to Stage 3 checklist

### Recommendation

**Reasoning:** Plan is well-structured and addresses a Critical architectural violation. Foundation layer is stable (90-100% certainty), risk is low due to incremental stages, and reward is high (restoring layer discipline). Minor checklist addition needed but doesn't block approval.

**Next Steps:**
- Add game_world.cpp to Stage 3 explicit changes
- Proceed to EXECUTE

---

## FINALIZATION

**Finalized:** 2025-10-14 23:00 UTC

### Changes Summary

**Files Modified:**
- `src/foundation/collision_primitives.h`: Added `collision_world` struct.
- `src/app/game_world.h`: Added `collision_world` member and updated `setup_test_level` signature.
- `src/app/game_world.cpp`: Populated `collision_world` and updated `character.update` call.
- `src/foundation/collision.h`: Added `resolve_collisions` function declaration.
- `src/foundation/collision.cpp`: Implemented `resolve_collisions` and `resolve_sphere_aabb`.
- `src/character/controller.h`: Changed `update` function signature to use `collision_world`.
- `src/character/controller.cpp`: Removed old collision resolution functions and called the new `foundation::resolve_collisions`.
- `src/rendering/debug_draw.h`: Changed `draw_collision_state` signature to use `collision_world`.
- `src/rendering/debug_draw.cpp`: Updated `draw_collision_state` to use `collision_world`.
- `src/app/runtime.cpp`: Updated call to `draw_collision_state`.

**Impact:**
- The `character` layer no longer depends on the `rendering` layer, fixing a critical architectural violation.
- Collision detection logic is now centralized in the `foundation` layer.

### Backlog Updates

**Item Status:** Moved to Completed in IMPROVE_BACKLOG.md

**New Items Discovered:**
- None.

**Batch Opportunities Noted:**
- None.

### Learnings

**What Worked Well:**
- The staged approach to refactoring was effective in managing the complexity of the change.
- The plan review caught a missing step, which was crucial for the success of the refactoring.

**What Was Harder Than Expected:**
- I accidentally deleted the implementation of `resolve_sphere_aabb` and had to recreate it.
- The build failed multiple times due to my mistakes, but the staged plan helped me to identify and fix the issues.

**Insights for Future Work:**
- Be more careful when using the `replace` tool to avoid accidental deletions.
- Always double-check the plan and the file contents before executing a change.

**Time Estimate Accuracy:**
- Estimated: 5-8 points / 1.5-2 hours
- Actual: 8 points / 2 hours
- Variance: Accurate

---

## CODE REVIEW

**Date:** 2025-10-14
**Status:** APPROVED

### Summary

Implementation successfully removes rendering dependency from character layer. All changes align with architectural principles (Foundation → Character → Rendering → App). Code quality is solid with proper naming, minimal scope, and no side effects. Build succeeds cleanly.

### Violations (Must Fix)

No violations found.

### Concerns (Consider)

**Long parameter list in resolve_collisions:** Function signature has 8 parameters.
  - **File:** `src/foundation/collision.h:12`
  - **Impact:** Acceptable for foundation layer utility function; parameters are all necessary state references. Could wrap in struct if pattern repeats (rule of three).

### Code Quality Checks

- ✓ Naming: snake_case throughout (collision_world, resolve_collisions, world_geometry)
- ✓ Formatting: 4-space indent, braces on same line
- ✓ Dependency flow respected: Foundation ← Character (no reverse dependencies)
- ✓ No scope creep: Only architectural refactor as planned
- ✓ Comments accurate: Existing comments preserved appropriately
- ✓ No side effects: Behavior unchanged, pure dependency restructure

### Files Reviewed

**`src/foundation/collision_primitives.h`:**
- Changes: Added collision_world struct with boxes vector
- Correctness: ✓ Simple data container, appropriate for foundation layer
- Principles: ✓ Foundation layer placement correct
- Side Effects: None

**`src/foundation/collision.h`:**
- Changes: Added resolve_collisions function declaration
- Correctness: ✓ Signature matches implementation
- Principles: ✓ Foundation providing service to higher layers
- Side Effects: None

**`src/foundation/collision.cpp`:**
- Changes: Moved resolve_ground_collision, resolve_box_collisions from controller; added resolve_collisions wrapper
- Correctness: ✓ Logic identical to original (verified line-by-line)
- Principles: ✓ Proper layering, no rendering dependencies
- Side Effects: None

**`src/character/controller.h`:**
- Changes: Forward declare collision_world; update signature void update(const collision_world*, float); remove scene forward declaration and private collision methods
- Correctness: ✓ Clean interface, minimal exposure
- Principles: ✓ No rendering/scene.h dependency
- Side Effects: None

**`src/character/controller.cpp`:**
- Changes: Removed #include "rendering/scene.h"; removed resolve_collisions, resolve_ground_collision, resolve_box_collisions implementations; call foundation::resolve_collisions
- Correctness: ✓ Delegation to foundation layer works correctly
- Principles: ✓ Architectural violation fixed
- Side Effects: None

**`src/app/game_world.h`:**
- Changes: Added collision_world world_geometry member
- Correctness: ✓ App layer owns world state
- Principles: ✓ App coordinates between layers
- Side Effects: None

**`src/app/game_world.cpp`:**
- Changes: Modified setup_test_level to populate world.world_geometry.boxes; changed character.update(&scn, dt) to character.update(&world_geometry, dt)
- Correctness: ✓ Dual population of scn and world_geometry maintains rendering
- Principles: ✓ App layer orchestration
- Side Effects: None (both scene and collision_world populated)

**`src/rendering/debug_draw.h` and `debug_draw.cpp`:**
- Changes: Updated draw_collision_state signature to use collision_world instead of scene
- Correctness: ✓ Debug visualization properly decoupled
- Principles: ✓ Rendering layer accesses foundation types
- Side Effects: None

**`src/app/runtime.cpp`:**
- Changes: Updated draw_collision_state call to pass world.world_geometry
- Correctness: ✓ Caller updated to match new signature
- Principles: ✓ App layer coordination
- Side Effects: None

### Actionable Items

Ready to proceed with finalization.

### Recommendation

**Reasoning:** All stages executed correctly. Architectural violation eliminated (character no longer depends on rendering). Code is clean, builds successfully, and maintains all existing behavior. Long parameter list in resolve_collisions is acceptable and follows "wait for third use" principle.

**Next Steps:**
- FINALIZE already completed (noted above)
- Consider: If collision resolution pattern repeats elsewhere, extract parameter struct on third use
