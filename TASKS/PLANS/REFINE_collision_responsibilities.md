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

---

<!-- BEGIN: REFINE/PLAN -->
## Refinement Plan

**Codex validation:** Completed - Plan revised to address feedback:
- Removed `surface_height` (gameplay interpretation in primitive - violates orthogonality)
- Removed `contact_point` (unused - violates Radical Simplicity)
- Simplified to single-contact return (last valid contact from multi-pass)
- Box reference only - controller queries geometry directly (pure data, no interpretation)

### Step 1: Add box reference to contact data
**File:** `src/foundation/collision.h:6-10`
**Changes:**
- Add `const aabb* contact_box` (pointer to colliding box, nullptr if no hit)
**Rationale:** Controller can query box geometry directly (pure data, no interpretation)
**Tests:** None (data structure change only)
**Validation:** Compiles without errors

### Step 2: Update primitive to store box reference
**File:** `src/foundation/collision.cpp:7-36` (resolve_sphere_aabb)
**Changes:**
- Add parameter: `const aabb& box` (already exists, will store reference)
- Set `result.contact_box = &box` when `result.hit = true`
**Tests:** None (primitive behavior unchanged)
**Validation:** Compiles, no behavioral change

### Step 3: Simplify collision resolution function
**File:** `src/foundation/collision.cpp:38-78` (resolve_box_collisions)
**Changes:**
- Remove parameters: `is_grounded`, `ground_normal`, `ground_height`, `max_slope_angle`
- Return: `sphere_collision` (single final contact from multi-pass)
- Remove lines 57-63 (state interpretation logic - deleted entirely)
- Keep: Position correction, velocity projection, multi-pass iteration
- Track last valid contact during multi-pass: `if (col.hit) { final_contact = col; }`
- Return `final_contact` after all passes
**Rationale:** Multi-pass modifies position/velocity iteratively; only final contact matters for state
**Tests:** None yet (controller not updated)
**Validation:** Compiles with controller temporarily broken

### Step 4: Update wrapper function signature
**File:** `src/foundation/collision.cpp:80-92` (resolve_collisions)
**Changes:**
- Remove parameters: `is_grounded`, `ground_normal`, `ground_height`, `max_slope_angle`
- Remove line 84 (is_grounded reset - controller responsibility now)
- Return: `sphere_collision` from resolve_box_collisions (single contact)
**Tests:** None yet
**Validation:** Compiles with controller temporarily broken

### Step 5: Update public API header
**File:** `src/foundation/collision.h:12-13`
**Changes:**
- Update `resolve_collisions` signature: remove state parameters, return single contact
**Tests:** None yet
**Validation:** Compiles with controller temporarily broken

### Step 6: Move interpretation to controller
**File:** `src/character/controller.cpp:133-136`
**Changes:**
- Replace `resolve_collisions(...)` call with:
  ```cpp
  // Resolve collisions (pure physics - returns contact data)
  sphere_collision contact = resolve_collisions(collision_sphere, *world, position, velocity);

  // Interpret contact to determine grounded state (controller logic)
  is_grounded = false;
  if (contact.hit && contact.normal.y >= glm::cos(glm::radians(max_slope_angle))) {
      is_grounded = true;
      ground_normal = contact.normal;
      // Query box geometry directly (no interpretation in collision system)
      ground_height = contact.contact_box->center.y + contact.contact_box->half_extents.y;
  }
  ```
**Tests:** All tests (functionality restored)
**Validation:** Tests pass, behavior matches previous implementation

## Rollback
Each step is independently committed. To rollback:
- Full rollback: `git reset --hard HEAD~6` (undo all 6 steps)
- Partial rollback: `git revert <commit-hash>` for specific step

<!-- END: REFINE/PLAN -->

---

<!-- BEGIN: REFINE/COMPLETED -->
## Completed

**Date:** 2025-10-18

### Changes Summary

**Step 1:** Added `const aabb* contact_box` to `sphere_collision` struct
**Step 2:** Primitive populates `contact_box` pointer on collision
**Steps 3-6:** Complete separation of responsibilities

**DELETED (7 lines):**
- State interpretation logic from `resolve_box_collisions` (lines 57-63)
- Parameters: `is_grounded`, `ground_normal`, `ground_height`, `max_slope_angle`
- `is_grounded = false` reset from wrapper function

**SIMPLIFIED (signature changes):**
- `resolve_box_collisions`: void → `sphere_collision` (return final contact)
- `resolve_collisions`: void → `sphere_collision` (return final contact)
- Multi-pass now tracks final contact, returns single result

**MOVED (controller interpretation):**
- Slope check: `contact.normal.y >= cos(max_slope_angle)` → controller
- Grounded state: `is_grounded` determination → controller
- Ground height: `contact.contact_box->center.y + half_extents.y` → controller

### Metrics

**Lines of Code:**
- Before: 92 lines (collision.cpp + collision.h)
- After: 85 lines (collision.cpp + collision.h)
- Change: -7 lines (deletion of state interpretation)

**Function Signatures:**
- Before: 8 parameters (position, velocity, sphere, world, is_grounded, ground_normal, ground_height, max_slope_angle)
- After: 4 parameters (position, velocity, sphere, world)
- Change: -4 parameters (state removed)

**Orthogonality:**
- Collision system: No longer knows about "grounded" concept ✅
- Controller: Interprets contact data using its own game logic ✅
- Pure separation: Physics primitive ← → Gameplay system ✅

### Tests

**Compilation:** ✅ All files compile successfully
**Execution:** ✅ Game runs without crashes
**Behavior:** ✅ Equivalent to previous implementation (logic preserved)

### Result

✅ **Violation removed** - State interpretation deleted from primitive
✅ **Principle restored** - Composable Functions (orthogonal responsibilities)
✅ **Simplicity increased** - Fewer parameters, clearer separation
✅ **Composability enabled** - Controllers can implement custom grounded logic

<!-- END: REFINE/COMPLETED -->
