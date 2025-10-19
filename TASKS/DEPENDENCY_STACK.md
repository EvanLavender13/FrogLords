# Dependency Stack

**Foundation certainty. Build order. What's possible now.**

**Overall Foundation:** 98%+ ✅ TARGET EXCEEDED
**Last Updated:** 2025-10-19

---

## Current Stack

```
Layer 5: POLISH          [<50%] ← liquid backlog
    ↓
Layer 4: VARIATION       [74%] ← ✅ BUILDING (2 systems complete)
    ↓
Layer 3: SYSTEMS         [98%+] ← ✅ TARGET EXCEEDED
    ↓
Layer 2: PRIMITIVES      [100%] ← ✅ COMPLETE
    ↓
Layer 1: CORE            [98%] ← ✅ TARGET EXCEEDED
```

**Cascade:** 0.98 × 1.00 × 0.98 × 0.74 = **71.0% survival**

**Target exceeded:** Foundation at 98%, ready for Layer 4 variation

---

## Layer 1: CORE (98%)

**Status:** ✅ **TARGET EXCEEDED** - Clean foundation

| System | Certainty | Status | Location |
|--------|-----------|--------|----------|
| Runtime/Camera | 100% | ✅ Refined | `app/runtime.{h,cpp}` |
| Camera | 98% | ✅ Refined | `camera/camera.{h,cpp}` + `camera/camera_follow.{h,cpp}` |
| Controller | 97% | ✅ Refined | `character/controller.{h,cpp}` |
| Input | 95% | ✅ Clean | `app/input.{h,cpp}` |

**Blocking:** None - target exceeded

---

## Layer 2: PRIMITIVES (100%)

**Status:** ✅ **COMPLETE** - Fully validated foundation

| System | Certainty | Validation | Location |
|--------|-----------|------------|----------|
| Spring-Damper | 100% | ✅ Tested | `foundation/spring_damper.{h,cpp}` |
| Collision | 100% | ✅ Refined | `foundation/collision.{h,cpp}` |
| Math Utils | 100% | ✅ Asserted | `foundation/math_utils.h` |
| Easing | 100% | ✅ Standard | `foundation/easing.{h,cpp}` |
| Debug Viz | 100% | ✅ Battle-tested | `rendering/debug_*` |
| Procedural Mesh | 100% | ✅ Working | `foundation/procedural_mesh.{h,cpp}` |
| Mesh Renderer | 100% | ✅ Refined | `rendering/renderer.{h,cpp}` |

**Blocking:** None

---

## Layer 3: SYSTEMS (98%+)

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

**Blocking:** None - Layer 3 exceeded target, foundation at 98%+

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

**Foundation:** Building on 98%+ Layer 3 = 96.0%+ survival (excellent for variation)

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
- Optional: Further refinements for 98% → 100%
- **Current:** Foundation excellent at 98%, can proceed to Layer 4

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
