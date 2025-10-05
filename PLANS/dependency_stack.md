# Dependency Stack

**Purpose:** Identify key dependencies to determine work order and reduce cascading uncertainty.

**Status:** Living document (updated as design solidifies)

---

## Current Stack

```
┌─────────────────────────────────────┐
│         DESIGN BACKLOG              │  ← Everything else (liquid)
├─────────────────────────────────────┤
│  Secondary Motion / Bone Wobble     │  ← Needs skeletal joints
│  Speed-Based Animation Scaling      │  ← May not be necessary
├─────────────────────────────────────┤
│  Reactive Animation Layer (100%) ✅  │  ← Interpret physics state
│  • Acceleration Tilt (100%) ✅       │     Character-local space, tilt matrix
│  • Landing Spring (100%) ✅          │     Spring-damper crouch/recover
│  • Animation Tuning UI (100%) ✅    │     Real-time parameter adjustment
├─────────────────────────────────────┤
│  Procedural Orientation (95%)       │  ← Velocity-based facing
│  Procedural Locomotion (95%)        │  ← Surveyor wheel, cubic interp
├─────────────────────────────────────┤
│    Character Controller (95%)       │  ← Physics, input, collision
│    Collision Detection (95%)        │  ← Sphere-AABB resolution
│    Camera System (90%)              │  ← Orbit camera, view matrices
│    Input System (95%)               │  ← WASD, mouse, keyboard
├─────────────────────────────────────┤
│      Foundation Primitives          │  ← Reusable building blocks
│  • Spring-Damper (100%, unused)     │
│  • Easing Functions (100%)          │
│  • Collision Primitives (100%)      │
├─────────────────────────────────────┤
│       Rendering Layer (90%)         │  ← Scene, debug draw, wireframe
│       Runtime/App (95%)             │  ← Main loop, lifecycle
└─────────────────────────────────────┘
```

## Layer Descriptions

### Foundation Layer (Implemented, ~95% certain)

**Status:** Solid foundation for building upward

- **Rendering Infrastructure:** Sokol-based renderer, wireframe system, debug draw
- **App Runtime:** Main loop, frame timing, window management
- **Foundation Primitives:** Spring-damper (critically damped), easing curves, collision math
- **Input System:** Keyboard/mouse capture and state tracking
- **Camera System:** Orbit camera with camera-relative controls

**Certainty:** High. These are derivative systems based on proven patterns.

### Core Gameplay Layer (Implemented, ~90% certain)

**Status:** Core physics loop working, needs iteration/testing

**Core Gameplay Definition:** Character sphere moving in 3D space with physics-driven control
- Irreducible minimum: Position, velocity, acceleration, collision, input response
- Already creates meaningful experience (run, jump, collide)

**Systems:**
- **Character Controller:** Single-sphere physics, grounded state, jump, friction
- **Collision Detection:** Sphere-AABB resolution, multi-pass for stability
- **Procedural Locomotion:** Distance-phased animation (surveyor wheel), speed blending
- **Procedural Orientation:** Smooth rotation toward velocity direction

**Certainty:** High for physics, medium-high for animation systems. Needs playtesting.

**Dependencies:** Everything below remains stable. Changes here won't cascade downward.

### Reactive Systems Layer (Implemented, ~100% certain) ✅

**Status:** Architecture proven, tuning UI complete, ready for iteration

**Completed: Acceleration Tilt ✅**
- Character-local space transformation of world acceleration
- Exponential smoothing for natural response
- Pure reactive layer ("do no harm" principle verified)
- 45 lines of code, 2 tunable parameters
- UI controls: tilt_smoothing, tilt_magnitude with real-time pitch/roll display

**Completed: Landing Spring ✅**
- Spring-damper driven crouch/recover on landing
- Impulse proportional to fall velocity
- Critically damped for natural feel (no bounce)
- Visual debug spring (collision sphere to body center)
- UI controls: stiffness, damping, impulse_scale with spring state display

**Completed: Animation Tuning UI ✅**
- Comprehensive parameter exposure across animation/orientation/locomotion systems
- Real-time adjustment during gameplay (zero rebuild iteration)
- Consistent UX pattern: collapsing headers + live state feedback
- See `animation_tuning_plan.md` for implementation details and learnings

**Architecture Validated:**
- `character::animation_state` pattern works well (proven twice)
- Update timing: after physics, after orientation, before render
- Transform injection: position → orient → landing offset → tilt → render
- Parameters over assets paradigm successful
- Flag-based event communication (just_landed) pattern works
- Public member access for tuning (no getters/setters needed)
- Direct UI binding to system parameters (no intermediate abstractions)

**Next Candidates:**
- Secondary motion / bone wobble (needs skeletal joints)
- Speed-based animation scaling (may not be necessary)
- Additional reactive systems as gameplay needs emerge

**Certainty:** ~100% for current systems. Architecture solid, tuning enabled, ready for feel iteration.

**Dependencies:** Requires stable physics core. Changes in controller would force redesign here.

### Polish Layer (Backlog, ~10% certain)

**Status:** Everything in design backlog

Most of these will be cut or heavily redesigned based on discoveries during iteration. Avoid detailed planning here—cascading uncertainty too high.

---

## Development Strategy

**Current Focus:** Iteration/playtesting of core feel with full tuning capability.

**Work Order:**
1. ✅ Foundation primitives (spring-damper, easing, collision math)
2. ✅ Core rendering and app runtime
3. ✅ Core gameplay (physics controller, basic animation)
4. ✅ Reactive animation systems (acceleration tilt, landing spring)
5. ✅ Animation tuning UI (comprehensive parameter exposure)
6. 🔄 Iteration/playtesting of core feel ← **YOU ARE HERE**
7. ⏸️ Pull next item from backlog based on playtest discoveries

**Planning Horizon:**
- Foundation: Weeks to months (high certainty, stable)
- Core gameplay: Weeks (testing/tuning, but proven)
- Reactive systems: Short iterations (2-4 hours per feature, proven pattern)
- Polish: Do not plan (pull from backlog as needed)

**Certainty Analysis:**
- Foundation layer: ~95% certain (stable, won't change)
- Core gameplay: ~95% certain (proven through testing)
- Reactive animation: ~90% certain (architecture validated)
- Next features: 50-70% certain (depends on rendering decisions)
- Higher layers: <30% certain (excessive cascading uncertainty)

**Risk Management:**
- Avoid building on uncertain foundation (spring-damper landings need visible body first)
- Defer IK until terrain complexity demands it (flat ground currently)
- Keep backlog liquid (no interlocking dependencies)
- Accept that most backlog items will be cut

---

**Notes:**
- Only strongest dependencies shown (deliberate reduction for focus)
- Circular dependencies exist but weakest ones ignored
- Stack will reshape as foundation solidifies
