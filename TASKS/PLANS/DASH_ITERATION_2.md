# Iteration 2: Dash

**Started:** 2025-10-19
**Previous:** [ITERATION_1.md](DASH_ITERATION_1.md)
**Status:** Ready for VALIDATE

---

<!-- BEGIN: ITERATE/CONTEXT -->
## Context from Previous Iteration

**Decision:** REVISE (from ITERATION_1)

**Required changes:**
1. Refactor speed cap system - replace hard clamp with velocity-dependent friction
2. Model forces such that equilibrium occurs at max_speed
3. Ensure net acceleration = 0 at target speed, negative above it
4. Re-implement dash with Option 1 (no air clamp) on natural speed cap

**Baseline contract:** See ITERATION_1/CONTRACT

**Root cause identified:**
Hard speed clamp (software truncation) violates Solid Mathematical Foundations. Propulsion exceeds friction, requiring clamp to prevent infinite acceleration. Mathematically correct solution: velocity-dependent drag creating natural equilibrium at max_speed.
<!-- END: ITERATE/CONTEXT -->

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

Properties to validate through iteration:

- [x] Velocity-dependent friction creates equilibrium at max_speed
  - Net acceleration = 0 when speed = max_speed (validated via user testing)
  - Net acceleration < 0 when speed > max_speed (dash deceleration validated)
  - No hard clamp needed (removed clamp_horizontal_speed function)
  - Commit: 5702c98
- [x] Friction balances propulsion mathematically
  - drag_coefficient calculated from ground_accel, base_friction, max_speed
  - Auto-adjusts base_friction if ground_accel too low (prevents crash)
  - Frame-rate independent (dt multiplied throughout)
  - Commit: 5702c98
- [x] Dash impulse naturally decelerates from overspeed
  - Apply impulse → speed exceeds max_speed (validated)
  - Friction brings speed back to equilibrium smoothly (validated)
  - No special-case logic (removed dash_active_timer, skip_friction)
  - Commit: 5702c98
- [x] Unified ground/air physics (momentum preserved)
  - Ground and air use same friction/acceleration (consistent feel validated)
  - Dash→jump carries momentum naturally (no abrupt clamp)
  - Air control restored (can steer mid-jump)
  - Commit: 5702c98
- [x] Ground-only dash constraint enforced
  - is_grounded check in apply_input (controller.cpp:112)
  - Commit: 5702c98
- [x] Dash cooldown prevents spam
  - Cooldown timer decay (controller.cpp:213)
  - Commit: 5702c98
- [x] Tuning system honest (bonus fix)
  - Removed time_to_max_speed (broken abstraction)
  - Exposed ground_accel, base_friction, dash_impulse
  - drag_coefficient shown as read-only calculated value
  - Commit: 5702c98
<!-- END: ITERATE/CONTRACT -->

---

<!-- BEGIN: ITERATE/COMPLETE -->
## Iteration Complete

**Contract:** ✓ PROVEN (all 7 items validated)

**Properties validated:**
- Velocity-dependent friction creates natural equilibrium at max_speed
- Unified physics (ground/air) provides consistent feel
- Dash naturally decelerates without special-case logic
- No hard clamp needed - friction handles all speed regulation

**Edges handled:**
- Extreme parameter ratios (high accel/low max_speed) auto-adjust base_friction
- Air movement feels natural (no abrupt transitions)
- Dash→jump momentum preserved (no clamp killing speed)

**Assertions:** 0 added (physics validated via manual testing, not foundation code)

**Manual testing:** Extensive user validation of feel and edge cases

**Bonus work:**
- Fixed broken tuning abstraction (time_to_max_speed removed)
- Exposed honest physics parameters (ground_accel, base_friction, dash_impulse)
- Added drag_coefficient read-only display

**Known limitations (defer):**
- Tuning parameter constraints (prevent nonsensical combinations like max_speed=1, ground_accel=50)
- Future: Add validation/clamping to tuning system

**Status:**
- [x] Contract proven
- [x] Stable through testing
- [x] Ready for VALIDATE
<!-- END: ITERATE/COMPLETE -->

---
