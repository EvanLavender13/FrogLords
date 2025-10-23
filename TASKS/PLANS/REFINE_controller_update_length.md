# Refinement: Controller Update Length

Extract sub-phases from monolithic update loop

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-23
**Complexity:** Standard (6 pts)
**Path:** B
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/character/controller.cpp:129`
**Principle:** Radical Simplicity
**Severity:** Low
**Type:** Function length - 163 lines (threshold 100)

**Current state:**
```
controller::update() - single function handling:
- Physics integration (gravity, drag, velocity, position)
- Collision resolution and grounding detection
- Landing event detection
- Jump timing forgiveness updates
- Locomotion state classification and phase calculation
```

**Why violation:**
- Cognitive overload: Five distinct phases in one function
- Testing: Cannot isolate phase behavior independently
- Maintenance: Changes to one phase require navigating entire function
- Reuse: Sub-phases locked inside monolithic structure

**Impact:**
- Blocks: Independent testing of locomotion classification, landing detection
- Cascades to: Future animation system integration, state machine clarity
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify

Extract five sub-phases into private helper methods called sequentially from update().

**From:** Monolithic 163-line update loop
**To:** Orchestrator calling five focused sub-phases

**Derive/Generalize:**
- Physics integration becomes `update_physics(dt)`
- Collision handling becomes `update_collision(world, dt)`
- Landing detection becomes `update_landing_state()`
- Jump timers become `update_jump_timers(dt)`
- Locomotion becomes `update_locomotion_state(dt)`

**Check duplicates:**
- No other functions perform these sub-phases
- Sub-phases remain private (single caller: update())

**Validation:**
- Behavior must remain identical (mathematical correctness)
- Epsilon comparison on position/velocity/state after extraction
- Verify debug visualization unchanged
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: REFINE/PLAN -->
## Refinement Plan

### Step 1: Extract physics integration
**Changes:** `src/character/controller.cpp:129-217` → Extract to `update_physics(dt)`
**Files:** controller.cpp, controller.h
**Details:** Includes gravity, drag, velocity integration, position update, acceleration reset at end
**Tests:** All tests must pass
**Validation:** Position/velocity identical before/after extraction

### Step 2: Extract collision resolution
**Changes:** `src/character/controller.cpp:218-240` → Extract to `update_collision(world, dt)` returning `float`
**Files:** controller.cpp, controller.h
**Details:** Returns `pre_collision_vertical_velocity` for landing detection
**Tests:** All tests must pass
**Validation:** Grounded state and debug info identical, pre-collision velocity captured

### Step 3: Extract landing detection
**Changes:** `src/character/controller.cpp:242-248` → Extract to `update_landing_state(float pre_collision_vy)`
**Files:** controller.cpp, controller.h
**Details:** Takes pre-collision vertical velocity from Step 2
**Tests:** All tests must pass
**Validation:** Landing events detected identically

### Step 4: Extract jump timer updates
**Changes:** `src/character/controller.cpp:250-256` → Extract to `update_jump_timers(dt)`
**Files:** controller.cpp, controller.h
**Tests:** All tests must pass
**Validation:** Timer behavior identical

### Step 5: Extract locomotion state update
**Changes:** `src/character/controller.cpp:258-288` → Extract to `update_locomotion_state(dt)`
**Files:** controller.cpp, controller.h
**Tests:** All tests must pass
**Validation:** Locomotion state and phase calculation identical

## Rollback
`git reset --hard HEAD` or `git revert HEAD~N` for last N commits
<!-- END: REFINE/PLAN -->

---

<!-- BEGIN: REFINE/REVIEW -->
## Second Opinion Review

**Tool:** Codex CLI
**Date:** 2025-10-23

**Question asked:**
Validate this refactoring plan for extracting sub-phases from controller::update(). Does the extraction preserve mathematical correctness? Are the boundaries clean? Any concerns about state dependencies between phases?

**Concerns evaluated:**
- Mathematical correctness preservation across phase boundaries
- Data flow between extracted phases
- Hidden state dependencies

**Feedback received:**
- **Data flow issue (controller.cpp:223)**: `update_collision` must return `pre_collision_vertical_velocity` for `update_landing_state` to use. Original plan showed parameter but not return value.
- **Acceleration reset (controller.cpp:291)**: Current code zeros `acceleration` at end to prevent gravity accumulation. Plan didn't specify where this reset lives after extraction.
- **Validation**: No other hidden state couplings. Boundaries align with comment blocks. Math preserves correctness if phases run in same order.

**Impact on implementation:**
- Modified Step 1: Added explicit note that `update_physics` includes acceleration reset at end
- Modified Step 2: Changed signature to return `float` (pre-collision vertical velocity)
- Modified Step 3: Confirmed parameter matches Step 2 return value
- No changes to Steps 4-5: Clean boundaries confirmed
<!-- END: REFINE/REVIEW -->

---

<!-- BEGIN: REFINE/COMPLETED -->
## Completed

**Date:** 2025-10-23

**Change:** Extracted five sub-phases from monolithic update() into focused helper methods

**Files Modified:**
- `src/character/controller.h`: Added 5 private method declarations
- `src/character/controller.cpp`: Extracted 5 methods, simplified update() from 163 lines to 10 lines

**Methods Extracted:**
1. `update_physics(dt)` - Physics integration: gravity, drag, velocity, position
2. `update_collision(world, dt)` - Collision resolution and grounding detection
3. `update_landing_state(pre_collision_vy)` - Landing event detection
4. `update_jump_timers(dt)` - Jump timing forgiveness (coyote time, buffering)
5. `update_locomotion_state(dt)` - Locomotion state classification and phase

**Tests:** All passing
**Manual Verification:** All behavior identical - physics, collision, landing, jump timing, locomotion

**Result:** ✓ Violation removed
- Function length: 163 lines → 10 lines
- Cognitive load: Reduced from 5 phases in one function to clear orchestration
- Testing: Each phase now independently testable
- Maintenance: Changes isolated to specific phase functions
<!-- END: REFINE/COMPLETED -->
