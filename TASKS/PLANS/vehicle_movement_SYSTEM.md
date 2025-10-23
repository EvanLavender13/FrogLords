# System: Vehicle Movement

Archive character systems and transform movement controller to circle-based vehicle physics.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-23
**Layer:** 3 (Systems transformation)
**Complexity:** Small (3 pts)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/REFERENCES -->
## References

**Technical documentation:**
- Circle-based turning: `turnRadius = wheelbase / sin(steeringAngle)`, `omega = speed / turnRadius`
- Position on arc: `circleCenter = position + Perpendicular(heading) * turnRadius`, then rotate by `omega * dt`
- Drift detection: `lateralAccel = speed * omega`, compare vs `maxLateralAccel = tireGripCoefficient * gravity`
- Speed-dependent steering: `max_angle = base_angle * (1.0 - (speed/maxSpeed) * reduction_factor)`
- Source: Research.md section 9 (code examples), section 1 (fundamentals)

**Inspiration/Prior art:**
- Single box collider for vehicle body (Rocket League approach)
- "Calculate where car should be, apply forces to reach it" vs letting physics resolve
- State machine: GRIP → DRIFT (start with 2 states, not 4)
- Arcade friction multipliers: 1.5-2.5× realistic values
- Source: Research.md sections 1, 3

**Constraints/Requirements:**
- Preserve frame-rate independence (matches existing controller integration)
- Keep existing collision system working (sphere-AABB)
- Maintain mathematical correctness for circle positioning
- Archive before deleting anything

<!-- END: SELECT/REFERENCES -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required (all exist):**
- Math Utilities (Layer 2) - angle ops, safe normalize
- Collision Primitives (Layer 2) - sphere-AABB tests
- Input System (Layer 1) - WASD handling
- Debug Visualization (Layer 1) - primitives for circles/vectors

**Enables:**
- Vehicle reactive systems (body roll/pitch springs)
- Vehicle tuning parameters
- Drift/boost mechanics (Layer 4)

<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
Archive character files to `archive/character/`. Transform `controller.{h,cpp}` to `vehicle_physics.{h,cpp}`. Replace linear acceleration with circle-based turning. Add traction state (GRIP/DRIFT). Remove jump/gravity/wall-slide. Keep collision resolution.

**Why a system, not a feature:**
Circle-based turning is the mathematical foundation for all vehicle behavior. Emergent cornering, drifting, and handling flow from this primitive. Not "adding vehicle mode" but replacing movement model at the physics level.

<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Needs proof:**
- Circle position calculation maintains time-independence
- Lateral acceleration correctly triggers drift threshold
- Collision resolution works with circle-based positioning
- Deterministic behavior across framerates

**Known uncertainties:**
- Drift speed loss tuning (start 0.3 = 30% reduction per Research.md)
- Steering reduction factor (research suggests starts matter, tune from there)

<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: SELECT/GRAYBOX -->
## Graybox

**Simplest implementation:**
Box collider for vehicle. WASD: W/S throttle/brake, A/D steering. Circle-based turning calculates position along arc. GRIP state: follow circle perfectly. DRIFT state: allow deviation, reduce speed. Collision: preserve existing AABB resolution. Debug viz: turning circle overlay, velocity vector, lateral accel indicator.

**Validation:**
- Vehicle moves forward/backward on throttle/brake
- Steering creates visible turning circle
- Position follows circle arc mathematically
- GRIP→DRIFT transition at lateral accel threshold
- Collision prevents track boundary penetration
- Same inputs = same outputs across framerates

<!-- END: SELECT/GRAYBOX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

**Step 1: Archive**
- [ ] `archive/character/` directory exists
- [ ] All character files copied to archive
- [ ] Archive committed: "archive: backup character systems before vehicle migration"

**Step 2: Transform Movement**
- [ ] `src/vehicle/vehicle_physics.{h,cpp}` created
- [ ] Circle-based turning math implemented
- [ ] Turning radius calculation correct
- [ ] Position-on-arc calculation correct
- [ ] Lateral acceleration calculation correct
- [ ] Traction state machine (GRIP/DRIFT) functional
- [ ] Drift triggers at lateral accel threshold
- [ ] Speed reduction during drift
- [ ] Jump mechanics removed
- [ ] Gravity as downward force removed
- [ ] Wall slide removed
- [ ] Collision resolution preserved
- [ ] Debug visualization shows turning circle and state
- [ ] Frame-rate independence verified
- [ ] Principles upheld (math correctness, time-independence)

<!-- END: SELECT/SUCCESS -->
