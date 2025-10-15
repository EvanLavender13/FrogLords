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
