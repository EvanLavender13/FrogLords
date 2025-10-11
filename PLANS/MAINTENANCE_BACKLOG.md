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

### Joint Override Angles Not Reset on Pose Change
- **Files:** `src/gui/character_panel.h`, `src/gui/character_panel.cpp`, `src/app/game_world.cpp`
- **Severity:** High (confusing UX behavior, functionally incorrect)
- **Description:** When user manually overrides joint angles and then switches to a different pose (either manually or by disabling manual pose selection), the override angles persist. This causes:
  1. Overrides applied to T-Pose don't visually affect the skeleton (appears broken)
  2. Overrides set on one walk pose remain applied when switching to other poses
  3. No clear way to reset overrides except manually zeroing all 24 sliders
- **Root Cause:** `character_panel_state` stores override angles (8 `glm::vec3` members) but never resets them on pose change. The state is persistent across all pose selections.
- **Expected Behavior:**
  - Option A (Simple): Reset all override angles to (0,0,0) when user switches poses or toggles manual pose selection
  - Option B (Preserve per-pose): Store separate override sets per pose_type (more complex, questionable value)
- **Suggested Fix (Option A):**
  1. Add reset function to `character_panel_state`:
     ```cpp
     void reset_joint_overrides() {
         left_shoulder_angles = glm::vec3(0.0f);
         left_elbow_angles = glm::vec3(0.0f);
         // ... all 8 joints
     }
     ```
  2. In `character_panel.cpp`, detect pose change and call reset:
     ```cpp
     static pose_type prev_selected_pose = pose_type::T_POSE;
     if (state.selected_pose != prev_selected_pose) {
         state.reset_joint_overrides();
         prev_selected_pose = state.selected_pose;
     }
     ```
  3. Also reset when toggling `use_manual_pose_selection` or `enable_joint_overrides`
- **Impact:** Improves debug tool usability significantly; prevents confusing "sticky" override behavior
- **Complexity:** 1-2 points (simple state management, straightforward logic)
- **Discovered:** 2025-10-11 during joint slider widget refactor validation

---

## Medium

(Minor inconsistencies or quality improvements)

### Commented-Out Code in runtime.cpp
- **File:** `src/app/runtime.cpp:105`
- **Severity:** Medium
- **Description:** Commented-out call to `gui::plot_value` suggests leftover debugging/experimentation code. Line reads: `// gui::plot_value("FPS", 1.0f / sapp_frame_duration(), 5.0f, 0.0f, 200.0f);`
- **Suggested Fix:** Remove commented-out code. If this represents a potential feature, document it in DESIGN_BACKLOG.md instead.

### printf Logging in Production Code
- **File:** `src/main.cpp:11`
- **Severity:** Medium
- **Description:** Printf statement `printf("sapp_sample_count = %d\n", sapp_sample_count());` logs MSAA sample count at startup. While useful for debugging, this is permanent logging in main.cpp without a debug flag.
- **Suggested Fix:** Either remove if not needed, or wrap in a debug compile flag (`#ifdef DEBUG` or similar) if startup diagnostics are valuable.

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

### C-Style Void Parameters
- **File:** `src/main.cpp:8, 14, 18`
- **Severity:** Medium
- **Description:** Functions use C-style `void` parameter lists (e.g., `static void init(void)`). This is C convention; C++ prefers empty parameter lists.
- **Suggested Fix:** Change `func(void)` to `func()` for C++ convention consistency.

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

### Unused Parameter Markers
- **File:** 
  - `src/main.cpp:27-28`
  - `src/rendering/debug_draw.cpp:73, 108`
- **Severity:** Low
- **Description:** Multiple `(void) param;` statements to suppress unused parameter warnings. While functionally correct, C++ attributes would be clearer.
- **Suggested Fix:** Consider using `[[maybe_unused]]` attribute instead of `(void)` casts for C++17 clarity. However, this is low priority and current approach is acceptable.

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
