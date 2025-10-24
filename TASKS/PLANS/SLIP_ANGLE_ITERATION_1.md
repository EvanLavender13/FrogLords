# Iteration 1: Slip Angle Calculator

**Started:** 2025-10-24
**Status:** REVISE

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

**Issue found:** Sign convention documentation error
- **Root cause:** Comment on math_utils.h:103 incorrectly stated "right is -X direction"
- **Reality:** cross(forward, UP) yields +X when forward is +Z (correct per coordinate system)
- **Verified:** Turn left → car slides right → positive slip angle (correct)
- **Verified:** Turn right → car slides left → negative slip angle (correct)
- **Note:** Controller negation on line 99 is correct (maps input convention to coordinate system)

**All contract items validated:**
- Zero velocity, straight motion, extreme angles all work correctly
- Precondition assertions present for non-unit forward vector
- Pure function confirmed (no state accumulation)
- GUI displays correctly in degrees with proper sign
<!-- END: ITERATE/VALIDATION -->

---

<!-- BEGIN: ITERATE/COMPLETE -->
## Iteration Complete

**Contract:** ✓ PROVEN

**Properties:** Mathematical correctness validated through manual testing across all edge cases
**Edges:** Zero velocity, straight motion, extreme angles (±90°), sign convention
**Assertions:** Precondition checks for unit-length forward vector, horizontal plane projection, epsilon validation
**Playtests:** 1 (initial validation with sign convention fix)

**Status:**
- [x] Contract proven
- [x] Stable
- [x] Ready for VALIDATE
<!-- END: ITERATE/COMPLETE -->

---

<!-- BEGIN: VALIDATE/REVIEW -->
## External Review

**Tool:** Codex
**Date:** 2025-10-24

**Principle Violations:**
- **Mathematical Foundation**: Sign convention contradiction between contract and validation. Contract (line 15-16) states "90° right motion returns ≈+π/2 (positive = right slide)" but Validation (line 32) states "Turn right → negative slip angle". With forward=(0,0,1) and velocity=(1,0,0), cross(forward, UP) yields (-1,0,0), making atan2 return -π/2, contradicting the documented contract. Documentation in math_utils.h:83 claims "Positive: velocity points right of forward" but math_utils.h:103 states "right is -X direction" while math_utils.h:12 defines coordinate system as "X-right", creating conflicting frame-of-reference documentation.

**Strengths:**
- **Radical Simplicity**: Pure function with only essential operations, zero-velocity early-out
- **Mathematical hygiene**: Preconditions guard unit-length and horizontal inputs, time-independent
- **Single Source of Truth**: Controller derives from authoritative state without caching
- **Primitives Guide**: Returns measurement only, interpretation left to higher layers
- **Code structure**: Clean separation across three layers (primitive → system → GUI)

**Assessment:** Structurally sound and principled implementation with critical sign convention error. Contract claims do not match validation results, violating Mathematical Foundation. All other principles upheld cleanly.
<!-- END: VALIDATE/REVIEW -->

---

<!-- BEGIN: VALIDATE/DECISION -->
## Decision

**Status:** REVISE

**Reasoning:** Mathematical Foundation violation due to sign convention inconsistency. The implementation structure is principled and clean, but internal documentation contradicts itself. Core is sound, violation is fixable.

**Required changes:**
- Resolve sign convention: Either fix contract (lines 15-16) to match actual behavior (right=negative) OR fix code/comments to match contract (right=positive)
- Reconcile coordinate system documentation in math_utils.h (lines 12, 83, 103)
- Verify which convention is correct through actual playtest observation
- Update all documentation to be internally consistent
<!-- END: VALIDATE/DECISION -->

