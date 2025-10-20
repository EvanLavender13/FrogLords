# Refinement: Mouse Scroll Accumulation

Mouse scroll events overwrite instead of accumulate - players lose input

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-20
**Complexity:** Trivial
**Path:** A (trivial)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/input/input.cpp:75`
**Principle:** Consistency (Input is Intent) | Score: Reduces system from 9/10 to 7/10
**Severity:** Medium
**Type:** Lost responsiveness - Input loss

**Current state:**
```cpp
// In handle_scroll_event (line 75):
mouse_scroll_delta_y = event->scroll_y;  // Overwrites previous events
```

**Why violation:**
- Multiple scroll events in single frame reduce to only the last one
- Players lose chunks of intended zoom input
- Violates "same input → same outcome" contract
- "Input is Intent" - player intent is discarded

**Impact:**
- Blocks: Predictable camera zoom behavior
- Cascades to: Camera system receives incomplete scroll data
- Foundation: 100% (no impact on foundation, isolated input handling)
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify

**SIMPLIFY:**
- From: Overwrite each scroll event
- To: Accumulate all scroll events in frame
- Derive: Total scroll delta = sum of all events
- Check duplicates: None - single event handler location

**Implementation concept:**
```
// Pseudocode - change from:
on_scroll(event):
    delta = event.scroll_y  // loses previous events

// To:
on_scroll(event):
    delta += event.scroll_y  // accumulates all events
```

**Validation:**
- Scroll multiple times rapidly → camera zooms proportional to total input
- Single scroll → same behavior as before (single += is identical to =)
- Zero scroll → no zoom (delta accumulates to 0)
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Violation resolved
- [ ] Principle score improved
- [ ] Tests passing
- [ ] No regressions

**Metrics:**
- Before: LOC 1, Principle 7/10, Complexity 1
- After: LOC 1 (±0), Principle 9/10 (+2), Complexity 1 (±0)

<!-- BEGIN: REFINE/COMPLETED -->
## Completed

**Change:** Changed assignment to accumulation in `src/input/input.cpp:75`
**Tests:** Build passing, manual verification successful
**Result:** ✓ Violation removed - all scroll events now accumulate
<!-- END: REFINE/COMPLETED -->
<!-- END: SELECT/SUCCESS -->
