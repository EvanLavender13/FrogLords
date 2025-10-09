# Maintenance Backlog

**Purpose:** Tracking for small improvements, refactors, and cleanup items that don't constitute full features but benefit code quality.

**Status:** Living document (updated after codebase reviews)

**Last Review:** October 7, 2025

---

## Critical

(Direct violations, bugs, correctness issues requiring immediate attention)

*No items currently tracked.*

---

## High

(Significant code smells impacting readability or maintainability)

### Duplicate Constants Across Files
- **Files:** 
  - `src/rendering/debug_draw.cpp:11-12`
  - `src/app/game_world.cpp:11-12`
- **Severity:** High
- **Description:** `WHEEL_RADIUS` and `TWO_PI` constants are duplicated in two files with identical values. This creates maintenance burden if the value needs to change.
- **Suggested Fix:** Extract to a shared header (e.g., `src/character/locomotion.h` as public constants) or create `src/foundation/math_constants.h` for shared math constants like `TWO_PI`.

### Redundant Include
- **File:** `src/rendering/debug_draw.cpp:3`
- **Severity:** High
- **Description:** `#include "character/controller.h"` is redundant—already included via `debug_draw.h:6`.
- **Suggested Fix:** Remove redundant include from `.cpp` file.

---

## Medium

(Minor inconsistencies or quality improvements)

### Commented-Out Debug Code
- **File:** `src/input/input.cpp:54, 61, 68, 76, 84`
- **Severity:** Medium
- **Description:** Multiple printf debug statements are commented out throughout the event processing function. These should either be removed or converted to a proper debug logging system if they're valuable.
- **Suggested Fix:** Remove commented debug code entirely. If logging is needed in the future, add a debug logging system rather than accumulating commented printfs.

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
