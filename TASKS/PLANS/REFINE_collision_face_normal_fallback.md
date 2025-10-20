# Refinement: Collision Face Normal Fallback

Delete arbitrary UP fallback - compute real contact normal from penetration geometry.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-20
**Complexity:** Standard
**Path:** B (standard)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/foundation/collision.cpp:35-37`
**Principle:** Mathematical Foundations | Score: Critical
**Severity:** Critical
**Type:** Special case enabling physics exploit

**Current state:**
```cpp
// Fallback: should not reach here if closest_point is truly on box surface
// If we do, use UP as last resort (same as old behavior)
return math::UP;
```

**Why violation:**
- Returns arbitrary UP normal when sphere is deeply penetrated (low frame rate, teleport)
- Converts wall contacts into floor-like normals
- Enables player to stick to or climb vertical walls
- Special case breaks "Physics is Law" - contact normal should derive from geometry
- Makes emergent movement unstable and unpredictable

**Impact:**
- Blocks: Foundation physics correctness
- Cascades to: Controller movement, wall sliding behavior
- Foundation: Layer 2 (Primitives) - prevents reaching 100%
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Delete special case, derive from geometry

**CRITICAL:** Conceptual approach only - actual implementation during REFINE.

**DELETE the special case:**
- Remove: Arbitrary `return math::UP` fallback
- Impact: Forces computation of real contact normal
- Tests: Deep penetration cases, low frame rate scenarios

**COMPUTE real normal from penetration:**
- From: Fallback to arbitrary UP
- To: Derive normal from penetration geometry
- Approach:
  - When closest point classification fails (deep penetration), sphere center is inside box
  - Compute signed distance from sphere center to each box face
  - Face with smallest penetration depth is the contact face
  - Return that face's normal (axis-aligned)
- Mathematical foundation: Penetration depth defines contact direction
- Single source of truth: Box geometry, sphere position

**Validation:**
- Add debug assertions for NaN/zero-length normals
- Existing collision tests should pass
- Deep penetration should return valid axis-aligned normal (not UP)
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Violation resolved - no arbitrary fallback
- [ ] Principle score improved - mathematical correctness restored
- [ ] Tests passing - existing collision scenarios work
- [ ] No regressions - wall sliding still functions correctly

**Metrics:**
- Before: LOC ~40, Principle 6/10 (Critical), Special case present
- After: LOC ~45-50 (+5-10), Principle 10/10 (+4), No special cases
<!-- END: SELECT/SUCCESS -->
