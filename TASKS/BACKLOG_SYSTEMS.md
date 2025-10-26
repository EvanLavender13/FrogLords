# Systems Backlog

**Systems to build.**

**Last Updated:** 2025-10-26

---

## Movement & Physics

**Layer 4 - Front/Rear Wheel Physics (Physics-Driven Drift):**

**Handbrake System Refactor**
- Move handbrake from friction_model composition to controller-level state
- Pure input state holder (no physics logic)
- Interface: `update(bool)`, `is_active()`
- Systems query handbrake state rather than own it
- Verifiable: Handbrake behavior unchanged after refactor
- Requires: None

**Front Axle System** ← NOW BUILDABLE (vehicle mass property complete)
- Calculate front slip angle from velocity + steering angle
- Compute lateral tire force: `cornering_stiffness * slip_angle * normal_load`
- Tunable: cornering_stiffness, axle_distance
- Returns force magnitude for physics integration
- Verifiable: GUI display of front slip angle and lateral force
- Requires: Vehicle Mass Property (complete)

**Rear Axle System** ← NOW BUILDABLE (vehicle mass property complete)
- Calculate rear slip angle from velocity (no steering term)
- Compute lateral tire force with handbrake grip reduction
- Handbrake multiplier: reduces effective cornering_stiffness
- Tunable: cornering_stiffness, handbrake_grip_multiplier, axle_distance
- Verifiable: GUI display of rear slip angle, force drops when handbrake engaged
- Requires: Vehicle Mass Property (complete), Handbrake System Refactor

**Heading Integrator System**
- Compose front/rear axle systems
- Compute yaw torque from tire force imbalance: `F_front * d_front - F_rear * d_rear`
- Integrate: torque → angular_accel → angular_velocity → heading_yaw
- Replace direct heading control with physics-driven rotation
- Tunable: moment_of_inertia, angular_drag
- Verifiable: Vehicle rotates from tire forces, handbrake induces oversteer
- Requires: Front Axle System, Rear Axle System

**Friction Model Refactor**
- Rename `compute_total_drag` → `compute_longitudinal_drag`
- Apply only to forward velocity component (not total horizontal)
- Remove handbrake ownership (accept as parameter)
- Add optional lateral drag coefficient (air resistance on side)
- Verifiable: Forward/backward motion unchanged, lateral motion independent
- Requires: Heading Integrator System (for coordinate transform)

**Local Coordinate Physics Integration**
- Transform velocity to vehicle-local frame (forward/lateral components)
- Apply longitudinal drag to forward velocity (friction_model)
- Apply lateral tire forces to lateral velocity (heading_integrator axles)
- Integrate yaw dynamics (heading_integrator)
- Transform back to world space
- Verifiable: Drift emerges naturally from handbrake, counter-steering required
- Requires: Friction Model Refactor, Heading Integrator System

**Weight Transfer System** (Optional Enhancement)
- Shift normal load between front/rear during accel/brake
- Formula: `load_transfer = (accel / wheelbase) * mass * height_COM`
- Affects tire force limits dynamically
- Enhances realism: power-on oversteer, brake-induced understeer
- Verifiable: Front dives under braking, rear squats under acceleration
- Requires: Local Coordinate Physics Integration

---

**Layer 4 - Arcade Enhancements (Independent of Drift Physics):**

**Boost/Nitrous System**
- Accumulates charge through gameplay (pickups, successful maneuvers)
- Three-tier system (mini/super/ultra)
- Acceleration multiplier: `acceleration *= boost_multiplier` (1.5x-2.5x)
- Works entirely through existing physics pipeline
- Visual effects and FOV boost during activation
- Verifiable: Speed exceeds normal max_speed, decays naturally
- Requires: None (modifies existing acceleration)

---

## Camera

**Camera Shake on Boost** ← NOW BUILDABLE (dynamic FOV pattern established)
- Rotation-based shake (less aggressive than translation)
- Speed-scaled magnitude: `shake_mag = base + (speed/maxSpeed) * max_shake`
- Activates above 80% max speed or during boost
- Requires: Dynamic FOV System (pattern complete)

**Lower Camera Positioning** ← NOW BUILDABLE (dynamic FOV pattern established)
- Dynamically lower camera at high speeds
- More "road pixels" moving = faster perception
- Smooth interpolation to avoid jarring transitions
- Requires: Dynamic FOV System (pattern complete)

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
- Intensity scales with speed: `intensity = clamp(speed/maxSpeed, 0, 1)`
- Activate during boost or above 80% max speed
- UV-based shader with polar coordinate conversion
- Near-camera reference points critical for speed perception
- Requires: Post-processing pipeline

**Particle Trail System**
- Tire smoke during drifts (triggered by rear slip angle threshold)
- Dust clouds on dirt surfaces
- Sparks from wall collisions
- Intensity scales with slip angle magnitude
- Spawn near camera for maximum speed perception
- Requires: Particle system foundation, Rear Axle System (for slip angle)

**Skid Marks**
- Persistent tire marks during slides
- Triggered by slip angle exceeding threshold (use axle slip angles)
- Fade over time or distance limit
- Different patterns for brake vs drift
- Surface-dependent (black on asphalt, grooves on dirt)
- Requires: Deferred decal system or mesh generation, Front/Rear Axle Systems

**Motion Blur** (Optional)
- Radial blur specifically for boost moments
- Lower resolution render target for performance
- Use sparingly - speed lines more effective
- Requires: Previous frame transform buffer

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

**Slip Angle Visualization** ✅ COMPLETE (see angle arc primitive system)
- Arc visualization between heading and velocity
- Integrated via angle arc primitive in debug_generation.cpp

**Steering Authority Visualization** ← NOW BUILDABLE (angle arc primitive complete)
- Arc showing current steering angle vs maximum steering authority
- Visualizes speed-dependent steering limits
- Color-coded: green (full authority), yellow (reduced), red (minimal)
- Requires: Angle Arc Primitive (complete)

**Counter-Steering Detection Visualization** ← NOW BUILDABLE (angle arc primitive complete)
- Arc showing driver input direction vs required correction
- Helps learn counter-steering technique during drifts
- Distinct color from slip angle arc (e.g., cyan vs white)
- Requires: Angle Arc Primitive (complete)

**Front/Rear Slip Angle Debug Display** ← NOW BUILDABLE (angle arc primitive complete)
- Display both front and rear slip angles separately using angle arcs
- Show lateral force magnitude per axle
- Visualize yaw torque contribution from each axle
- Color-coded: green (grip), yellow (sliding), red (loss of grip)
- Helps tune cornering_stiffness and understand drift mechanics
- Requires: Front Axle System, Rear Axle System, Angle Arc Primitive (complete)

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
