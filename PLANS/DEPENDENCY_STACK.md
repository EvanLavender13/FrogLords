# Dependency Stack

**Purpose:** Identify key dependencies to determine work order and reduce cascading uncertainty.

**Status:** Living document (updated as design solidifies)

**Visualization Philosophy:** Inspired by Tynan Sylvester's *Designing Games* (Part Three: Dependencies). The tree structure makes cascading uncertainty viscerally obvious—changes at the bottom ripple upward through all dependent systems.

---

## Dependency Tree

```
═══════════════════════════════════════════════════════════════════════════
                          DESIGN BACKLOG (LIQUID POOL)
                         Everything below: <50% certain
                         No dependencies assumed, pull as needed
═══════════════════════════════════════════════════════════════════════════
    [Freeze Trail] [Axis Gizmo] [Extended Keyframes] [Running Gait]
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

                    ┌──────────────────────┐
                    │ Walk/Run Transition  │ ✅ 100%
                    │ (smoothed max_speed) │
                    └──────────┬───────────┘
                               │
              ┌────────────────┴────────────────┐
              ▼                                 ▼
    ┌──────────────────┐            ┌──────────────────────┐
    │  Pose Blending   │ ✅ 100%    │ Debug Visual Overhaul│ ✅ 100%
    │ (quat slerp)     │            │ (trail, plots, ring) │
    └────────┬─────────┘            └──────────────────────┘
             │
             ▼
    ┌──────────────────┐
    │ Secondary Motion │ ✅ 100%
    │ (spring lag)     │
    └────────┬─────────┘
             │
             ▼
    ┌────────────────────────┐
    │ Primary Skeletal Anim  │ ✅ 100%
    │ (distance-phased)      │
    └────────┬───────────────┘
             │
             ▼
    ┌────────────────────────┐
    │ Static Keyframe Preview│ ✅ 100%
    │ (quat validation)      │
    └────────┬───────────────┘
             │
             ▼
    ┌────────────────────────┐
    │ Attach Skeleton to Body│ ✅ 100%
    │ (transform follow)     │
    └────────┬───────────────┘
             │
             ▼
    ┌────────────────────────┐
    │ Skeleton Debug System  │ ✅ 100%
    │ (hierarchy + visual)   │
    └────────┬───────────────┘
             │
             └─────────────────┐
                               │
─────────────────────────────────────────────────────────────────────────── 
                    CORE GAMEPLAY LAYER (~95% certain)
                    Stable foundation, proven patterns
───────────────────────────────────────────────────────────────────────────
                               │
              ┌────────────────┴────────────────┐
              ▼                                 ▼
    ┌──────────────────┐            ┌──────────────────────┐
    │ Reactive Anim    │ ✅ 100%    │  Procedural Anim     │ 95%
    │ • Accel Tilt     │            │  • Orientation       │
    │ • Landing Spring │            │  • Locomotion        │
    │ • Tuning UI      │            │  (surveyor wheel)    │
    └────────┬─────────┘            └──────────┬───────────┘
             │                                 │
             └────────────────┬────────────────┘
                              ▼
                   ┌────────────────────┐
                   │ Character Controller│ 95%
                   │ (physics, input)    │
                   └──────────┬──────────┘
                              │
              ┌───────────────┼───────────────┐
              ▼               ▼               ▼
    ┌─────────────┐  ┌──────────────┐  ┌──────────┐
    │ Collision   │  │ Camera System│  │  Input   │
    │ (sphere-box)│  │ (orbit+zoom) │  │ (WASD)   │
    └─────────────┘  └──────────────┘  └──────────┘
           95%              95%              95%

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

**Status:** Validated through iteration, patterns proven

These features are built on the stable foundation below. They represent completed experiments that successfully solved specific problems.


**Reactive Animation Systems:**
- **Acceleration Tilt:** Character leans into acceleration (character-local space, tilt matrix)
- **Landing Spring:** Spring-damper crouch/recover on landing
- **Animation Tuning UI:** Real-time parameter adjustment panel
- **Walk/Run Transition:** Smoothed max_speed blending based on SHIFT input
  - Fixed circular dependency: separated `run_speed` (reference) from `max_speed` (state)
  - Pattern validated: `max_speed = lerp(max_speed, target, rate * dt)`
  - See [implementation_smoothed_walk_transition.md](implementation_smoothed_walk_transition.md)

**Skeletal Animation Stack:**
- **Skeleton Debug System:** Hierarchical joints, T-pose asset, debug visualization
- **Attach Skeleton to Body:** Skeleton follows character transform
- **Static Keyframe Preview:** Quaternion keyframe architecture validated in isolation
  - Critical learning: validate data representation before building complex features on it
  - See [ARCHIVE/dependency_stack_snapshot_2025-10-10.md](ARCHIVE/dependency_stack_snapshot_2025-10-10.md)
- **Primary Skeletal Animation:** Distance-phased pose switching (surveyor wheel pattern)
- **Secondary Motion:** Per-bone spring-damper lag for natural follow-through
- **Pose Blending:** Quaternion slerp between keyframes
  - Eliminates pops at phase boundaries
  - Hemisphere correction for shortest-path interpolation
  - See [iteration_pose_blending.md](iteration_pose_blending.md)

**Debug Tooling:**
- **Debug Visual Overhaul:** Velocity trail, plot functions, speed gradient ring
- **Camera Zoom:** Mouse wheel zoom in orbit + follow modes
- **Unified Panel Architecture:** Collapsing headers, real-time displays
- See [implementation_debug_visual_overhaul.md](implementation_debug_visual_overhaul.md)

**Key Systemic Learnings:**
- **Motion vs. Structure:** Reactive layers need motion sources (dynamic state), not just data structures
- **Velocity-injection approach:** Superior to direct offset manipulation for spring systems
- **Distance-phased triggering:** Surveyor-wheel pattern scales from locomotion to skeletal animation
- **Graybox discipline:** Parameters over polish; white spheres over art assets
- **Data structure validation:** Validate novel representations in isolation before complex integration

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
1. ✅ Foundation layer (primitives, rendering, runtime, input, camera)
2. ✅ Core gameplay (physics controller, procedural locomotion/orientation)
3. ✅ Reactive animation (acceleration tilt, landing spring, tuning UI)
4. ✅ Skeleton debug + refactor (game world separation)
5. ✅ Keyframe foundation (static preview → primary → secondary → blending)
6. ✅ Debug tooling (camera zoom, unified panels, visual overhaul)
7. ✅ Walk/run transition (smoothed max_speed blending)

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

**Last Updated:** 2025-10-11 (Post-Walk/Run Transition iteration; tree visualization added)
