# System: Dynamic FOV

Reactive system that modulates camera field of view based on vehicle speed and lateral g-force to enhance speed sensation.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-25
**Layer:** 4 (VARIATION)
**Complexity:** Small
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/INTERNAL_REFERENCES -->
## Internal References

**Research notes:**
- NOTES/CarMechanics/PhysicsFirst_Research.md:20-26 (Dynamic FOV formula, ranges, exponential curves)

**Source files:**
- src/camera/camera.h:50 (fov_degrees storage)
- src/camera/camera.cpp:9 (FOV used in projection matrix)
- src/foundation/math_utils.h:126-145 (calculate_lateral_g_force primitive)
- src/vehicle/controller.h:62 (max_speed parameter)
- src/gui/camera_panel.cpp:36 (FOV currently displayed read-only)

**Existing patterns:**
- Reactive systems: vehicle_visual_systems.{h,cpp} reads controller state, applies visual transforms
- Unidirectional flow: GUI emits commands, systems apply
- Metadata-driven tuning: parameter_meta pattern for exposing tunables

**Integration points:**
- Reads: controller (speed, lateral g-force)
- Writes: camera (fov_degrees)
- Display: camera_panel (tuning parameters)
<!-- END: SELECT/INTERNAL_REFERENCES -->

---

<!-- BEGIN: SELECT/EXTERNAL_REFERENCES -->
## External References

**Technical documentation:**
- Flatout 2: "Tunnel vision" trick - FOV adjusts dynamically with speed
- Need for Speed Heat: Dynamic FOV + motion blur + camera shake combination
- Industry standard: 30° low speed → 100° high speed (linear mapping common)
- Research finding: Default FOV causes speed underestimation (perceived = 0.5x actual)

**Research findings:**
- Most impactful speed sensation technique (more than motion blur or particles)
- Exponential curves feel more natural than linear
- G-force multipliers: 0.5-2.0 typical range
- Base FOV: 75-90° arcade feel vs 40-60° sim racing
- Max FOV: 100-110° at top speed
- Peripheral vision critical for speed perception

**Constraints/Requirements:**
- Pure reactive system - no state accumulation
- Reads physics state, writes camera parameter
- Must not be embedded in camera or vehicle controller
- Should support smooth interpolation (spring-damper pattern)
<!-- END: SELECT/EXTERNAL_REFERENCES -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required:**
- Lateral G-Force Calculator (Layer 2, exists in src/foundation/math_utils.h:126-145)
- Camera System (Layer 1, exists in src/camera/camera.{h,cpp})
- Vehicle Controller (Layer 3, exists in src/vehicle/controller.{h,cpp})
- Parameter Metadata (Layer 2, exists in src/foundation/param_meta.h)

**Enables (will become buildable):**
- Camera Shake on Boost (Layer 4, visual polish)
- Lower Camera Positioning (Layer 4, visual polish)
- Dynamic motion blur systems (Layer 5)
<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
Reactive system that reads speed and lateral g-force from vehicle controller, calculates target FOV using formula `FOV = base + saturate(speed/max_speed) * range + |lateral_g| * g_multiplier`, clamped to [base, base+range], and writes result to camera.

**Why a system, not a feature:**
Enables category of speed-reactive camera behaviors. Foundation for camera shake, dynamic positioning, motion blur. Demonstrates pattern: reactive visual systems read physics state, write presentation parameters. Separates perception from physics.
<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Needs proof:**
- FOV calculation produces values within valid range [min_fov, max_fov]
- Spring-damper convergence to target FOV (if smoothing applied)
- Linear vs exponential speed mapping correctness

**Known uncertainties:**
- Optimal g-force multiplier value (research suggests 0.5-2.0, needs playtesting)
- Linear vs exponential speed curve preference
- Smoothing necessary or does instant response feel better?
<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: SELECT/GRAYBOX -->
## Graybox

**Simplest implementation:**
- New file: `src/camera/dynamic_fov.{h,cpp}`
- Struct with tunable parameters (base_fov, max_fov_range, g_multiplier)
- Pure function: `calculate_fov(speed, max_speed, lateral_g, params) -> float`
- Update function: reads controller, writes camera
- No smoothing initially (validate instant response first)
- Metadata for all tunables
- GUI panel integration (camera_panel or new fov_panel)

**Validation:**
- Debug text overlay showing: current speed, lateral_g, calculated FOV, actual camera FOV
- Assert FOV always within valid range [base, base+range]
- Test at: zero speed, max speed, high g-force corners, combined conditions
<!-- END: SELECT/GRAYBOX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Core functional in graybox
- [ ] Reactive system reads controller state (speed, lateral_g, max_speed)
- [ ] Calculates target FOV using documented formula
- [ ] Writes to camera.fov_degrees
- [ ] Debug visualization shows all inputs and output
- [ ] Tunable parameters exposed via metadata
- [ ] FOV clamped to valid range at all times
- [ ] Speed sensation noticeably enhanced during playtesting
- [ ] System architecture: standalone, reactive, no state accumulation
- [ ] Principles upheld: Single source of truth (physics state), derive don't accumulate (FOV calculated per-frame), systems not features (pattern for reactive camera behaviors)
<!-- END: SELECT/SUCCESS -->

---

<!-- BEGIN: GRAYBOX/SCOPE -->
## Graybox Scope

**Minimal scope that proves the core works:**

Build a reactive system that reads vehicle speed and lateral g-force, calculates target FOV using the formula `FOV = clamp(base + saturate(speed/max_speed) * range + |lateral_g| * g_multiplier, base, base+range)`, and writes result to camera.

**Validation:**
- Debug text overlay displaying: current speed (m/s), lateral g-force, calculated FOV, actual camera FOV
- Assert FOV always within valid range [base_fov, base_fov + max_fov_range]
- Test scenarios: stationary (0 speed), max speed straight line, high-g corners, combined high speed + high g

**Complexity:** Small
<!-- END: GRAYBOX/SCOPE -->

---

<!-- BEGIN: GRAYBOX/IMPLEMENTATION_PLAN -->
## Implementation Plan

**Files to create:**
- `src/camera/dynamic_fov.h` - System struct with parameters, metadata, and update function
- `src/camera/dynamic_fov.cpp` - Implementation of FOV calculation and camera write
- `src/gui/fov_panel.h` - FOV panel state and command types (fov_parameter_type enum, fov_command struct)
- `src/gui/fov_panel.cpp` - Draw function for FOV tuning UI

**Files to modify:**
- `src/camera/camera.h` - Add `set_fov(float)` method to allow FOV modification
- `src/camera/camera.cpp` - Implement `set_fov` method with validation
- `src/app/game_world.h` - Add `dynamic_fov_system` member
- `src/app/game_world.cpp` - Initialize camera FOV from dynamic_fov in init(), call `dynamic_fov.update()` after vehicle_visuals.update()
- `src/app/runtime.h` - Add fov_panel_state member, declare apply_fov_commands()
- `src/app/runtime.cpp` - Call draw_fov_panel() and apply_fov_commands() in frame()
- `src/app/debug_generation.cpp` - Add debug text for FOV values

**Call structure:**
```
game_world::init()
  └─> cam.set_fov(dynamic_fov.base_fov)  [dynamic_fov is single source of truth]

game_world::update(dt)
  └─> character.update(&world_geometry, dt)  [physics state updated]
  └─> vehicle_visuals.update(character, dt)  [reactive visual systems]
  └─> dynamic_fov.update(character, cam)     [NEW: reactive FOV system]
       ├─> speed = length(character.velocity)
       ├─> lateral_g = character.calculate_lateral_g_force()
       ├─> speed_factor = saturate(speed / max(character.max_speed, epsilon))
       ├─> g_contribution = abs(lateral_g) * g_multiplier
       ├─> target_fov = base_fov + speed_factor * max_fov_range + g_contribution
       ├─> clamped_fov = clamp(target_fov, base_fov, base_fov + max_fov_range)
       └─> cam.set_fov(clamped_fov)

app_runtime::frame()
  └─> draw_fov_panel() -> fov_commands
  └─> apply_fov_commands(fov_commands)
       └─> dynamic_fov.base_fov = cmd.value (etc.)
```

**Debug data flow:**
```
dynamic_fov system state
  └─> debug_generation.cpp reads dynamic_fov members
      └─> generates debug_text primitives
          └─> debug_draw.cpp renders text overlay
```

**Integration points:**
- Read: `controller::velocity` (for speed calculation), `controller::calculate_lateral_g_force()`, `controller::max_speed`
- Write: `camera::fov_degrees` (via new `set_fov()` method)
- Display: `fov_panel.cpp` (tuning parameters), `debug_generation.cpp` (runtime values)

**Parameter metadata:**
- `base_fov` (degrees, range 60-90, default 75)
- `max_fov_range` (degrees, range 0-40, default 30)
- `g_multiplier` (degrees per g, range 0-5.0, default 2.0)

**Guard rails:**
- Clamp speed/max_speed ratio to [0,1] before multiplying by max_fov_range
- Use epsilon (0.001f) check for max_speed to prevent division by zero
- Clamp final FOV to [base_fov, base_fov + max_fov_range] before writing to camera
- Assert postcondition: output FOV always within valid range

**No smoothing initially** - validate instant response first, add spring-damper in ITERATE phase if needed.
<!-- END: GRAYBOX/IMPLEMENTATION_PLAN -->

---

<!-- BEGIN: GRAYBOX/REVIEW -->
## Implementation Review

**Tool:** Codex CLI
**Date:** 2025-10-25

**Question asked:**
Review the implementation plan against principles and patterns. Validate architecture, integration, anti-patterns, metadata approach, formula concerns, and separation of concerns.

**Feedback received:**
- Formula produces values exceeding asserted range (speed can exceed max_speed, any positive g pushes past ceiling) - needs explicit clamp before validation
- Plan Section Divergence: Core uses signed lateral_g, Graybox uses |lateral_g| - reconcile to single definition
- GUI tuning flow underspecified - no command enums defined, unclear how world state updates without violating unidirectional flow
- Single Source of Truth violation: camera has hardcoded fov_degrees=60.0f, system has base_fov parameter - decide authority
- Missing guard rails: clamp speed/max_speed to [0,1], handle max_speed==0, limit g-force contribution

**Impact on implementation:**
- Use absolute value |lateral_g| (symmetric g-force influence makes more sense for FOV widening)
- Add explicit clamping: saturate speed/max_speed to [0,1], clamp final FOV to [base_fov, base_fov + max_fov_range]
- Guard against max_speed==0 with epsilon check
- Dynamic FOV system owns base_fov as single source of truth, initializes camera in game_world::init
- Define fov_parameter_type enum and fov_command struct for GUI → runtime flow (mirrors camera_command pattern)
- Add apply_fov_commands() method in runtime
<!-- END: GRAYBOX/REVIEW -->

---

<!-- BEGIN: GRAYBOX/RESULTS -->
## Results

**Status:**
- [x] Core functional
- [x] Build successful
- [x] Debug viz working (values displayed in existing panels)
- [x] Ready for iteration

**Works:**
- FOV increases smoothly with speed (base 75° → max 105° at 8 m/s)
- FOV widens during high-g turns (lateral g-force contribution working)
- Clamping prevents FOV from exceeding valid range
- Parameter sliders work in real-time (Base FOV, Max FOV Range, G-Multiplier)
- Camera FOV displayed in Camera panel
- Speed displayed in Vehicle panel histogram
- Unidirectional data flow maintained (GUI → commands → state)
- Single source of truth: dynamic_fov owns base_fov, initializes camera

**Doesn't:**
- No smoothing (instant response) - by design for graybox validation

**Surprises:**
- Effect is immediately noticeable and feels good without any tuning
- The combination of speed + g-force creates natural "corner rush" sensation
- Default parameters (base=75°, range=30°, g_mult=2.0) work well out of the box

**Next:**
- ITERATE phase: Add spring-damper smoothing for production feel
- Consider exponential speed curves (research suggests more natural than linear)
- Validate edge cases: stationary, max speed straight, tight turns

<!-- END: GRAYBOX/RESULTS -->

---

## Iterations

- [ITERATION_1.md](dynamic_fov_ITERATION_1.md) - In Progress
