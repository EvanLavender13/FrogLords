# Iteration 1: Slip Angle Calculator

**Started:** 2025-10-24
**Status:** In Progress

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

Mathematical correctness and edge case handling for slip angle primitive.

- [x] Zero-velocity returns 0.0 without division errors
- [x] Straight-line motion (velocity parallel to forward) returns ≈0.0
- [x] 90° right motion returns ≈+π/2 (positive = right slide)
- [x] 90° left motion returns ≈-π/2 (negative = left slide)
- [x] Backing up returns ±π correctly (N/A - no backward movement in current system)
- [x] Non-unit forward vector asserts in debug builds (verified in code)
- [x] Sign convention consistent: positive = velocity right of heading (fixed cross product order)
- [x] No accumulated state (pure function, derived from inputs) (verified in code)
- [x] GUI displays slip angle in degrees with correct sign
<!-- END: ITERATE/CONTRACT -->

---

<!-- BEGIN: ITERATE/VALIDATION -->
## Validation

**Issue found:** Sign convention inverted
- **Root cause:** Cross product order `cross(UP, forward)` produced left vector (+X), but project convention is right = -X
- **Fix:** Changed to `cross(forward, UP)` to match coordinate system (math_utils.h:104)
- **Verified:** Turn left → positive slip angle, turn right → negative slip angle

**All contract items validated:**
- Zero velocity, straight motion, extreme angles all work correctly
- Precondition assertions present for non-unit forward vector
- Pure function confirmed (no state accumulation)
- GUI displays correctly in degrees with proper sign
<!-- END: ITERATE/VALIDATION -->

---
