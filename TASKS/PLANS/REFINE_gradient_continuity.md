# Refinement: Gradient Continuity

**Replace discontinuous color branching with continuous mathematical interpolation**

---

## Selected

**Date:** 2025-10-18
**Complexity:** Trivial (1-2 points)
**Path:** A (trivial)

---

## Violation

**Location:** `src/app/debug_generation.cpp:59-68`
**Principle:** Mathematical Foundations | Score: 7/10
**Severity:** Low
**Type:** Discontinuous, Unjustified implementation

**Current state:**
```cpp
if (speed_ratio < 0.33f) {
    float t = speed_ratio / 0.33f;
    color = glm::vec4(0.0f, t, 1.0f - t, 0.8f);
} else if (speed_ratio < 0.66f) {
    float t = (speed_ratio - 0.33f) / 0.33f;
    color = glm::vec4(t, 1.0f, 0.0f, 0.8f);
} else {
    float t = (speed_ratio - 0.66f) / 0.34f;
    color = glm::vec4(1.0f, 1.0f - t, 0.0f, 0.8f);
}
```

**Why violation:**
- Hard-coded thresholds (0.33, 0.66) create discontinuous steps
- If/else branching instead of continuous mathematical function
- Creates sharp color jumps instead of smooth gradient
- Adds unnecessary complexity (branching logic vs. pure interpolation)

**Impact:**
- Blocks: Nothing
- Cascades to: Nothing (debug visualization only)
- Foundation: 97% (low-priority refinement)

---

## Fix

**Approach:** Simplify

**SIMPLIFY:**
- **From:** Branching logic with hard thresholds
- **To:** Continuous interpolation across color gradient
- **Derive:**
  ```
  Define gradient as array of color stops
  Map speed_ratio [0.0, 1.0] to continuous position in gradient
  Interpolate between adjacent color stops based on position
  Result: smooth transition without branching
  ```

---

## Completed

**Change:** Deleted branching logic, replaced with continuous gradient interpolation
**Tests:** All passing
**Metrics:** LOC 10→8 (-2) | Mathematical Foundations 7/10→9/10 (+2)
**Result:** ✓ Violation removed

**Implementation:**
- Deleted: 3 if/else branches with hard-coded thresholds (0.33, 0.66)
- Added: Gradient array with 4 color stops
- Replaced: Discontinuous branches with continuous interpolation (glm::mix)
- Fixed: Index clamping before t calculation to ensure speed_ratio=1.0 → Red

**Verification:**
- Build: Successful
- Tests: All passing
- Visual: Speed ring shows continuous gradient blue→cyan→yellow→red
- Edge case: Maximum speed displays solid red (no flickering)

---

## Risk

**Could go wrong:**
- Color array indexing could go out of bounds if `speed_ratio` exceeds 1.0 (already clamped upstream)
- Gradient might look visually different from current stepped version

**Mitigation:**
- `speed_ratio` already clamped to [0.0, 1.0] at line 57
- Index clamping provides defense-in-depth
- Visual difference is intentional (continuous is correct)

**Rollback:**
```bash
git checkout main -- src/app/debug_generation.cpp
```

---

**Delete branching. Replace with continuous mathematics.**
