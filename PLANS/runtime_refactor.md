# Runtime Refactor Plan

## Problem
`runtime.cpp` violates core principles:
- **422 lines** doing initialization, simulation, GUI, rendering, input handling, debug visualization
- Dependencies flow upward and sideways (rendering calls character, locomotion, orientation)
- Multiple responsibilities obscure single purpose
- Debug visualization tightly coupled to render loop

## Principles Violated
1. **Clarity over cleverness** - Too much in one place
2. **Simplicity** - Complex interdependencies
3. **Dependency Management** - Higher layers shouldn't orchestrate lower layers

## Current Structure Analysis

### What runtime.cpp does:
1. **Application lifecycle** - init, shutdown, frame loop
2. **Input interpretation** - camera orbit, scroll, mouse state
3. **Simulation update** - character, orientation, locomotion tick
4. **Parameter translation** - `intuitive_character_params` bridge
5. **GUI building** - ImGui panel for character tuning
6. **Debug visualization** - 15+ wireframe draws for spheres, springs, wheels, feet
7. **Static mesh management** - unit shapes cache
8. **Scene rendering** - world objects, collision boxes

### Dependency violations:
- `app/` depends on `character/`, `rendering/`, `input/`, `gui/`, `camera/`
- Should be: Foundation → Character → Camera → Rendering → App

## Refactor Strategy

### Phase 1: Extract Debug Visualization
**New module: `rendering/debug_draw.h/cpp`**

```cpp
namespace debug {
    struct draw_context {
        wireframe_renderer& renderer;
        camera& cam;
        float aspect;
    };

    void draw_character_state(draw_context& ctx, const character_controller& c, 
                              const locomotion_system& loco, const orientation_system& orient);
    void draw_physics_springs(draw_context& ctx, const character_controller& c, 
                              const locomotion_system& loco);
    void draw_locomotion_wheel(draw_context& ctx, const character_controller& c, 
                               const locomotion_system& loco, float wheel_angle);
    void draw_foot_positions(draw_context& ctx, const character_controller& c, 
                            const locomotion_system& loco, const orientation_system& orient);
}
```

**Rationale:**
- 200+ lines of visualization → isolated module
- Reactive layer (interprets state, doesn't control it)
- Can be disabled without affecting simulation
- Single responsibility: translate game state to debug geometry

### Phase 2: Extract Parameter Translation
**New module: `character/tuning.h/cpp`**

```cpp
namespace character {
    struct tuning_params {
        float time_to_max_speed;
        float jump_height;
        
        void apply_to(character_controller& c) const;
        void read_from(const character_controller& c);
    };

    void sync_locomotion_targets(character_controller& c, locomotion_system& loco);
}
```

**Rationale:**
- Designer-friendly params separate from physics constants
- Lives with character code (dependency correctness)
- Reusable across runtime and editor tools

### Phase 3: Extract GUI Building
**New module: `gui/character_panel.h/cpp`**

```cpp
namespace gui {
    struct character_panel_state {
        bool show = true;
        // Tuning state lives here
    };

    void draw_character_panel(character_panel_state& state, 
                             character_controller& c, 
                             character::tuning_params& params);
}
```

**Rationale:**
- GUI is reactive layer above character
- Panel logic separate from runtime orchestration
- Consistent with existing `gui::` namespace pattern

### Phase 4: Simplify Runtime
**Result: `app/runtime.h/cpp` becomes pure orchestration**

```cpp
// Initialization
// Frame pump: input → simulation → render → gui
// Event delegation
// Static mesh caching (if still needed)
```

**Core responsibilities:**
- Application lifecycle
- Frame timing
- Coordinate subsystem updates
- Scene management

~150 lines vs 422.

## Implementation Order

1. **Create `rendering/debug_draw.h/cpp`**
   - Copy visualization code from `render_world()`
   - Extract static mesh dependency (unit_circle, unit_sphere_*)
   - Test: Should see same debug visuals

2. **Create `character/tuning.h/cpp`**
   - Move `intuitive_character_params` 
   - Move `sync_locomotion_speed_targets()`
   - Update includes in runtime

3. **Create `gui/character_panel.h/cpp`**
   - Extract `build_character_panel()` logic
   - State management for panel visibility
   - Test: Panel still works

4. **Refactor `app/runtime.cpp`**
   - Remove extracted code
   - Call new module functions
   - Verify dependency flow

## Dependency Flow After Refactor

```
foundation/
    ↓
character/
    ├── character_controller
    ├── locomotion
    ├── orientation
    └── tuning  [NEW]
    ↓
camera/
    └── camera
    ↓
rendering/
    ├── renderer
    ├── scene
    ├── wireframe
    └── debug_draw  [NEW]
    ↓
gui/
    ├── gui (core)
    └── character_panel  [NEW]
    ↓
app/
    └── runtime (orchestration only)
```

Clean bottom-up flow. No sideways dependencies.

## Benefits

1. **Testability** - Debug draw can be tested with mock state
2. **Reusability** - Tuning params useful for editor, save files
3. **Clarity** - Each file has single, obvious purpose
4. **Maintainability** - Changes localized (add debug vis → edit debug_draw.cpp)
5. **Iteration speed** - Can disable debug layer without touching core

## Non-Goals

- Don't create abstraction "just in case"
- Don't add ECS or entity management
- Don't refactor working character/locomotion code
- Keep it simple - solve the "runtime does too much" problem

---

**Status:** Ready for implementation
**Estimated effort:** 2-3 hours
**Risk:** Low (purely organizational, no logic changes)
