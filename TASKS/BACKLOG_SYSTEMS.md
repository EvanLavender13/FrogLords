# Systems Backlog

**Systems to build.**

**Last Updated:** 2025-10-20

---

## Layer 2 - Primitives

**Quaternion Swing-Twist Decomposition**
- Decompose rotation into swing (directional pointing) + twist (axial roll)
- Anatomical joint limits (swing cone + twist range)
- Prevents "candy-wrapper" bone twisting effect
- Drives twist helper bones for mesh deformation
- Requires: None (pure mathematical primitive)

**Quaternion Interpolation (Slerp)**
- Spherical linear interpolation for smooth rotation between keyframes
- Constant angular velocity interpolation
- Shortest-path rotation handling
- Requires: None (pure mathematical primitive)

---

## Movement & Character

**Layer 4 - Buildable Now (No Dependencies):**

**Acceleration Tilt**
- Character model tilts in direction of acceleration
- Visual feedback for movement dynamics
- Procedural rotation applied to character mesh
- Requires: None (works with current physics system)

**Layer 4 - Blocked (Requires Friction Refactor #17):**

**Dash Mechanic** - @TASKS/PLANS/DASH_SYSTEM.md (DEFERRED)
- Burst movement in input direction
- Cooldown/resource management
- Maintains responsiveness (no control loss)
- Status: Rejected iteration 2 - requires frame-rate independent friction foundation

**Surveyor Wheel Locomotion**
- Distance-based keyframe triggering (pass pose + reach pose)
- Eliminates foot sliding at any speed
- Blends stride lengths for walk/run transitions
- Complements: Walk/run speed states (complete)
- Requires: Skeletal animation system

**Drift Movement Mechanics**
- Low acceleration creates racing-game drift (velocity lags orientation)
- Environmental modifiers (ice, mud, oil slicks)
- Character abilities or movement modes (drift boost, slide)
- Emerges naturally from physics/animation independence
- Discovery: REFINE_FRICTION testing revealed drift at ~2s time-to-max-speed
- Source: Retrospective pattern analysis (RETRO_2025-10-20)

---

## Camera

**Orientation-Locked Camera Mode** - @TASKS/PLANS/CAMERA_LOCK_SYSTEM.md (DEFERRED)
- Camera locks behind character orientation (forward direction)
- Automatically rotates to follow character facing
- Toggle between free orbit mode and locked mode
- Maintains distance and height offset behavior
- Status: Graybox validated math, discovered prerequisite
- Requires: Car-like control scheme (Layer 2) - current WASD world-space controls conflict with orientation lock
- Complements: Existing orbit camera system

---

## Animation

**Layer 4 - Foundation (Unlocks All Below):**

**Skeletal Animation System**
- Bone hierarchy and transforms
- Procedural pose generation
- Integration with physics-driven character
- Debug visualization for skeleton
- Requires: Quaternion swing-twist decomposition, quaternion interpolation

**Layer 4 - Requires Skeletal Animation:**

**Joint Constraint System**
- Swing-twist decomposition for anatomical limits
- Swing cone + twist range per joint
- Prevents unrealistic bone twisting ("candy-wrapper" effect)
- Drives twist helper bones for mesh deformation
- Requires: Skeletal animation system, quaternion swing-twist decomposition

**IK Systems**
- Inverse kinematics solver (two-bone, simple trigonometry)
- Foot placement on terrain
- Hand reaching to targets
- Look-at targets (head/torso turn independently)
- Requires: Skeletal animation system

**Secondary Physics System**
- Bone "softness" parameter for procedural motion
- Wobble/sway from primary character movement
- Natural blending between physical states
- Requires: Skeletal animation system

**Active Ragdoll System**
- Pose matching (ragdoll attempts to hold last pose)
- Animation matching (ragdoll tries to enact animation under forces)
- Contextual reactions (flail when airborne, curl near surfaces, brace for impact)
- Emergent life-like behavior from physics
- Requires: Skeletal animation system

**Delayed Ragdoll Transitions**
- Timer delay before ragdoll activation on death
- "Acted" death sequences within physics
- Emergent gameplay moments (dying enemy still threatens)
- Requires: Active ragdoll system

---

## Infrastructure & Tools

**Drift Angle Visualization**
- Circular arc on speed ring showing angle between heading and course
- Visualizes difference between orientation (where character faces) and velocity direction
- Enhances understanding of momentum and drift behavior during turning
- Complements: Existing debug visualization (speed ring, orientation/velocity arrows)

**Test Infrastructure**
- Test runner script and discovery
- Framework evaluation (if needed)
- Integration into dev workflow
- Trigger: When test count reaches 3-5 files

**Designer Parameter Facade**
- Separate designer intent from implementation math
- Tunable parameters (jump height) → derived values (jump velocity)
- GUI shows adjustable params + read-only derived values
- Generalizes to: movement feel, weapon timing, camera behavior, UI responsiveness

**Runtime Validation & Instrumentation**
- Measure actual vs intended behavior
- Trajectory visualization
- Automated validation tests
- World-scale reference measurements
- Validates contract: Designer Intent → Math → Implementation → Measured Reality

**Level Editor**
- Replace hard-coded game world with editable environment
- Simple text file format for level data
- Save/load level state to/from disk
- Object placement and world composition
- Requires: Game world system (existing)

---

## Future (Keep Liquid)

**Layer 5 - Polish:**
- Visual hierarchy system (UI/HUD polish)
- Input assistance framework (feel refinement)
- Audio system
- UI/HUD expansion
- Visual effects
- Terrain/world generation
- Combat mechanics

**Don't detail until dependencies clear.**

---

## Selection Criteria

**Before building:**
1. Dependencies must exist (check DEPENDENCY_STACK.md)
2. Problem must be understood (not guessed)
3. Create `/SYSTEM/SELECT` plan before starting

**Don't build from uncertainty. Build from clarity.**
