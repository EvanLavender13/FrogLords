# Dependency Stack

**Foundation certainty. Build order. What's possible now.**

**Overall Foundation:** 92.5%
**Last Updated:** 2025-10-18

---

## Current Stack

```
Layer 5: POLISH          [<50%] ← liquid backlog
    ↓
Layer 4: VARIATION       [<50%] ← blocked (need Layer 3 ≥95%)
    ↓
Layer 3: SYSTEMS         [95%] ← ✅ TARGET REACHED
    ↓
Layer 2: PRIMITIVES      [100%] ← ✅ COMPLETE
    ↓
Layer 1: CORE            [90%] ← stable (documented issues)
```

**Cascade:** 0.90 × 1.00 × 0.95 = **85.5% survival**

**Target:** 0.95 × 1.00 × 0.95 = **90% survival** (Layer 1 → 95%)

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

## Layer 3: SYSTEMS (95%)

**Status:** ✅ **TARGET REACHED** - Ready for Layer 4

| System | Certainty | Status | Location |
|--------|-----------|--------|----------|
| Rendering | 100% | ✅ Solid | `rendering/scene.{h,cpp}` |
| GUI | 95% | ✅ Working | `gui/*` |
| Game World | 95% | ✅ Clean | `app/game_world.{h,cpp}` |
| Tuning | 95% | ✅ Fixed | `character/tuning.{h,cpp}` |
| Landing Anim | 96% | ✅ Exemplary | `character/animation.{h,cpp}` |
| Orientation | 100% | ✅ Refined | `character/orientation.{h,cpp}` |
| Reactive Systems | 100% | ✅ Clean | `character/character_reactive_systems.{h,cpp}` |
| Controller | 100% | ✅ Refined | `character/controller.{h,cpp}` |

**Blocking:** None - Layer 3 at target

**Optional refinements:**
- Accumulated state pattern documentation (would improve Layer 1)

---

## Layer 4: VARIATION (<50%)

**Status:** ✅ **UNBLOCKED** - Layer 3 at 95%

**Ready to build:**
- Skeletal Animation System (5-8 points)
- Walk/Run Speed States (3-5 points)
- Wall Detection & Sliding (3-5 points)
- Dash Mechanic (3-5 points)
- IK Systems (8 points)

**Foundation:** Building on 95% Layer 3 = 85.5% survival (acceptable for variation)

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

### 🔧 Should Repair (Optional)
- Layer 3 at 95% ✅ TARGET REACHED
- Optional: Document accumulated state pattern (would improve Layer 1 to 95%)
- **Current:** Layer 3 complete, can proceed to Layer 4

### 🚫 Cannot Build
- Layer 5 blocked (need Layer 4 ≥95%)
- Polish deferred until variation solid
- **Current:** Layer 4 unblocked, can build variations

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

**Layer 3 at target (95%) - ready for Layer 4:**
- Build skeletal animation system, OR
- Build walk/run speed states, OR
- Build wall detection & sliding, OR
- Refine accumulated state pattern (optional, improves Layer 1)

**Foundation:** 0.90 × 1.00 × 0.95 = **85.5% survival** (acceptable for variation)

---

## Recent Changes

- 2025-10-18: Controller → 100% (extracted reactive systems, unidirectional data flow)
- 2025-10-18: Layer 3 → 95% ✅ TARGET REACHED
- 2025-10-17: Orientation → 100% (spring-damper replacement)
- 2025-10-17: Layer 2 → 100% (validation complete)

**See:** `PLANS/REFINE_*.md` for detailed metrics

---

**Build upward. Never skip layers. Strengthen continuously.**
