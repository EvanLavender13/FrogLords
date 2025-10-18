# Dependency Stack

**Foundation certainty. Build order. What's possible now.**

**Overall Foundation:** 97% ✅ TARGET EXCEEDED
**Last Updated:** 2025-10-18

---

## Current Stack

```
Layer 5: POLISH          [<50%] ← liquid backlog
    ↓
Layer 4: VARIATION       [<50%] ← ✅ UNBLOCKED (foundation at 97%)
    ↓
Layer 3: SYSTEMS         [97%] ← ✅ TARGET EXCEEDED
    ↓
Layer 2: PRIMITIVES      [100%] ← ✅ COMPLETE
    ↓
Layer 1: CORE            [95.5%] ← ✅ TARGET EXCEEDED
```

**Cascade:** 0.955 × 1.00 × 0.97 = **92.6% survival**

**Target exceeded:** Foundation at 97%, ready for Layer 4 variation

---

## Layer 1: CORE (95.5%)

**Status:** ✅ **TARGET EXCEEDED** - Clean foundation

| System | Certainty | Status | Location |
|--------|-----------|--------|----------|
| Runtime | 100% | ✅ Refined | `app/runtime.{h,cpp}` |
| Input | 95% | ✅ Clean | `app/input.{h,cpp}` |
| Camera | 95% | ✅ Working | `rendering/camera.{h,cpp}` |
| Physics | 95% | ✅ Documented | `character/controller.{h,cpp}` |

**Blocking:** None - target exceeded

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
| Mesh Renderer | 100% | ✅ Refined | `rendering/renderer.{h,cpp}` |

**Blocking:** None

---

## Layer 3: SYSTEMS (97%)

**Status:** ✅ **TARGET EXCEEDED** - Ready for Layer 4

| System | Certainty | Status | Location |
|--------|-----------|--------|----------|
| Rendering | 100% | ✅ Solid | `rendering/scene.{h,cpp}` |
| GUI | 95% | ✅ Working | `gui/*` |
| Game World | 97% | ✅ Refined | `app/game_world.{h,cpp}` |
| Tuning | 97% | ✅ Unidirectional | `character/tuning.{h,cpp}` |
| Landing Anim | 96% | ✅ Exemplary | `character/animation.{h,cpp}` |
| Orientation | 100% | ✅ Refined | `character/orientation.{h,cpp}` |
| Reactive Systems | 100% | ✅ Clean | `character/character_reactive_systems.{h,cpp}` |
| Controller | 100% | ✅ Refined | `character/controller.{h,cpp}` |

**Blocking:** None - Layer 3 exceeded target, foundation at 97%

---

## Layer 4: VARIATION (<50%)

**Status:** ✅ **UNBLOCKED** - Layer 3 at 96%

**Ready to build:**
- Skeletal Animation System (5-8 points)
- Walk/Run Speed States (3-5 points)
- Wall Detection & Sliding (3-5 points)
- Dash Mechanic (3-5 points)
- IK Systems (8 points)

**Foundation:** Building on 97% Layer 3 = 92.6% survival (excellent for variation)

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
- Foundation at 95% ✅ TARGET REACHED
- Optional: Further refinements for 95% → 100%
- **Current:** Foundation solid, can proceed to Layer 4

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

**Foundation exceeded target (97%) - ready for Layer 4:**
- Build skeletal animation system, OR
- Build walk/run speed states, OR
- Build wall detection & sliding, OR
- Build dash mechanic

**Foundation:** 0.955 × 1.00 × 0.97 = **92.6% survival** (excellent for variation)

---

## Recent Changes

- 2025-10-18: Runtime → 100% (removed embedded test logic)
- 2025-10-18: Layer 1 → 95.5% ✅ TARGET EXCEEDED
- 2025-10-18: Foundation → 97% ✅ TARGET EXCEEDED
- 2025-10-18: Mesh Renderer → 100% (persistent dynamic buffers)
- 2025-10-18: Layer 2 → 100% ✅ COMPLETE
- 2025-10-18: Tuning → 97% (unidirectional data flow established)
- 2025-10-18: Game World → 97% (tuning system refinement)
- 2025-10-18: Layer 3 → 97% ✅ TARGET EXCEEDED
- 2025-10-18: Controller → 100% (extracted reactive systems)
- 2025-10-17: Orientation → 100% (spring-damper replacement)

**See:** `PLANS/REFINE_*.md` for detailed metrics

---

**Build upward. Never skip layers. Strengthen continuously.**
