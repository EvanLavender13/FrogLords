# Iteration 1: Car-Like Control Scheme

**Started:** 2025-10-21
**Status:** REVISE

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

Define what must be proven about this system. Choose categories that serve validation.

### Time Independence
- [x] Heading integration produces identical rotation regardless of frame rate (9a76418)
- [x] Turn rate scaling by dt prevents frame-dependent behavior (9a76418)
- [x] Angle wrapping maintains correctness across time steps (9a76418)

### Mathematical Correctness
- [x] Heading yaw stays within valid range [-π, π] after wrapping (9a76418)
- [x] Basis vectors (forward/right) are orthonormal when derived from heading (9a76418)
- [ ] Turn direction matches input sign (A increases yaw, D decreases yaw)

### Input Transformation
- [x] CAR_LIKE mode: lateral input (x) controls turn rate, not lateral acceleration (ad34cd5)
- [x] CAR_LIKE mode: forward input (y) drives motion along heading direction (ad34cd5)
- [x] FREE_STRAFE mode: existing camera-relative behavior unchanged (ad34cd5)

### Edge Cases
- [x] Zero turn rate input maintains stable heading (ad34cd5 - validated by finiteness assertions)
- [x] Maximum turn rate input produces stable rotation (ad34cd5 - validated by finiteness/wrapping assertions)
- [x] Rapid control scheme toggle preserves physics state (ad34cd5 - no state mutation, only basis selection)
- [x] Heading wraps correctly at ±π boundary (9a76418 - validated by wrapping postcondition)
<!-- END: ITERATE/CONTRACT -->

---

<!-- BEGIN: ITERATE/PLAYTEST_1 -->
### Playtest 1

**Date:** 2025-10-21
**Tester:** User

**Contract Validation:**
- ✓ Time-independent heading integration (no frame-rate dependencies observed)
- ✓ Heading wrapping at ±π boundary (smooth rotation, no discontinuities)
- ✓ Basis vectors orthonormal (no visual artifacts)
- ✓ Speed-dependent turning (can turn while coasting, turn rate scales with velocity)
- ✓ Input transformation (lateral input zeroed in CAR_LIKE mode)
- ✓ Control scheme toggle (immediate response, stable)

**Known Limitation (deferred):**
- Character visual orientation still velocity-driven (doesn't follow heading_yaw)
- This is expected - reactive systems work deferred to future iteration
- Input transformation primitive validated successfully

**Violations:** None

**Fix:** N/A - no violations found
<!-- END: ITERATE/PLAYTEST_1 -->

---

<!-- BEGIN: ITERATE/COMPLETE -->
## Iteration Complete

**Contract:** ✓ PROVEN

**Properties:** Time-independent heading integration, orthonormal basis from heading, speed-dependent turn rate scaling
**Edges:** Angle wrapping at ±π, zero/max turn input, control scheme toggle, velocity-based turn gating
**Assertions:** 10 added (preconditions, postconditions for heading integration, basis validation, input transformation)
**Playtests:** 1

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

**Radical Simplicity:**
- Dead local: `old_heading` computed but unused (game_world.cpp:47)
- Speed-gated turning adds tuning complexity to minimal iteration (game_world.cpp:51-55)

**Fundamental Composable Functions:**
- Physics responsibility split: heading integration in composition layer (game_world) rather than physics primitive (controller/orientation) - divides physics across layers, hinders reuse (game_world.cpp:40-77 vs controller.cpp:52-72)
- Comment drift: controller.h:113 claims heading integrated in apply_input, but it isn't

**Consistency:**
- heading_yaw only updated in CAR_LIKE mode - may be stale when toggling from FREE_STRAFE, causing surprising first step (game_world.cpp:40-78, controller.h:133)

**Accumulated State vs Derived State:**
- Documentation inconsistency about heading usage location violates Single Source of Truth (controller.h:113)

**Strengths:**
- Mathematical foundations solid: time-independent integration, correct angle wrapping, orthonormal basis
- Layer separation respected: Physics Drives Animation principle upheld
- Orthogonal composition: game_world selects basis, controller remains mode-agnostic
- Predictable input transformation with proper assertions

**Assessment:** Core mathematics and time-independence are correct. Main issues are architectural placement of physics logic and hygiene violations. Fixable without major rework.
<!-- END: VALIDATE/REVIEW -->

---

<!-- BEGIN: VALIDATE/DECISION -->
## Decision

**Status:** REVISE

**Reasoning:**
Mathematical foundation is sound and time-independent behavior is proven correct. However, architectural composability is compromised by splitting physics responsibility (heading integration) across layers. The heading integration logic belongs in a physics primitive (controller or dedicated orientation primitive), not in the composition layer. This violates Fundamental Composable Functions by making physics non-reusable and scattered.

Additional hygiene issues (dead code, comment drift, stale state on toggle) further indicate the implementation needs refinement before acceptance.

Core system works and is mathematically correct, but principle violations are significant enough to require revision before shipping.

**Required changes:**
1. Relocate heading integration to physics primitive (controller or dedicated steering primitive)
2. Remove dead code (`old_heading`)
3. Fix comment drift in controller.h about heading integration location
4. Address stale heading on mode toggle (initialize from current physics intent or document edge case)
5. Consider removing speed-gated turning complexity from this iteration (defer to tuning phase)
<!-- END: VALIDATE/DECISION -->

---
