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

---

## Medium

(Nice-to-have improvements)

### [Constants] Proliferation of magic numbers
- *File(s):* `src/app/debug_generation.cpp`, `src/character/tuning.cpp`, and others.
- *Issue:* Numerous magic numbers are used for colors, sizes, and physics constants.
- *Fix:* Replace magic numbers with named constants, grouped in a relevant header or namespace.
- *Rationale:* Improves readability and maintainability. Makes it easier to tune and adjust values.
- *Complexity:* 3 points
- *Tags:* #constants #cleanup

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
