# Refinement: Friction/Acceleration Frame-Rate Independence

Frame-rate dependent terminal velocity violates mathematical foundations and blocks dash mechanic.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-19
**Complexity:** Standard
**Path:** B (standard)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/character/controller.cpp:68,151`
**Principle:** Solid Mathematical Foundations, Consistency, Time-Independence | Score: Critical Foundation Error
**Severity:** Critical
**Type:** Sequential update produces frame-rate dependent equilibrium

**Current state:**
```cpp
// Line 68: Apply ground acceleration
velocity.x += ground_accel.x * dt;
velocity.z += ground_accel.z * dt;

// Line 151: Apply friction (later in update)
velocity.x -= velocity.x * drag * dt;
velocity.z -= velocity.z * drag * dt;
```

**Why violation:**
- Sequential update (add acceleration, then subtract friction) produces frame-rate dependent terminal velocity
- Equilibrium occurs at `max_speed - ground_accel * dt`, not `max_speed`
- At 60fps: ~7.67 m/s, at 30fps: ~7.33 m/s
- Unaccounted `drag * ground_accel * dt²` error term
- Violates Time-Independence pattern from PRINCIPLES.md
- Creates hidden frame-rate dependency that compounds over time

**Impact:**
- Blocks: Dash mechanic, any burst movement system requiring overspeed
- Cascades to: All Layer 4 movement variations
- Foundation: 99% → potentially unstable for movement mechanics
- Mathematical correctness violated - equilibrium is not analytically proven
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify

**CRITICAL:** Describe the approach conceptually. Use pseudocode and high-level explanations. Do NOT write actual source code—that happens during REFINE.

**SIMPLIFY:**

**From:**
- Sequential updates: acceleration then friction
- Two separate operations with dt applied independently
- Frame-rate dependent equilibrium point

**To:**
- Combined friction/acceleration update with analytically proven frame-rate independence
- Single operation deriving exact equilibrium

**Derive/Generalize:**

Derive combined update from differential equation:
```
dv/dt = accel - drag * v
```

Solve analytically for discrete timestep update that:
1. Produces exact equilibrium at `max_speed` regardless of frame rate
2. Eliminates the `drag * accel * dt²` error term
3. Converges smoothly to terminal velocity

The solution involves:
- Deriving closed-form solution for velocity update
- Proving equilibrium point is exactly `accel / drag` (which equals `max_speed`)
- Implementing single update operation instead of sequential

**Check duplicates:**
- Verify no other systems use sequential acceleration/friction pattern
- Ensure tuning parameters remain physically meaningful after derivation
- Validate that existing movement feel is preserved (or improved)

**Validation:**
- Add automated cross-FPS verification tests (30/60/120fps)
- Verify equilibrium velocity matches `max_speed` exactly
- Test convergence behavior at different frame rates
- Ensure debug assertions catch any violations

<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Violation resolved - frame-rate independent update implemented
- [ ] Principle score improved - mathematical correctness proven
- [ ] Tests passing - cross-FPS verification added
- [ ] No regressions - movement feel preserved

**Metrics:**
- Before: Sequential update, equilibrium varies with dt
- After: Analytical update, equilibrium exact at max_speed
- Mathematical proof: Yes
- Cross-FPS tests: 30/60/120fps verified identical
- Dash mechanic: Unblocked

<!-- END: SELECT/SUCCESS -->
