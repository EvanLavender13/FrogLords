# Improve Backlog

Track code quality issues, architectural violations, tech debt, pattern extraction opportunities, and cleanup items.

**Status:** Living document (updated after reviews and feature work)

**Last Review:** October 14, 2025

---

## Guidance

**Severity/Priority Guide**:
- **Critical**: Architectural violations, correctness bugs, blocking issues
- **High**: Significant tech debt, pattern extraction ready for use
- **Medium**: Quality improvements, minor inconsistencies
- **Low**: Nitpicks, polish

**Rule of Three**: Pattern extraction requires 3+ occurrences unless establishing canonical data source (document exception).

**Stability Gate**: Only improve systems ≥70% certainty (check `DEPENDENCY_STACK.md`).

---

## Critical

(Architectural violations, bugs, blocking issues requiring immediate attention)

### [Regression] Restore Missing Debug Visualizations
- *File(s):* `src/app/debug_generation.cpp`, `src/rendering/wireframe.h`, `src/rendering/wireframe.cpp`
- *Issue:* Several debug visualizations (locomotion wheel, physics spring, character body, foot positions, speed circle) disappeared after the `debug_draw` refactoring. This was a regression caused by moving the visualization logic to a layer that could not access the necessary procedural mesh generation functions.
- *Fix:* Move the procedural mesh generation functions (e.g., `generate_box`, `generate_spring`) from the `rendering` layer to a new file in the `foundation` layer (e.g., `foundation/procedural_mesh.cpp`). Then, update `app/debug_generation.cpp` to call these functions and re-implement the logic for the missing visuals.
- *Rationale:* This restores critical debug feedback that was lost in a recent refactoring. It completes the architectural improvement by correctly placing the geometry generation utilities in a shared, foundational layer.
- *Complexity:* 5 points
- *Tags:* #regression #critical #debug-viz #architecture

---

## High

(Significant improvements to clarity, maintainability, or extensibility)

### [Architecture] Sideways Dependency between Character and Camera
- *File(s):* `src/character/controller.cpp:13`
- *Issue:* `character::controller` directly includes `camera/camera.h` to get the camera's forward and right vectors for processing input. This creates a tight coupling between two separate top-level subsystems (`character` and `camera`).
- *Fix:* Modify `controller::apply_input` to accept the camera's orientation vectors as parameters (e.g., `apply_input(const camera_vectors& cam_vectors, float dt)`). The `game_world` or `runtime` can be responsible for getting the vectors from the camera and passing them to the controller.
- *Rationale:* Decouples the character from the camera system, allowing them to be developed and tested independently. Adheres to the "avoid sideways includes" principle.
- *Complexity:* 3 points
- *Tags:* #architecture #decoupling #character #camera

### [Pattern Extraction] Duplicate Logic in GUI Plotting
- *File(s):* `src/gui/gui.cpp:133`, `src/gui/gui.cpp:171`
- *Issue:* The functions `gui::plot_value` and `gui::plot_histogram` contain identical logic for managing and pruning the rolling data buffer. This is a direct violation of the "Don't Repeat Yourself" (DRY) principle.
- *Fix:* Extract the buffer management logic into a private static helper function (e.g., `update_plot_buffer`). Both `plot_value` and `plot_histogram` would then call this helper before their respective ImGui plot calls.
- *Rationale:* Consolidates duplicated code, reducing maintenance overhead and risk of inconsistent changes.
- *Complexity:* 2 points
- *Tags:* #pattern-extraction #cleanup #gui

### [Pattern Extraction] Duplicate Pose Mirroring Logic
- *File(s):* `src/character/keyframe.cpp`
- *Issue:* The file contains boilerplate code for creating mirrored poses. For example, `create_walk_reach_right_pose` is a manual copy-paste of `create_walk_reach_left_pose` with left/right members swapped. This is repeated for the run cycle.
- *Fix:* Create a `mirror_keyframe(const keyframe& kf)` helper function that takes a keyframe and returns a new one with all left and right joint rotations swapped.
- *Rationale:* Simplifies the creation of new animation cycles and reduces the chance of error from manual copying. Enforces symmetry and elegance.
- *Complexity:* 3 points
- *Tags:* #pattern-extraction #simplification #animation #keyframe

---

## Medium

(Nice-to-have improvements)

### [Simplification] Duplicated Gait Blending Logic
- *File(s):* `src/gui/character_panel.cpp:31`, `src/character/locomotion.cpp:53`
- *Issue:* `gui/character_panel.cpp` has a `compute_base_walk_factor` function that re-implements the logic from `locomotion_system::get_run_blend`. The GUI should not be re-implementing core game logic.
- *Fix:* Remove `compute_base_walk_factor` from the GUI and have it call `locomotion.get_run_blend()` directly (and compute `1.0f - run_blend` if needed).
- *Rationale:* Consolidates game logic within the responsible system (`locomotion`), making the GUI purely a view/controller. Reduces duplication and improves maintainability.
- *Complexity:* 1 point
- *Tags:* #simplification #cleanup #gui #locomotion

### [Constants] Magic Numbers in Velocity Trail
- *File(s):* `src/app/game_world.cpp:27`
- *Issue:* The maximum number of samples in the velocity trail is hardcoded to `25`.
- *Fix:* Define a named constant in `rendering/velocity_trail.h`, such as `constexpr size_t MAX_TRAIL_SAMPLES = 25;`, and use it in `game_world.cpp`.
- *Rationale:* Improves readability and makes the trail length easier to find and tune.
- *Complexity:* 1 point
- *Tags:* #constants #cleanup #batch-candidate

### [Cleanup] Inconsistent Constant Scoping
- *File(s):* `src/character/locomotion.h`, `src/character/controller.cpp`, `src/character/tuning.cpp`
- *Issue:* `constexpr` values are defined in global scope in some files (`locomotion.h`) and in anonymous namespaces in others (`controller.cpp`).
- *Fix:* Standardize on one approach. For constants only used within a single `.cpp` file, prefer an anonymous namespace. For constants that are part of a header's public API, they should remain in the header.
- *Rationale:* Improves code consistency and adheres to best practices for constant scoping.
- *Complexity:* 1 point
- *Tags:* #conventions #cleanup #batch-candidate

### [Dependencies] `sokol_app.h` included for keycodes
- *File(s):* `src/character/controller.cpp:6`
- *Issue:* `controller.cpp` includes the large `sokol_app.h` header solely for access to `SAPP_KEYCODE_*` constants. This creates an unnecessary dependency on a platform-specific header in a core gameplay file.
- *Fix:* Create a new header, e.g., `input/keycodes.h`, that defines the necessary keycodes, and include that instead. This isolates the dependency.
- *Rationale:* Reduces coupling and improves compilation times by avoiding the inclusion of a large, unrelated header.
- *Complexity:* 2 points
- *Tags:* #dependencies #cleanup #decoupling

### Rendering / Debug
**Extract Speed/Age Gradient Helpers**
- *Files:* `src/rendering/debug_draw.cpp:42-55` (speed ring), `src/rendering/debug_draw.cpp:393-403` (trail age)
- *Issue:* Two hand-coded gradient evaluations: 4-stop gradient (blue→green→yellow→red) for speed ring and 2-stop lerp (scale + alpha) for trail age
- *Fix:* Add `evaluate_gradient()` helper to `foundation/easing.h` with multi-stop support and presets
- *Rationale:* Parameters over assets; 2 occurrences found, likely 3rd+ use coming in future debug visualizations
- *Complexity:* 4 points
- *Tags:* #pattern-extraction #debug-viz

---

## Low

(Nitpicks and polish)

### [Conventions] Over-commenting in `renderer.cpp`
- *File(s):* `src/rendering/renderer.cpp`
- *Issue:* Contains comments that state the obvious, such as `// Create shader from generated header` and `// Apply pipeline and bindings`.
- *Fix:* Remove these redundant comments.
- *Rationale:* Code should be self-documenting where possible. Removing obvious comments cleans up the code.
- *Complexity:* 1 point
- *Tags:* #conventions #cleanup #batch-candidate

### [Documentation] Missing comment for `sync_locomotion_targets`
- *File(s):* `src/character/tuning.h:20`
- *Issue:* The free function `sync_locomotion_targets` has no documentation explaining its purpose, which is to synchronize tunable controller parameters with the locomotion system's internal state.
- *Fix:* Add a Doxygen-style comment explaining what the function does, its parameters, and why it's necessary.
- *Rationale:* Improves code clarity and makes the API easier to understand for future development.
- *Complexity:* 1 point
- *Tags:* #documentation #cleanup #batch-candidate

---

## Deferred

(Tracked but not ready; needs prerequisites or more data)

### Foundation / Math
**Consider Rotation Matrix Helper**
- *Files:* `src/character/controller.cpp:138`, `src/rendering/debug_draw.cpp:242`
- *Occurrences:* 2 uses of `glm::rotate(transform, yaw, math::UP)`
- *Defer Reason:* Rule of three not met; wait for 3rd use
- *Proposed:* `math::rotation_y(float)` helper in `foundation/math_utils.h`
- *Tags:* #deferred #rule-of-three

---

## Completed

(Recent completions; see `PLANS/ARCHIVE/improve_completed_*.md` for history)

### Critical / Architecture
**Reverse Dependency in Rendering Layer** ✓
- *Completed:* October 14, 2025
- *Files:* `src/rendering/debug_draw.h`, `src/rendering/debug_draw.cpp`, `src/app/runtime.cpp`, `src/app/game_world.h`, `src/app/game_world.cpp`, `src/app/debug_generation.h`, `src/app/debug_generation.cpp`, `CMakeLists.txt`
- *Fix:* Decoupled the debug rendering system from the character module by introducing a generic `debug_primitive_list`. The app layer now generates this list, and the rendering layer simply draws it.
- *Learning:* Architectural refactoring requires careful staging. Always remember to update the build system (`CMakeLists.txt`) when adding new files. A temporary regression (missing visuals) was introduced and immediately added to the backlog as a new critical item.
- *Document:* `PLANS/IMPROVE_reverse_dependency_in_rendering_layer.md`

### Critical / Character & Rendering
**Architectural Dependency Violations** ✓
- *Completed:* October 13-14, 2025
- *Files:* Character controller, debug draw, collision, game world
- *Learning:* Fix architectural violations immediately to prevent spread; move logic to appropriate layer (collision → foundation)

### Medium / Character
**Extract Pattern Helpers** ✓
- *Completed:* October 13-14, 2025
- *Files:* Tuning, controller, skeleton, animation, keyframe
- *Learning:* File-scope constants in anonymous namespaces for derived values; static helpers for localized patterns; include glm headers when adding functions using glm types

### Medium / GUI
**Extract Buffer Pruning Helper** ✓
- *Completed:* October 14, 2025
- *Files:* `src/gui/gui.cpp`
- *Learning:* Straightforward extraction with no behavior changes; -14 LOC

### Medium / Input & Controller
**Remove Unused stdio Includes** ✓
- *Completed:* October 13, 2025
- *Files:* `src/input/input.cpp`, `src/character/controller.cpp`
- *Learning:* Remove unused includes; keep `<cstdio>` only where GUI plot overlays need it

---

## Tags Reference

- `#architecture` - Layering, dependency flow, structural issues
- `#pattern-extraction` - Repeated code ready for abstraction
- `#cleanup` - Remove dead code, unused includes, etc.
- `#simplification` - Reduce complexity without changing behavior
- `#constants` - Consolidate or reorganize constant definitions
- `#includes` - Include dependency management
- `#conventions` - Coding style and naming consistency
- `#layers` - Specific to layer architecture violations
- `#collision` - Collision system
- `#locomotion` - Character locomotion
- `#skeleton` - Skeleton and joint transforms
- `#gui` - GUI systems
- `#debug-viz` - Debug visualization
- `#controller` - Character controller
- `#deferred` - Not ready yet
- `#rule-of-three` - Waiting for third occurrence