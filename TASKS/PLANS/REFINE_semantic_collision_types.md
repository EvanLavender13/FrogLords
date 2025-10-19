# Refinement: Semantic Collision Types

Add semantic type information to collision system for reliable debug visualization and future gameplay systems.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-19
**Complexity:** Standard (5 pts)
**Path:** B (standard)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/foundation/collision.h`, `src/app/debug_generation.cpp:207-225`
**Principle:** Radical Simplicity, Fundamental Composable Functions, Consistency | Score: 7.5/10
**Severity:** Medium
**Type:** Missing semantic information, Coupling to geometry

**Current state:**
```cpp
// collision.h - no semantic type information
struct sphere_collision {
    bool hit = false;
    glm::vec3 normal{0.0f};
    float penetration = 0.0f;
    const aabb* contact_box = nullptr;

    bool contacted_floor = false;  // Inferred during resolution
    glm::vec3 floor_normal{0.0f};
    bool is_wall = false;           // Inferred during resolution
};

// debug_generation.cpp - gave up on categorization
// All boxes now single color because dimension-based heuristics failed
constexpr glm::vec4 COLLISION_BOX_COLOR = {0.3f, 1.0f, 0.3f, 1.0f}; // Green
```

**Why violation:**
- **Radical Simplicity violated:** Debug visualization tried to infer semantic meaning from geometry (height < 0.4f = wall). Inference is complexity. Geometry doesn't encode intent.
- **Fundamental Composable Functions violated:** Collision boxes lack semantic identity. Future systems (wall-run, ledge-grab, platform) will need to infer type again, duplicating logic.
- **Consistency violated:** Same geometry could be floor or platform depending on context, but no way to distinguish. Intent is ambiguous.

**Impact:**
- **Blocks:** Future variation systems (wall-run detection, ledge-grab, one-way platforms)
- **Cascades to:** Any system that needs semantic collision understanding
- **Foundation:** Layer 2 (Primitives) - 100% → ~95% (missing semantic info)
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify (add semantic type to encode intent directly)

**CRITICAL:** Conceptual approach only. No actual source code.

### Add Semantic Type Enum

**Concept:** Create lightweight enum to encode collision intent:
```
collision_surface_type enum:
- NONE      (default, no special behavior)
- FLOOR     (supports standing, slope walking)
- WALL      (blocks horizontal, enables wall-slide)
- PLATFORM  (one-way passthrough from below)
- DYNAMIC   (movable objects, future)
```

**Why these categories:**
- **FLOOR:** Distinguishes from walls for grounding logic
- **WALL:** Enables future wall-run, wall-slide refinements
- **PLATFORM:** Enables future one-way platform mechanics
- **DYNAMIC:** Future movable objects, conveyor belts
- **NONE:** Catch-all for generic obstacles

### Update Collision Primitives

**Concept:** Add optional type field to `aabb`:
```
struct aabb {
    glm::vec3 center;
    glm::vec3 half_extents;
    collision_surface_type type = collision_surface_type::NONE;  // Optional semantic info
};
```

**Why optional:** Most boxes don't need special behavior. Defaults to NONE.

### Simplify Debug Visualization

**Current problem:** Tries to infer type from dimensions (height < 0.4f = wall)

**Fix:** Use semantic type directly:
```
Pseudocode:
if box.type == FLOOR: color = BLUE
else if box.type == WALL: color = RED
else if box.type == PLATFORM: color = YELLOW
else: color = GREEN (generic)
```

**Remove:** All dimension-based heuristics. Delete inference logic.

### Update Test Arena Construction

**Concept:** Tag boxes with semantic types during creation:
```
Pseudocode:
// Floor
world.add_box({center, extents, collision_surface_type::FLOOR});

// Walls
world.add_box({center, extents, collision_surface_type::WALL});

// Platforms
world.add_box({center, extents, collision_surface_type::PLATFORM});
```

**Impact:** Makes intent explicit. No more "is this a wall or floor?" ambiguity.

### Check for Duplicates

**Existing inference:**
- `src/app/debug_generation.cpp:211-214` - Old heuristic comment (delete)
- `src/foundation/collision.cpp` - Floor detection based on normal (keep, orthogonal)

**Floor normal detection** is separate concern (physics-based). Type is **authoring-time intent**. Both can coexist:
- Type: "This box is authored as a floor"
- Normal: "This contact behaves like a floor surface"

No duplication—complementary concerns.

### Migration Path

1. Add enum and update `aabb` struct
2. Update test arena to specify types
3. Update debug visualization to use types
4. Verify collision resolution still works identically
5. Future: Expose in level data format

**Backward compatibility:** Type defaults to NONE, existing code unaffected.

<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] `collision_surface_type` enum added to `collision.h`
- [ ] `aabb` struct includes optional type field
- [ ] Test arena boxes tagged with semantic types
- [ ] Debug visualization uses types directly (no dimension inference)
- [ ] All collision tests passing
- [ ] No behavioral regressions

**Metrics:**
- Before: LOC ~400, Principle 7.5/10, Complexity 6 (inference heuristics)
- After: LOC ~410 (+10), Principle 9/10 (+1.5), Complexity 3 (-3, no inference)

**Principle improvements:**
- Radical Simplicity: +2 (no more dimension inference)
- Composable Functions: +1 (collision boxes now self-describing)
- Consistency: +1 (intent explicit, no ambiguity)
<!-- END: SELECT/SUCCESS -->
