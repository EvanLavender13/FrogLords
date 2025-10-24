# Systems Backlog

**Systems to build.**

**Last Updated:** 2025-10-24

---

## 🔄 MIGRATION: Character → Vehicle Systems

**Completed:**
- Vehicle controller operational (`src/vehicle/controller.{h,cpp}`)
- Speed-dependent steering system (APPROVED)
- Vehicle tuning system with metadata (`src/vehicle/tuning.{h,cpp}`)
- Vehicle GUI panel (`src/gui/vehicle_panel.{h,cpp}`)
- Traction state classification (grip/drift/airborne)

**Remaining:**
- Circle-based turning physics (bicycle model)
- Drift detection via lateral acceleration
- Remove legacy character-specific code
- Vehicle dynamics visuals (body roll, pitch)

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

**Layer 4 - Buildable Now:**

**Vehicle Tilt System**
- Vehicle model tilts based on lateral acceleration (visual lean in corners)
- Forward/backward pitch during acceleration/braking
- Visual weight transfer without complex physics
- Requires: Vehicle movement system

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
