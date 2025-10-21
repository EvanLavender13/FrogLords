# Iteration 1: Orientation-Locked Camera

**Started:** 2025-10-21
**Status:** REVISE

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

Define what must be proven about this system.

**Mathematical Correctness:**
- [x] Locked position calculation produces finite results for all valid inputs
- [x] Zero-length direction vector is rejected (assertion fires)
- [x] Negative distance is rejected (assertion fires)
- [x] Camera positioning matches expected geometry (behind direction at correct distance)

**Integration Correctness:**
- [x] Mode toggle switches between free orbit and locked behavior
- [x] Locked mode uses orientation system's smoothed yaw (not raw controller input)
- [x] Zoom controls work identically in both modes
- [x] Height offset applies correctly in locked mode
- [x] Look target remains consistent across mode switches

**Behavioral Properties:**
- [x] Locked camera snaps instantly (no additional smoothing beyond upstream orientation)
- [x] Free orbit mode unaffected by lock implementation
- [x] Direction vector derived once per frame (no redundant computation)
- [x] No accumulated state in camera lock (pure function verification)

**Edge Cases:**
- [x] Switching modes during motion maintains stability
- [x] Extreme zoom values (min/max) work in locked mode
- [x] Rapid orientation changes produce expected camera response
<!-- END: ITERATE/CONTRACT -->

---

<!-- BEGIN: ITERATE/PLAYTEST_1 -->
### Playtest 1

**Date:** 2025-10-21
**Tester:** User

**Violations:**
- [x] Duplicate camera panel - two camera control panels in GUI
  - Root cause: Camera mode incorrectly placed in character_panel instead of camera_panel
  - Fix: Moved camera_mode to camera_follow.h, added MODE to camera_command pattern, removed duplicate from character_panel ✓ FIXED
  - Commits: f1bc090

**Emergent:**
- Yellow heading arrow (car control) had fixed length while green/red arrows scaled with speed
- Improved: Yellow arrow now scales with horizontal speed for visual consistency

**Fix:**
- Files changed: src/gui/camera_panel.h, src/gui/camera_panel.cpp, src/gui/character_panel.h, src/gui/camera_command.h, src/camera/camera_follow.h, src/app/game_world.h, src/app/game_world.cpp, src/app/runtime.cpp, src/app/debug_generation.cpp
- Commits: f1bc090 (SSOT fix), 21fdc15 (debug viz improvement)
- ✅ VERIFIED: All contract items pass
<!-- END: ITERATE/PLAYTEST_1 -->

---

<!-- BEGIN: ITERATE/COMPLETE -->
## Iteration Complete

**Contract:** ✓ PROVEN

**Properties:** All mathematical correctness validated through debug assertions (finite outputs, valid inputs). Integration correctness confirmed through manual testing (mode switching, zoom, orientation source). Behavioral properties verified (instant snap, pure function, single computation).

**Edges:** Mode switching during motion stable, extreme zoom values work, rapid orientation changes handled correctly.

**Assertions:** 8 preconditions + 3 postconditions in camera_follow.cpp covering direction normalization, distance bounds, finite outputs.

**Playtests:** 1 (found and fixed SSOT violation)

**Status:**
- [x] Contract proven
- [x] Stable
- [x] Ready for VALIDATE
<!-- END: ITERATE/COMPLETE -->

---

<!-- BEGIN: VALIDATE/REVIEW -->
## External Review

**Tool:** Codex CLI
**Date:** 2025-10-21

**Principle Violations:**
- **Radical Simplicity**: Unused `min_dist`/`max_dist` fields in `camera_panel_state` (src/gui/camera_panel.h:12-13) duplicate concept and risk drift
- **Radical Simplicity**: Inline trig for heading basis (src/app/game_world.cpp:74-75) should use `math::yaw_to_forward/right` helpers for consistency
- **Minor duplication**: Camera target height offset computed in two places (acceptable but could be unified)

**Strengths:**
- Pure function is stateless and deterministic (src/camera/camera_follow.cpp:81-102)
- Single Source of Truth maintained for camera_mode
- Consistent behavior across modes with predictable results
- Minimal core with no extra state or smoothing
- Zero-length direction vectors properly rejected via preconditions

**Assessment:** Core principles upheld. Pure function implementation is exemplary. SSOT maintained for primary state. Minor cleanup needed to fully align with Radical Simplicity. Foundation is sound and violations are fixable.
<!-- END: VALIDATE/REVIEW -->

<!-- BEGIN: VALIDATE/DECISION -->
## Decision

**Status:** REVISE

**Reasoning:** Core implementation exemplifies Pure Functions and Consistency principles. SSOT maintained for primary state (camera_mode). However, unused GUI fields violate both Radical Simplicity ("could anything be removed?") and SSOT (duplicate concept). Inline trig inconsistent with established math helpers. These are fixable refinements that strengthen principle adherence without touching the proven foundation.

**Required changes:**
- Remove unused `min_dist`/`max_dist` from `camera_panel_state` (GUI cleanup)
- Replace inline trig with `math::yaw_to_forward/right` helpers in heading basis construction
- Optional: Unify camera target height offset computation
<!-- END: VALIDATE/DECISION -->

---
