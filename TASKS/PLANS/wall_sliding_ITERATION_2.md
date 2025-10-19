# Iteration 2: Wall Sliding

**Started:** 2025-10-19
**Previous:** [ITERATION_1.md](wall_sliding_ITERATION_1.md)
**Status:** In Progress

---

<!-- BEGIN: ITERATE/CONTEXT -->
## Context from Previous Iteration

**Decision:** REVISE (from ITERATION_1)

**Required changes:**

1. **Fix Single Source of Truth** (CRITICAL):
   - `WALL_THRESHOLD = 0.707f` hardcoded in collision.cpp
   - `max_slope_angle` exists in controller
   - Systems will silently disagree if tuned
   - Fix: Derive WALL_THRESHOLD from authoritative slope angle OR pass as parameter

2. **Address degenerate contact handling** (HIGH):
   - `safe_normalize` fallback to UP (math_utils.h:59-72)
   - Deep wall penetrations misclassified as floors
   - Can spike players upward instead of sliding
   - Fix: Validate fallback behavior, add assertion or alternative

3. **Document plan deviation** (MEDIUM):
   - Plan promised normal averaging (wall_sliding_SYSTEM.md:53)
   - Implementation uses sequential iteration
   - Fix: Justify sequential approach OR implement averaging

4. **Fix debug visualization** (LOW):
   - `final_contact` shows only last contact
   - Floor+wall contacts appear as "wall" in UI
   - Fix: Represent combined state OR document limitation

**Baseline contract:** See ITERATION_1/CONTRACT

**Previous scores:**
- Self: 9.3/10
- Gemini: 9.3/10
- Codex: 7.8/10 (critical violations)
- Consensus: 7.8/10

**Target:** Fix violations → achieve ≥8.5/10 average, all scores ≥7.0
<!-- END: ITERATE/CONTEXT -->

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

**Mathematical properties:**
- [x] Surface classification consistent: `abs(dot(normal, up)) < threshold` with single source threshold
  - Fixed (16d2e8b): Added `wall_threshold` parameter derived from `max_slope_angle`
  - Controller calculates once, collision system uses consistently
- [x] Projection preserves tangent motion: `v_tangent = v - n * dot(v, n)` removes only normal component ✓ (from ITERATION_1)
- [x] Projection magnitude bounded: `|v_projected| ≤ |v_original|` ✓ Assertion (from ITERATION_1)
- [x] Zero normal velocity after projection: `dot(v_projected, normal) ≈ 0` ✓ Assertion (from ITERATION_1)
- [x] Degenerate contacts handled: Deep penetrations don't misclassify or spike player
  - Fixed (0b6c09b): `compute_face_normal()` uses geometric context instead of UP fallback
  - Fallback respects actual contact surface (wall remains wall, floor remains floor)

**Edge cases:**
- [x] 90° wall collision (pure normal): projected velocity = zero ✓ (from ITERATION_1)
- [x] Parallel to wall (pure tangent): projected velocity = original ✓ (from ITERATION_1)
- [x] Inside corner (opposing normals): resolution handles correctly ✓ (from ITERATION_1)
- [x] Multiple simultaneous walls: iteration resolves complex scenarios
  - Documented (a0d88ed): Sequential iteration handles N-walls, deterministic
- [x] Wall-to-floor transition: single threshold value used consistently
  - Fixed (16d2e8b): Single source (`max_slope_angle` → `wall_threshold`)

**Consistency:**
- [x] Same input → same output ✓ (from ITERATION_1)
- [x] Frame-rate independent ✓ (from ITERATION_1)
- [x] No special cases ✓ (from ITERATION_1)
- [x] Single source of truth for all thresholds
  - Fixed (16d2e8b): Removed hardcoded `WALL_THRESHOLD`, derive from `max_slope_angle`
- [x] Radical Simplicity applied
  - Fixed (a0d88ed): Removed unused `velocity_before`/`velocity_after` debug fields
<!-- END: ITERATE/CONTRACT -->

---
