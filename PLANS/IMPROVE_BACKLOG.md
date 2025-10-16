# Improve Backlog

Track code quality issues, architectural violations, tech debt, pattern extraction opportunities, and cleanup items.

**Status:** Living document (updated after reviews and feature work)

**Last Review:** October 15, 2025

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

### [Performance] Inefficient rendering in `renderer.cpp`
- *File(s):* `src/rendering/renderer.cpp`
- *Issue:* Vertex and index buffers are created and destroyed for every draw call in `wireframe_renderer::draw`. This is highly inefficient and will cause performance bottlenecks as the number of objects in the scene grows.
- *Fix:* Implement a strategy for buffer reuse. For static geometry, create buffers once and reuse them. For dynamic geometry, use dynamic buffers that can be updated or resized.
- *Rationale:* Drastically improves rendering performance and reduces CPU overhead.
- *Complexity:* 5 points
- *Tags:* #performance #rendering

### [Cleanup] Dead files `wireframe.cpp` and `wireframe.h`
- *File(s):* `src/rendering/wireframe.cpp`, `src/rendering/wireframe.h`
- *Issue:* These files are empty and only contain comments indicating that their content has been moved. They are dead code.
- *Fix:* Delete both files from the project.
- *Rationale:* Removes dead code, simplifying the codebase and preventing confusion.
- *Complexity:* 1 point
- *Tags:* #cleanup #rendering

### [Pattern Extraction] Duplicate Logic in GUI Plotting
- *File(s):* `src/gui/gui.cpp:133`, `src/gui/gui.cpp:171`
- *Issue:* The functions `gui::plot_value` and `gui::plot_histogram` contain identical logic for managing and pruning the rolling data buffer. This is a direct violation of the "Don't Repeat Yourself" (DRY) principle.
- *Fix:* Extract the buffer management logic into a private static helper function (e.g., `update_plot_buffer`). Both `plot_value` and `plot_histogram` would then call this helper before their respective ImGui plot calls.
- *Rationale:* Consolidates duplicated code, reducing maintenance overhead and risk of inconsistent changes.
- *Complexity:* 2 points
- *Tags:* #pattern-extraction #cleanup #gui

---

## Medium

(Nice-to-have improvements)

### [Simplification] Manual space transformation in `animation.cpp`
- *File(s):* `src/character/animation.cpp`
- *Issue:* `update_acceleration_tilt` performs a manual 2D rotation using `sin` and `cos` to transform world-space acceleration to local space.
- *Fix:* Use a 2x2 rotation matrix or a quaternion to perform the transformation. This is more readable and less error-prone.
- *Rationale:* Improves code clarity and robustness.
- *Complexity:* 2 points
- *Tags:* #simplification #character #animation

### [Performance] Inefficient angle wrapping in `math_utils.h`
- *File(s):* `src/foundation/math_utils.h`
- *Issue:* The `wrap_angle_radians` and `wrap_angle_degrees` functions use `while` loops, which can be inefficient for large angles.
- *Fix:* Use `fmod` to perform the wrapping in a single operation.
- *Rationale:* Improves performance, especially for large input values.
- *Complexity:* 1 point
- *Tags:* #performance #foundation #math

### [Constants] Proliferation of magic numbers
- *File(s):* `src/app/debug_generation.cpp`, `src/character/tuning.cpp`, and others.
- *Issue:* Numerous magic numbers are used for colors, sizes, and physics constants.
- *Fix:* Replace magic numbers with named constants, grouped in a relevant header or namespace.
- *Rationale:* Improves readability and maintainability. Makes it easier to tune and adjust values.
- *Complexity:* 3 points
- *Tags:* #constants #cleanup

### [State Management] Static variables in `gui/camera_panel.cpp`
- *File(s):* `src/gui/camera_panel.cpp`
- *Issue:* The `draw_camera_panel` function uses `static` variables for `min_dist` and `max_dist`. This makes the function non-reentrant and can lead to unexpected behavior if the panel is used in multiple places.
- *Fix:* Move the state into the `camera_panel_state` struct.
- *Rationale:* Improves state management and makes the GUI more robust and predictable.
- *Complexity:* 1 point
- *Tags:* #gui #state-management

### [Build] Local `#define GLM_ENABLE_EXPERIMENTAL`
- *File(s):* `src/character/animation.cpp`
- *Issue:* `#define GLM_ENABLE_EXPERIMENTAL` is defined locally. This can lead to inconsistent behavior and ODR violations if other files include the same GLM headers without this define.
- *Fix:* Move the define to a global build flag in the `CMakeLists.txt` file.
- *Rationale:* Ensures consistent compilation and avoids hard-to-debug issues.
- *Complexity:* 1 point
- *Tags:* #build #dependencies

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

### [Conventions] Naming convention violations in `quaternion_validation.cpp`
- *File(s):* `src/foundation/quaternion_validation.cpp`
- *Issue:* Functions in this test file do not follow the `snake_case` convention.
- *Fix:* Rename the functions to follow the `snake_case` convention.
- *Rationale:* Improves code consistency.
- *Complexity:* 1 point
- *Tags:* #conventions #cleanup

### [Simplification] Verbose WASD input handling in `controller.cpp`
- *File(s):* `src/character/controller.cpp`
- *Issue:* The WASD input handling is verbose and can be simplified.
- *Fix:* Use a more compact form to check the keys and build the `move_direction` vector.
- *Rationale:* Improves readability.
- *Complexity:* 1 point
- *Tags:* #simplification #character #input

### [Consistency] Use of `atan2f` in `orientation.cpp`
- *File(s):* `src/character/orientation.cpp`
- *Issue:* `atan2f` is used instead of `glm::atan`.
- *Fix:* Use `glm::atan` for consistency with the rest of the codebase.
- *Rationale:* Improves code consistency.
- *Complexity:* 1 point
- *Tags:* #consistency #character

### [Consistency] Redefinition of `TWO_PI`
- *File(s):* `src/character/locomotion.h`, `src/app/debug_generation.cpp`, `src/app/game_world.cpp`
- *Issue:* `TWO_PI` is redefined in multiple places.
- *Fix:* Use `glm::two_pi<float>()` for consistency.
- *Rationale:* Improves code consistency and avoids potential issues with multiple definitions.
- *Complexity:* 1 point
- *Tags:* #consistency #cleanup

### [Refactoring] `safe_normalize` lambda
- *File(s):* `src/app/debug_generation.cpp`
- *Issue:* The `safe_normalize` lambda is a useful utility that could be used in other places.
- *Fix:* Move the lambda to `foundation/math_utils.h` as a free function.
- *Rationale:* Promotes code reuse.
- *Complexity:* 1 point
- *Tags:* #refactoring #foundation #math

### [Data] Hardcoded test level in `game_world.cpp`
- *File(s):* `src/app/game_world.cpp`
- *Issue:* The test level is hardcoded in the `setup_test_level` function.
- *Fix:* Move the level data to an external file (e.g., JSON, XML) and load it at runtime.
- *Rationale:* Decouples data from code, making it easier to create and modify levels.
- *Complexity:* 3 points
- *Tags:* #data #architecture

### [Refactoring] Mouse input handling in `runtime.cpp`
- *File(s):* `src/app/runtime.cpp`
- *Issue:* Mouse input for the camera is handled directly in the `app_runtime`.
- *Fix:* Move the mouse input handling to the `input` system.
- *Rationale:* Centralizes input handling, making the code cleaner and easier to maintain.
- *Complexity:* 2 points
- *Tags:* #refactoring #input #app

### [Performance] Inefficient color comparison in `debug_draw.cpp`
- *File(s):* `src/rendering/debug_draw.cpp`
- *Issue:* Color comparison for line batching is done component-by-component.
- *Fix:* Pack the `glm::vec4` color into a `uint32_t` and compare the integers for faster sorting and comparison.
- *Rationale:* Improves performance of debug drawing.
- *Complexity:* 1 point
- *Tags:* #performance #rendering #debug-viz

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
- `#performance` - Performance improvements
- `#state-management` - State management issues
- `#build` - Build system issues
- `#dependencies` - Dependency issues
- `#character` - Character subsystem
- `#animation` - Animation subsystem
- `#foundation` - Foundation subsystem
- `#math` - Math utilities
- `#rendering` - Rendering subsystem
- `#input` - Input subsystem
- `#app` - Application layer
- `#refactoring` - Refactoring opportunities
- `#data` - Data-driven design
- `#consistency` - Consistency improvements