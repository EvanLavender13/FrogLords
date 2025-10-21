# Iteration 2: Orientation-Locked Camera

**Started:** 2025-10-21
**Previous:** [CAMERA_LOCK_ITERATION_1.md](CAMERA_LOCK_ITERATION_1.md)
**Status:** REVISE

---

<!-- BEGIN: ITERATE/CONTEXT -->
## Context from Previous Iteration

**Decision:** REVISE (from ITERATION_1)

**Required changes:**
- Remove unused `min_dist`/`max_dist` from `camera_panel_state` (GUI cleanup)
- Replace inline trig with `math::yaw_to_forward/right` helpers in heading basis construction
- Optional: Unify camera target height offset computation

**Baseline contract:** See ITERATION_1/CONTRACT
<!-- END: ITERATE/CONTEXT -->

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

**Refinement (from ITERATION_1):**
- [x] Unused fields removed from camera_panel_state (Radical Simplicity)
- [x] Inline trig replaced with math helpers (Consistency)
- [x] All ITERATION_1 contract items remain valid (no regression)

**Baseline remains proven:**
All mathematical correctness, integration correctness, behavioral properties, and edge cases from ITERATION_1 must continue to hold.
<!-- END: ITERATE/CONTRACT -->

---

<!-- BEGIN: ITERATE/PLAYTEST -->
### Playtest 1

**Date:** 2025-10-21
**Tester:** User

**Verification checklist:**
- [x] W key - Character moves forward in facing direction
- [x] A/D keys - Character turns left/right (not strafing)
- [x] Controls feel natural and responsive
- [x] No reversed/inverted controls
- [x] Free strafe mode (WASD camera-relative) unaffected
- [x] Camera lock mode works correctly
- [x] No assertion failures

**Violations:**
None

**Emergent:**
None - refinement only

**Result:**
✅ All contract items verified. No regressions from ITERATION_1.
<!-- END: ITERATE/PLAYTEST -->

---

<!-- BEGIN: ITERATE/COMPLETE -->
## Iteration Complete

**Contract:** ✓ PROVEN

**Properties:** Radical Simplicity enforced (unused fields removed). Consistency enforced (math helpers replace inline trig). All ITERATION_1 properties remain valid.

**Edges:** No new edge cases introduced. All ITERATION_1 edge handling remains valid.

**Assertions:** No new assertions needed. Existing orthonormality checks validate helper correctness.

**Playtests:** 1 (verified no regression, correct control behavior)

**Changes:**
- Removed unused `min_dist`/`max_dist` from camera_panel_state (src/gui/camera_panel.h)
- Replaced inline trig with `math::yaw_to_forward/right` helpers (src/app/game_world.cpp:74-75)

**Commits:**
- e879aed - Remove unused fields
- f83c1b2 - Use math helpers

**Status:**
- [x] Contract proven
- [x] Stable
- [x] Ready for VALIDATE
<!-- END: ITERATE/COMPLETE -->

---

<!-- BEGIN: VALIDATE/REVIEW -->
## External Review

**Tool:** Codex
**Date:** 2025-10-21

**Principle Violations:**
- **Process Inherits Principles (SSOT)**: Plan documentation claims camera_mode should live in GUI panel state (CAMERA_LOCK_SYSTEM.md:104,116,140), but implementation stores it in camera_follow component (src/camera/camera_follow.h:5,10; src/app/runtime.cpp:176). This is Contract/Implementation Mismatch (PATTERNS.md:9). The code has a single source (cam_follow.mode), so runtime behavior is consistent, but docs are out of sync.
- **Lists Are Intent, Not History**: Iteration doc lists commit IDs (lines 82-84), duplicating git history. Per CONVENTIONS.md:54-59 and PRINCIPLES.md:94-99, history belongs to git; docs should not duplicate it.
- **Plan Section Divergence**: CAMERA_LOCK_SYSTEM.md references `gui/character_panel.*` (line 104-107) but actual system uses `gui/camera_panel.*`. Impact section claims SSOT in panel_state (line 140) but code differs.

**Strengths:**
- Mathematical correctness: Direction-lock formula is correct and implemented as pure function with proper pre/postcondition checks (src/camera/camera_follow.cpp:95-101)
- Orthonormal basis construction validated for car-like input using yaw_to_forward/right helpers (src/app/game_world.cpp:73-80)
- Radical Simplicity: Pure, composable math core with minimal parameters; no accumulated state
- Consistency: Deterministic mapping produces consistent output; pure function plus explicit branch
- Clean data flow: Direction derived once per frame, reused in composition layer
- Assertions enforce invariants throughout

**Assessment:**
Implementation is excellent: math is correct, behavior is consistent and simple, composition is sound. Core design aligns strongly with Six Pillars. Violations are documentation/process drift issues, not fundamental design flaws. The code itself demonstrates proper adherence to principles; the plans need to be synchronized with reality.

**Notable:** Referenced src/gameplay/camera_controller.{h,cpp} do not exist; actual implementation is in src/app/game_world.cpp:117 and src/camera/camera_follow.{h,cpp}.
<!-- END: VALIDATE/REVIEW -->

---

<!-- BEGIN: VALIDATE/DECISION -->
## Decision

**Status:** REVISE

**Reasoning:** Implementation demonstrates excellent adherence to the Six Pillars—math is correct, behavior is consistent, composition is sound. All violations are documentation/process drift issues, not fundamental design flaws. The code already follows principles; documentation must be synchronized with implemented reality. These are fixable without changing any implementation.

**Required changes:**
- Update CAMERA_LOCK_SYSTEM.md to document actual SSOT location (camera_mode lives in cam_follow component, not GUI panel state)
- Fix file path references in CAMERA_LOCK_SYSTEM.md (camera_panel not character_panel)
- Remove commit history duplication from iteration docs (lines 82-84)
<!-- END: VALIDATE/DECISION -->

---
