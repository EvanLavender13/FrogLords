# Iteration 1: Dynamic FOV

**Started:** 2025-10-25
**Status:** In Progress

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

Mathematical properties and edge cases that must be proven for production stability.

### Mathematical Correctness
- [ ] FOV output always within [base_fov, base_fov + max_fov_range] for all inputs
- [ ] Division by zero prevented (max_speed == 0 case handled)
- [ ] Speed factor saturates to [0, 1] even when speed > max_speed
- [ ] Lateral g-force contribution bounded (no unbounded FOV from extreme g-forces)
- [ ] Formula produces deterministic output (same inputs → same output)

### Edge Cases
- [ ] Stationary vehicle (speed = 0): FOV equals base_fov
- [ ] Max speed straight line: FOV equals base_fov + max_fov_range (no g contribution)
- [ ] Tight turn at low speed: FOV equals base_fov + g_contribution (clamped)
- [ ] Combined max speed + max g-force: FOV clamped to ceiling (base + range)
- [ ] Negative speed (reverse): absolute value used or clamped to zero
- [ ] Zero max_speed parameter: epsilon guard prevents division by zero

### Parameter Validation
- [ ] base_fov constrained to reasonable range [60, 90] degrees
- [ ] max_fov_range constrained to [0, 40] degrees
- [ ] g_multiplier constrained to [0, 5.0] degrees per g
- [ ] All metadata ranges enforced via GUI sliders
- [ ] Invalid parameter combinations rejected (e.g., base + range > 120°)

### System Integration
- [ ] Updates after vehicle physics (read fresh velocity/g-force)
- [ ] Writes camera FOV exactly once per frame (no redundant updates)
- [ ] No state accumulation (pure reactive calculation)
- [ ] Debug visualization matches actual camera FOV (no display lag)

### Smoothing (Spring-Damper)
- [ ] Spring-damper smoothing eliminates jarring FOV snaps
- [ ] Smooth transitions during lateral g-force changes
- [ ] Smooth transitions during acceleration/deceleration
- [ ] Spring parameters tunable (frequency, damping_ratio)
- [ ] Convergence stable at all framerates (time-independent)
- [ ] Current FOV state stored in system (spring-damper requires velocity state)
<!-- END: ITERATE/CONTRACT -->

---

<!-- BEGIN: ITERATE/LEARNING -->
## Learning

**Pattern discovered during implementation:**

Before using a foundation primitive, review existing usage patterns in the codebase.

**Context:**
When adding `spring_damper` smoothing to dynamic_fov, initial implementation attempted to expose frequency/damping_ratio parameters and convert them to stiffness/damping via angular frequency formulas. This added unnecessary complexity.

**Pattern in codebase:**
- `vehicle_visual_systems.cpp:6-16` - Exposes `stiffness` directly, uses `critical_damping()` helper
- Constructor sets stiffness to tunable default, calculates damping via `critical_damping(stiffness)`
- No frequency/ratio conversions, no per-frame math

**Corrected approach:**
- Expose `spring.stiffness` as tunable parameter (like vehicle tilt system)
- Call `critical_damping()` to calculate damping (maintains no-overshoot behavior)
- User tunes stiffness directly (10-500 range matches existing patterns)

**Why this matters:**
- Consistency: Same primitive, same usage pattern
- Simplicity: No conversion math, no derived parameters
- Predictability: Stiffness behaves the same across all systems

**Principle:**
Study existing usage before implementing. Patterns exist for a reason.
<!-- END: ITERATE/LEARNING -->

---
