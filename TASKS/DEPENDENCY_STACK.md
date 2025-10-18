# Dependency Stack

**Foundation certainty. Build order. What's possible now.**

**Overall Foundation:** 92%
**Last Updated:** 2025-10-17

---

## Current Stack

```
Layer 5: POLISH          [<50%] ← liquid backlog
    ↓
Layer 4: VARIATION       [<50%] ← blocked (need Layer 3 ≥95%)
    ↓
Layer 3: SYSTEMS         [94%] ← active (2 refinements to 95%)
    ↓
Layer 2: PRIMITIVES      [100%] ← ✅ COMPLETE
    ↓
Layer 1: CORE            [90%] ← stable (documented issues)
```

**Cascade:** 0.90 × 1.00 × 0.94 = **85% survival**

**Target:** 0.95 × 1.00 × 0.95 = **90% survival** (Layer 3 → 95%)

---

## Layer 1: CORE (90%)

**Status:** ✅ Stable - working, known issues documented

| System | Certainty | Status | Issues |
|--------|-----------|--------|--------|
| Runtime | 100% | ✅ Solid | None |
| Input | 95% | ✅ Clean | None |
| Camera | 95% | ✅ Working | None |
| Physics | 90% | ⚠️ Documented | Accumulated state pattern |

**Blocking:** Accumulated state needs documentation or fix (not urgent)

---

## Layer 2: PRIMITIVES (100%)

**Status:** ✅ **COMPLETE** - Fully validated foundation

| System | Certainty | Validation | Location |
|--------|-----------|------------|----------|
| Spring-Damper | 100% | ✅ Tested | `foundation/spring_damper.{h,cpp}` |
| Collision | 100% | ✅ Proven | `foundation/collision_primitives.h` |
| Math Utils | 100% | ✅ Asserted | `foundation/math_utils.h` |
| Easing | 100% | ✅ Standard | `foundation/easing.{h,cpp}` |
| Debug Viz | 100% | ✅ Battle-tested | `rendering/debug_*` |
| Procedural Mesh | 100% | ✅ Working | `foundation/procedural_mesh.{h,cpp}` |

**Blocking:** None

---

## Layer 3: SYSTEMS (94%)

**Status:** 🔧 Repair mode - 2 refinements to 95%

| System | Certainty | Status | Location |
|--------|-----------|--------|----------|
| Rendering | 100% | ✅ Solid | `rendering/scene.{h,cpp}` |
| GUI | 95% | ✅ Working | `gui/*` |
| Game World | 95% | ✅ Clean | `app/game_world.{h,cpp}` |
| Tuning | 95% | ✅ Fixed | `character/tuning.{h,cpp}` |
| Landing Anim | 96% | ✅ Exemplary | `character/animation.{h,cpp}` |
| Orientation | 100% | ✅ Refined | `character/orientation.{h,cpp}` |
| Controller | 90% | ⚠️ Mixed concerns | `character/controller.{h,cpp}` |

**Blocking:**
1. Controller mixed concerns (HIGH) - physics + animation + orientation coupled
2. Accumulated state pattern (MEDIUM) - needs documentation

**Path to 95%:** Fix controller concerns (~2 refinements)

---

## Layer 4: VARIATION (<50%)

**Status:** 🚫 Blocked - don't build until Layer 3 ≥95%

**Waiting on repairs:**
- Skeletal Animation System (5-8 points)
- Walk/Run Speed States (3-5 points)
- Wall Detection & Sliding (3-5 points)
- Dash Mechanic (3-5 points)
- IK Systems (8 points)

**Why blocked:** Building on 94% foundation = 85% survival. Need 95% → 90% survival.

---

## Layer 5: POLISH (<50%)

**Status:** 🚫 Liquid backlog only

Defer until Layer 4 solid:
- Audio system
- UI/HUD
- Visual effects
- Terrain
- Combat

---

## Build Rules

### ✅ Can Build
- Layer 2 primitives (if needed)
- Foundation ≥98% at Layer 2 ✓
- **Current:** Layer 2 complete, can add new primitives

### 🔧 Should Repair (Current Mode)
- Layer 3 at 94%, need 95%
- Known violations documented
- ~2 refinements remaining
- **Current:** Fix controller, document accumulated state

### 🚫 Cannot Build
- Layers 4-5 blocked
- Foundation <95% for those layers
- High probability of redesign
- **Current:** Wait for Layer 3 repairs

---

## Certainty Scores

**How scores are calculated:**

Rate each system 1-10 on these criteria:
1. Mathematical correctness proven
2. Emergent behaviors documented
3. Debug visualization functional
4. No special cases
5. Composable with siblings
6. Player control preserved
7. Consistent behavior
8. Can't be simplified further
9. Passed multiple playtests
10. Survived integration

**Score × 10% = certainty**

---

## Next Actions

**To reach 95% Layer 3:**
1. Fix controller mixed concerns (3 points)
2. Document accumulated state pattern (2 points)

**Then:**
- Layer 4 unlocked
- Can build variations confidently
- Foundation: 95% × 100% × 95% = 90% survival

---

## Recent Changes

- 2025-10-17: Orientation → 100% (spring-damper replacement)
- 2025-10-17: Layer 2 → 100% (validation complete)
- 2025-10-17: Tuning → 95% (defaults fixed)

**See:** `PLANS/REFINE_*.md` for detailed metrics

---

**Build upward. Never skip layers. Strengthen continuously.**
