# Maintenance Backlog

**Purpose:** Tracking for small improvements, refactors, and cleanup items that don't constitute full features but benefit code quality.

**Status:** Living document (updated after codebase reviews)

**Last Review:** October 11, 2025

---

## Critical

(Direct violations, bugs, correctness issues requiring immediate attention)

*No items currently tracked.*

---

## High

(Significant code smells impacting readability or maintainability)

*No items currently tracked.*

---

## Medium

(Minor inconsistencies or quality improvements)

### Commented-Out Code in runtime.cpp
- **File:** `src/app/runtime.cpp:105`
- **Severity:** Medium
- **Description:** Commented-out call to `gui::plot_value` suggests leftover debugging/experimentation code. Line reads: `// gui::plot_value("FPS", 1.0f / sapp_frame_duration(), 5.0f, 0.0f, 200.0f);`
- **Suggested Fix:** Remove commented-out code. If this represents a potential feature, document it in DESIGN_BACKLOG.md instead.

### TODO Comments for Future Features
- **Files:**
  - `src/input/input.h:67-75`
  - `src/input/input.cpp:20-22, 44-46, 156-165`
- **Severity:** Medium
- **Description:** Multiple TODO comments for mouse delta tracking and gamepad support. These are tracked features but cluttering implementation files.
- **Suggested Fix:** Move these feature requests to `DESIGN_BACKLOG.md` and remove TODO comments from code. Keep implementation files focused on current functionality.

### Unused Variable Suppression Comment
- **File:** `src/character/controller.cpp:104`
- **Severity:** Medium
- **Description:** `// cppcheck-suppress variableScope` comment suggests a code smell. The variable `pre_collision_vertical_velocity` is defined early but only used later.
- **Suggested Fix:** Move the variable declaration closer to its use point (right before line 111 where it's assigned) to eliminate the need for suppression.

### Magic Number in Easing
- **File:** `src/character/animation.cpp:89`
- **Severity:** Medium
- **Description:** Hardcoded max_speed value `8.0f` in legacy `update()` method should use the actual parameter or be named.
- **Suggested Fix:** Pass max_speed as a parameter to the update method or use a named constant.

---

## Low

(Nitpicks and polish items)

### Magic Number for PI
- **Files:**
  - `src/rendering/debug_draw.cpp:129, 160, 171`
- **Severity:** Low
- **Description:** Uses literal `3.14159f` for PI instead of named constant. Project uses `glm::pi<float>()` elsewhere (e.g., `orientation.cpp:13`).
- **Suggested Fix:** Replace `3.14159f` with `glm::pi<float>()` or define `TWO_PI` constant for consistency with project style. Very minor—current code is functional.

### NOLINT Comment for Intentional Branch Clone
- **File:** `src/character/animation.cpp:129`
- **Severity:** Low
- **Description:** `// NOLINT(bugprone-branch-clone)` suppresses warning for intentional duplicate branch in threshold-based pose selection.
- **Suggested Fix:** None needed—this is appropriate use of NOLINT for intentional design choice where both branches assign NEUTRAL pose.

### Inconsistent Constant Naming Scope
- **File:** Various (e.g., `src/character/controller.cpp:15-16`)
- **Severity:** Low
- **Description:** Some constants are in anonymous namespaces, others are `constexpr` at function scope. While both are valid, inconsistency exists.
- **Suggested Fix:** Establish convention—prefer anonymous namespace for file-scope constants, constexpr at function scope only when truly function-local.

### NOLINT Comments for Design Choices
- **Files:**
  - `src/character/animation.h:35`
  - `src/character/animation.cpp:14, 62`
  - `src/foundation/easing.cpp:30`
- **Severity:** Low
- **Description:** NOLINT comments suppress "easily-swappable-parameters" warnings for functions with multiple float parameters. These are intentional design choices.
- **Suggested Fix:** None needed—these suppressions are appropriate. Consider documenting why parameters aren't using structs if questioned during code reviews.

---

## Completed

(Resolved items moved here for reference; prune periodically)

### printf Logging in Production Code
- **Files:** src/main.cpp
- **Severity:** Medium
- **Description:** Printf statement `printf("sapp_sample_count = %d\n", sapp_sample_count());` logs MSAA sample count at startup. While useful for debugging, this is permanent logging in main.cpp without a debug flag.
- **Resolution:** Removed the unconditional startup printf and dropped the unused `<cstdio>` include so release builds stay silent.
- **Completed:** 2025-10-11
- **Learnings:**
  - Entry-point diagnostics should flow through the debug UI or conditional logging instead of stdout prints.
  - Keeping trivial maintenance items batched by touch point reduces context churn.
- **Document:** PLANS/maintenance_printf_logging_main.md

### C-Style Void Parameters
- **Files:** src/main.cpp
- **Severity:** Medium
- **Description:** Functions use C-style `void` parameter lists (e.g., `static void init(void)`). This is C convention; C++ prefers empty parameter lists.
- **Resolution:** Updated the static callbacks to use empty parameter lists so the entry point adheres to C++ style guidelines.
- **Completed:** 2025-10-11
- **Learnings:**
  - When wiring sokol callbacks, default to C++ signatures to avoid cleanup passes later.
  - Worth adding a quick code review checklist note for entry-point glue.
- **Document:** PLANS/maintenance_c_style_void_parameters.md

### Unused Parameter Markers
- **Files:** src/main.cpp, src/rendering/debug_draw.cpp
- **Severity:** Low
- **Description:** Multiple `(void) param;` statements to suppress unused parameter warnings. While functionally correct, C++ attributes would be clearer.
- **Resolution:** Replaced the casts with `[[maybe_unused]]` attributes on the relevant parameters in `main.cpp` and `debug_draw.cpp`.
- **Completed:** 2025-10-11
- **Learnings:**
  - Attribute-based suppression reads cleaner and keeps intent near declarations; scan other subsystems opportunistically.
  - Path B classification was accurate; spanning two files justified the review loop.
- **Document:** PLANS/maintenance_unused_parameter_markers.md

### Redundant Include in character_panel.cpp
- **File:** `src/gui/character_panel.cpp:3`
- **Severity:** High
- **Description:** `#include "character/tuning.h"` was redundant—already included via `character_panel.h:7`.
- **Resolution:** Removed redundant include from `.cpp` file.
- **Completed:** October 8, 2025

### Include Case Inconsistency
- **File:** `src/gui/character_panel.cpp:5`
- **Severity:** High
- **Description:** Include directive uses `#include "ImGui.h"` (PascalCase) instead of `#include "imgui.h"` (lowercase). This may cause portability issues on case-sensitive filesystems.
- **Resolution:** Changed include directive to use lowercase `imgui.h` to match standard imgui header naming.
- **Completed:** October 8, 2025

### Duplicate Constants Across Files
- **Files:** 
  - `src/rendering/debug_draw.cpp:11-12`
  - `src/app/game_world.cpp:11-12`
- **Severity:** High
- **Description:** `WHEEL_RADIUS` and `TWO_PI` constants are duplicated in two files with identical values. This creates maintenance burden if the value needs to change.
- **Resolution:** Extracted constants to `src/character/locomotion.h` as public `constexpr` declarations.
- **Completed:** October 9, 2025

### Redundant Include
- **Files:** `src/rendering/debug_draw.cpp:3`
- **Severity:** High
- **Description:** `#include "character/controller.h"` is redundant—already included via `debug_draw.h:6`.
- **Resolution:** Removed redundant #include "character/controller.h" from debug_draw.cpp.
- **Completed:** October 9, 2025

### Commented-Out Debug Code
- **File:** `src/input/input.cpp:54, 61, 68, 76, 84`
- **Severity:** Medium
- **Description:** Multiple printf debug statements were commented out throughout the event processing function.
- **Resolution:** Removed all commented-out debug code for clarity and code health. No logic or behavior changed.
- **Completed:** October 10, 2025

### Joint Override Angles Not Reset on Pose Change (T-Pose Override Bug)
- **Files:** `src/gui/character_panel.h`, `src/gui/character_panel.cpp`, `src/app/game_world.cpp`, `src/character/animation.h`, `src/character/animation.cpp`, `src/character/keyframe.cpp`
- **Severity:** High (confusing UX behavior, functionally incorrect)
- **Description:** Joint overrides applied to T-Pose didn't visually affect the skeleton, and override angles persisted incorrectly when switching poses.
- **Root Cause (Discovered):** Two issues:
  1. `glm::quat()` default constructor creating incorrect identity quaternion in `create_identity_pose()`
  2. GUI state not resetting override angles on pose transitions
- **Resolution:** 
  - Fixed identity quaternion construction with explicit `glm::quat(1.0f, 0.0f, 0.0f, 0.0f)`
  - Added `reset_joint_overrides()` method called on pose changes and mode toggles
  - Restructured animation pipeline to centralize `update_global_transforms()` in `game_world.cpp`
  - Externalized secondary motion as explicit step (clearer separation of concerns)
- **Learnings:** 
  - Initial hypothesis (multiple transform updates) led to valuable pipeline improvements
  - Root cause was data representation issue (quaternion identity)
  - Follows AGENTS.md principle: "Capture serendipity: reorganize around discoveries"
  - Pipeline now matches procedural layering: pose → secondary motion → transform propagation
- **Completed:** October 11, 2025
- **Document:** PLANS/maintenance_joint_override_reset.md

