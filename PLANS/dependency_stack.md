# Dependency Stack

**Purpose:** Identify key dependencies to determine work order and reduce cascading uncertainty.

**Status:** Living document (updated as design solidifies)

---

## Current Stack

```
┌─────────────────────────────────────┐
│         DESIGN BACKLOG              │  ← Everything else (liquid)
├─────────────────────────────────────┤
│     Acceleration Tilt (NEXT)        │  ← Next implementation
├─────────────────────────────────────┤
│  Procedural Orientation (90%)       │  ← Velocity-based facing
│  Procedural Locomotion (90%)        │  ← Surveyor wheel, cubic interp
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

### Reactive Systems Layer (In Progress, ~40% certain)

**Status:** Next implementation target

**Next Up: Acceleration Tilt**
- Simple reactive animation interpreting physics state
- Does not affect controller physics ("do no harm")
- See `acceleration_tilt_implementation.md` for details

**Other Planned:**
- Spring-damper landings (detect airborne→grounded, apply impulse)
- Secondary motion (if character gets visible body)
- IK foot placement (if terrain becomes complex)

**Certainty:** Low. Acceleration tilt is ~70% (simple), others ~30% (depend on rendering/content decisions not yet made).

**Dependencies:** Requires stable physics core. Changes in controller would force redesign here.

### Polish Layer (Backlog, ~10% certain)

**Status:** Everything in design backlog

Most of these will be cut or heavily redesigned based on discoveries during iteration. Avoid detailed planning here—cascading uncertainty too high.

---

## Development Strategy

**Current Focus:** Complete acceleration tilt implementation, then iterate/playtest.

**Work Order:**
1. ✅ Foundation primitives (spring-damper, easing, collision math)
2. ✅ Core rendering and app runtime
3. ✅ Core gameplay (physics controller, basic animation)
4. 🔄 First reactive system (acceleration tilt) ← **YOU ARE HERE**
5. ⏸️ Iteration/playtesting of core feel
6. ⏸️ Pull next item from backlog when foundation solidifies

**Planning Horizon:**
- Foundation: Days to weeks (high certainty, derivative work)
- Core gameplay: Days (testing needed, but simple scope)
- Reactive systems: Short iterations (2-4 hours per feature)
- Polish: Do not plan (pull from backlog as needed)

**Certainty Analysis:**
- Foundation layer: ~95% certain (won't change)
- Core gameplay: ~90% certain (minor tuning expected)
- Acceleration tilt: ~70% certain (simple, but untested)
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
