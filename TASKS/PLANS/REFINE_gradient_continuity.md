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
- **To:** Continuous tri-color interpolation using `glm::mix`
- **Derive:**
  - Blue (0.0) → Cyan (0.5) → Yellow/Red (1.0)
  - Use continuous interpolation across full `speed_ratio` range
  - Two-stage mix: blue→cyan→yellow or similar smooth gradient

**Implementation:**
Replace if/else blocks with continuous interpolation:
```cpp
// Continuous gradient: blue -> cyan -> yellow -> red
glm::vec3 blue = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cyan = glm::vec3(0.0f, 1.0f, 1.0f);
glm::vec3 yellow = glm::vec3(1.0f, 1.0f, 0.0f);
glm::vec3 red = glm::vec3(1.0f, 0.0f, 0.0f);

glm::vec3 rgb;
if (speed_ratio < 0.5f) {
    rgb = glm::mix(blue, cyan, speed_ratio * 2.0f);
} else {
    float t = (speed_ratio - 0.5f) * 2.0f;
    glm::vec3 mid = glm::mix(cyan, yellow, t);
    rgb = t < 0.5f ? mid : glm::mix(yellow, red, (t - 0.5f) * 2.0f);
}
color = glm::vec4(rgb, 0.8f);
```

**Wait - still has branching.** Better approach:
```cpp
// Fully continuous 4-color gradient
glm::vec3 colors[] = {
    glm::vec3(0.0f, 0.0f, 1.0f),  // blue
    glm::vec3(0.0f, 1.0f, 1.0f),  // cyan
    glm::vec3(1.0f, 1.0f, 0.0f),  // yellow
    glm::vec3(1.0f, 0.0f, 0.0f),  // red
};
int num_colors = 4;
float scaled = speed_ratio * (num_colors - 1);
int index = (int)scaled;
float t = scaled - index;
index = glm::clamp(index, 0, num_colors - 2);
glm::vec3 rgb = glm::mix(colors[index], colors[index + 1], t);
color = glm::vec4(rgb, 0.8f);
```

---

## Success

- [ ] Violation resolved
- [ ] Principle score improved
- [ ] Tests passing
- [ ] No regressions

**Metrics:**
- Before: LOC 10, Mathematical Foundations 7/10, Complexity (branching)
- After: LOC ~8 (-2), Mathematical Foundations 9/10 (+2), Complexity (pure math)

**Verification:**
- Run game, observe debug visualization
- Speed ring should show smooth color gradient from blue→cyan→yellow→red
- No sharp color jumps at speed thresholds

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
