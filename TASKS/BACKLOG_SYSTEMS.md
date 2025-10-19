# Systems Backlog

**Systems to build. Foundation: 98%+ (Layer 3 complete, Layer 4 building).**

**Foundation:** 98%+
**Last Updated:** 2025-10-19

---

## Movement & Character

**Dash Mechanic**
- Burst movement in input direction
- Cooldown/resource management
- Maintains responsiveness (no control loss)

---

## Animation

**Skeletal Animation System**
- Bone hierarchy and transforms
- Procedural pose generation
- Integration with physics-driven character
- Debug visualization for skeleton

**IK Systems**
- Inverse kinematics solver
- Foot placement on terrain
- Hand reaching to targets
- Requires: Skeletal animation system

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

**Vague ideas. Don't detail until dependencies clear:**
- Audio system
- UI/HUD
- Visual effects
- Terrain/world generation
- Combat mechanics

---

## Selection Criteria

**Before building:**
1. Foundation must support it (Layer 3 at 95%+) ✅
2. Dependencies must be complete
3. Problem must be understood (not guessed)
4. Create `/SYSTEM/SELECT` plan before starting

**Don't build from uncertainty. Build from clarity.**
