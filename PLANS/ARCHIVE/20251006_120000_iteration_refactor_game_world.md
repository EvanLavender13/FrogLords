# Iteration Plan: Refactor Game World Separation

### 1. Feature Overview

*   **Description:** Extract game-specific simulation state (scene, character, locomotion, orientation) from `app_runtime` into a dedicated `game_world` struct. Move hardcoded test level geometry into a dedicated setup function. This isolates the main application loop from simulation concerns and improves testability.
*   **Core Principle Alignment:** Clarity over cleverness—separates lifecycle concerns (app) from domain logic (game). Supports iteration by making the test level easy to swap. Maintains dependency flow (Foundation → Character → Rendering → App).

### 2. Design Rationale

*   **Problem/Goal:** `app_runtime` currently mixes application lifecycle (Sokol init, frame loop, event handling) with game simulation state (character, scene, camera) and hardcoded test geometry. This makes it harder to reason about responsibilities and prevents easy level swapping or testing alternate scenarios.
*   **Hypothesis:** Extracting `game_world` will clarify `app_runtime`'s role as a thin host for the simulation. The test level setup becomes a replaceable function. Changes to gameplay logic won't touch lifecycle code. Future multi-world or level-loading features become simpler.

### 3. Dependencies & Uncertainty

*   **Placement in Stack:** This is a horizontal refactor within the App layer (top of stack). Does not change behavior or introduce new dependencies. All existing systems remain unchanged.
*   **Direct Dependencies:** Requires character, camera, scene, rendering systems (all stable, 90–95% certain).
*   **Dependents:** GUI panels, debug rendering, future level/scenario systems will interact with `game_world` instead of reaching into `app_runtime`.
*   **Certainty Score:** 95%. This is a structural refactor with no new mechanics or unknowns. The pattern (separate host from simulation) is proven. Risk is minimal: mistakes are compile-time errors, not runtime bugs.

### 4. Implementation & Testing Loop

*   **Graybox Scope (The "Stupid Simple Core"):**
    *   Create `src/app/game_world.h/cpp` with a `game_world` struct containing:
        *   `camera cam`
        *   `scene scn`
        *   `controller character`
        *   `orientation_system orientation`
        *   `locomotion_system locomotion`
        *   `character::tuning_params character_params`
        *   `character::skeleton t_pose_skeleton`
        *   Animation state: `wheel_spin_angle`, `skeleton_animation_time`
    *   Add `game_world` methods:
        *   `void init()` — initialize camera, character, systems
        *   `void update(float dt, const gui::character_panel_state& panel_state)` — simulation step
        *   Accessors as needed for rendering/GUI (public members acceptable per code standards)
    *   Create `setup_test_level(scene& scn)` function in `game_world.cpp`:
        *   Move all hardcoded geometry (grid floor, platforms, walls, steps) from `app_runtime::initialize()` into this function.
    *   Refactor `app_runtime`:
        *   Replace individual simulation members with `game_world world;`
        *   Keep lifecycle concerns: Sokol init/shutdown, input/GUI init, pass_action, static mesh initialization
        *   Delegate simulation to `world.update(dt, panel_state)` in `update_simulation()`
        *   Pass `world` members to rendering/debug functions (or expose via accessors)
    *   Preserve all existing behavior: camera controls, GUI integration, debug rendering, animation toggles
*   **Testing Protocol:**
    *   **Self-test:** Build and run. Verify identical behavior (movement, collision, camera, GUI, debug draw).
    *   **Compilation check:** Ensure clean build with no warnings. Run `clang-tidy` to catch ownership/lifetime issues.
    *   **Visual inspection:** Compare frame-by-frame with previous build (same test level, same controls, same debug visuals).
    *   **No over-the-shoulder needed:** This is an internal refactor with no player-facing changes.
*   **Success Metrics (Definition of "Proven"):**
    *   Identical runtime behavior (no regressions).
    *   `app_runtime` responsibilities clearly separated: lifecycle only, no simulation state.
    *   `game_world` responsibilities clearly defined: simulation state and update logic.
    *   Test level geometry isolated in `setup_test_level()`, easy to replace.
    *   Code compiles cleanly, passes lint checks.
*   **Iteration Cadence & Horizon:**
    *   Single-pass refactor: implement, test, merge (2–4 hours).
    *   No iteration expected unless unexpected coupling discovered (low risk).

### 5. System Integration

*   **Mechanic Interactions:** No new mechanics. Existing systems (character, camera, rendering, GUI) continue to work unchanged. They now access simulation state through `game_world` instead of scattered `app_runtime` members.
*   **Interface & Feedback:** No user-facing changes. Internal code organization only.
*   **Future Integration Points:**
    *   **Level loading:** Replace `setup_test_level()` with procedural generation or file loading.
    *   **Multi-world/scenarios:** Instantiate multiple `game_world` instances for split-screen or parallel simulations.
    *   **Automated testing:** `game_world` can be instantiated without Sokol/windowing for headless tests.

### 6. Risks & Future Work

*   **Potential Risks:**
    *   **Tight coupling exposure:** May discover hidden dependencies between app lifecycle and simulation (e.g., static mesh initialization timing). Mitigation: keep `ensure_static_meshes()` in `app_runtime` and pass meshes to rendering.
    *   **Ownership confusion:** Lifetime of `game_world` must match `app_runtime`. Not an issue here (composition, not pointers).
    *   **Over-engineering:** Risk of adding unnecessary abstraction layers. Mitigation: keep it simple—single `game_world` struct, no interfaces or polymorphism.
*   **Content Restrictions:** None. This refactor does not affect gameplay or impose constraints.
*   **Decision Impacts (Checklist):**
    *   **Implementation cost:** Low (2–4 hours). Straightforward move of members and methods.
    *   **Immaturity burden:** None. No unfinished systems introduced.
    *   **Critical failure risk:** Very low. Compile-time errors catch mistakes; behavior unchanged.
    *   **Process burden:** None. Single-person project, no coordination needed.
    *   **Political/cultural effects:** N/A (solo project).
    *   **Decision cost:** Low. Clear benefits (clarity, testability), minimal risk.
*   **Backlog Candidates:**
    *   **Level format/loader:** Replace `setup_test_level()` with JSON/binary level format (defer until level design needs stabilize).
    *   **Scenario system:** Multiple test setups (collision stress test, locomotion playground, etc.) switchable at runtime (defer until testing workflow demands it).
    *   **Headless testing:** Instantiate `game_world` without rendering for automated physics/collision tests (defer until bug patterns justify it).

---

## Implementation Checklist

- [x] Create `src/app/game_world.h` with struct definition
- [x] Create `src/app/game_world.cpp` with init/update methods
- [x] Move test level geometry to `setup_test_level()` function
- [x] Refactor `app_runtime` to use `game_world` member
- [x] Update `CMakeLists.txt` if needed (should auto-detect new files)
- [x] Build and verify identical behavior
- [x] Run lint checks (`clang-tidy`)
- [x] Commit with clear message: "refactor: separate game_world from app_runtime"

## Approval

- **Reviewer:** Gemini
- **Date:** 2025-10-06

## Completion

- **Date:** 2025-10-06
- **Final Certainty:** 100%
- **Code Review:** `PLANS/ARCHIVE/20251006_120000_code_review_refactor_game_world.md`
- **Outcome:** The refactor successfully separated game simulation state from the application runtime, improving code clarity and testability without any behavioral changes.
