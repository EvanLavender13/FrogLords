# Refinement: Mouse Camera Jump

Delete stale state to prevent control betrayal.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-18
**Complexity:** Trivial
**Path:** A (trivial)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/app/runtime.cpp:69-86`
**Principle:** **Prime Directive** (Do No Harm to Gameplay), Consistency | Score: TBD
**Severity:** High - Control is sacred
**Type:** Lost responsiveness, Unpredictable response

**Current state:**
```cpp
// From runtime.cpp mouse handling
if (io.WantCaptureMouse) {
    // GUI owns mouse - don't update camera
    // BUT: last_mouse_x/y remain frozen
} else {
    // GUI releases mouse
    // First orbit uses ENTIRE accumulated delta
    // Result: Large, surprising view snap
    camera.orbit(
        current_x - last_mouse_x,  // ← Stale delta
        current_y - last_mouse_y   // ← Stale delta
    );
}
```

**Why violation:**
- **Prime Directive**: "The player's intent must flow unimpeded from thought to action"
- **Consistency**: "Every input must produce the same output given the same state"
- When ImGui captures mouse, `last_mouse_x/y` freeze while cursor continues moving
- When GUI releases control, accumulated delta causes unexpected camera snap
- Player didn't intend large motion - they intended small motion that was delayed
- Betrays player expectation: "move mouse a little" → "camera jumps a lot"

**Impact:**
- Blocks: Player trust in camera control
- Cascades to: All UI interactions that capture mouse (panels, sliders, text input)
- Foundation: 97%+ (affects user experience, not mathematical correctness)
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Delete

**DELETE:**
- Remove: Stale delta accumulation
- Impact: Camera behaves predictably when GUI releases mouse
- Tests: Manual - open/close GUI panels while moving mouse, verify no camera jump

**Two approaches (choose simpler):**

**Option 1: Always update last_mouse (even when GUI owns it)**
```pseudocode
// Every frame, regardless of ImGui state:
if (first_frame) {
    last_mouse_x = current_x
    last_mouse_y = current_y
    first_frame = false
    return  // No delta on first frame
}

// Calculate delta
dx = current_x - last_mouse_x
dy = current_y - last_mouse_y

// Always update last_mouse (prevents staleness)
last_mouse_x = current_x
last_mouse_y = current_y

// Only apply orbit if GUI doesn't own mouse
if (!io.WantCaptureMouse && mouse_button_down) {
    camera.orbit(dx, dy)
}
// Else: Delta is discarded, but state stays fresh
```

**Option 2: Reset last_mouse when GUI releases control**
```pseudocode
// Track previous frame's GUI ownership
static bool prev_gui_owned_mouse = false

if (io.WantCaptureMouse) {
    prev_gui_owned_mouse = true
    return  // GUI owns mouse, do nothing
}

// GUI doesn't own mouse this frame
if (prev_gui_owned_mouse) {
    // Transition: GUI just released control
    // Reset to prevent stale delta
    last_mouse_x = current_x
    last_mouse_y = current_y
    prev_gui_owned_mouse = false
    return  // Skip first frame after release
}

// Normal camera orbit with fresh delta
dx = current_x - last_mouse_x
dy = current_y - last_mouse_y
last_mouse_x = current_x
last_mouse_y = current_y

if (mouse_button_down) {
    camera.orbit(dx, dy)
}
```

**Recommendation: Option 1**
- Simpler - no extra state tracking
- Conceptually clearer - `last_mouse` always reflects "last frame's position"
- Fewer branches - works uniformly

**Philosophy:**
The issue is accumulated state becoming stale. The fix is to never let state become stale - either always update it, or reset it when resuming control. Option 1 embodies "never let state drift from truth."
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Violation resolved
- [ ] Principle score improved
- [ ] Tests passing
- [ ] No regressions

**Metrics:**
- Before: Prime Directive __/10, Consistency __/10, Mouse jump observed
- After: Prime Directive __/10 (+__), Consistency __/10 (+__), No camera jump

**Testing:**
1. Open character panel while moving mouse
2. Close character panel - verify camera doesn't jump
3. Drag ImGui slider while mouse moves
4. Release slider - verify camera doesn't jump
5. Type in text field while moving mouse
6. Exit text field - verify camera doesn't jump
<!-- END: SELECT/SUCCESS -->
