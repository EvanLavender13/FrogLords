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

---

<!-- BEGIN: REFINE/REVIEW -->
## Second Opinion Review

**Tool:** Codex CLI
**Date:** 2025-10-20

**Question asked:**
Validate this refinement plan. Does it correctly address the mathematical correctness violation? Will computing the normal from minimum penetration depth produce the correct contact normal for deep penetration cases? Are there any edge cases or concerns with this approach?

**Concerns evaluated:**
- Correctness of minimum penetration approach for deep penetration
- Integration with existing multi-pass collision solver
- Edge cases and ambiguities

**Feedback received:**
- Approach correctly resolves "Physics is Law" violation - normal becomes deterministic and geometry-based
- Minimum translation along axes gives correct contact normal when sphere center is inside box
- Works with existing multi-pass solver - correct axis direction prevents wall→floor flip
- **Edge case - Ties**: Center exactly at box center or equidistant from faces needs deterministic tie-breaking
- **Clarity**: Should explicitly note that closest_point equals sphere center in degenerate case
- **Validation**: Add guards for zero half-extents or denormal values

**Impact on implementation:**
- Added deterministic tie-breaking rule (Y, then X, then Z) matching existing priority in code
- Will add debug assertions to catch bad inputs early
- Confirmed approach is mathematically sound for deep penetrations
<!-- END: REFINE/REVIEW -->

---

<!-- BEGIN: REFINE/PLAN -->
## Refinement Plan

### Step 1: Replace fallback with penetration-based normal computation
**Changes:** `src/foundation/collision.cpp:35-37` - Replace `return math::UP;` with computation of contact normal from penetration depth
**Approach:**
- Compute distance from closest_point to each box face (6 faces)
- Select face with smallest distance (least penetration)
- Return that face's axis-aligned normal
- Tie-breaking: Prioritize Y (floor/ceiling), then X, then Z (deterministic, matches existing priority)
**Tests:** Existing collision tests should pass
**Validation:** Deep penetration returns valid axis-aligned normal (not UP), tests pass, no regressions

### Step 2: Add debug assertions for normal validity
**Changes:** `src/foundation/collision.cpp:compute_face_normal` - Add postcondition assertions
**Details:**
- Assert computed normal is normalized
- Assert normal is non-zero
- Assert normal is finite
**Tests:** Debug builds verify contract
**Validation:** Assertions never fire during normal gameplay

## Rollback
`git reset --hard HEAD` to revert all changes if approach fails

### Status
- [x] Step 1: Replace fallback with penetration-based normal computation - COMPLETE
- [x] Step 2: Add debug assertions for normal validity - COMPLETE
<!-- END: REFINE/PLAN -->

---

<!-- BEGIN: REFINE/COMPLETED -->
## Completed

**Changes:**
- Deleted arbitrary `math::UP` fallback at `src/foundation/collision.cpp:35-37`
- Replaced with geometry-based computation: distance to each face, return nearest face normal
- Added debug assertions: non-zero, normalized, finite
- Deterministic tie-breaking: Y > X > Z

**Implementation:**
- Compute distance from closest_point to all 6 box faces
- Select face with minimum distance (least penetration)
- Return that face's axis-aligned normal
- Handles deep penetration without converting walls to floors

**Tests:** All passing
**Manual verification:** Confirmed - wall sliding correct, no wall climbing, normal floor/ceiling contact
**Result:** ✓ Violation removed - Mathematical Foundations restored
**Commit:** 34d12d1
<!-- END: REFINE/COMPLETED -->

---

<!-- BEGIN: MEASURE/METRICS -->
## Metrics

**Files:**
- `src/foundation/collision.cpp`: 195 → 233 (+38 lines)

**Total:** +38 lines (added geometric computation replacing 3-line special case)

**Violations removed:**
- Arbitrary UP fallback: 1 → 0 (-1)
- Special cases in face normal computation: 1 → 0 (-1)

**Principle:** Mathematical Foundations
- Before: 6/10 (Critical violation)
- After: 10/10 (Mathematically correct)
- Improvement: +4

**Foundation:**
- Layer 2 (Primitives): 99%+ → 100% (+<1%)
- Overall: 99%+ → 99%+ (critical violation eliminated)
<!-- END: MEASURE/METRICS -->

---

<!-- BEGIN: MEASURE/LEARNING -->
## Learning

**Root cause:** Degenerate case (deep penetration) lacked geometric computation, fell back to arbitrary constant.

**Prevention:** Never return arbitrary constants for geometric queries - compute from available geometry or fail explicitly.

**Pattern:** This is the last critical special case in collision primitives.
<!-- END: MEASURE/LEARNING -->
