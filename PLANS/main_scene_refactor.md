# Phase 3.10: Runtime Decomposition & Collision Ownership

## Why Now
- `main.cpp` bundles initialization, per-frame simulation, debug overlays, and GUI plumbing in one file (~480 lines). Hard to reason about dependencies and violates **clarity over cleverness**.
- `scene.cpp` depends on `character_controller` to resolve collisions, inverting the foundation - character - rendering flow. This stalls future physics work and blocks reuse of collision primitives.

## Refactor Goals
1. Separate core loop stages (init/update/render/gui/shutdown) into clear entry points with minimal global state.
2. Keep `scene` as a dumb container + queries; move collision math to a lower-level module usable by character, step-up probes, etc.
3. Maintain current feel and debug affordances while making it obvious where to hook future systems.

## Decomposing `main.cpp`
- **Create `app/runtime.{h,cpp}`** (or similar) that owns the lifetime of camera, scene, renderer, character, orientation, locomotion, GUI params, and cached meshes. Expose `app_init`, `app_shutdown`, `app_frame`, and `app_event` wrappers called by sokol.
- **Split frame work into focused helpers** inside the runtime module:
  - `update_simulation(dt)` - apply input, character update, orientation/locomotion sync, spring bookkeeping.
  - `build_gui()` - all ImGui panels and history graphs (no rendering dependency).
  - `draw_world()` - renderer calls + debug overlays. Move spring/foot/velocity drawings out of the sokol callback body.
- **Consolidate static meshes** into a `debug_mesh_pool` owned by runtime; initialize lazily once.
- **Bundle history buffers** (spring graph, wheel debug) with runtime instead of global statics.
- **Character parameter adapter** (`intuitive_character_params`) lives alongside runtime so the GUI module can reference it without direct globals.
- Sokol entry points (`init`, `frame`, `cleanup`, `event`) become thin shims that forward to runtime.

### Implementation Order
1. Introduce `struct app_runtime` with zeroed members; migrate global pointers into it.
2. Move existing init logic into `app_runtime::initialize()` and cleanup into `shutdown()`.
3. Pull frame body into `app_runtime::tick(float dt)`; inside call `update_simulation`, `build_gui`, `draw_world` helpers.
4. Relocate GUI panel code into `build_gui`; provide runtime references to character/locomotion as needed.
5. Remove now-unnecessary globals/static flags from `main.cpp`; ensure sokol callbacks instantiate + forward to a singleton runtime instance (allocated static or function-local static per principles).

## Collision Ownership Refactor
- **Move geometry primitives** (`sphere`, `aabb`) into `foundation/geometry.h` (or `foundation/collision_primitives.h`) so rendering + character can share without cross-layer includes.
- **Extract collision math** (`resolve_sphere_collision`) into `foundation/collision.cpp`. Provide pure functions that operate on primitives and return minimal data (penetration vector/normal) without mutating unrelated state.
- Update `scene` to:
  - Drop inclusion of `character_controller.h`.
  - Store `aabb` data only and expose `const std::vector<aabb>&` for callers (already present).
  - Remove collision resolution; instead, character controller calls `collision::resolve_sphere_aabb` using scene-provided boxes.
- Character controller adapts to new API (likely returns struct `{bool hit; glm::vec3 resolved_center; glm::vec3 normal;}`) while maintaining the weightlifter-first logic.
- Future systems (step-up, IK) can reuse the same collision helpers without touching rendering code.

### Phased Execution
1. **Prep:** introduce new foundation headers with duplicates of existing structs; update includes; ensure build compiles with both definitions before deleting old ones.
2. **Migration:** switch character controller to new collision helpers; run build + manual playtest to confirm feel.
3. **Cleanup:** remove collision code from `scene.cpp`, eliminate redundant includes, and ensure scene stays read-only container.

## Testing / Validation
- Rebuild (`cmake --build build`) after each major step.
- Manual smoke playtest: walking, jumping, landing impacts, and debug overlays rendering correctly.
- Verify weightlifter + bumper visualization still match spring compression, ensuring no regression before Phase 4 features.

## Risks & Mitigations
- **Risk:** Refactor breaks ordering dependencies between update/draw helpers - Keep sequencing identical inside `tick` while extracting functions.
- **Risk:** Collision helper API introduces floating-point drift - maintain semi-implicit integration by returning resolved positions instead of applying additional acceleration.
- **Risk:** Runtime singleton lifecycle mismatch with sokol callbacks - store runtime as function-local static in `sokol_main` scope to guarantee construction before init.


## Progress (2025-10-04)
- Completed: app_runtime scaffolding replaces main.cpp globals; sokol callbacks forward to runtime singleton.
- Completed: Frame loop split into simulation/gui/render helpers; history buffers and static meshes live inside runtime.
- Completed: Collision helpers moved to foundation; scene is now a container only.
- Next: polish collision module naming + run focused playtest once IK prep begins.
