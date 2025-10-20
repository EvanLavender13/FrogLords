# Dependency Stack

**Foundation certainty. Build order. What's possible now.**

**Overall Foundation:** 99%+ ✅ TARGET EXCEEDED
**Last Updated:** 2025-10-20

---

## Current Stack

```
Layer 5: POLISH          [<50%] ← liquid backlog
    ↓
Layer 4: VARIATION       [74%] ← ✅ BUILDING (2 systems complete)
    ↓
Layer 3: SYSTEMS         [99%+] ← ✅ TARGET EXCEEDED
    ↓
Layer 2: PRIMITIVES      [100%] ← ✅ COMPLETE
    ↓
Layer 1: CORE            [100%] ← ✅ COMPLETE
```

**Cascade:** 1.00 × 1.00 × 0.99 × 0.74 = **73.3% survival**

**Target exceeded:** Foundation at 99%, ready for Layer 4 variation

---

## Layer 1: CORE (100%)

**Status:** ✅ **COMPLETE** - Perfect foundation

| System | Certainty | Status | Location |
|--------|-----------|--------|----------|
| Runtime/Camera | 100% | ✅ Refined | `app/runtime.{h,cpp}` |
| Camera | 98% | ✅ Refined | `camera/camera.{h,cpp}` + `camera/camera_follow.{h,cpp}` |
| Controller | 100% | ✅ Refined | `character/controller.{h,cpp}` |
| Input | 95% | ✅ Clean | `app/input.{h,cpp}` |

**Blocking:** None - layer complete

---

## Layer 2: PRIMITIVES (100%)

**Status:** ✅ **COMPLETE** - Fully validated foundation

| System | Certainty | Validation | Location |
|--------|-----------|------------|----------|
| Spring-Damper | 100% | ✅ Tested | `foundation/spring_damper.{h,cpp}` |
| Collision | 100% | ✅ Refined (2025-10-20) | `foundation/collision.{h,cpp}`, `collision_primitives.h` |
| Math Utils | 100% | ✅ Asserted | `foundation/math_utils.h` |
| Easing | 100% | ✅ Standard | `foundation/easing.{h,cpp}` |
| Debug Viz | 100% | ✅ Battle-tested | `rendering/debug_*` |
| Procedural Mesh | 100% | ✅ Working | `foundation/procedural_mesh.{h,cpp}` |
| Mesh Renderer | 100% | ✅ Refined | `rendering/renderer.{h,cpp}` |

**Blocking:** None

---

## Layer 3: SYSTEMS (99%+)

**Status:** ✅ **TARGET EXCEEDED** - Ready for Layer 4

| System | Certainty | Status | Location |
|--------|-----------|--------|----------|
| Rendering | 100% | ✅ Solid | `rendering/scene.{h,cpp}` |
| GUI | 100% | ✅ Refined | `gui/*` |
| Game World | 98% | ✅ Refined | `app/game_world.{h,cpp}` |
| Tuning | 97%+ | ✅ Physical | `character/tuning.{h,cpp}` |
| Landing Anim | 96% | ✅ Exemplary | `character/animation.{h,cpp}` |
| Orientation | 100% | ✅ Refined | `character/orientation.{h,cpp}` |
| Reactive Systems | 100% | ✅ Clean | `character/character_reactive_systems.{h,cpp}` |
| Debug Draw | 100% | ✅ Refined | `rendering/debug_draw.cpp` |

**Blocking:** None - Layer 3 exceeded target, foundation at 99%+

---

## Layer 4: VARIATION (74%)

**Status:** ✅ **BUILDING** - 2 systems complete

**Certainty:** 74%
**Last Updated:** 2025-10-19

**Systems at this layer:**
- **Wall Sliding** (principle avg: 9.3/10 = 93%)
- **Walk/Run Speed States** (principle avg: 8.7/10 = 87%)

**Layer calculation:**
(93% + 87%) / 2 systems = 90% actual
Conservative estimate: 74% (accounting for breadth needed - 2-3 more systems to 95%)

| System | Certainty | Status | Location |
|--------|-----------|--------|----------|
| Wall Sliding | 93% | ✅ Complete | `foundation/collision.{h,cpp}` |
| Walk/Run Speed States | 87% | ✅ Complete | `character/controller.{h,cpp}` |

**Ready to build:**
- Skeletal Animation System (5-8 points)
- Dash Mechanic (3-5 points)
- IK Systems (8 points - requires skeletal animation)

**Foundation:** Building on 99%+ Layer 3 = 97.0%+ survival (excellent for variation)

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
- Foundation at 95% ✅ TARGET EXCEEDED
- Optional: Further refinements for 99% → 100%
- **Current:** Foundation excellent at 99%, can proceed to Layer 4

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

**To reach 95% Layer 4:**
1. Build 2-3 more variation systems (currently 2/4-5)
2. Each system should score ≥8.5/10 average

**Systems now buildable:**
- Skeletal Animation System (5-8 points) - No new dependencies
- Dash Mechanic (3-5 points) - No new dependencies

---

**Build upward. Never skip layers. Strengthen continuously.**
