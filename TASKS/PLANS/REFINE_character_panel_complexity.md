# Refinement: Character Panel Complexity

Decompose monolithic panel function into composable sections

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-23
**Complexity:** Trivial (2 pts)
**Path:** A
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/gui/character_panel.cpp:9`
**Principle:** Radical Simplicity
**Severity:** Low
**Type:** Function Complexity - CCN 20 (threshold 15), Length 116 (threshold 100)

**Current state:**
Single function handles four distinct sections: Character Tuning, Landing Spring, Orientation, Locomotion State. Each section follows identical pattern: collapsing header check, local copies, widget calls, command generation.

**Why violation:**
- Cognitive complexity (CCN 20) exceeds threshold by 33%
- Four distinct concerns bundled into single scope
- Pattern repetition suggests extractable abstraction
- Length hinders scanning and navigation

**Impact:**
- Blocks: Clean addition of new character parameter sections
- Cascades to: Other panel implementations may follow this pattern
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify

**FROM:** Single function with four inline sections
**TO:** Main function delegates to four section helpers

**Structure:**
Main function iterates section helpers, each returns commands for their domain. Helper signature matches pattern: take const state refs, return command vector.

**Section boundaries:**
1. Character Tuning - movement parameters + jump forgiveness + derived displays
2. Landing Spring - animation spring parameters + state display
3. Orientation - orientation spring parameters + state display
4. Locomotion State - speed tier + phase + cycle length display

**Verify:**
- Each helper has single concern
- No duplicate local copy patterns across helpers
- Command aggregation remains in main function
- Zero behavior change in output commands
<!-- END: SELECT/FIX -->
