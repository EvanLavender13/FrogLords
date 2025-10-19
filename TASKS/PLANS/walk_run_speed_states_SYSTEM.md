# System: Walk/Run Speed States

**Multiple speed tiers with smooth input-driven transitions**

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-19
**Layer:** 4
**Foundation:** 98%+
**Complexity:** Medium (3-5 points)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required (≥90%):**
- Controller: 97% ✅
- Spring-Damper: 100% ✅
- Input: 95% ✅

**Enables (will become buildable):**
- Stamina systems
- Sprint mechanics
- Movement-based gameplay variations
<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
Input-driven speed selection with smooth transitions between discrete speed tiers (walk, run, sprint). Remove tiers → single speed only. Remove transitions → jarring state changes. Remove input control → automatic movement.

**Why a system, not a feature:**
Enables emergence through speed modulation—players express intent through movement speed. Creates movement variety without adding features. Composes naturally with jump, dash, and other mechanics to create unexpected combinations.
<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Needs proof:**
- Transition smoothness preserves player control (no input lag)
- Speed tiers feel distinct yet connected (clear boundaries, smooth crossings)
- Input responsiveness maintained across all tiers (no dead zones)
- Acceleration curves don't violate physics consistency

**Known uncertainties:**
- Exact speed thresholds (walk: 0-3 m/s? run: 3-6 m/s? sprint: 6+ m/s?)
- Transition method (spring-damper vs simple lerp vs instant)
- Transition duration (feels responsive but not jarring)
- Input magnitude mapping (linear vs curved response)
<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: SELECT/GRAYBOX -->
## Graybox

**Simplest implementation:**
- Capsule/cube character with color-coded speed tiers (walk=green, run=yellow, sprint=red)
- Debug overlay showing: current tier, target speed, actual speed, input magnitude
- Trail renderer visualizing speed history
- Grid floor for distance/speed reference

**Validation:**
- Smooth transitions visible in trail color gradient
- Input changes immediately update target tier
- No "stuck" states between tiers
- All tier combinations reachable (walk→sprint, sprint→walk, etc.)
<!-- END: SELECT/GRAYBOX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Core functional in graybox
- [ ] Mathematical correctness proven (transition equations validated)
- [ ] Debug visualization shows behavior clearly
- [ ] Emergent behaviors appear (speed dancing, precise positioning, momentum tricks)
- [ ] Principle scores ≥7.0 average
- [ ] No control loss during transitions
- [ ] Feels responsive across all speed ranges
<!-- END: SELECT/SUCCESS -->
