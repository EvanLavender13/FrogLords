# Refinement: Frame Complexity

Extract mechanical command dispatch to reveal game loop structure.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-22
**Complexity:** Standard
**Path:** B (standard)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/app/runtime.cpp:61`
**Principle:** Fundamental Composable Functions
**Severity:** Low
**Type:** Function does too many things, mechanical details obscure semantic structure

**Current state:**
Function `app_runtime::frame()` is 130 lines with CCN 22. Orchestrates game loop but buries structure in ~80 lines of mechanical switch statement command dispatch.

**Why violation:**
- Function does 5 distinct things: mouse tracking, world update, GUI rendering, command dispatch (×2), final rendering
- Mechanical boilerplate (case X: apply value; break) obscures semantic flow
- Cognitive demand to parse switches hides game loop insight
- "Delete until only insight remains" - the insight is hidden by ceremony

**Impact:**
- Blocks: Clear understanding of main loop structure
- Cascades to: Future developers reading frame() must mentally filter noise to find structure
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify

**SIMPLIFY:**
- From: frame() handles orchestration + mechanical command dispatch
- To: frame() shows pure structure, delegates mechanical application
- Derive/Generalize: Extract parameter command dispatch and camera command dispatch into dedicated functions that apply commands to world state
- Check duplicates: Both switch statements follow identical pattern (iterate commands, switch on type, apply to world state)

**Validation:**
- Read extracted function signatures - semantic clarity improved
- Read frame() after extraction - game loop structure immediately obvious
- Verify behavior unchanged - same commands applied in same order
<!-- END: SELECT/FIX -->
