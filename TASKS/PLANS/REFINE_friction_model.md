# Refinement: Friction Model

**Replace non-physical friction calculation with simple kinetic friction coefficient**

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-18
**Complexity:** Standard (3-5 pts)
**Path:** B (Standard)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/character/tuning.cpp:8-38`
**Principle:** Mathematical Foundations | Score: 6/10
**Severity:** Medium
**Type:** Unjustified decision, Non-physical model

**Current state:**
```cpp
// Reverse-engineers friction from desired acceleration ratio
constexpr float FRICTION_RATIO = 0.75f;
constexpr float NET_FRACTION = 0.25f;

// Inflates ground_accel to compensate for friction
c.ground_accel = desired_accel / NET_FRACTION;  // 20 / 0.25 = 80 m/s²
c.friction = (c.ground_accel * FRICTION_RATIO) / gravity_mag;  // μ ≈ 6.12 (!!)
```

**Why violation:**
- Produces physically impossible friction coefficient (μ ≈ 6.12, real-world max ≈ 1.0)
- Backwards calculation: starts from desired ratio, derives friction
- Dimensionally strange: friction calculated from acceleration ratio
- No physical interpretation or real-world analogue
- Violates "Mathematics is truth, Physics is law" (Principle #3)

**Impact:**
- Blocks: Tuning predictability (can't reason about "0.75 friction ratio")
- Cascades to: All ground movement behavior
- Foundation: Layer 1 (Core) - affects character tuning certainty
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify - Replace with standard kinetic friction

**Physical Model:**

Standard kinetic friction opposes motion:
```
F_friction = μ * |F_normal|
```

For ground movement (normal force = gravity):
```
F_friction = μ * m * |g|
a_friction = μ * |g|  (force per unit mass)
```

**Current System (Non-Physical):**
1. Choose FRICTION_RATIO = 0.75 (arbitrary)
2. Inflate ground_accel = 80 m/s² (to compensate for friction)
3. Calculate friction coefficient from ratio
4. Apply: decel = friction * |g| ≈ 6.12 * 9.8 ≈ 60 m/s²
5. Net result: 80 - 60 = 20 m/s² (desired acceleration)

**New System (Physical):**
1. Choose friction_coefficient μ = 0.7 (rubber-like, physically valid)
2. Set ground_accel = 20 m/s² directly (no inflation)
3. Apply: decel = μ * |g| = 0.7 * 9.8 ≈ 6.86 m/s²
4. Net result: Input-dependent (player controls acceleration, friction opposes)

**Key Difference:**
- **Current**: Friction is pre-baked into inflated acceleration
- **Physical**: Friction is independent force that opposes motion

**Implementation Changes:**

**1. tuning.h - Add friction parameter:**
```
Add tunable parameter:
  float friction_coefficient = 0.7f;  // dimensionless, range [0.0, 1.0]

Document physical meaning:
  - 0.0 = ice (no friction)
  - 0.7 = rubber on asphalt (snappy)
  - 1.0 = maximum practical friction
```

**2. tuning.cpp - Simplify apply_to():**
```
Remove:
  - FRICTION_RATIO constant
  - NET_FRACTION constant
  - Acceleration inflation logic

Change ground_accel calculation:
  From: c.ground_accel = desired_accel / NET_FRACTION;
  To:   c.ground_accel = desired_accel;

Change friction assignment:
  From: c.friction = (c.ground_accel * FRICTION_RATIO) / gravity_mag;
  To:   c.friction = friction_coefficient;
```

**3. controller.cpp - No changes needed:**
```
Friction application already physically correct:
  friction_decel = friction * |gravity| * dt

This now becomes:
  friction_decel = μ * |g| * dt  (standard kinetic friction!)
```

**Tuning Impact:**

Current feel comes from net 20 m/s² acceleration:
- ground_accel = 80 m/s² (inflated)
- friction_decel = 60 m/s² (huge)
- net = 20 m/s² when moving

New model with μ = 0.7:
- ground_accel = 20 m/s² (direct)
- friction_decel = 6.86 m/s² (realistic)
- net when accelerating = ~13 m/s² (faster)
- stopping behavior = 6.86 m/s² decel (gentler)

**To match current feel**, reduce ground_accel proportionally:
- If current net ≈ 20 m/s², set ground_accel ≈ 27 m/s² with μ = 0.7
- Or keep ground_accel = 20 m/s² and reduce μ to ~0.3 (more slippery)

**Tuning exploration needed** - try values and iterate.

<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Violation resolved (friction coefficient physically valid 0.0-1.0)
- [ ] Principle score improved (Mathematical Foundations 6→9)
- [ ] Tests passing
- [ ] No regressions
- [ ] Game feel acceptable (may need tuning iteration)

**Metrics:**
- Before: LOC 42, Mathematical Foundations 6/10, μ = 6.12 (impossible)
- After: LOC ~30 (-12), Mathematical Foundations 9/10 (+3), μ = 0.0-1.0 (physical)

**Validation:**
- Friction coefficient in range [0.0, 1.0] ✓
- No acceleration inflation logic ✓
- Derivation from standard physics ✓
- Tunable via simple, understandable parameter ✓

<!-- END: SELECT/SUCCESS -->
