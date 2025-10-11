# Dependency Stack

**Purpose:** Identify key dependencies to determine work order and reduce cascading uncertainty.

**Status:** Living document (updated as design solidifies)

---

## Current Stack

```
┌─────────────────────────────────────┐
│         DESIGN BACKLOG              │  ← Everything else (liquid)  ← YOU ARE HERE
├─────────────────────────────────────┤
│  Debug Visual Overhaul (100%) ✅    │  ← Velocity trail, plot functions, gradient ring
├─────────────────────────────────────┤
│  Secondary Motion (100%) ✅         │  ← Per-bone spring-damper lag
├─────────────────────────────────────┤
│ Primary Skeletal Animation (100%) ✅│  ← Distance-phased pose switching
├─────────────────────────────────────┤
│  Static Keyframe Preview (100%) ✅  │  ← Quaternion keyframes validated
├─────────────────────────────────────┤
│  Attach Skeleton to Body (100%) ✅  │  ← Skeleton follows character transform
├─────────────────────────────────────┤
│  Skeleton Debug System (100%) ✅     │  ← Hierarchical joints + visualizer
│  • Skeleton Data + Updates (100%) ✅ │     `skeleton.h/.cpp`
│  • T-Pose Asset (100%) ✅            │     Hardcoded humanoid
│  • Debug Draw & GUI Toggles (100%) ✅│     Render + labels + animation
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
│    Camera System (95%)              │  ← Orbit/follow, zoom, debug UI
│    Input System (95%)               │  ← WASD, mouse, keyboard
├─────────────────────────────────────┤
│      Foundation Primitives          │  ← Reusable building blocks
│  • Spring-Damper (100%, unused)     │
│  • Easing Functions (100%)          │
│  • Collision Primitives (100%)      │
├─────────────────────────────────────┤
│       Rendering Layer (90%)         │  ← Scene, debug draw, wireframe
│       Runtime/App (100%) ✅         │  ← Main loop, lifecycle
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

### Completed Foundation (100%) ✅

**Skeletal Animation Stack:**
- Skeleton debug system → Static keyframe preview → Primary skeletal animation (distance-phased) → Secondary motion (spring-damper lag)
- Quaternion keyframe architecture validated; surveyor-wheel pattern proven for animation triggering
- See [ARCHIVE/dependency_stack_snapshot_2025-10-10.md](ARCHIVE/dependency_stack_snapshot_2025-10-10.md) for detailed retrospectives

**Reactive Animation Systems:**
- Acceleration tilt, landing spring, animation tuning UI complete
- `character::animation_state` pattern proven; transform injection pipeline stable
- See [implementation_reactive_animation.md](implementation_reactive_animation.md) and snapshot archive

**Debug Tooling:**
- Debug visual overhaul (velocity trail, plot functions, speed gradient ring)
- Unified debug panel architecture with collapsing headers
- Camera zoom with mouse wheel (orbit + follow modes)
- See [implementation_debug_visual_overhaul.md](implementation_debug_visual_overhaul.md) and [feature_camera_zoom.md](feature_camera_zoom.md)

**Key Systemic Learnings:**
- **Motion vs. Structure:** Reactive layers need motion sources (dynamic state), not just data structures
- **Velocity-injection approach:** Superior to direct offset manipulation for spring-damper systems
- **Distance-phased triggering:** Surveyor-wheel pattern scales from locomotion to skeletal animation
- **Graybox discipline:** Parameters over polish; white spheres over art assets

### Polish Layer (Backlog, ~10% certain)

**Status:** Everything in design backlog

Most of these will be cut or heavily redesigned based on discoveries during iteration. Avoid detailed planning here—cascading uncertainty too high.

---

## Development Strategy

**Current Focus:** Completed foundation ready. Pull next feature from backlog based on learnings and dependencies. ← **YOU ARE HERE**

**Work Order (Completed):**
1. ✅ Foundation layer (primitives, rendering, runtime, input, camera)
2. ✅ Core gameplay (physics controller, procedural locomotion/orientation)
3. ✅ Reactive animation (acceleration tilt, landing spring, tuning UI)
4. ✅ Skeleton debug + refactor (game world separation)
5. ✅ Keyframe foundation (static preview → primary animation → secondary motion)
6. ✅ Debug tooling (camera zoom, unified panel, visual overhaul)

**Planning Horizon:**
- Foundation: Large chunks (high certainty, stable)
- Core gameplay: Moderate chunks (testing/tuning, but proven)
- Reactive systems: Short iterations (1-2 complexity points per feature, proven pattern)
- Keyframe systems: Short iterations (1-2 points, proven with Static Keyframe Preview and Primary Skeletal Animation)
- Debug/iteration tools: Very short iterations (1 point, simple feature workflow proven)
- Polish: Do not plan (pull from backlog as needed)

**Certainty Analysis:**
- Completed foundation: 100% certain (6 validated iterations, patterns proven)
- Core gameplay: ~95% certain (physics controller stable, procedural systems working)
- Next features: 50-70% certain (depends on gameplay priorities and rendering decisions)
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
- **Motion vs. Structure:** Reactive animation layers require **motion sources** (systems that change state each frame), not just data structures. Example: Secondary motion needs skeletal joints that animate (move each frame), not just a static skeleton struct. When analyzing dependencies for reactive systems, verify the prerequisite provides dynamic state changes over time, not just structural existence.
