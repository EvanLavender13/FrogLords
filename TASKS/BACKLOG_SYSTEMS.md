# Systems Backlog

**Systems to build. Foundation: 98%+ (Layer 3 complete, Layer 4 building).**

**Foundation:** 98%+
**Last Updated:** 2025-10-19

---

## Movement & Character

**Layer 4 - Buildable Now (No Dependencies):**

**Dash Mechanic**
- Burst movement in input direction
- Cooldown/resource management
- Maintains responsiveness (no control loss)

**Surveyor Wheel Locomotion**
- Distance-based keyframe triggering (pass pose + reach pose)
- Eliminates foot sliding at any speed
- Blends stride lengths for walk/run transitions
- Complements: Walk/run speed states (complete)

---

## Animation

**Layer 4 - Foundation (Unlocks All Below):**

**Skeletal Animation System**
- Bone hierarchy and transforms
- Procedural pose generation
- Integration with physics-driven character
- Debug visualization for skeleton

**Layer 4 - Requires Skeletal Animation:**

**Joint Constraint System**
- Swing-twist decomposition for anatomical limits
- Swing cone + twist range per joint
- Prevents unrealistic bone twisting ("candy-wrapper" effect)
- Drives twist helper bones for mesh deformation
- Requires: Skeletal animation system

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

---

## Future (Keep Liquid)

**Layer 5 - Defer until Layer 4 ≥95%:**
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
1. Foundation must support it (Layer 3 at 95%+) ✅
2. Dependencies must be complete
3. Problem must be understood (not guessed)
4. Create `/SYSTEM/SELECT` plan before starting

**Don't build from uncertainty. Build from clarity.**
