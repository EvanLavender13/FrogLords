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

### Rendering
**Debug Draw Depends on App**
- *File:* `src/rendering/debug_draw.cpp:5`
- *Issue:* Includes `app/game_world.h` to access `velocity_trail_state`, violating layered architecture
- *Principle Violated:* Rendering must not depend on App
- *Fix:* Extract `velocity_trail_state` to shared header (e.g., `rendering/velocity_trail.h`) or pass minimal data via parameters
- *Complexity:* 2-3 points
- *Tags:* #architecture #layers

### Character
**Character Controller Depends on Rendering**
- *File:* `src/character/controller.cpp:4`
- *Issue:* Includes `rendering/scene.h`, pulling rendering into character layer
- *Principle Violated:* Layered architecture (Foundation → Character → Rendering → App)
- *Fix:* Move collision queries to foundation interface or pass narrow collision struct from app
- *Complexity:* 5-8 points
- *Tags:* #architecture #layers #collision

---

## High

(Significant improvements to clarity, maintainability, or extensibility)

### Character / Locomotion
**Extract Walk/Run Blend Calculation**
- *Files:* `src/character/locomotion.cpp` (`update()` and `get_current_pose()`)
- *Issue:* Identical blend factor calculation duplicated
- *Fix:* Extract `compute_walk_run_blend()` private method
- *Rationale:* Inevitable third use as locomotion expands; abstract now while system is stable
- *Impact:* -12 LOC, single source of truth
- *Complexity:* 3 points
- *Tags:* #pattern-extraction #locomotion

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

### Character / Tuning
**Consolidate Tuning Constants**
- *Files:* `src/character/tuning.cpp`
- *Issue:* `FRICTION_RATIO = 0.75f` duplicated in two functions
- *Fix:* Move to file-level anonymous namespace
- *Complexity:* 1 point
- *Tags:* #cleanup #constants

### Character / Controller
**Simplify Velocity Clamping**
- *Files:* `src/character/controller.cpp`
- *Issue:* Two blocks extract/modify/reassign horizontal velocity
- *Fix:* Local helper `clamp_horizontal_speed(glm::vec3&, float)`
- *Impact:* -10 LOC
- *Complexity:* 2 points
- *Tags:* #simplification #controller

### Character / Controller
**Unused stdio Include**
- *File:* `src/character/controller.cpp:11`
- *Issue:* `<cstdio>` unused (leftover from debugging)
- *Fix:* Remove
- *Complexity:* 1 point
- *Tags:* #cleanup #includes

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

### Medium / Input
**Unused stdio Include** (done)
- *Completed:* October 13, 2025
- *Files:* `src/input/input.cpp`
- *Fix:* Removed unused `<cstdio>` include
- *Learning:* GUI plot overlays still rely on `<cstdio>`; keep include there until helper exists
- *Document:* `PLANS/IMPROVE_remove_unused_stdio_include_input.md`

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
