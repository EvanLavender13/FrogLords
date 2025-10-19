# Iteration 2: Wall Sliding

**Started:** 2025-10-19
**Previous:** [ITERATION_1.md](wall_sliding_ITERATION_1.md)
**Status:** In Progress

---

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

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

**Mathematical properties:**
- [ ] Surface classification consistent: `abs(dot(normal, up)) < threshold` with single source threshold
- [ ] Projection preserves tangent motion: `v_tangent = v - n * dot(v, n)` removes only normal component
- [ ] Projection magnitude bounded: `|v_projected| ≤ |v_original|` (never amplifies velocity)
- [ ] Zero normal velocity after projection: `dot(v_projected, normal) ≈ 0` (within epsilon)
- [ ] Degenerate contacts handled: Deep penetrations don't misclassify or spike player

**Edge cases:**
- [ ] 90° wall collision (pure normal): projected velocity = zero (graceful stop)
- [ ] Parallel to wall (pure tangent): projected velocity = original (no change)
- [ ] Inside corner (opposing normals): resolution handles opposing normals correctly
- [ ] Multiple simultaneous walls: iteration resolves complex scenarios without order-dependence
- [ ] Wall-to-floor transition: single threshold value used consistently

**Consistency:**
- [ ] Same input → same output (deterministic projection)
- [ ] Frame-rate independent (no velocity accumulation artifacts)
- [ ] No special cases (single projection formula for all walls)
- [ ] Single source of truth for all thresholds
<!-- END: ITERATE/CONTRACT -->

---
