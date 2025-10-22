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
