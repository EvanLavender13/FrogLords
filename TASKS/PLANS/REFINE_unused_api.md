# Refinement: Unused API Functions

Remove 14 unused API functions flagged by cppcheck

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-21
**Complexity:** Trivial
**Path:** A (trivial)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** Multiple files (14 functions)
**Principle:** Radical Simplicity
**Severity:** Low
**Type:** Dead code accumulation

**Current state:**
14 functions defined but never called:
- `src/gui/gui.cpp`: button, checkbox, color_edit, plot_value, wants_keyboard (5 functions)
- `src/input/input.cpp`: is_key_released, is_mouse_button_pressed, is_mouse_button_released, get_mouse_position_i, mouse_x, mouse_y (6 functions)
- `src/foundation/easing.cpp`: cubic_hermite (2 overloads), smooth_mix (scalar_span variant) (3 functions)
- `src/foundation/spring_damper.cpp`: reset (1 function)
- `src/camera/camera.h`: set_fov, set_clip_planes (2 functions, inline)

**Note:** plot_histogram IS used in runtime.cpp and should NOT be removed

**Why violation:**
- Every element must justify its existence - unused code fails this test
- Code bloat reduces clarity and increases maintenance burden
- False sense of API completeness

**Impact:**
- Blocks: Nothing - purely additive cleanup
- Cascades to: Improved code clarity, reduced surface area for static analysis warnings
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Delete

**DELETE:**
- Remove unused function declarations from headers
- Remove unused function implementations from source files
- Verify no test code references these functions
- Re-run cppcheck to confirm warnings cleared

**Impact:**
After deletion, codebase will contain only code that actually executes. Zero behavioral change. Static analysis will be cleaner.

**Tests:**
Compilation confirms no consumers exist. Runtime behavior unchanged.
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: REFINE/COMPLETED -->
## Completed

**Change:** Deleted 12 unused API functions (2 fewer than initially identified)

**Files Modified:**
- `src/gui/gui.h` - Removed 5 function declarations (button, checkbox, color_edit, plot_value, wants_keyboard)
- `src/gui/gui.cpp` - Removed 5 function implementations
- `src/input/input.h` - Removed 3 function declarations (is_key_released, is_mouse_button_pressed, is_mouse_button_released, get_mouse_position_i)
- `src/input/input.cpp` - Removed 3 function implementations
- `src/foundation/easing.h` - Removed 3 function declarations (cubic_hermite float, cubic_hermite vec3, smooth_mix scalar_span)
- `src/foundation/easing.cpp` - Removed 3 function implementations
- `src/foundation/spring_damper.h` - Removed 1 function declaration (reset)
- `src/foundation/spring_damper.cpp` - Removed 1 function implementation
- `src/camera/camera.h` - Removed 2 inline function definitions (set_fov, set_clip_planes)

**Correction During Implementation:**
Initially planned to remove mouse_x() and mouse_y(), but discovered they ARE used in `src/app/runtime.cpp:76,77,88,89` for camera orbit control. Kept these functions. Actual count: 12 functions removed (not 14).

**Tests:** All passing - build successful with zero errors

**Result:** ✓ Violation removed - codebase now contains only functions that are actually called
<!-- END: REFINE/COMPLETED -->
