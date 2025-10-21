# Iteration 1: Orientation-Locked Camera

**Started:** 2025-10-21
**Status:** Ready for VALIDATE

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
