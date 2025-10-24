# System: Circle-Based Turning

Calculate position along circular arc from speed and steering angle. Foundation for predictable arcade handling.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-24
**Layer:** 2
**Complexity:** Small
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/REFERENCES -->
## References

**Technical documentation:**
- "Implementing Racing Games" - Circle-based turning algorithm (Research.md:217-236)
- Marco Monster's "Car Physics for Games" - Vehicle dynamics foundation
- Bicycle model geometry: `turnRadius = wheelbase / sin(steeringAngle)`
- Angular velocity: `omega = speed / turnRadius`
- Lateral acceleration threshold: `lateralAccel = speed * omega` vs `maxLateralAccel = gripCoeff * gravity`

**Visual references:**
- Debug visualization showing circular arc, circle center, desired position vs actual
- Turning radius visualization decreasing with speed (speed-dependent steering)

**Inspiration/Prior art:**
- Ridge Racer - rail-based drift system using circular arcs
- Mario Kart - predictable turning with clear feedback
- Burnout - "calculate exact positions applying forces to reach them" philosophy

**Research findings:**
- Circle-based turning "creates predictable, satisfying control that's trivial to tune compared to realistic slip-angle calculations" (Research.md:18-20)
- System naturally detects drift conditions when lateral acceleration exceeds grip threshold
- Enables brake-to-drift, rail-based cornering, and speed-dependent steering

**Constraints/Requirements:**
- Pure mathematical primitive - no side effects
- Time-independent calculation
- Returns data only - systems decide how to apply
- Must handle edge cases (zero speed, zero steering, singularities)
<!-- END: SELECT/REFERENCES -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required:**
- GLM math library (exists in Layer 1)
- Math utilities: safe normalization, angle operations (exists in Layer 2)

**Enables (will become buildable):**
- Arcade Drift System (Layer 4)
- Rail-Based Cornering (Layer 4)
- Speed-Dependent Steering improvements
- Drift angle visualization
<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
Given speed, steering angle, heading, wheelbase, and dt, calculate position along circular arc and detect if lateral acceleration exceeds grip threshold.

**Why a system, not a feature:**
Provides foundation for entire category of arcade handling behaviors. Drift detection, predictable cornering, rail-based systems all emerge from this primitive. Not tied to any specific mechanic.
<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Needs proof:**
- Turn radius calculation handles singularity at zero steering angle
- Angular velocity integration is time-independent (same result at any dt)
- Lateral acceleration threshold correctly identifies grip vs slide conditions
- Circle center calculation remains stable across frame boundaries

**Known uncertainties:**
- Ackermann geometry: should inner/outer wheels have different angles? (Research suggests arcade games skip this)
- Speed floor: minimum speed threshold to prevent divide-by-zero or unrealistic behavior
- Steering angle limits: how to clamp/smooth steering input before calculation
<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: SELECT/GRAYBOX -->
## Graybox

**Simplest implementation:**
- Debug draw circular arc showing turn path (10-20 segments)
- Visualize circle center with small sphere
- Show desired position vs actual position (different colors)
- Draw lateral acceleration vector
- Color-code arc by grip state (green = within grip, red = exceeding grip)

**Validation:**
- Assert turn radius > 0 when steering != 0
- Assert omega finite
- Assert lateral acceleration values reasonable (< 50 m/s²)
- Visual confirmation: turning left produces leftward arc
- Continuous testing: print lateral accel threshold vs actual during turns
<!-- END: SELECT/GRAYBOX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Core functional in graybox
- [ ] Mathematical correctness proven (time-independence, edge cases)
- [ ] Debug visualization shows circular turning behavior
- [ ] Lateral acceleration threshold detection working
- [ ] Principles upheld (pure function, no accumulated state, time-independent)
<!-- END: SELECT/SUCCESS -->
