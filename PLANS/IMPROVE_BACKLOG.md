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



---

## High

(Significant improvements to clarity, maintainability, or extensibility)

### [Architecture] Sideways Dependency between Character and Camera ✓
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

### [Batch] Consolidate Minor Cleanup Tasks ✓
- *Completed:* October 14, 2025
- *Fix:* Batched four small cleanup tasks: extracted a constant, standardized constant scoping (already done), removed redundant comments, and added missing documentation.
- *Learning:* Batching trivial items is an efficient way to improve code quality without significant overhead. Verifying backlog items are still relevant is important.
- *Document:* `PLANS/IMPROVE_consolidate_minor_cleanup_tasks.md`

### [Simplification] Duplicated Gait Blending Logic ✓
- *Completed:* October 14, 2025
- *Files:* `src/gui/character_panel.cpp`, `src/character/locomotion.cpp`
- *Fix:* Removed `compute_base_walk_factor` from the GUI and had it call `locomotion.get_run_blend()` directly.
- *Learning:* Consolidating duplicated logic into the authoritative system (`locomotion`) cleans up the GUI code and improves maintainability. A trivial but valuable architectural cleanup.
- *Document:* `PLANS/IMPROVE_simplify_gait_blending_logic.md`

### Medium / Regression
**Restore Missing Debug Visualizations** ✓
- *Completed:* October 14, 2025
- *Files:* `src/foundation/procedural_mesh.h`, `src/foundation/procedural_mesh.cpp`, `src/rendering/wireframe.h`, `src/rendering/wireframe.cpp`, `src/app/debug_generation.cpp`, `src/rendering/debug_draw.cpp`, `CMakeLists.txt`
- *Fix:* Moved procedural mesh generation to the foundation layer to fix a regression where several debug visuals disappeared. Also fixed multiple follow-up bugs with transformations and color batching.
- *Learning:* Refactoring rendering code requires careful visual verification. A simplified plan from a code review was crucial for success.
- *Document:* `PLANS/IMPROVE_restore_missing_debug_visuals.md`

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
