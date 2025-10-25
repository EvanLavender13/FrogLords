# Iteration 1: Vehicle Tilt Foundation

**Started:** 2025-10-25
**Status:** In Progress

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

Validate mathematical correctness and parameter physicality. Ensure visual tilt system follows physics-first tuning principles.

### Mathematical Correctness
- [ ] Lean angle proportional to lateral g-force produces visually correct cornering behavior at all speeds
- [ ] Pitch angle from longitudinal acceleration shows weight transfer during speed changes
- [ ] Transform composition (translate → yaw → roll → pitch) produces expected visual without gimbal issues
- [ ] Local-space rotation axes prevent spurious tilt when driving straight

### Parameter Validation (Physics-First Tuning)
- [ ] Lean multiplier (radians/g) has physical justification and valid range
- [ ] Pitch multiplier (radians/(m/s²)) has physical justification and valid range
- [ ] Tilt stiffness (N/m) follows spring constant conventions and produces stable damping
- [ ] All parameters explainable to physicist per PHYSICS_FIRST_TUNING.md

### Edge Cases
- [ ] Zero velocity (stationary): no orientation change, no spurious rotation
- [ ] High-speed straight: no roll from numerical drift or accumulated error
- [ ] Sharp corners: lean magnitude bounded and stable (no overshoot with critical damping)
- [ ] Rapid direction changes: spring damping prevents jitter

### System Integration
- [x] Visual system reads controller state without modifying physics
- [x] Unidirectional flow maintained: physics → visual → debug rendering
- [x] Debug visualization shows vehicle proportions and tilt correctly
- [x] Zero physics behavior changes (collision sphere unaffected)
<!-- END: ITERATE/CONTRACT -->

---

<!-- BEGIN: ITERATE/VALIDATION -->
## Validation Work

### Mathematical Correctness (Code Review)

**Transform composition (vehicle_visual_systems.cpp:47-64):**
- ✅ Translate → Yaw → Roll → Pitch order correct
- ✅ Local-space axes (glm::vec3(0,0,1) forward, (1,0,0) right) prevent world-space issues
- ✅ Orientation derived from velocity (not heading), prevents stationary rotation

**Calculations (vehicle_visual_systems.cpp:21-48):**
- ✅ Lean: `lateral_g * lean_multiplier` - proportional, dimensionally correct (rad/g)
- ✅ Pitch: `forward_accel * pitch_multiplier` - proportional, dimensionally correct (rad/(m/s²))
- ✅ Acceleration derived: `(velocity - previous_velocity) / dt` - time-independent
- ✅ Forward projection: `dot(accel, forward)` - extracts longitudinal component
- ✅ Spring damping: critical damping prevents overshoot

### Assertions Added (vehicle_visual_systems.cpp:16-58)

**Preconditions:**
- Parameter ranges validated against metadata (lean: [0,1], pitch: [0,0.2], stiffness: [10,500])
- dt validation (positive and finite)

**Intermediate checks:**
- lateral_g, target_lean, acceleration, forward_accel, target_pitch all finite

**Postconditions:**
- Spring positions finite after update

**Total assertions added:** 11 (3 preconditions, 6 intermediate, 2 postconditions)

### Physics-First Trade-Off Documentation

**Added to vehicle_visual_systems.h:29-35:**
Documented that multipliers are arcade conversion factors for visual feedback, not physical properties. Explained trade-off: exaggerated motion readability vs physical realism. Noted that real vehicles compress suspension vertically, don't tilt bodies proportionally to g-forces.

**Added to vehicle_tilt_SYSTEM.md:87-88:**
Documented physics-first trade-off in system plan. Explained spring damping remains physically accurate even though mechanism being damped is non-physical.

### System Integration (Architectural Review)

**Unidirectional flow verified:**
- controller.update() → physics state changes
- vehicle_visuals.update(controller, dt) → reads physics via const reference
- get_visual_transform(controller) → reads physics and visual state
- Zero data flow from visual → physics

**Separation validated:**
- vehicle_visual_systems owns all visual-only state (previous_velocity, springs, orientation)
- controller unchanged (no visual pollution)
- Debug rendering only consumer of visual transform
<!-- END: ITERATE/VALIDATION -->

---
