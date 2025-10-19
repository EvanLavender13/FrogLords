# Refinement: Debug Visualization Color Logic

Replace magic numbers in collision debug color logic with named constants and clear semantic meaning.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-19
**Complexity:** Trivial (1 point)
**Path:** A (trivial)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/app/debug_generation.cpp:214-223` (generate_collision_state_primitives)
**Principle:** Radical Simplicity, Mathematical Foundations, Consistency | Score: 6/10
**Severity:** Medium
**Type:** Magic numbers, Arbitrary logic

**Current state:**
```cpp
glm::vec4 color;
if (box.half_extents.y < 0.4f && box.half_extents.x < 1.2f && box.half_extents.z < 1.2f &&
    box.center.y < 1.0f) {
    float height_ratio = box.half_extents.y / 0.3f;
    color = {0.2f, 0.5f + height_ratio * 0.5f, 1.0f, 1.0f};
} else if (box.half_extents.y > box.half_extents.x &&
           box.half_extents.y > box.half_extents.z) {
    color = {1.0f, 0.5f, 0.2f, 1.0f};
} else {
    color = {0.3f, 1.0f, 0.3f, 1.0f};
}
```

**Why violation:**
- Magic numbers (0.4f, 1.2f, 1.0f, 0.3f) have no semantic meaning
- No explanation for what these thresholds represent
- No mathematical or physical justification for values
- Inconsistent with principle of mathematical foundations
- Color logic appears arbitrary - unclear what categories mean

**Impact:**
- Blocks: None (debug visualization only)
- Cascades to: Code clarity, maintainability
- Foundation: 99% (minimal impact - isolated to debug)
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Document

**CRITICAL:** Describe the approach conceptually. Use pseudocode and high-level explanations. Do NOT write actual source code—that happens during REFINE.

**DOCUMENT:**
- Why keep: Debug visualization aids developer understanding. Color-coded categories help visually distinguish collision box types in test arena.
- Derivation:
  - Small platforms: Low height (<0.4m), moderate width/depth (<1.2m), near ground (y < 1.0m) → Blue gradient
  - Walls: Tall vertical boxes (y-extent dominant) → Orange
  - Floors/platforms: Default horizontal boxes → Green
  - Height ratio gradient: Visual feedback for step/platform height variations
- Trade-off: Accept heuristic thresholds for debug-only visualization rather than adding semantic box types to core collision system
- Define named constants:
  ```
  // Platform detection thresholds (test arena specific)
  PLATFORM_MAX_HEIGHT = 0.4f      // Small step/platform max height
  PLATFORM_MAX_SIZE = 1.2f         // Platform max horizontal extent
  PLATFORM_MAX_Y = 1.0f            // Near-ground threshold
  PLATFORM_REFERENCE_HEIGHT = 0.3f // Height normalization for gradient

  // Color semantic mapping
  BLUE_GRADIENT = small platforms (walkable steps)
  ORANGE = walls (vertical obstacles)
  GREEN = floors/large platforms (default)
  ```
- Check duplicates: Single occurrence in debug visualization

**After fix:**
- Constants defined with semantic names
- Comments explain categorization logic
- Color choices documented
- No change to runtime behavior
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Violation resolved (magic numbers → named constants)
- [ ] Principle score improved (6/10 → 8+/10)
- [ ] Build passing
- [ ] No regressions (visual output unchanged)

**Metrics:**
- Before: LOC ~15, Principle 6/10, Magic numbers: 5
- After: LOC ~25 (+10 for constants/comments), Principle 8+/10 (+2), Magic numbers: 0
<!-- END: SELECT/SUCCESS -->
