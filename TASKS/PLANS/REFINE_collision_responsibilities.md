# Refinement: Collision Responsibilities

**Split collision resolution from state interpretation**

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-18
**Complexity:** Standard (4 points)
**Path:** B (standard)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/foundation/collision.cpp:38-78` (resolve_box_collisions)
**Principle:** Composable Functions | Score: 6/10 (estimated)
**Severity:** High
**Type:** Mixed concerns, Non-orthogonal design

**Current state:**
```cpp
void resolve_box_collisions(sphere& collision_sphere, const collision_world& world,
                            glm::vec3& position, glm::vec3& velocity, bool& is_grounded,
                            glm::vec3& ground_normal, float& ground_height, float max_slope_angle) {
    // ...
    if (col.hit) {
        // Push out of collision
        position += col.normal * col.penetration;
        collision_sphere.center = position;

        // Check if this is a ground surface (upward-facing normal)
        if (col.normal.y >= glm::cos(glm::radians(max_slope_angle))) {
            is_grounded = true;        // ← STATE INTERPRETATION
            ground_normal = col.normal; // ← STATE INTERPRETATION
            ground_height = box.center.y + box.half_extents.y; // ← STATE INTERPRETATION
        }

        // Remove velocity into surface
        float vel_into_surface = glm::dot(velocity, col.normal);
        if (vel_into_surface < 0.0f) {
            velocity -= col.normal * vel_into_surface;
        }
    }
}
```

**Why violation:**
- Function performs TWO orthogonal responsibilities:
  1. **Collision resolution** (physics primitive) - Correct position/velocity based on penetration
  2. **State interpretation** (controller logic) - Interpret collision normals to determine grounded state
- Collision system understands gameplay concepts (slopes, grounded state)
- Controller cannot implement alternative grounded logic without changing collision system
- Tight coupling prevents reuse and testing

**Impact:**
- Blocks: Alternative movement systems (wall-running, magnetic boots, etc.)
- Cascades to: Any system needing collision without hardcoded state interpretation
- Foundation: Layer 2 (Primitives) at 100%, but this violates composability principle
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify - Split responsibilities

**CRITICAL:** Describing approach conceptually. No actual source code—that happens during REFINE.

**FROM:** Collision system interprets contact data and sets controller state
**TO:** Collision system returns rich contact data, controller interprets to set its own state

**High-level approach:**

1. **Enrich contact data structure:**
   - Add contact point position to `sphere_collision`
   - Add surface information (box reference or surface height)
   - Keep existing: hit flag, normal, penetration

2. **Simplify collision resolution:**
   - Remove parameters: `is_grounded`, `ground_normal`, `ground_height`, `max_slope_angle`
   - Keep parameters: `collision_sphere`, `world`, `position`, `velocity`
   - Return: Collection of contact data (or modify sphere_collision to support multiple contacts)
   - Responsibility: Pure collision resolution only

3. **Move interpretation to controller:**
   - Controller calls collision resolution
   - Controller examines returned contact data
   - Controller determines grounded state by checking contact normals against max_slope_angle
   - Controller updates its own state: `is_grounded`, `ground_normal`, `ground_height`

**Resulting architecture:**
- **Collision system (primitive):** Takes geometry, returns contact information
- **Controller (system):** Interprets contact information, updates gameplay state
- **Orthogonality:** Collision doesn't know about "grounded," Controller doesn't know collision math

**Benefits:**
- Collision system becomes pure primitive (testable in isolation)
- Controllers can implement custom state logic (wall-running, ceiling-walking, etc.)
- Separation of concerns: physics vs gameplay interpretation

<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Violation resolved
- [ ] Principle score improved
- [ ] Tests passing
- [ ] No regressions

**Metrics:**
- Before: LOC ~92, Composable Functions 6/10, Coupling High
- After: LOC ~100 (+8), Composable Functions 9/10 (+3), Coupling Low (-orthogonal)

**Expected impact:**
- Layer 2 composability restored
- Foundation maintained at 97%+
- Enables future movement variations (wall-run, dash, etc.)
<!-- END: SELECT/SUCCESS -->
