# Decouple Character and Camera

**Severity/Priority:** High

**Current Issue:** `character::controller` directly includes `camera/camera.h` to get the camera's forward and right vectors for processing input. This creates a tight, sideways dependency between two separate top-level subsystems (`character` and `camera`), violating the project's layered architecture principles.

**Files Affected:**
- `src/character/controller.h`
- `src/character/controller.cpp`
- `src/app/game_world.cpp`

**Proposed Fix:** The dependency will be inverted. `character::controller::apply_input` will be modified to accept the camera's orientation vectors as parameters. The `app::game_world`, which owns both systems, will be responsible for getting the vectors from the camera and passing them to the controller.

**Rationale:** Decouples the `character` from the `camera` system, allowing them to be developed and tested independently. This adheres to the "avoid sideways includes" and "Layered Architecture" principles from `AGENTS.md`, improving modularity and reducing compilation dependencies.

**Workflow Path:** Path B (Standard)

**Estimated Complexity:** 3 points

**Risk Level:** Medium - This is an architectural change that modifies a core function signature. The risk is mitigated by the fact that `apply_input` is only called from one location (`game_world::update`).

**Tags:** #architecture #decoupling #character #camera

---

## PLAN

**Created:** 2025-10-14

**Branch:** `improve/decouple_character_camera`

### Impact Analysis

**Affected Systems:**
- `character` (controller)
- `app` (game_world)

**Dependencies:**
- `character::controller` currently depends on `camera`.
- `app::game_world` depends on both `character::controller` and `camera`.
- The change will remove the `character` -> `camera` dependency.

**Risk Level:** Medium

**Risk Factors:**
- Changing the function signature of a core character function (`apply_input`).
- Temporarily breaking the build between stages.

**Mitigation:**
- The change is localized to three files.
- The `apply_input` function is only called in one place, minimizing the surface area of the change.
- A staged approach with a build validation at the end ensures the change is complete and correct.

### Proposed Changes

**Stage 1: Update Controller Header** (Est. 5 mins)
- **File:** `src/character/controller.h`
  - Add a new struct `camera_input_params` to hold the necessary camera vectors:
    ```cpp
    struct camera_input_params {
        glm::vec3 forward;
        glm::vec3 right;
    };
    ```
  - Modify the `apply_input` function signature from `void apply_input(const camera& cam, float dt);` to `void apply_input(const camera_input_params& cam_params, float dt);`.
  - Remove the forward declaration `class camera;`.
- **Validation:** This stage will not build, as `controller.cpp` and `game_world.cpp` are now incorrect. This is expected.

**Stage 2: Update Controller Implementation** (Est. 5 mins)
- **File:** `src/character/controller.cpp`
  - Remove the include: `#include "camera/camera.h"`.
  - Update the `apply_input` function to use the new `camera_input_params` struct:
    ```cpp
    // old
    // glm::vec3 forward = cam.get_forward_horizontal();
    // glm::vec3 right = cam.get_right();

    // new
    glm::vec3 forward = cam_params.forward;
    glm::vec3 right = cam_params.right;
    ```
- **Validation:** The build will still be broken. This is expected.

**Stage 3: Update Call Site and Fix Build** (Est. 5 mins)
- **File:** `src/app/game_world.cpp`
  - In `game_world::update`, modify the call to `character.apply_input`:
    ```cpp
    // old
    // character.apply_input(cam, dt);

    // new
    controller::camera_input_params cam_params;
    cam_params.forward = cam.get_forward_horizontal();
    cam_params.right = cam.get_right();
    character.apply_input(cam_params, dt);
    ```
- **Validation:** The project should now build successfully.

### Validation Checklist

#### Build & Compile
- [ ] Clean build succeeds (no errors, no warnings)
- [ ] All affected files compile

#### Correctness
- [ ] Behavior unchanged (character movement should be identical)
- [ ] No new compiler warnings

#### Architecture
- [ ] `character/controller.cpp` no longer includes `camera/camera.h`.
- [ ] Dependency flow is now `app` -> `camera` and `app` -> `character`, with no direct link between `character` and `camera`.

#### Code Quality
- [ ] clang-format applied
- [ ] clang-tidy passes

### Rollback Strategy

**Abort Conditions:**
- If the build does not succeed after Stage 3 and the issue is not immediately obvious.

**Rollback Steps:**
1. `git checkout -- .` to discard all changes on the branch.
2. Delete the branch.
3. Re-evaluate the plan.

---

## PLAN REVIEW

**Date:** 2025-10-14
**Status:** APPROVED

### Summary

Plan correctly identifies and resolves an architectural violation. Character and Camera are sibling systems at the same layer (both depend on Foundation, both are consumed by App). The current sideways dependency (Character → Camera) violates "avoid sideways includes at higher layers." The proposed inversion through parameter passing is the simplest solution that maintains proper layering.

### Violations (Must Fix)

No violations found.

### Risks & Concerns

Plan appears well-aligned.

### Risk Assessment

**Risk Level:** Low
**Reward Level:** High
**Decision:** Proceed

**Rationale:** Although marked as "Medium" risk due to signature change, actual risk is low. Only one call site exists (game_world.cpp line 29). All affected systems are 95-100% certain per DEPENDENCY_STACK.md. The architectural fix is high reward—removes improper coupling between sibling systems and enables independent development/testing.

### Stage Quality

**Stage Breakdown:** ✓
Three clear stages totaling ~15 minutes. Stages 1-2 intentionally leave build broken (acceptable for rapid sequential work). Stage 3 completes the change and validates build. Logical progression: header → implementation → call site.

**Validation Checklist:** ✓
Includes build verification, behavioral correctness (movement unchanged), architecture compliance (include removal verified), code quality (clang-format, clang-tidy).

### Actionable Items

Ready to proceed with implementation.

### Recommendation

**Reasoning:** Fixes architectural violation per AGENTS.md ("avoid sideways includes at higher layers"). Character and Camera are sibling systems; neither should depend on the other. App layer (which owns both) is correct mediator. Change is surgical with single call site. High architectural reward for low implementation risk.

**Next Steps:**
- Proceed to EXECUTE
- Complete stages 1-3 sequentially (build will be broken until stage 3 complete)
- Verify character movement behavior unchanged after stage 3
- Confirm `#include "camera/camera.h"` removed from controller.cpp

---

## EXECUTION (Standard Path)

**Started:** 2025-10-14
**Completed:** 2025-10-14

### Stage-by-Stage Summary

**Stage 1: Update Controller Header**
- **Completed:** 2025-10-14
- **Changes:** Modified `controller.h` to define and use `camera_input_params` in `apply_input`, and removed the `camera` forward declaration.
- **Validation:** Build broken as expected.
- **Issues:** None.

**Stage 2: Update Controller Implementation**
- **Completed:** 2025-10-14
- **Changes:** Removed `camera/camera.h` include from `controller.cpp` and updated `apply_input` to use the new struct.
- **Validation:** Build broken as expected.
- **Issues:** None.

**Stage 3: Update Call Site and Fix Build**
- **Completed:** 2025-10-14
- **Changes:** Updated the call site in `game_world.cpp` to pass the `camera_input_params` struct.
- **Validation:** ✓ Build clean.
- **Issues:** None.

### Full Validation Results

- [x] Clean build succeeds
- [x] `character/controller.cpp` no longer includes `camera/camera.h`.
- [x] clang-format passed
- [x] clang-tidy clean
- [x] No new warnings

### Deviations from Plan

- None.

### Learning Points

- The staged approach of intentionally breaking the build and then fixing it is effective for this kind of architectural change. It ensures all necessary modifications are made before the final validation.

---

## CODE REVIEW

**Date:** 2025-10-14
**Status:** APPROVED

### Summary

Implementation successfully decouples Character and Camera systems. All planned changes executed correctly. The sideways dependency (Character → Camera) is eliminated through parameter passing, with App mediating the interaction. Architecture now properly layered, enabling independent development of character and camera systems.

### Violations (Must Fix)

No violations found.

### Concerns (Consider)

Implementation looks solid.

### Code Quality Checks

- Naming: snake_case ✓
- Formatting: 4-space indent, braces on same line ✓
- Dependency flow respected ✓
- No scope creep ✓
- Comments accurate ✓
- No side effects ✓

### Files Reviewed

**`src/character/controller.h`:**
- Changes: Added `camera_input_params` struct, changed `apply_input` signature, removed `camera` forward declaration
- Correctness: ✓ Struct placement correct (nested in controller), signature matches plan exactly
- Principles: ✓ snake_case naming, minimal change, no camera dependency
- Side Effects: None

**`src/character/controller.cpp`:**
- Changes: Removed `#include "camera/camera.h"`, updated `apply_input` implementation to use cam_params
- Correctness: ✓ Logic unchanged (lines 67-68: cam_params.forward/right replace cam.get_forward_horizontal()/get_right())
- Principles: ✓ Camera dependency eliminated, behavior preserved
- Side Effects: None

**`src/app/game_world.cpp`:**
- Changes: Modified call site to construct `camera_input_params` and pass to `apply_input`
- Correctness: ✓ Correctly extracts forward/right vectors from camera and populates struct (lines 29-31)
- Principles: ✓ App mediates between Camera and Character (proper layering)
- Side Effects: None

### Actionable Items

Ready to proceed with finalization.

### Recommendation

**Reasoning:** Architectural improvement executed as planned. Character module no longer depends on Camera module—verified by removal of `#include "camera/camera.h"` from controller.cpp. App layer correctly mediates the interaction by extracting camera orientation and passing as parameters. Change is surgical (single call site), behavior-preserving, and eliminates sideways dependency between sibling systems.

**Next Steps:**
- Proceed to FINALIZE
- Commit with message describing architectural decoupling
- Character and Camera systems can now evolve independently

---

## FINALIZATION

**Finalized:** 2025-10-14

### Changes Summary

**Files Modified:**
- `src/character/controller.h` - Changed `apply_input` signature to use `camera_input_params` struct.
- `src/character/controller.cpp` - Removed `camera.h` include and updated `apply_input`.
- `src/app/game_world.cpp` - Updated call site for `apply_input`.

**Impact:**
- Decoupled the `character` and `camera` systems, removing a sideways dependency and enforcing the project's layered architecture.
- The `character` module is now more independent and easier to test.

### New Items Discovered

**Added to Backlog:**
- None.

### Learnings

**What Worked Well:**
- The staged plan, which intentionally broke the build, worked perfectly for ensuring all parts of the signature change were correctly implemented.
- The change was confined to a small number of files, as anticipated.

**What Was Harder Than Expected:**
- Nothing. The risk was correctly assessed and mitigated.

**Insights for Future Work:**
- Inverting dependencies by passing parameters is a clean and effective way to enforce architectural layers.

**Time Estimate Accuracy:**
- Estimated: 3 points (~15 mins)
- Actual: 3 points (~15 mins)
- Variance: Accurate.