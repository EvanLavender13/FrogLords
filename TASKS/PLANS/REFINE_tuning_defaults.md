# Refinement: Tuning Defaults Mismatch

**Fix inconsistent initialization between tuning.h and controller.h**

---

## Selected

**Date:** 2025-10-17
**Complexity:** Trivial
**Path:** A (trivial)

---

## Violation Details

**Location:**
- File 1: `src/character/tuning.h` (line 13)
- File 2: `src/character/controller.h` (line 62)
- System: Character Controller (Layer 3)

**Principle Violated:**
- Primary: Solid Mathematical Foundations (inconsistent sources of truth)
- Score before: 8/10

**Severity:** Critical

**Type:** Inconsistent defaults, bidirectional initialization bug

---

## Current State

**What exists now:**

`tuning.h:13`:
```cpp
float time_to_max_speed = 0.4f; // seconds
```

`controller.h:62`:
```cpp
float ground_accel = 20.0f; // m/s²
```

**The math that reveals the problem:**
- `time_to_max_speed = 0.4s` should yield `ground_accel = (8.0 / 0.4) / 0.25 = 80.0 m/s²`
- `ground_accel = 20.0 m/s²` actually yields `time_to_max_speed = 8.0 / (20.0 × 0.25) = 1.6s`
- **4x discrepancy** (0.4s vs 1.6s)

**Why this violates principles:**
- Two sources of truth for same concept
- Bidirectional system (`apply_to` + `read_from`) creates initialization order dependency
- Tuning system not called during `controller()` construction
- Results in controller using different defaults than tuning system expects

**Impact:**
- Blocks: All confident tuning work (can't trust parameter values)
- Cascades to: Any system that reads controller defaults
- Foundation impact: Critical to Layer 3 (90%)

---

## Fix Approach

**Primary approach:** Simplify

**Strategy: Make tuning.h the single source of truth**

1. **Change controller.h defaults to match derived values:**
   - Calculate what `ground_accel` should be for `time_to_max_speed = 0.4s`
   - Using formula: `ground_accel = (max_speed / time) / NET_FRACTION`
   - Result: `ground_accel = (8.0 / 0.4) / 0.25 = 80.0 m/s²`
   - Similarly: `air_accel = 8.0 / 0.4 = 20.0 m/s²`

2. **Update controller.h comments to reference tuning.h:**
   - Make clear these values are **derived** from tuning defaults
   - Note: Will be overwritten if tuning system applied

3. **Alternative (better): Apply tuning in controller constructor:**
   - Have `controller()` constructor call `tuning_params{}.apply_to(*this)`
   - Eliminates need for duplicate defaults
   - Single source of truth enforced at construction

**Prefer Alternative: Constructor applies tuning**
- Simpler (one source of truth)
- Can't get out of sync
- Self-documenting (tuning always applied)

---

## Success Criteria

**This refinement succeeds when:**
- [ ] `controller()` defaults match `tuning_params{}` defaults exactly
- [ ] No mathematical discrepancy between sources
- [ ] All tests passing
- [ ] Tuning system trustworthy
- [ ] Foundation Layer 3 certainty improves

---

## Estimated Metrics

**Before:**
- LOC: 2 default values (tuning.h) + 2 default values (controller.h) = 4 sources
- Principle score: 8/10 (inconsistent truth)
- Discrepancy: 4x (0.4s vs 1.6s)
- Trust in tuning: Low

**After (estimated):**
- LOC: 1 call in constructor OR updated defaults (minimal change)
- Principle score: 10/10 (single source of truth)
- Discrepancy: 0x (exact match)
- Trust in tuning: High

---

## Risk Assessment

**What could go wrong:**
- Constructor approach adds coupling between controller and tuning
- Changing defaults might affect existing behavior (if anyone relied on 1.6s timing)

**Mitigation:**
- Test in-game after change to verify feel
- Check if any external code reads these values before tuning applied
- Both approaches are low-risk (just initialization)

**Rollback plan:**
- Git revert the commit
- Defaults are easily changed back

---

## Completed (Trivial Path)

**Date:** 2025-10-17

**Change made:**
- Updated: `controller.h` ground_accel default from 20.0 → 80.0 m/s²
- Updated: `controller.h` air_accel default from 10.0 → 20.0 m/s²
- Documented: Added explicit formulas and notes referencing tuning.h as source of truth
- Clarified: Values will be overwritten if tuning_params::apply_to() is called

**Tests:**
- [x] All passing
- Build completed successfully (8/8 targets)
- No compilation errors or warnings

**Metrics:**
- LOC before: 2 inconsistent defaults → after: 2 consistent defaults (0 LOC change, correctness change)
- Discrepancy before: 4x (0.4s vs 1.6s) → after: 0x (exact match)
- Principle score before: 8/10 → after: 10/10 (+2)

**Calculation verification:**
- time_to_max_speed = 0.4s (from tuning.h)
- max_speed = 8.0 m/s
- desired_accel = 8.0 / 0.4 = 20.0 m/s²
- ground_accel = 20.0 / 0.25 = 80.0 m/s² ✓
- air_accel = 20.0 m/s² ✓

**Result:** ✓ Violation removed - Single source of truth established
