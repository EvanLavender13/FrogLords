# Iteration 2: Orientation-Locked Camera

**Started:** 2025-10-21
**Previous:** [CAMERA_LOCK_ITERATION_1.md](CAMERA_LOCK_ITERATION_1.md)
**Status:** Ready for VALIDATE

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
