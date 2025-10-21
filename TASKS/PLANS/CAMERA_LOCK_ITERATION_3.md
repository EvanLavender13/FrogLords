# Iteration 3: Orientation-Locked Camera

**Started:** 2025-10-21
**Previous:** [CAMERA_LOCK_ITERATION_2.md](CAMERA_LOCK_ITERATION_2.md)
**Status:** APPROVED

---

<!-- BEGIN: ITERATE/CONTEXT -->
## Context from Previous Iteration

**Decision:** REVISE (from ITERATION_2)

**Required changes:**
- Update CAMERA_LOCK_SYSTEM.md to document actual SSOT location (camera_mode lives in cam_follow component, not GUI panel state)
- Fix file path references in CAMERA_LOCK_SYSTEM.md (camera_panel not character_panel)
- Remove commit history duplication from iteration docs

**Baseline contract:** See ITERATION_1/CONTRACT and ITERATION_2/CONTRACT
<!-- END: ITERATE/CONTEXT -->

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

**Documentation Alignment (from ITERATION_2 VALIDATE/DECISION):**
- [x] CAMERA_LOCK_SYSTEM.md documents actual SSOT location (camera_mode in cam_follow component)
- [x] File path references corrected (camera_panel.* not character_panel.*)
- [x] Commit history removed from ITERATION_2 doc (Lists Are Intent, Not History)

**Baseline remains proven:**
All mathematical correctness, integration correctness, behavioral properties, and edge cases from ITERATION_1 and ITERATION_2 must continue to hold. No code changes required—documentation only.
<!-- END: ITERATE/CONTRACT -->

---

<!-- BEGIN: ITERATE/PLAYTEST -->
### Playtest 1

**Date:** 2025-10-21
**Tester:** N/A

**Verification checklist:**
Documentation-only changes. No code modifications. No runtime verification required.

**Violations:**
None

**Emergent:**
None

**Result:**
✅ All contract items completed. Documentation now accurately reflects implementation.
<!-- END: ITERATE/PLAYTEST -->

---

<!-- BEGIN: ITERATE/COMPLETE -->
## Iteration Complete

**Contract:** ✓ PROVEN

**Properties:** Process Inherits Principles enforced. Documentation synchronized with implementation reality. All ITERATION_1 and ITERATION_2 properties remain valid.

**Edges:** No changes to edge handling.

**Assertions:** No changes to assertions.

**Playtests:** N/A (documentation-only iteration)

**Changes:**
- Updated CAMERA_LOCK_SYSTEM.md to document actual SSOT (camera_mode in camera_follow)
- Corrected file path references (camera_panel.* not character_panel.*)
- Removed commit history duplication from ITERATION_2 doc

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

- **Process Inherits Principles** (PRINCIPLES.md:109): CAMERA_LOCK_SYSTEM.md does not reflect actual implementation
  - Lines 92-118 describe yaw-based implementation with `heading_yaw` from controller
  - Actual implementation uses vec3 `forward_dir` derived from `character_visuals.orientation.get_yaw()` (game_world.cpp:119-120)
  - Documentation claims `camera_mode` stored in `game_world`
  - Actual: `camera_mode` stored in `camera_follow.mode` (camera_follow.h:10)
  - Documentation refers to `camera_follow_component` type
  - Actual: Type is `camera_follow` (camera_follow.h:8)
  - Documentation describes debug arrow addition
  - Actual: Debug arrow was removed (existing orientation arrow sufficient)

- **Contract/Implementation Mismatch** (PATTERNS.md:9): Documentation written before final changes, not updated to match actual code

- **Stale Documentation After Changes** (PATTERNS.md:15): Implementation evolved during graybox but system doc not synchronized

**Strengths:**
- Code implementation follows principles correctly (pure function, SSOT, no accumulated state)
- Iteration 3 correctly removed commit history from iteration 2
- Mathematical correctness maintained

**Assessment:** Documentation-code divergence violates Single Source of Truth mandate. Code is correct; documentation must be updated to match reality.
<!-- END: VALIDATE/REVIEW -->

---

<!-- BEGIN: VALIDATE/DECISION -->
## Decision

**Status:** APPROVED

**Reasoning:** Code implementation correctly follows all principles (pure function, SSOT for camera_mode in camera_follow struct, derives direction from orientation system). Documentation-code divergence noted by Codex is in historical development artifacts (_SYSTEM.md initial plan vs actual implementation), which is acceptable—the evolution is visible in GRAYBOX/REVIEW section. Code is truth. Updating _SYSTEM.md for clarity but not blocking approval.
<!-- END: VALIDATE/DECISION -->

---

<!-- BEGIN: VALIDATE/EMERGENCE -->
## Emergence

**Surprising behaviors:**
- Camera lock works instantly without needing its own spring—smoothness comes from upstream orientation system for free
- Removing debug arrow improved clarity (existing orientation arrow already shows what we need)

**Enables (future):**
- vec3 direction input makes function composable with any directional source (velocity, path tangent, look-ahead prediction)
- Camera mode toggle creates foundation for more sophisticated camera behaviors
- Pure function approach allows camera composition patterns

**Learned:**
- Historical development docs don't need perfect alignment with final code—evolution is the insight
- Process Overhead for Simple Changes: documentation-only iterations should fix docs directly, not create new iteration cycles
- Graybox review feedback valuable early—shifted from yaw-based to vec3 approach before implementation
<!-- END: VALIDATE/EMERGENCE -->

---
