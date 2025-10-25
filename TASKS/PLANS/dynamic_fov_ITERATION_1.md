# Iteration 1: Dynamic FOV

**Started:** 2025-10-25
**Status:** In Progress

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

Mathematical properties and edge cases that must be proven for production stability.

### Mathematical Correctness
- [x] FOV output always within [base_fov, base_fov + max_fov_range] for all inputs (dynamic_fov.cpp:59,61)
- [x] Division by zero prevented (max_speed == 0 case handled) (dynamic_fov.cpp:33-34)
- [x] Speed factor saturates to [0, 1] even when speed > max_speed (dynamic_fov.cpp:38)
- [x] Lateral g-force contribution bounded (no unbounded FOV from extreme g-forces) (dynamic_fov.cpp:49)
- [x] Formula produces deterministic output (same inputs → same output) (pure calculation, no randomness)

### Edge Cases
- [ ] Stationary vehicle (speed = 0): FOV equals base_fov
- [ ] Max speed straight line: FOV equals base_fov + max_fov_range (no g contribution)
- [ ] Tight turn at low speed: FOV equals base_fov + g_contribution (clamped)
- [ ] Combined max speed + max g-force: FOV clamped to ceiling (base + range)
- [ ] Negative speed (reverse): absolute value used or clamped to zero
- [ ] Zero max_speed parameter: epsilon guard prevents division by zero

### Parameter Validation
- [x] base_fov constrained to [60, 90] degrees (precondition via base_fov_meta)
- [x] max_fov_range constrained to [0, 40] degrees (precondition via max_fov_range_meta)
- [x] g_multiplier constrained to [0, 5.0] deg/g (precondition via g_multiplier_meta)
- [x] spring_stiffness constrained to [10, 500] 1/s² (precondition via spring_stiffness_meta)
- [x] All metadata ranges enforced via GUI sliders (tunable_param uses param_meta.min/max)
- [x] Preconditions validate parameters using metadata (single source of truth)

### System Integration
- [x] Updates after vehicle physics (game_world.cpp:93 calls after character.update)
- [x] Writes camera FOV exactly once per frame (single cam.set_fov call)
- [x] No state accumulation (spring state derived from target, not accumulated)
- [x] Debug visualization matches actual camera FOV (Camera panel shows cam.fov_degrees)

### Smoothing (Spring-Damper)
- [x] Spring-damper smoothing eliminates jarring FOV snaps
- [x] Smooth transitions during lateral g-force changes
- [x] Smooth transitions during acceleration/deceleration
- [x] Spring parameters tunable (stiffness exposed, damping derived)
- [x] Convergence stable at all framerates (time-independent)
- [x] Current FOV state stored in system (spring-damper requires velocity state)
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
