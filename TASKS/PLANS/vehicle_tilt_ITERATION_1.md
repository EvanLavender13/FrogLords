# Iteration 1: Vehicle Tilt Foundation

**Started:** 2025-10-25
**Status:** Ready for VALIDATE

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

Validate mathematical correctness and parameter physicality. Ensure visual tilt system follows physics-first tuning principles.

### Mathematical Correctness
- [x] Lean angle proportional to lateral g-force produces visually correct cornering behavior at all speeds
- [x] Pitch angle from longitudinal acceleration shows weight transfer during speed changes
- [x] Transform composition (translate → yaw → roll → pitch) produces expected visual without gimbal issues
- [x] Local-space rotation axes prevent spurious tilt when driving straight

### Parameter Validation (Physics-First Tuning)
- [x] Lean multiplier (radians/g) has physical justification and valid range
- [x] Pitch multiplier (radians/(m/s²)) has physical justification and valid range
- [x] Tilt stiffness (N/m) follows spring constant conventions and produces stable damping
- [x] All parameters explainable to physicist per PHYSICS_FIRST_TUNING.md

### Edge Cases
- [x] Zero velocity (stationary): no orientation change, no spurious rotation
- [x] High-speed straight: no roll from numerical drift or accumulated error
- [x] Sharp corners: lean magnitude bounded and stable (no overshoot with critical damping)
- [x] Rapid direction changes: spring damping prevents jitter

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

<!-- BEGIN: ITERATE/PLAYTEST -->
### Playtest 1

**Date:** 2025-10-25
**Tester:** EvanUhhh

**Initial test - pitch inversion bug found:**
- ❌ Pitch direction inverted (accelerating pitched nose DOWN instead of UP)
- ✅ Lean direction correct (leans AWAY from turn center due to inertia)

**Root cause:** Missing negation in pitch calculation. Positive forward_accel should produce negative pitch (nose up).

**Fix:** vehicle_visual_systems.cpp:48 - Added negation: `target_pitch = -forward_accel * pitch_multiplier`

**Commit:** 182c5d9 (initial validation), then pitch fix

### Playtest 2

**Date:** 2025-10-25
**Tester:** EvanUhhh

**All behaviors verified:**

✅ **Lean direction:**
- Turn left → leans RIGHT (away from turn, physically correct)
- Turn right → leans LEFT (away from turn, physically correct)

✅ **Pitch direction:**
- Accelerating forward → nose pitches UP (weight to rear)
- Decelerating → nose pitches DOWN (weight to front)

✅ **Edge cases:**
- Stationary: no rotation/tilt ✓
- High-speed straight: stays upright, no drift ✓
- Sharp corners: smooth lean, no overshoot ✓
- Rapid direction changes: no jitter ✓

**Emergent behavior discovered:**
Collision with walls creates dramatic tilt response from sudden velocity change, then smoothly recovers via critical damping. System handles extreme inputs without special cases - validates:
- Time-independent acceleration derivation
- Spring stability under extreme targets
- No accumulated state errors

**Insight:** This emergent collision response could inform future reactive collision systems (damage tilt, impact reactions). System boundary proven correct - handles cases not explicitly designed for.

✅ VERIFIED: All contract items proven through playtest
<!-- END: ITERATE/PLAYTEST -->

---

<!-- BEGIN: ITERATE/COMPLETE -->
## Iteration Complete

**Contract:** ✓ PROVEN

**Properties validated:**
- Mathematical correctness: Transform composition, local-space rotations, time-independent acceleration
- Physics-first parameters: Arcade multipliers documented with trade-off, spring constants physical
- Edge case stability: Stationary, high-speed, sharp turns, rapid changes all stable
- Emergent robustness: Collision impacts handled gracefully without special cases

**Assertions added:** 11 (3 preconditions, 6 intermediate, 2 postconditions)

**Playtests:** 2 (1 bug found and fixed, 1 full validation)

**Bug fixed:** Pitch direction inverted - added negation in acceleration-to-pitch conversion

**Emergent insight:** Collision-induced tilt validates system robustness. Spring damping handles extreme inputs without special cases. Potential foundation for future reactive collision systems.

**Status:**
- [x] Contract proven
- [x] Stable through edge cases
- [x] Ready for VALIDATE
<!-- END: ITERATE/COMPLETE -->

