# Systems Backlog

**Systems to build.**

**Last Updated:** 2025-10-23 (shift to car/racing mechanics)

---

## 🔄 MIGRATION: Character → Vehicle Systems

**Special section for systematic transition from character to car mechanics.**

### Phase 1: Backup & Branch
**Status:** Not Started

1. **Archive Character Systems**
   - Create `archive/character/` directory
   - Copy these files before modification:
     - `src/character/controller.{h,cpp}` → `archive/character/`
     - `src/character/character_reactive_systems.{h,cpp}` → `archive/character/`
     - `src/character/tuning.{h,cpp}` → `archive/character/`
     - `src/character/animation.{h,cpp}` → `archive/character/`
     - `src/character/orientation.{h,cpp}` → `archive/character/`
   - Git commit: "archive: backup character systems before vehicle migration"

### Phase 2: Core Repurposing
**Status:** Not Started

2. **Transform Movement System** (Incremental - Each Step Compiles/Runs)

   **2.1 Rename Files (Zero Logic Changes)**
   - Move `src/character/controller.{h,cpp}` → `src/vehicle/vehicle_controller.{h,cpp}`
   - Update all `#include` statements in dependent files
   - Test: Build succeeds, game runs identically

   **2.2 Rename Core Type**
   - `struct controller` → `struct vehicle_controller`
   - Update all references in game_world and consumers
   - Keep all member variables, all logic identical
   - Test: Build succeeds, game runs identically

   **2.3 Add Vehicle Parameters (Preparation)**
   - Add `float wheelbase = 2.5f;` // meters - distance between axles
   - Add `float max_steering_angle = 25.0f;` // degrees
   - Add `float grip_coefficient = 1.2f;` // lateral grip multiplier
   - Don't use them yet, just add the fields
   - Test: Build succeeds, game runs identically

   **2.4 Remove Jump Mechanics**
   - Delete `jump_velocity`, `coyote_window`, `jump_buffer_window` parameters
   - Delete `coyote_timer`, `jump_buffer_timer` state variables
   - Delete jump input handling in `apply_input()` (lines 54-126)
   - Delete `update_jump_timers()` function
   - Remove `jump_pressed` from `controller_input_params`
   - Test: Build succeeds, no jump input, movement still works

   **2.5 Transform Gravity to Weight/Downforce**
   - Rename `float gravity = -9.8f;` → `float weight = 15000.0f;` // Newtons
   - Keep applying it the same way in update_physics (downward force)
   - Update comments to reflect "weight pulling down" instead of "gravity"
   - Test: Build succeeds, vertical physics unchanged

   **2.6 Remove Wall Slide Detection**
   - Remove `is_wall` field from `contact_debug_info`
   - Remove wall-specific logic in `update_collision()`
   - Keep basic collision resolution, just remove wall classification
   - Test: Build succeeds, collision still resolves

   **2.7 Replace locomotion_state with traction_state**
   - Add `enum class traction_state { GRIP, DRIFT, AIRBORNE };`
   - Add `traction_state traction = traction_state::GRIP;`
   - Delete `locomotion_speed_state`, `locomotion_state` struct, `locomotion` member
   - Delete `update_locomotion_state()` function
   - Delete distance_traveled, cycle tracking variables
   - Set `traction = is_grounded ? GRIP : AIRBORNE;` in update()
   - Test: Build succeeds, state tracks grounded/airborne

   **2.8 Add Speed-Dependent Steering**
   - In `apply_input()`, replace heading integration with:
   - Calculate `speed`, compute `steering_limit = max_steering_angle * (1.0f - (speed/max_speed) * 0.7f)`
   - Scale `turn_rate` by `(steering_limit / max_steering_angle)`
   - Test: Steering gets tighter at high speeds

   **2.9 Add Circle-Based Position Calculation (Major)**
   - In `update_physics()`, after velocity integration, before position integration
   - Calculate `turn_radius = wheelbase / tan(steering_angle)` with singularity guard
   - Calculate `omega = speed / turn_radius`
   - Calculate `circle_center = position + perpendicular(heading) * turn_radius`
   - Rotate position around circle center by `omega * dt`
   - Rotate heading to match: `heading_yaw += angle_change`
   - Replaces standard `position += velocity * dt` for horizontal movement
   - Test: Car follows circular arcs when turning

   **2.10 Add Drift Detection (Lateral Acceleration)**
   - After circle calculation, compute `lateral_accel = speed * omega`
   - Compare to `max_lateral_accel = grip_coefficient * 9.8f`
   - If exceeded: set `traction = DRIFT`, reduce speed by `drift_speed_loss * dt`
   - Else: set `traction = GRIP`
   - Test: High-speed sharp turns trigger DRIFT state, speed bleeds

   **2.11 Separate Circle-Following from Drift**
   - Only apply perfect circle position when `traction == GRIP`
   - When `traction == DRIFT`: use standard `position += velocity * dt`
   - Test: GRIP = perfect arcs, DRIFT = looser sliding

   **2.12 Add Steering Metadata**
   - Add parameter metadata for GUI:
   - `max_steering_angle_meta`, `grip_coefficient_meta`
   - Test: Vehicle tuning panel shows new parameters

   **2.13 Remove Dead Code**
   - Delete `walk_threshold`, `run_threshold`, cycle_length variables
   - Delete `get_cycle_length()` function
   - Clean up unused includes
   - Test: Build succeeds, no warnings

3. **Repurpose Reactive Visuals**
   - File: `src/character/character_reactive_systems.{h,cpp}` → `src/vehicle/vehicle_dynamics.{h,cpp}`
   - Transform: Yaw spring → Body roll (lateral acceleration)
   - Transform: Landing spring → Pitch (longitudinal acceleration)
   - Add: Suspension compression per wheel (if visible wheels)
   - Keep: Spring-damper integration logic

4. **Convert Tuning Parameters**
   - File: `src/character/tuning.{h,cpp}` → `src/vehicle/vehicle_tuning.{h,cpp}`
   - Replace parameters:
     - `max_speed` → `top_speed` (with gear ratios consideration)
     - `acceleration` → `engine_torque` + `traction_coefficient`
     - `gravity` → `downforce` + `weight`
     - `jump_velocity` → `handbrake_force`
     - `turn_rate` → `max_steering_angle`
   - Add new parameters:
     - `drift_threshold` (slip angle to enter drift)
     - `drift_speed_loss` (0.0-1.0 multiplier)
     - `boost_acceleration` (nitrous multiplier)

### Phase 3: GUI Migration
**Status:** Not Started

5. **Update Panel Systems**
   - File: `src/gui/character_panel.{h,cpp}` → `src/gui/vehicle_panel.{h,cpp}`
   - Keep: Metadata-driven widget generation
   - Replace: Character-specific controls with vehicle parameters
   - Add: Drift angle display, boost charge meter, gear indicator

6. **Enhance Camera Controls**
   - File: `src/gui/camera_panel.{h,cpp}`
   - Add: Dynamic FOV controls (min/max/speed scaling)
   - Add: Camera shake intensity slider
   - Add: Chase camera height/distance presets

### Phase 4: Integration
**Status:** Not Started

7. **Update Game World**
   - File: `src/app/game_world.{h,cpp}`
   - Replace: `character_controller` with `vehicle_controller`
   - Update: Debug visualization to show drift angle, speed vectors
   - Add: Track boundary collision checks
   - Keep: Core update loop structure

8. **Adjust Debug Visualization**
   - File: `src/app/debug_generation.{h,cpp}`
   - Transform: Character capsule → Car bounding box
   - Add: Turning radius visualization (circle at current steering)
   - Add: Drift angle arc on speed ring
   - Keep: Velocity vectors, orientation arrows

### Phase 5: Cleanup
**Status:** Not Started

9. **Remove Character-Specific Code**
   - Delete: Jump buffering logic
   - Delete: Locomotion tier calculations
   - Delete: Wall slide specific handling
   - Delete: Landing crouch animation
   - Update: Comments and documentation

10. **Validation & Testing**
    - Verify: Car-like controls still function
    - Test: Collision with track boundaries
    - Validate: Spring-damper still critically damped
    - Ensure: Parameter metadata generates correct UI

### Migration Rules

- **Never delete without archiving** - Character systems may have useful patterns
- **Incremental transformation** - One system at a time, test between changes
- **Preserve working foundation** - Don't touch Layer 1 (CORE) or stable Layer 2 primitives
- **Maintain principles** - Mathematical correctness, frame-rate independence
- **Git commit each phase** - Easy rollback if needed

### Success Criteria

✅ Vehicle responds to WASD with forward/back + steering
✅ Collision detection works with track boundaries
✅ Debug visualization shows vehicle state clearly
✅ Parameter tuning UI functions with new vehicle parameters
✅ Spring-damper systems create smooth visual motion
✅ No character-specific code remains in active systems

---

## Layer 2 - Primitives

**Circle-Based Turning**
- Calculate position along circular arc from speed and steering angle
- Check if required lateral acceleration exceeds grip threshold
- Foundation for predictable arcade handling
- Enables: Speed-dependent steering, drift initiation, rail-based cornering
- Requires: None (pure mathematical primitive)

**Slip Angle Calculation**
- Angle between velocity vector and vehicle heading
- Foundation for drift detection and tire force modeling
- Simplified arcade version (no Pacejka curves)
- Requires: None (pure mathematical primitive)

---

## Movement & Physics

**Layer 4 - Buildable Now (No Dependencies):**

**Vehicle Tilt System**
- Vehicle model tilts based on lateral acceleration (visual lean in corners)
- Forward/backward pitch during acceleration/braking
- Visual weight transfer without complex physics
- Requires: None (works with current physics system)

**Speed-Dependent Steering**
- Limit max steering angle based on current speed
- Formula: max_angle = base_angle * (1.0 - (speed/maxSpeed) * reduction_factor)
- Prevents unrealistic tight turns at high speed
- Critical for arcade racing feel
- Requires: None (parameter-based modifier)

**Layer 4 - Requires Circle-Based Turning:**

**Arcade Drift System**
- Brake-to-drift activation (handbrake + steering at speed)
- State machine: GRIP → DRIFT_INITIATE → DRIFT_MAINTAIN → DRIFT_EXIT
- Maintained speed during drift (arcade style, not realistic)
- Visual and physics separation (car slides but maintains momentum)
- Requires: Circle-based turning, slip angle calculation

**Boost/Nitrous System**
- Accumulates charge through drifting or pickups
- Three-tier system (mini/super/ultra like Mario Kart research)
- Speed multiplier with visual effects
- Integration with drift system for charge-while-boosting
- Requires: Drift system for charge mechanics

**Rail-Based Cornering**
- Ridge Racer style - car locks to ideal corner arc
- Player makes minor adjustments while system handles curve
- Activates in designated corner zones or at high drift angles
- Hybrid with free movement for best of both
- Requires: Circle-based turning

---

## Camera

**Dynamic FOV System**
- FOV increases with speed (base 75° → max 110°)
- Additional g-force multipliers for acceleration
- Formula: FOV = baseMin + (speed/maxSpeed) * (baseMax - baseMin)
- Critical for speed sensation per research
- Requires: None (camera parameter modification)

**Camera Shake on Boost**
- Rotation-based shake (less aggressive than translation)
- Speed-scaled magnitude
- Activates above speed threshold or during boost
- Requires: None (camera transform modification)

---

## Racing Systems

**Layer 4 - Foundation:**

**Lap/Checkpoint System**
- Track progress through ordered checkpoints
- Lap completion detection
- Wrong-way detection and warnings
- Position tracking for multi-vehicle races
- Requires: Collision detection with trigger volumes

**Layer 4 - Requires Checkpoint System:**

**Time Trial Mode**
- Ghost recording and playback
- Best lap storage
- Sector timing
- Deterministic physics requirement (fixed timestep validation)
- Requires: Lap/checkpoint system

**Racing Line Visualization**
- Ideal path through corners
- Color-coded by recommended speed
- Dynamic adjustment based on current speed
- Assists for learning tracks
- Requires: Track data, checkpoint system

---

---

## Visual Effects

**Layer 5 - Polish:**

**Speed Lines System**
- Post-processing radial lines emanating from screen center
- Intensity scales with speed
- Activate during boost or above speed threshold
- UV-based shader with polar coordinate conversion
- Requires: Post-processing pipeline

**Motion Blur**
- Camera motion blur (velocity-based sampling)
- Radial blur for boost moments
- Depth-based optimization (lower res render target)
- Controversial per research but smooths 30 FPS
- Requires: Previous frame transform buffer

**Particle Trail System**
- Tire smoke during drifts
- Dust clouds on dirt surfaces
- Sparks from wall collisions
- Speed-based intensity scaling
- Requires: Particle system foundation

**Skid Marks**
- Persistent tire marks during slides
- Fade over time or distance limit
- Different patterns for brake vs drift
- Surface-dependent (black on asphalt, grooves on dirt)
- Requires: Deferred decal system or mesh generation

---

## Infrastructure & Tools

**Generic Parameter Inspector**
- Auto-generate UI from metadata for any struct with param_meta fields
- Iterate struct metadata at runtime, create widgets dynamically
- Enables zero-boilerplate debug panels for new systems
- Pattern proof: designer_parameter_interface proved metadata-driven widgets work
- Requires: Metadata-driven design pattern (exists), reflection or code-gen for iteration
- Source: Designer parameter interface iteration 3 emergence analysis

**Parameter Sweep Testing Framework**
- Automated parameter space exploration using metadata-defined ranges
- Generate test cases across valid parameter combinations
- Detect edge cases, instabilities, unexpected emergent behaviors
- Validate mathematical contracts across full parameter space
- Requires: Metadata-driven design pattern (exists), test infrastructure
- Source: Designer parameter interface iteration 3 emergence analysis

**Drift Angle Visualization**
- Circular arc on speed ring showing angle between heading and course
- Visualizes difference between orientation (where character faces) and velocity direction
- Enhances understanding of momentum and drift behavior during turning
- Complements: Existing debug visualization (speed ring, orientation/velocity arrows)

**Vehicle Telemetry System**
- Record speed, acceleration, steering, drift angle over time
- CSV export for analysis
- Real-time graphs in debug UI
- Lap comparison overlays
- Requires: Debug UI system (exists)

**Track Editor**
- Define track boundaries and racing line
- Place checkpoints and triggers
- Set surface properties (grip levels)
- Spline-based corner definition
- Requires: Level editor foundation

**Test Infrastructure**
- Test runner script and discovery
- Framework evaluation (if needed)
- Integration into dev workflow
- Trigger: When test count reaches 3-5 files

---

## Future (Keep Liquid)

**Layer 5 - Advanced Racing:**
- AI opponents with rubber-band difficulty
- Multiplayer/split-screen support
- Weather effects on handling
- Vehicle customization/tuning
- Championship/career progression
- Track variety (circuits, point-to-point, drift zones)

**Don't detail until dependencies clear.**

---

## Selection Criteria

**Before building:**
1. Dependencies must exist (check DEPENDENCY_STACK.md)
2. Problem must be understood (not guessed)
3. Create `/SYSTEM/SELECT` plan before starting

**Don't build from uncertainty. Build from clarity.**
