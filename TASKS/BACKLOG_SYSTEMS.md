# Systems Backlog

**Systems to build.**

**Last Updated:** 2025-10-25

---

## Movement & Physics

**Layer 4 - Buildable Now:**

**Boost/Nitrous System**
- Accumulates charge through gameplay (pickups, successful maneuvers)
- Three-tier system (mini/super/ultra)
- Acceleration multiplier: `acceleration *= boost_multiplier` (1.5x-2.5x)
- Works entirely through existing physics pipeline
- Visual effects and FOV boost during activation
- Requires: None (modifies existing acceleration)

**Enhanced Speed-Dependent Steering**
- Improve existing steering reduction formula
- Add exponential curves for more natural feel
- Separate low-speed and high-speed response profiles
- Smooth transitions between speed ranges
- Requires: None (enhancement of existing system)

**Brake Input Extension** ← NOW BUILDABLE
- Add brake/handbrake input to controller_input_params
- Map to Space key or controller trigger
- Binary input: pressed/not pressed
- Pass through input system to controller
- Foundation for drift initiation
- Requires: None (input system extension)

**Drift Detection Primitive** ← AFTER BRAKE INPUT
- Calculate drift intensity from slip angle (continuous 0-1 value)
- Threshold detection: 5-8° initiation, maintain up to 23° (0.4 rad)
- Speed gating: require speed > soft_threshold (3 m/s)
- Brake boost: multiply intensity when brake held
- Pure calculation, no state modification
- Returns: drift_intensity float for other systems to consume
- Requires: Slip Angle Calculator (complete), Brake Input Extension

**Continuous Friction Scaling** ← AFTER DRIFT DETECTION
- Smoothly interpolate friction based on drift_intensity
- No discrete states, continuous modification
- Formula: `friction = lerp(1.0, drift_friction, drift_intensity)`
- Rear wheels: 0.3x at full drift, Front wheels: 0.7x at full drift
- Applied in physics update, affects acceleration calculation
- Requires: Drift Detection Primitive

**Drift Speed Maintenance** ← AFTER FRICTION SCALING
- Boost acceleration during drift to maintain forward momentum
- Scale factor: `accel_multiplier = 1.0 + (drift_intensity * 0.5)`
- Works through exponential drag model (k = accel/max_speed)
- Physics solver naturally maintains equilibrium speed
- No velocity hacks, pure force modification
- Requires: Continuous Friction Scaling

**Drift Visual Feedback** ← AFTER FRICTION SCALING
- Enhance existing vehicle_visual_systems for drift
- Additional lean during drift (beyond g-force lean)
- Counter-steer visualization (wheels point opposite to drift)
- Integrates with existing spring-damped tilt system
- Pure visual, no physics modification
- Requires: Continuous Friction Scaling

**Centripetal Drift Assist** (Optional Enhancement)
- Apply subtle outward force during high-speed turns
- Magnitude: function of angular_velocity and speed
- Enhances arcade feel without breaking physics
- Formula: `outward_force = -lateral_dir * (angular_vel * speed * assist_factor)`
- Requires: Drift Speed Maintenance

**Drift Tuning Parameters**
- Expose all drift thresholds and multipliers via metadata
- Slip angle thresholds (initiation/maintain angles)
- Friction reduction factors (rear/front separately)
- Speed maintenance multiplier
- Brake boost factor
- Integrates with existing vehicle_panel GUI
- Requires: Any drift system to tune

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
- Tire smoke during drifts (triggered by drift state)
- Dust clouds on dirt surfaces
- Sparks from wall collisions
- Speed-based intensity scaling
- Spawn near camera for maximum speed perception
- Requires: Particle system foundation

**Skid Marks**
- Persistent tire marks during slides
- Triggered by slip angle exceeding threshold
- Fade over time or distance limit
- Different patterns for brake vs drift
- Surface-dependent (black on asphalt, grooves on dirt)
- Requires: Deferred decal system or mesh generation

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

**Slip Angle Visualization** ← NOW BUILDABLE
- Arc on speed ring showing angle between heading and velocity
- Visualizes difference between where vehicle points vs where it's going
- Pure measurement of existing physics state
- Enhances understanding of drift behavior
- Complements: Existing debug visualization (speed ring, orientation/velocity arrows)
- Requires: Slip Angle Calculator (complete)

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
