# Dependency Stack

**What exists. What it provides. What can be built.**

**Last Updated:** 2025-10-26

---

## Purpose

Inventory of capabilities organized by architectural layer.

**Answers:**
- What does each layer provide?
- What can be built with what exists?
- What dependencies does X require?

**Single Source of Truth:** The codebase. This is a derived view.

---

## Layer 1: CORE

Software infrastructure required to run the application.

**Provides:**

- **App Entry** - Sokol callback wiring (`src/main.cpp`, `src/sokol_impl.cpp`)
- **Runtime Orchestrator** - Main loop, initialization, update/render (`src/app/runtime.{h,cpp}`)
- **Input System** - Keyboard/mouse event handling and state queries (`src/input/input.{h,cpp}`, `src/input/keycodes.h`)
- **GUI Framework** - ImGui wrapper with lifecycle and plotting (`src/gui/gui.{h,cpp}`)
- **GUI Commands** - Unidirectional flow command types (`src/gui/parameter_command.h`, `src/gui/camera_command.h`)
- **Designer Parameter Interface** - Metadata-driven widgets for tuning (tunable/readonly/derived params) (`src/gui/gui.{h,cpp}`, `src/foundation/param_meta.h`)
- **Vehicle Panel** - Tuning sliders and state display using metadata-driven widgets (`src/gui/vehicle_panel.{h,cpp}`)
- **Camera Panel** - Follow camera controls using metadata-driven widgets (`src/gui/camera_panel.{h,cpp}`)
- **Rendering Camera** - View/projection matrices from eye/target/FOV (`src/camera/camera.{h,cpp}`)
- **Wireframe Renderer** - Sokol pipeline for line meshes (`src/rendering/renderer.{h,cpp}`)
- **Scene Container** - Mesh list for world rendering (`src/rendering/scene.{h,cpp}`)
- **Debug Primitives** - Sphere/line/box/arrow/text rendering (`src/rendering/debug_primitives.h`, `src/rendering/debug_draw.{h,cpp}`)
- **Debug Viz Toggle** - Global debug visualization on/off (`src/rendering/debug_visualization.{h,cpp}`)
- **Debug Validation** - Startup checks for math invariants (`src/rendering/debug_validation.{h,cpp}`)
- **Debug Helpers** - Convenience functions for debug drawing (`src/rendering/debug_helpers.h`)
- **Debug Generation** - Translates game state to debug visuals (`src/app/debug_generation.{h,cpp}`)

**Dependencies to build something new here:**
- Sokol app/gfx initialized
- Shader headers generated (`generated/wireframe_shader.h`)
- GUI and input lifecycle wired via runtime

---

## Layer 2: PRIMITIVES

Mathematical building blocks. Pure functions with no side effects.

**Provides:**

- **Math Utilities** - World UP, horizontal projection, safe normalize, angle operations (`src/foundation/math_utils.h`)
- **Slip Angle Calculator** - Angle between velocity vector and forward direction (`src/foundation/math_utils.h`)
- **Lateral G-Force Calculator** - Centripetal acceleration from speed and angular velocity (`src/foundation/math_utils.h`)
- **Spring-Damper** - 1D time-step integration with critical damping (`src/foundation/spring_damper.{h,cpp}`)
- **Easing** - Cubic smoothstep, smooth mixes, cubic Hermite (`src/foundation/easing.{h,cpp}`)
- **Procedural Mesh** - Generates sphere/box/grid/arrow/circle/spring wireframes (`src/foundation/procedural_mesh.{h,cpp}`)
- **Collision Primitives** - Sphere/AABB types, collision world, surface types (`src/foundation/collision_primitives.h`)
- **Collision Math** - Sphere-AABB tests, multi-pass resolution, wall-slide projection (`src/foundation/collision.{h,cpp}`)
- **Car-Like Control Scheme** - Transforms WASD input to vehicle-relative forward/back and turn rate (`src/vehicle/controller.h`, `src/app/game_world.{h,cpp}`)
- **Parameter Metadata** - Semantic annotations for tunable parameters (name, units, range, type) (`src/foundation/param_meta.h`)

**Dependencies to build something new here:**
- GLM available
- CORE assertions optional for validation
- No runtime/GUI/renderer coupling required
- Car-like control scheme enables: Orientation-locked camera mode, drift movement mechanics
- Parameter metadata enables: Generic tuning UIs, parameter validation, presets/serialization
- Slip angle calculator enables: Physics-based drift system, drift detection, tire force modeling, slip angle visualization
- Lateral g-force calculator enables: Vehicle tilt system, dynamic FOV, drift grip thresholds, physics-based drift system

---

## Layer 3: SYSTEMS

High-level gameplay and interaction systems. Compose primitives into behavior.

**Provides:**

- **Camera Follow System** - Orbit/zoom constraints, computes eye/target from lat/lon/distance (`src/camera/camera_follow.{h,cpp}`)
  - Uses: Layer 2 math utilities, Layer 1 assertions
- **Vehicle Movement System** - Input→acceleration, drag, weight, heading integration, collision resolution, traction state (`src/vehicle/controller.{h,cpp}`)
  - Uses: Layer 2 collision + math, Layer 1 assertions
- **Speed-Dependent Steering** - Reduces steering authority at high speeds for stable cornering (`src/vehicle/controller.{h,cpp}`)
  - Uses: Vehicle movement system, heading integration
- **Vehicle Tuning** - Metadata-driven parameter system for vehicle physics (`src/vehicle/tuning.{h,cpp}`)
  - Uses: Layer 2 parameter metadata
- **Game World** - Composes all systems, handles update loop, input polling, velocity trail (`src/app/game_world.{h,cpp}`)
  - Uses: Layer 1 input/scene/debug, Layer 2 math + collision

**Dependencies to build something new here:**
- Layer 1: Runtime loop, input, rendering camera
- Layer 2: Math, collision, springs
- Prefer unidirectional flow (GUI emits commands, systems apply)

---

## Layer 4: VARIATION

Variations within systems. Different modes, states, or behaviors.

**Provides:**

- **Traction State Classification** - Grip/drift/airborne state based on speed thresholds (`src/vehicle/controller.h`)
  - Extends: Vehicle Movement System
- **Follow Orbit Mode** - Maya-style orbit + zoom with clamped lat/lon/distance (`src/camera/camera_follow.{h,cpp}`)
  - Extends: Camera Follow System
- **Orientation-Locked Camera** - Camera locks behind vehicle facing direction (`src/camera/camera_follow.{h,cpp}`, `src/gui/camera_panel.{h,cpp}`)
  - Extends: Camera Follow System
  - Enables: Velocity-locked camera, look-ahead camera, dynamic chase modes
- **Vehicle Tilt System** - Visual weight transfer through spring-damped lean/pitch based on lateral g-force and acceleration (`src/vehicle/vehicle_reactive_systems.{h,cpp}`)
  - Uses: Layer 2 lateral g-force calculator, spring-damper, orientation system
  - Extends: Vehicle Movement System (visual layer)
  - Enables: Enhanced drift visual feedback, suspension animation foundation
- **Dynamic FOV System** - Speed-reactive camera FOV with spring-damped transitions (`src/camera/dynamic_fov.{h,cpp}`, `src/gui/fov_panel.{h,cpp}`)
  - Uses: Layer 2 lateral g-force calculator, spring-damper
  - Extends: Rendering Camera (modulates FOV parameter)
  - Enables: Camera shake on boost, dynamic camera positioning, motion blur intensity, perceptual feedback systems
- **Handbrake Input System** - Binary input (Space key) for drift initiation via brake force opposing velocity (`src/vehicle/handbrake_system.{h,cpp}`)
  - Uses: Layer 3 vehicle movement system (composed into controller)
  - Establishes: Active/reactive composition pattern (handbrake modifies physics, reactive systems respond)
  - Enables: Drift detection primitive, continuous friction scaling, drift speed maintenance, drift visual feedback

**Dependencies to build something new here:**
- Target parent system in Layer 3
- Layer 2 primitives as needed
- No direct renderer coupling
- Vehicle tilt provides: Visual weight transfer pattern for drift system, foundation for per-wheel suspension
- Dynamic FOV provides: Pattern for speed-reactive camera behaviors, perceptual enhancement foundation
- Handbrake provides: Drift initiation input, active/reactive composition pattern for vehicle systems

---

## Layer 5: POLISH

Feel, aesthetics, and refinement.

**Provides:**

- **Velocity Trail Visual** - Time-sampled position trail for motion feedback (`src/rendering/velocity_trail.h`)
  - Uses: Layer 1 debug viz
- **Debug Visual Polish** - Speed gradient ring, orientation arrows, steering cone, color-coded collision visualization (`src/app/debug_generation.cpp`)
  - Uses: Layer 1 debug viz + Layer 2 procedural meshes

**Dependencies to build something new here:**
- Layer 3 systems producing motion/contacts
- Layer 2 springs/easing
- Layer 1 debug viz or renderer to display effects

---

## Build Guidelines

### Bottom-Up Construction
Build from Layer 1 → Layer 5. Never skip. Each layer depends on everything below.

### Dependency Flow
Truth flows upward. Layer N can only depend on Layers 1 through N-1.

### When to Add vs Build Up
- **Layer 1-2 incomplete?** → Add missing infrastructure/primitives
- **Layer 3 thin?** → Build core systems
- **Layer 3 solid?** → Explore variations (Layer 4)
- **Layer 4 rich?** → Polish (Layer 5)

### Recognition Test
Not sure what layer something belongs in? Ask:

- "Could the game run without this?" → No = Layer 1
- "Is this pure math with no side effects?" → Yes = Layer 2
- "Does this define a core gameplay system?" → Yes = Layer 3
- "Is this one way to do something that already exists?" → Yes = Layer 4
- "Does this make existing gameplay feel better?" → Yes = Layer 5

---

**Truth from codebase. Layers from concept. Build from certainty.**
