# Dependency Stack

**Purpose:** Identify key dependencies to determine work order and reduce cascading uncertainty.

**Status:** Living document (updated as design solidifies)

Historical snapshots: See `PLANS/ARCHIVE/`

**Visualization Philosophy:** Inspired by Tynan Sylvester's *Designing Games* (Part Three: Dependencies). The tree structure makes cascading uncertainty viscerally obvious—changes at the bottom ripple upward through all dependent systems.

---

## Triggers to Update
- After changes to core logic/rules/physics that can ripple upward (e.g., `src/foundation/*`, `src/character/controller.*`).
- When introducing a new system or changing a public interface between modules.
- When certainty of any node meaningfully shifts (±10–20%) due to playtests or metrics.
- When pulling a backlog item into active work (add it, connect dependencies, tag certainty).
- After refactors that change data representations (e.g., quaternion formats, keyframe structures).
- When external dependencies change constraints (toolchain, libraries, platforms).
- Before starting a feature that depends on nodes <90% certain—reassess ordering and risk.

## Dependency Tree

```
═══════════════════════════════════════════════════════════════════════════
                          DESIGN BACKLOG (LIQUID POOL)
                         Everything below: <50% certain
                         No dependencies assumed, pull as needed
═══════════════════════════════════════════════════════════════════════════
    [Extended Keyframes] [Running Gait] [Bounce Gravity]
    [IK Systems] [Wall Detection] [Ragdoll] [Dash] [Terrain] [Combat]
                    [Audio] [UI Polish] [🐸 Frog Ideas 🐸]
═══════════════════════════════════════════════════════════════════════════
                                      ▲
                                      │
                              ← YOU ARE HERE (pull next feature)
                                      │
───────────────────────────────────────────────────────────────────────────
                         COMPLETED FEATURES (100%) ✅
───────────────────────────────────────────────────────────────────────────

**Skeletal Animation Stack:** Debug system → attach to body → keyframe preview → primary anim → secondary motion → pose blending
**Debug & Polish:** Visual overhaul (trail, plots, ring), walk/run transition, freeze velocity trail, character axis gizmo
**Reactive Animation:** Acceleration tilt, landing spring, tuning UI

See [ARCHIVE/](ARCHIVE/) for detailed retrospectives and [implementation_*.md](.) for specifics

───────────────────────────────────────────────────────────────────────────
                    CORE GAMEPLAY LAYER (~95% certain)
                    Stable foundation, proven patterns
───────────────────────────────────────────────────────────────────────────

              ┌────────────────┬────────────────┐
              ▼                ▼                ▼
    ┌──────────────────┐  ┌──────────────────────┐  ┌──────────────────┐
    │ Reactive Anim    │  │  Procedural Anim     │  │ Skeletal System  │
    │ • Accel Tilt     │  │  • Orientation       │  │ • Keyframes      │
    │ • Landing Spring │  │  • Locomotion        │  │ • Pose Blending  │
    │ • Tuning UI      │  │  (surveyor wheel)    │  │ • Spring Motion  │
    └────────┬─────────┘  └──────────┬───────────┘  └────────┬─────────┘
             │                       │                        │
             └───────────────────────┴────────────────────────┘
                                     ▼
                          ┌────────────────────┐
                          │ Character Controller│ 95%
                          │ (physics, input)    │
                          └──────────┬──────────┘
                                     │
                   ┌─────────────────┼─────────────────┐
                   ▼                 ▼                 ▼
         ┌─────────────┐    ┌──────────────┐    ┌──────────┐
         │ Collision   │    │ Camera System│    │  Input   │
         │ (sphere-box)│    │ (orbit+zoom) │    │ (WASD)   │
         └─────────────┘    └──────────────┘    └──────────┘
                95%                95%                95%

─────────────────────────────────────────────────────────────────────────── 
                     FOUNDATION LAYER (90-100% certain)
                     Derivative systems, proven tech
───────────────────────────────────────────────────────────────────────────

              ┌────────────────┴────────────────┐
              ▼                                 ▼
    ┌──────────────────┐            ┌──────────────────────┐
    │ Foundation Utils │ 100%       │   Rendering Layer    │ 90%
    │ • Spring-Damper  │            │   • Scene            │
    │ • Easing Curves  │            │   • Debug Draw       │
    │ • Collision Math │            │   • Wireframe        │
    └──────────────────┘            └──────────┬───────────┘
                                               │
                                               ▼
                                    ┌──────────────────────┐
                                    │   Runtime/App        │ 100% ✅
                                    │   (main loop, frame) │
                                    └──────────────────────┘
```

## Reading the Tree

**Arrows flow bottom → top**: Lower systems are dependencies; upper systems depend on them.

**Cascading uncertainty**: A change in any box forces changes in ALL boxes above it that connect via arrows. Example: If `Character Controller` changes, it could cascade through `Reactive Anim`, `Skeletal Animation`, and `Pose Blending`.

**Parallel branches**: Systems on separate branches can develop independently until they merge. Example: `Debug Visual Overhaul` and `Pose Blending` could be worked on simultaneously—they don't block each other.

**Certainty levels**: 
- **100% ✅**: Completed and validated through iteration
- **90-95%**: Working but may need tuning based on playtesting
- **<50%**: Design Backlog—premature to build until foundation stabilizes

---

## Layer Details

### Foundation Layer (90-100% certain) 🟢

**Status:** Stable, derivative systems based on proven tech

**Why certain?** These are implementations of well-understood patterns from other games/engines. Risk is low because we're not inventing anything novel here.

**Systems:**
- **Runtime/App (100% ✅):** Main loop, frame timing, window management — bedrock of everything
- **Rendering Layer (90%):** Scene management, debug draw, wireframe visualization
- **Foundation Utils (100%):** Spring-damper, easing curves, collision primitives — reusable math

**Cascading Risk:** Changes here would devastate everything above. Fortunately, these are complete and stable.

---

### Core Gameplay Layer (95% certain) 🟢

**Status:** Working physics loop, proven procedural systems

**Why certain?** Core character controller is stable. Procedural locomotion/orientation systems validated through multiple iterations. The game is "playable" at this layer—you can run, jump, and collide.

**Core Gameplay Definition:** Character sphere moving in 3D space with physics-driven control
- **Irreducible minimum:** Position, velocity, acceleration, collision, input response
- **Already creates meaningful experience:** Run, jump, explore, collide

**Systems:**
- **Character Controller (95%):** Single-sphere physics, grounded state, jump, friction
- **Input (95%):** WASD keyboard, mouse camera control
- **Camera (95%):** Orbit + follow modes, zoom, camera-relative movement
- **Collision (95%):** Sphere-AABB resolution, multi-pass stability
- **Procedural Orientation (95%):** Velocity-based facing with smooth rotation
- **Procedural Locomotion (95%):** Distance-phased surveyor wheel, speed blending

**Dependency Risk:** Changes in core gameplay would cascade through reactive animation, skeletal systems, and all completed features above. But risk is low—this layer has been tested and works.

---

### Completed Features (100% ✅) 🎉

**Status:** Foundation proven through 8 completed iterations

**Completed Stack:**
- **Skeletal Animation:** Full pipeline from debug → attach → keyframes → primary → secondary → blending ✅
- **Reactive Animation:** Acceleration tilt, landing spring, walk/run transitions, tuning UI ✅
- **Debug Tooling:** Velocity trail (with freeze-on-stop), plots, speed ring, camera zoom, unified panels, character axis gizmo ✅
- **Running Gait Keyframes:** Four-pose run cycle (REACH_LEFT → PASS_RIGHT → REACH_RIGHT → PASS_LEFT) with larger limb extension than walk ✅

**Key Patterns Validated:**
- Quaternion keyframe architecture with hemisphere-safe slerp
- Distance-phased triggering (surveyor wheel pattern)
- Position-delta gating for debug visualizations
- Velocity-injection for spring systems
- Dual-reference pattern for smooth transitions
- Debug visualization layer pattern (toggle-driven, zero gameplay impact)
- Gait clarity through silhouette scaling (run vs walk magnitude differentiation)

See [ARCHIVE/dependency_stack_snapshot_2025-10-10.md](ARCHIVE/dependency_stack_snapshot_2025-10-10.md) and `implementation_*.md` files for detailed retrospectives

---

### Design Backlog (<50% certain) ⚠️

**Status:** Liquid pool of ideas—most will be cut or redesigned

**Why uncertain?** These ideas haven't been tested. They sound good on paper, but cascading uncertainty from changes in foundation layers means detailed planning is premature and likely wasteful.

**Philosophy:** Keep backlog non-interlocking and liquid. Pull features only when foundation stabilizes to 90%+ certainty and actual gameplay needs become clear through iteration.

**Categories:**
- **Debug Visualization:** Freeze velocity trail on stop, character axis gizmo
- **Extended Animation:** Running gait keyframes, extended joint sets, IK systems
- **Movement Abilities:** Dash, wall jump, ledge grab, climbing
- **Combat:** Melee, ranged, enemy AI (uncertain—game design unclear)
- **Environment:** Terrain, moving platforms, destructibles
- **Polish:** Art, audio, particles, post-processing (premature—graybox iteration first)
- **🐸 Frog Ideas:** Tongue grapple, hop charge, sticky surfaces, fly catching (wild speculation)

See [DESIGN_BACKLOG.md](DESIGN_BACKLOG.md) for full list with rationale.

**Cascading Risk:** Building these now = high probability of wasted work when foundation shifts.

---

## Development Strategy

**Current Focus:** ← **YOU ARE HERE** — Pull next feature from Design Backlog

Foundation is stable (90-100% certainty). Core gameplay loop proven. Ready to expand based on actual playtesting needs rather than speculative planning.

**Completed Work Order:**
1. ✅ Foundation (primitives, rendering, runtime)
2. ✅ Core gameplay (physics, procedural systems)
3. ✅ Skeletal animation pipeline (8-feature stack)
4. ✅ Reactive animation (tilt, spring, tuning)
5. ✅ Debug tooling & polish (trail, ring, transitions, axis gizmo)
6. ✅ Running gait keyframes (4-pose run cycle)

**Planning Horizon (Adaptive):**

The more certain a layer, the longer you can plan ahead:

| Layer Certainty | Planning Horizon | Rationale |
|-----------------|------------------|-----------|
| 90-100% (Foundation) | Large chunks (3-8 complexity points) | Derivative systems, proven patterns |
| 70-90% (Core Gameplay) | Moderate chunks (2-4 points) | Working but needs tuning |
| 50-70% (Next Features) | Short iterations (1-2 points) | Test assumptions quickly |
| <50% (Backlog) | **Do not plan** | Cascading uncertainty too high |

**Why this matters:** Original/uncertain work needs tight feedback loops. Derivative/certain work can be planned in larger increments. Lower test cost → shorter horizon.

**Risk Management:**
- ✅ Build bottom-up: Validate foundation before upper layers
- ✅ Accept temporary roughness early: Quality comes from iteration count, not perfect first passes
- ✅ Keep backlog liquid: No interlocking dependencies
- ✅ Graybox expensive things: Levels, creatures, UI, audio (art hides mechanical problems)
- ✅ Stabilize before expanding: Don't build on uncertain foundations
- ⚠️ Watch for "big leaps": Intentional jumps to escape local maxima are allowed, but return to tight loops after

**Cascading Uncertainty Formula:**

If each layer has `certainty C`, and a feature depends on `N` layers below it, the probability it survives unchanged is:

```
P(unchanged) = C^N
```

**Example:** Building a feature 5 layers up the stack:
- If each layer is 80% certain: `0.8^5 = 0.33` (33% chance of surviving unchanged)
- If each layer is 50% certain: `0.5^5 = 0.03` (3% chance—effectively zero)

**Implication:** Upper-stack features WILL change. Don't over-invest in detailed planning. Embrace iteration.

---

## Meta-Notes

**What this document does:**
- Makes cascading uncertainty **visible** through tree structure
- Guides work order: build bottom-up, stabilize before expanding
- Prevents premature planning of upper-stack features
- Tracks completed work and proven patterns

**What this document doesn't do:**
- Predict the future (upper layers WILL change)
- Lock design decisions (foundation stability ≠ unchangeable)
- Guarantee backlog features will ship (most won't)
- Replace playtesting (testing creates knowledge; documents record it)

**Dependency clarification:**
- **Dependency = changes in A force changes in B** (not "A must exist for B to work")
- Only showing **strongest dependencies** (deliberate reduction for focus)
- Circular dependencies exist but weakest ones ignored
- Stack will reshape as discoveries happen

**Key learnings embedded in this stack:**
- **Motion vs. Structure:** Reactive systems need dynamic state sources, not just data structures
- **Velocity-injection pattern:** Superior to offset manipulation for spring systems
- **Distance-phased triggering:** Surveyor wheel scales from locomotion to skeletal animation
- **Data structure validation:** Validate novel representations (quaternions) in isolation first
- **Graybox discipline:** Parameters over assets; iteration over polish

**Historical snapshots:**
- Detailed retrospectives in [ARCHIVE/dependency_stack_snapshot_2025-10-10.md](ARCHIVE/dependency_stack_snapshot_2025-10-10.md)
- Implementation notes in `PLANS/implementation_*.md` and `PLANS/code_review_*.md`

**Source material:**
- Dependency stack concept from Tynan Sylvester's *Designing Games* (Part Three: Process - Dependencies)
- Adapted for solo experimental game dev with emphasis on rapid iteration and uncertainty quantification

---

**Last Updated:** 2025-10-12 (Running Gait Keyframes completed and moved to Completed Features)
