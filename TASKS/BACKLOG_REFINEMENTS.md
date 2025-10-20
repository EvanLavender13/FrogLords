# Refinements Backlog

**Current violations. Priority order. Patterns to watch.**

**Foundation:** 100% ✅ PERFECT
**Last Updated:** 2025-10-20
**Last Scan:** 2025-10-19 (format, lint, analyze complete)

---

## Active Violations

### Critical Priority

None - All critical violations resolved ✅

### High Priority

None - All high-priority violations resolved ✅

### Medium Priority

**#18: Enum Naming Convention Violations**
- **Location:** Multiple files (22 instances)
  - `src/character/controller.h:126` - locomotion_speed_state: walk, run, sprint
  - `src/gui/parameter_command.h:20-28` - parameter_type: 9 constants
  - `src/gui/camera_command.h:14-17` - camera_parameter: 4 constants
- **Tool:** clang-tidy
- **Severity:** Medium
- **Issue:** Enum constants use snake_case instead of UPPER_CASE, violating CONVENTIONS.md
- **Fix:** Mechanical rename of all enum constants to UPPER_CASE. Update all usage sites.
- **Impact:** Code style consistency across codebase

**#20: Const-Correctness for Local Arrays**
- **Location:** `src/app/debug_generation.cpp:54,60,162`, `src/foundation/collision.cpp:16`
- **Tool:** clang-tidy
- **Severity:** Medium
- **Issue:** Local constant arrays/variables not marked UPPER_CASE per conventions
- **Fix:** Rename to UPPER_CASE: gradient→GRADIENT, num_stops→NUM_STOPS, circle_segments→CIRCLE_SEGMENTS, epsilon→EPSILON
- **Impact:** Code style consistency

**#21: Uninitialized Renderer Buffer Members**
- **Location:** `src/rendering/renderer.cpp:6`
- **Tool:** cppcheck (uninitMemberVar)
- **Severity:** Medium
- **Issue:** `wireframe_renderer::dynamic_vertex_buffer` and `dynamic_index_buffer` not initialized in constructor
- **Fix:** Initialize in constructor or document sokol lifecycle dependency
- **Impact:** Potential undefined behavior if accessed before sokol initialization

**#14: Mouse Scroll Events Overwrite Instead of Accumulate**
- **Location:** `src/input/input.cpp:75`
- **Principles:** Consistency
- **Severity:** Medium
- **Type:** Lost responsiveness
- **Description:** Scroll events overwrite `mouse_scroll_delta_y` instead of accumulating. Multiple wheel ticks in a single frame reduce to only the last one. Players lose chunks of intended zoom input, undermining "same input → same outcome" contract.
- **Fix:** Simplify - Accumulate scroll deltas or process events immediately.
- **Impact:** Input loss, unpredictable camera zoom response
- **Audit Source:** **Codex**

**#15: Input Scaling Outside Camera System**
- **Location:** `src/app/runtime.cpp:76-77,83`
- **Principles:** Consistency, Composable Functions
- **Severity:** Medium
- **Type:** Lost responsiveness
- **Description:** Mouse delta and scroll are scaled by magic `0.5f` before passing to camera system. This violates "Input is Intent" - input should be pure player intent, scaling should live in camera system as tunable parameters. Makes camera feel less responsive and harder to tune.
- **Fix:** Delete - Remove scaling from input processing. Move sensitivity parameters into camera system.
- **Impact:** Camera tuning difficulty, responsiveness perception
- **Audit Source:** **Gemini**

### Low Priority

**#22: Function Complexity - app_runtime::frame**
- **Location:** `src/app/runtime.cpp:61`
- **Tool:** lizard
- **Severity:** Low
- **Issue:** CCN 22 (threshold 15), Length 128 (threshold 100) - main game loop
- **Fix:** Extract subsystems: input handling, update phase, render phase
- **Impact:** Maintainability, testability of game loop

**#23: Function Complexity - gui::draw_character_panel**
- **Location:** `src/gui/character_panel.cpp:9`
- **Tool:** lizard
- **Severity:** Low
- **Issue:** CCN 20 (threshold 15), Length 116 (threshold 100) - ImGui panel drawing
- **Fix:** Extract parameter sections into helper functions
- **Impact:** GUI code maintainability

**#24: Function Length - controller::update**
- **Location:** `src/character/controller.cpp:109`
- **Tool:** lizard
- **Severity:** Low
- **Issue:** Length 121 (threshold 100) - physics update loop
- **Fix:** Extract sub-phases: ground detection, movement, collision response
- **Impact:** Controller maintainability

**#25: Function Length - debug_viz::validation::run_startup_checks**
- **Location:** `src/rendering/debug_validation.cpp:9`
- **Tool:** lizard
- **Severity:** Low
- **Issue:** Length 150 (threshold 100) - startup validation checks
- **Fix:** Extract validation categories into separate functions or accept for debug code
- **Impact:** Debug code maintainability

**#26: Excessive Function Arguments**
- **Location:** `src/gui/gui.cpp:126,157`
- **Tool:** lizard
- **Severity:** Low
- **Issue:** plot_value and plot_histogram take 6 arguments (threshold 5)
- **Fix:** Group parameters into config struct or accept for utility functions
- **Impact:** API usability

**#27: Variable Scope Reduction**
- **Location:** `src/character/controller.cpp:157`
- **Tool:** cppcheck (variableScope)
- **Severity:** Low
- **Issue:** `pre_collision_vertical_velocity` scope can be reduced
- **Fix:** Move declaration closer to usage
- **Impact:** Code clarity

**#28: Const Parameter References**
- **Location:** `src/gui/character_panel.cpp:9`, `src/gui/camera_panel.cpp:8`
- **Tool:** cppcheck (constParameterReference)
- **Severity:** Low
- **Issue:** State parameters could be const references
- **Fix:** Add const to parameter declarations
- **Impact:** API clarity, const-correctness

**#29: Unused API Functions**
- **Location:** Multiple files (14 functions)
  - `src/gui/gui.cpp`: button, checkbox, color_edit, plot_value, plot_histogram, wants_keyboard
  - `src/input/input.cpp`: is_key_released, is_mouse_button_pressed/released, get_mouse_position variants
  - `src/foundation/easing.cpp`: cubic_hermite, smooth_mix
  - `src/foundation/spring_damper.cpp`: reset
  - `src/camera/camera.h`: set_fov, set_clip_planes
  - Others
- **Tool:** cppcheck (unusedFunction)
- **Severity:** Low
- **Issue:** Functions defined but never called
- **Fix:** Accept - API functions reserved for future use, or remove if truly unnecessary
- **Impact:** Code bloat

**#16: Unused Controller State Fields**
- **Location:** `src/character/controller.h:59-60`
- **Principles:** Radical Simplicity
- **Severity:** Low
- **Type:** Complexity
- **Description:** `ground_normal` and `ground_height` are maintained but never read anywhere. They duplicate derived state without consumers, bloating controller state and inviting future divergence from single source of truth.
- **Fix:** Delete - Remove unused fields or justify their existence.
- **Impact:** Code clarity, state bloat
- **Audit Source:** **Codex**

**#9: Magic Number for Collision Passes**
- **Location:** `src/foundation/collision.cpp:136`
- **Principles:** Mathematical Foundations
- **Severity:** Low
- **Type:** Magic number (DOCUMENTED)
- **Description:** Hard-coded `for (int pass = 0; pass < 3; ++pass)` for collision resolution. Number `3` is arbitrary, lacks mathematical guarantee. NOTE: Already documented with rationale in comments (lines 133-135).
- **Fix:** Accept - Already documented with clear rationale. Consider named constant if reused elsewhere.
- **Impact:** Code clarity
- **Status:** Effectively resolved via documentation ✅

**#11: Hardcoded Test Level Literals**
- **Location:** `src/app/game_world.cpp:62-111`
- **Principles:** Radical Simplicity, Emergent Behavior
- **Severity:** Low
- **Type:** Scattered literals, Special cases
- **Description:** Test arena hard-coded as scattered literals without named primitives (tile size, offsets). Every tweak requires manual edit, reads as pile of special cases rather than composable system.
- **Fix:** Simplify - Extract named constants for dimensions/offsets. OR Accept - Temporary test code, note for future data-driven/procedural system.
- **Impact:** Code clarity, maintainability (non-critical - test only)
- **Audit Source:** **Codex** + Gemini convergence

---

## Patterns Library

**Pattern detection and prevention guidance:**

@TASKS/PATTERNS.md

---

**Remove aggressively. Simplify relentlessly. Document reluctantly.**
