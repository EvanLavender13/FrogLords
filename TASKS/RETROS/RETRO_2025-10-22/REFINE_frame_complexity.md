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

---

<!-- BEGIN: REFINE/PLAN -->
## Refinement Plan

### Step 1: Extract parameter command dispatch
**Changes:**
- `src/app/runtime.h:22` - Add declaration after `render_world()`
- `src/app/runtime.cpp:113-147` - Extract switch to `apply_parameter_commands()`
**Details:**
- Add private method declaration: `void apply_parameter_commands(const std::vector<gui::parameter_command>& commands);`
- Define method in .cpp with lines 113-147 moved into it
- Replace original switch with call: `apply_parameter_commands(param_commands);`
- Method applies commands to `world.character`, `world.character_params`, `world.character_visuals`
- Preserve all invariant enforcement (apply_to calls)
**Tests:** `bash scripts/bash/dev.sh`
**Validation:** Build succeeds, app runs, character tuning controls work identically

### Step 2: Extract camera command dispatch
**Changes:**
- `src/app/runtime.h:23` - Add declaration after `apply_parameter_commands()`
- `src/app/runtime.cpp:154-180` - Extract switch to `apply_camera_commands()`
**Details:**
- Add private method declaration: `void apply_camera_commands(const std::vector<gui::camera_command>& commands);`
- Define method in .cpp with lines 154-180 moved into it
- Replace original switch with call: `apply_camera_commands(camera_commands);`
- Method applies commands to `world.cam_follow` with invariant enforcement (clamps)
- Preserve exact iteration order and all invariant clamps
**Tests:** `bash scripts/bash/dev.sh`
**Validation:** Build succeeds, app runs, camera controls work identically, min/max invariants enforced

### Step 3: Verify frame() semantic clarity
**Changes:** None (verification only)
**Details:**
- Read `frame()` method after extraction
- Verify structure is now clearly visible: input → update → GUI → command application → rendering
- Confirm ~80 lines of mechanical ceremony removed from main loop
**Tests:** `bash scripts/bash/dev.sh`
**Validation:** Build succeeds, code review confirms cognitive clarity

## Rollback
Per-file: `git checkout -- src/app/runtime.cpp src/app/runtime.h` or `git revert <commit-sha>` for individual steps
<!-- END: REFINE/PLAN -->

---

<!-- BEGIN: REFINE/REVIEW -->
## Second Opinion Review

**Tool:** Codex CLI
**Date:** 2025-10-22

**Question asked:**
Validate this refinement plan. Does the extraction of command dispatch into dedicated functions properly restore Fundamental Composable Functions principle? Are there concerns about the approach, rollback strategy, or validation steps? Will the extracted methods reduce cognitive complexity while maintaining identical behavior?

**Concerns evaluated:**
- Does extraction properly restore the principle?
- Is the approach sound for reducing cognitive load?
- Are rollback and validation steps adequate?
- Will behavior remain identical?

**Feedback received:**
- Extraction will strip mechanical ceremony from frame(), revealing semantic flow (confirms principle restoration)
- Must add declarations to runtime.h private section (missing from original plan)
- Behavior preservation requires same iteration order and preserving invariant enforcement (clamps, apply_to calls)
- Should explicitly run build command rather than implicit "Build succeeds"
- Rollback should use scoped `git checkout` instead of `git reset --hard`

**Impact on implementation:**
- Added header file updates to both steps
- Made build validation explicit with `bash scripts/bash/dev.sh`
- Changed rollback to scoped checkout
- Added explicit notes about preserving invariant enforcement in both methods
<!-- END: REFINE/REVIEW -->

---

<!-- BEGIN: REFINE/COMPLETED -->
## Completed

**Change:** Extracted two mechanical switch statements from frame() into dedicated methods
**Files Modified:**
- `src/app/runtime.h:22-23` - Added two private method declarations
- `src/app/runtime.cpp:113,120` - Replaced switches with method calls
- `src/app/runtime.cpp:177-243` - Added apply_parameter_commands() and apply_camera_commands() definitions

**Line changes:**
- frame() reduced from ~130 lines to ~70 lines
- Removed ~62 lines of mechanical ceremony (35 + 27 line switch statements)
- Added 2 semantic function calls revealing unidirectional flow

**Tests:** All passing (manual verification confirmed)

**Semantic clarity achieved:**
- Game loop structure immediately visible: input → update → GUI → command application → rendering
- Mechanical details delegated to appropriately named methods
- Unidirectional flow pattern now obvious
- Cognitive complexity dramatically reduced

**Result:** ✓ Violation removed, Fundamental Composable Functions principle restored
<!-- END: REFINE/COMPLETED -->

---

<!-- BEGIN: MEASURE/METRICS -->
## Metrics

**Files:**
- `src/app/runtime.h`: 41 → 43 (+2 lines)
- `src/app/runtime.cpp`: 240 → 248 (+8 lines)

**frame() function (measured by lizard):**
- Lines: 131 → 71 (-60, -46%)
- NLOC: 102 → 45 (-57, -56%)
- CCN: 23 → 7 (-16, -70%)
- **Status: Above threshold (CCN>15) → Below threshold ✓**

**Extracted methods:**
- `apply_parameter_commands()`: CCN=11, Length=37 (below threshold)
- `apply_camera_commands()`: CCN=7, Length=29 (below threshold)

**Violations removed:**
- Case statements in frame(): 14 → 0
- Mechanical switch statements: 2 → 0
- CCN violations: 1 → 0
<!-- END: MEASURE/METRICS -->

---

<!-- BEGIN: MEASURE/LEARNING -->
## Learning

**Root cause:** Convenience - easier to inline command dispatch than extract during initial implementation; no complexity check enforced at time of writing.

**Prevention:** Run `bash scripts/bash/analyze.sh` after implementing any function >50 lines. Extract immediately when CCN>15 detected.

**Pattern:** Command pattern naturally segregates mechanical dispatch from semantic orchestration - applies to all GUI command handlers.
<!-- END: MEASURE/LEARNING -->
