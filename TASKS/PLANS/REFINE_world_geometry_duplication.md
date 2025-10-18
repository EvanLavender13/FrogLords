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

**Files:**
- `src/rendering/scene.h`: 22 → 18 lines (-4)
- `src/rendering/scene.cpp`: 26 → 17 lines (-9)
- `src/app/game_world.cpp`: 120 → 112 lines (-8)
- **Total:** -21 lines

**Violations removed:**
- Duplicate storage locations: 2 → 1 (-1)
- Dead code methods: 3 methods deleted (add_collision_box, collision_boxes, unnecessary include)
- Duplicate storage calls: 8 removed from setup_test_level()

**Principle Score:**
- **Before:** 6.5/10 (duplicate storage, manual sync, unclear ownership)
- **After:** 8.5/10 (single source of truth, orthogonal systems)
- **Improvement:** +2.0 points

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

## Learning

**Root cause:** The scene class originally had `add_collision_box()` and `collision_boxes()` methods, suggesting it was designed to own collision geometry. However, when `collision_world` was added as the authoritative physics source, the scene storage was never removed - both systems existed in parallel with manual duplication.

**Why it persisted:** The duplicate storage was never actually used:
- Renderer never called `scene.collision_boxes()`
- Collision system only used `world_geometry.boxes`
- Tests never verified scene storage
- Dead code is invisible until you look for it

**Prevention:**
1. **Never add storage without a single clear reader** - if no system accesses it, delete it
2. **Grep for usage before adding new storage** - check if data already exists elsewhere
3. **Question dual add() calls** - pattern `a.add(x); b.add(x);` is a red flag
4. **Audit storage locations during system design** - who owns what?

**Pattern discovered:** This is not isolated - whenever two systems might "need" the same data:
- **First:** Verify both actually use it (one might not)
- **Second:** Decide single owner based on orthogonality (physics owns physics data)
- **Third:** Other systems read via reference/getter, never duplicate

**Remaining work:** Audit for similar duplication patterns:
- Does `player_state` duplicate any physics data?
- Does animation system store physics state?
- Are any other world properties duplicated between systems?

**Key insight:** The best refactoring is often pure deletion. The scene storage served no purpose - it was waste waiting to be removed. When you find duplication, check if one side is completely unused before building elaborate unification schemes.

---

## Principle Validation

**Principle:** Radical Simplicity, Composable Functions

**Before:** 6.5/10
- Violations: Duplicate storage, manual sync required, unclear ownership
- Evidence: 2 storage locations, 8 duplicate calls, 3 dead code methods

**After:** 8.5/10
- Violations: None
- Evidence: Single source of truth (`collision_world.boxes`), no sync needed, orthogonal ownership

**Improvement:** +2.0 points

**Foundation Impact:**
- Layer 3 (Systems): 95% → 96% (+1%)
- Overall: 95% → 95.5% (+0.5%)
- Cascade to Layer 4: 90.25% → 91% survival

**Verdict:** ✓ Principle restored, foundation strengthened

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
