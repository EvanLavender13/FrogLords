# Improve Backlog

Track code quality issues, architectural violations, tech debt, pattern extraction opportunities, and cleanup items.

**Status:** Living document (updated after reviews and feature work)

**Last Review:** October 13, 2025

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

---

## Medium

(Nice-to-have improvements)

### Character / Animation
**Extract Joint Transform Application**
- *Files:* `src/character/keyframe.cpp`, `src/character/animation.cpp`
- *Issue:* Repeated lambdas `apply_joint` / `apply_blended_joint`
- *Fix:* Add `set_joint_rotation(skeleton&, int, const glm::quat&)` in `skeleton.h`
- *Impact:* -20–30 LOC; removes 4 lambdas
- *Complexity:* 3 points
- *Tags:* #pattern-extraction #skeleton

### GUI
**Extract Buffer Pruning Helper**
- *Files:* `src/gui/gui.cpp` (plots)
- *Issue:* Identical prune logic in `plot_value()` and `plot_histogram()`
- *Fix:* Internal helper `prune_plot_buffer(buffer, current_time, max_samples)`
- *Impact:* -16 LOC
- *Complexity:* 2 points
- *Tags:* #pattern-extraction #gui

### Rendering / Debug
**Extract Speed/Age Gradient Helpers**
- *Files:* `src/rendering/debug_draw.cpp`
- *Issue:* Hand-coded multi-stop gradients (speed ring, trail age)
- *Fix:* Add `evaluate_gradient()` to `foundation/easing.h` + presets
- *Rationale:* Parameters over assets; likely 3rd+ use coming
- *Complexity:* 4 points
- *Tags:* #pattern-extraction #debug-viz

---

## Low

(Nitpicks and polish)

### Character
**Inconsistent Constant Naming Scope**
- *Files:* Various (e.g., `src/character/controller.cpp:15-16`)
- *Issue:* Some constants in anonymous namespaces, others `constexpr` at function scope
- *Fix:* Document preferred placement in `CONVENTIONS.md`; align opportunistically
- *Complexity:* 2 points (docs + alignment)
- *Tags:* #conventions #polish

---

## Deferred

(Tracked but not ready; needs prerequisites or more data)

### Foundation / Math
**Consider Rotation Matrix Helper**
- *Files:* `controller.cpp`, `debug_draw.cpp`
- *Occurrences:* 2 uses of `glm::rotate(..., yaw, up)`
- *Defer Reason:* Rule of three not met; wait for 3rd use
- *Proposed:* `math::rotation_y(float)` in foundation
- *Tags:* #deferred #rule-of-three

---

## Completed

(Archive periodically; keep ~1 month for learning capture)

### Critical / Character
**Character Controller Depends on Rendering** ✓
- *Completed:* October 14, 2025
- *Files:* `src/character/controller.cpp`, `src/character/controller.h`, `src/foundation/collision.h`, `src/foundation/collision.cpp`, `src/app/game_world.h`, `src/app/game_world.cpp`, `src/rendering/debug_draw.h`, `src/rendering/debug_draw.cpp`, `src/app/runtime.cpp`
- *Fix:* Moved collision resolution logic from the character controller to the foundation layer to remove the dependency of the character layer on the rendering layer.
- *Learning:* Architectural violations should be fixed as soon as they are found to prevent them from spreading and making the codebase harder to maintain.
- *Document:* `PLANS/IMPROVE_character_controller_depends_on_rendering.md`

### Medium / Character / Controller
**Simplify Velocity Clamping** ✓
- *Completed:* October 13, 2025
- *Files:* `src/character/controller.cpp`
- *Fix:* Created a local helper function `clamp_horizontal_speed` to reduce code duplication.
- *Learning:* Encapsulating repetitive logic in helper functions improves code readability and maintainability.
- *Document:* `PLANS/IMPROVE_simplify_velocity_clamping.md`

### Medium / Character / Tuning
**Consolidate Tuning Constants** ✓
- *Completed:* October 13, 2025
- *Files:* `src/character/tuning.cpp`
- *Fix:* Moved duplicated `FRICTION_RATIO` constant to a file-level anonymous namespace.
- *Learning:* Simple cleanups like this are quick wins for improving code quality.
- *Document:* `PLANS/IMPROVE_consolidate_tuning_constants.md`

### Critical / Rendering
**Debug Draw Depends on App** ✓
- *Completed:* October 13, 2025
- *Files:* `src/rendering/debug_draw.cpp`, `src/app/game_world.h`, `src/rendering/velocity_trail.h`
- *Fix:* Extracted `velocity_trail_state` from `game_world.h` to new header `rendering/velocity_trail.h` to break App → Rendering dependency.
- *Learning:* Architectural violations, even if small, should be fixed promptly to prevent them from spreading.
- *Document:* `PLANS/IMPROVE_debug_draw_app_dependency.md`

### Medium / Input
**Unused stdio Include** (done)
- *Completed:* October 13, 2025
- *Files:* `src/input/input.cpp`
- *Fix:* Removed unused `<cstdio>` include
- *Learning:* GUI plot overlays still rely on `<cstdio>`; keep include there until helper exists
- *Document:* `PLANS/IMPROVE_remove_unused_stdio_include_input.md`

### Medium / Character / Controller
**Unused stdio Include** (done)
- *Completed:* October 13, 2025
- *Files:* `src/character/controller.cpp`
- *Fix:* Removed unused `<cstdio>` include
- *Learning:* Rendering debug formatting still uses `<cstdio>`; remove only where unused
- *Document:* `PLANS/IMPROVE_remove_unused_stdio_include_controller.md`

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
