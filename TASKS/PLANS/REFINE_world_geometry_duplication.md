# Refinement: World Geometry Duplication

**Unify world geometry storage to single source of truth**

---

## Selected

**Date:** 2025-10-18
**Complexity:** Standard (5 points)
**Path:** B (standard)

---

## Violation

**Location:** `src/app/game_world.cpp:66-74`
**Principle:** Radical Simplicity, Composable Functions | Score: 6.5/10
**Severity:** High
**Type:** Duplication, Lost Single Source of Truth

**Current state:**
```cpp
// game_world.cpp:66-74
void setup_test_level(game_world& world) {
    // ...
    aabb ground_plane;
    ground_plane.center = glm::vec3(0.0f, -0.1f, 0.0f);
    ground_plane.half_extents = glm::vec3(100.0f, 0.1f, 100.0f);
    world.scn.add_collision_box(ground_plane);         // ← stored in scene
    world.world_geometry.boxes.push_back(ground_plane); // ← stored again

    // Repeated for every platform...
}
```

**Why violation:**
- Same geometry stored in two places: `scene.boxes` and `world_geometry.boxes`
- Must manually synchronize both on every add/remove
- No single source of truth - which one is authoritative?
- Violates "Radical Simplicity" - data exists in duplicate
- Violates "Composable Functions" - systems should read from shared source, not maintain copies

**Impact:**
- Blocks: Clean level editing/creation (must remember to update both)
- Cascades to: Collision detection, rendering, debug visualization
- Foundation: Layer 3 (Systems) - affects game_world at 95%

---

## Fix

**Approach:** Simplify - Unify to single authoritative source

**SIMPLIFY:**

**From:** Dual storage requiring manual synchronization
```cpp
world.scn.add_collision_box(ground_plane);         // Storage 1
world.world_geometry.boxes.push_back(ground_plane); // Storage 2
```

**To:** Single authoritative list, both systems reference it
```cpp
world.world_geometry.boxes.push_back(ground_plane); // Single storage
// scene accesses collision_world directly for rendering
```

**Design decision - where should single source live?**

**Option A: scene owns geometry**
- `scene` becomes single source
- Remove `world_geometry` from `game_world`
- Collision detection reads from `scene.collision_boxes()`
- **Pro:** Scene already manages visual meshes
- **Con:** Mixes rendering with physics (not orthogonal)

**Option B: world_geometry owns, scene references**
- `collision_world` becomes single source
- `scene` gets reference/pointer to `collision_world`
- Scene reads boxes from `collision_world` for rendering
- **Pro:** Orthogonal - physics owns physical data, rendering reads it
- **Con:** Added dependency (acceptable)

**Chosen: Option B** (orthogonal systems, clear ownership)

**Implementation:**
1. Remove `std::vector<aabb> boxes` from `scene` class
2. Add `const collision_world*` reference to `scene` constructor/init
3. Update `scene::collision_boxes()` to return `world_geometry->boxes`
4. Remove all `scene.add_collision_box()` calls
5. Keep only `world_geometry.boxes.push_back()` calls
6. Update renderer to read from `scene.collision_boxes()` (already does)

---

## Completed

**Change:** Deleted all collision box storage from scene class
**Approach:** Even simpler than planned - boxes were dead code (never used)
**Tests:** Build passing, no tests affected (test_spring_damper unrelated)
**Metrics:**
- LOC: 21 lines deleted, 0 added (-21)
- Files: 3 modified (scene.h, scene.cpp, game_world.cpp)
- Principle Score: 6.5/10 → 8.5/10 (+2.0)
**Result:** ✓ Violation removed

**Actual changes:**
1. Removed `std::vector<aabb> boxes` from scene.h
2. Removed `void add_collision_box(const aabb&)` from scene.h
3. Removed `const std::vector<aabb>& collision_boxes() const` from scene.h
4. Removed `#include "foundation/collision_primitives.h"` from scene.h (no longer needed)
5. Deleted `add_collision_box()` implementation from scene.cpp
6. Deleted `collision_boxes()` implementation from scene.cpp
7. Updated `scene::clear()` to not clear boxes (scene.cpp)
8. Removed 8 calls to `world.scn.add_collision_box()` from game_world.cpp

**Discovery:** The `scene.boxes` storage was completely unused dead code:
- Never accessed by renderer
- Never accessed by collision system
- `collision_boxes()` method never called
- Only `world_geometry.boxes` was actually used

This is deletion at its purest: removing code that served no purpose.

---

## Principle Validation

**Principle:** Radical Simplicity, Composable Functions
**Before:** 6.5/10 | Violations: Duplicate storage, manual sync required, unclear ownership
**After:** 8.5/10 | Violations: None
**Improvement:** +2.0 points
**Evidence:**
- Single source of truth: `collision_world.boxes`
- No synchronization needed
- Clear ownership: physics owns physics data
- Scene simplified to only visual meshes
**Verdict:** ✓ Principle restored

---

## Risk

**Could go wrong:**
- Renderer might assume scene owns boxes internally
- Lifetime issues if `collision_world` destroyed before scene
- Tests might check both locations

**Mitigation:**
- Check all renderer code accessing collision boxes
- Ensure `game_world` member order (collision_world before scene)
- Update tests to verify single source only

**Rollback:**
```bash
git checkout main -- src/app/game_world.{h,cpp} src/rendering/scene.{h,cpp}
```

---

## Files Affected

**Modified:**
- `src/app/game_world.cpp` - Remove scene.add_collision_box() calls
- `src/rendering/scene.h` - Remove boxes vector, add collision_world reference
- `src/rendering/scene.cpp` - Update collision_boxes() to return reference

**Potentially affected:**
- `src/app/debug_generation.cpp` - May access collision boxes
- `src/rendering/renderer.cpp` - Draws collision boxes
- `tests/*` - Any tests checking collision geometry

---

**One source. One truth. No synchronization.**
