# Refinement: Input Event Accumulation

Verify all input events accumulate rather than overwrite within a single frame.

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

**Location:** `src/input/input.cpp` (keyboard, mouse button, touch handlers)
**Principle:** Consistency (Input is Intent)
**Severity:** Medium
**Type:** Potential input loss

**Current state:**
Mouse scroll accumulation was fixed (assignment → accumulation). Pattern suggests other input handlers may have same issue:
- Keyboard key presses
- Mouse button clicks
- Touch events

**Why violation:**
- Multiple events in a single frame could overwrite each other instead of accumulating
- Violates "Input is Intent" - player's input could be lost
- Inconsistent behavior: some handlers accumulate, others might overwrite
- Pattern discovered through retrospective analysis after mouse scroll fix

**Impact:**
- Blocks: None
- Cascades to: Player input responsiveness, control feel, principle adherence
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Document

**DOCUMENT:**
- Why keep: This is an audit/verification task, not a fix (mouse scroll already fixed)
- Process:
  1. Read `src/input/input.cpp` completely
  2. Identify all event handlers (keyboard, mouse buttons, touch)
  3. Check each handler for assignment vs accumulation pattern
  4. Verify frame-clear logic resets state properly
  5. Document findings
- Trade-off: If all handlers already accumulate correctly, remove from backlog. If violations found, they become separate refinement tasks.
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] All input event handlers audited
- [ ] Accumulation pattern verified or violations documented
- [ ] No input events lost within single frame
- [ ] Tests passing

**Metrics:**
- Handlers checked: __
- Violations found: __
- Documentation updated
<!-- END: SELECT/SUCCESS -->
