# System: Wall Sliding

**Preserve player intent when colliding with vertical surfaces**

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-18
**Layer:** 4 (Variation)
**Foundation:** 98%+
**Complexity:** Medium (4-6 points)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required (≥90%):**
- Collision System: 100% ✅
- Controller: 97% ✅
- Debug Visualization: 100% ✅

**Enables (will become buildable):**
- Wall-running mechanic
- Corner sliding/navigation
- Climbing detection
- Advanced traversal systems
<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
Classify collision surfaces by normal (wall vs floor vs ceiling). When hitting a wall, project movement vector along the wall surface instead of stopping. Preserve player control throughout.

**Why a system, not a feature:**
This establishes the foundation for how characters interact with vertical geometry. It enables emergent navigation patterns (corner sliding, wall approach angles) and future mechanics (wall-running, climbing) without special cases.
<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Provable correctness:**
- Surface classification: `dot(normal, up_vector)` → wall if abs(dot) < threshold
- Velocity projection: `velocity - normal * dot(velocity, normal)` → tangent velocity
- Edge case: Multiple simultaneous wall contacts → average normals before projection

**Known uncertainties:**
- Optimal threshold for wall vs slope classification (likely 0.707 = 45°)
- Handling inside corners (two opposing wall normals)
- Transition behavior when wall becomes slope

**Validation approach:**
- Unit tests for normal classification at boundary angles
- Debug visualization of collision normals and projected velocities
- Manual testing: approach walls at various angles, verify smooth sliding
<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: SELECT/GRAYBOX -->
## Graybox

**Simplest implementation:**
- Test level: Grid of vertical wall cubes + sloped surfaces at various angles
- Debug visuals: Collision normals (green = floor, red = wall, blue = ceiling)
- Debug visuals: Original velocity vector vs projected velocity vector
- Character controller: Existing capsule collision

**Validation:**
- Walk into wall at 90° → stop (no tangent component)
- Walk into wall at 45° → slide along wall
- Walk along wall-floor edge → maintain forward motion
- Walk into inside corner → graceful stop (opposing normals cancel)
<!-- END: SELECT/GRAYBOX -->

---

<!-- BEGIN: GRAYBOX/SCOPE -->
## Graybox Scope

**Visual:**
- Core: Cubes (walls), sphere (player), lines (velocity vectors, normals)
- Debug: Color-coded normals (green=floor, red=wall, blue=ceiling)
- Debug: Velocity visualization (original vs projected)
- Input: WASD movement only

**Included:**
- [x] Core mechanic: surface classification and velocity projection
- [x] Math implementation: dot product classification, vector projection
- [x] Debug visualization: normals and velocity vectors
- [ ] Validation harness: unit tests for edge cases
- [ ] Self-test: manual playtest at various angles

**Excluded:**
- Art/models/audio
- Polish/juice (particles, screen shake, etc.)
- Optional features (wall-running, climbing)
- Variations (corner sliding optimizations)

**Complexity:** Medium
<!-- END: GRAYBOX/SCOPE -->

---

<!-- BEGIN: GRAYBOX/RESULTS -->
## Graybox Results

**Status:**
- [x] Core functional
- [x] Math validation passing (build successful, debug validation passed)
- [x] Debug viz working (normals color-coded, velocity projection visible)
- [ ] Edge cases handled (needs playtesting)
- [ ] Emergence observed (needs playtesting)

**Implemented:**
- Surface classification: `is_wall()` uses dot product with threshold 0.707 (45°)
- Velocity projection: `project_along_wall()` removes normal component
- Integration: Modified `resolve_box_collisions()` to classify and project
- Debug visualization:
  - Normals: Red=wall, Green=floor, Blue=ceiling
  - Velocity vectors: Gray=original, Yellow=projected (walls only)

**Mathematical Foundation:**
- Classification: `abs(normal.y) < 0.707` → wall (vertical surface)
- Projection: `v_tangent = v - n * dot(v, n)` → preserves parallel motion
- Zero overhead: Classification happens during collision resolution (no extra passes)

**Next Steps:**
- Manual playtest: Walk into walls at various angles
- Validate: 90° → stop, 45° → smooth slide
- Test: Inside corners (opposing normals)
- Observe: Any emergent behaviors

**Build Status:**
```
Build: ✅ Success (16/16 compiled)
Debug: ✅ All validation checks passed
Run:   ✅ Program started successfully
```
<!-- END: GRAYBOX/RESULTS -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Surface classification functional (floor/wall/ceiling)
- [ ] Velocity projection maintains player intent
- [ ] Debug visualization shows normals and projected vectors
- [ ] No "sticking" or sudden stops at walls
- [ ] Inside corners handled gracefully
- [ ] Principle scores ≥7.0 average
  - Radical Simplicity: Vector projection only
  - Consistency: Same math for all walls
  - Player Control: Intent preserved during slides
<!-- END: SELECT/SUCCESS -->

---

## Implementation Notes

**Surface classification thresholds:**
```cpp
// dot(normal, up) interpretation:
//  1.0  = floor (upward normal)
//  0.0  = wall (horizontal normal)
// -1.0  = ceiling (downward normal)
constexpr float wall_threshold = 0.707f; // 45° slope

bool is_wall(vec3 normal) {
    return abs(dot(normal, vec3(0, 1, 0))) < wall_threshold;
}
```

**Velocity projection:**
```cpp
// Remove velocity component into wall normal
vec3 project_along_wall(vec3 velocity, vec3 wall_normal) {
    return velocity - wall_normal * dot(velocity, wall_normal);
}
```

**Multiple contact handling:**
```cpp
// Average normals if multiple walls hit simultaneously
vec3 combined_normal = normalize(normal_a + normal_b);
vec3 projected = project_along_wall(velocity, combined_normal);
```

**Integration point:**
Modify `controller::move()` collision response - classify surface, apply projection if wall detected.
