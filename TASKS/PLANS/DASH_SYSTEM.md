# System: Dash Mechanic

**On-demand burst movement that respects physics and player control**

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-19
**Layer:** 4
**Foundation:** 99%+
**Complexity:** Small (3-5 points)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/REFERENCES -->
## References

**Technical documentation:**
- Web research: Dash as impulse-based mechanic (instant force application)
- Physics: Impulse + friction = natural deceleration curve
- Common patterns: Cooldown-based to prevent spam

**Design philosophy:**
- @NOTES/GDC/GDC_DesignPhilosophy.md - "Do No Harm to Gameplay"
- Controller input = character acceleration (impulse is instant acceleration)
- Physics-first approach: Apply force, let existing systems handle the rest
- @NOTES/DesigningGames/DG_Elegance.md - Simple mechanics, emergent behavior

**Inspiration/Prior art:**
- Celeste: Dash as core movement primitive
- Hollow Knight: Dash with cooldown, directional control
- Common pattern: Instant velocity boost + natural physics deceleration

**Constraints/Requirements:**
- Ground-only (to start) - no air dash complexity yet
- Tunable impulse magnitude (single designer parameter)
- No direction lock - player maintains steering control
- Cooldown-based (prevent spam)
- Existing friction naturally decelerates back to normal speed
- No special debug visual required (movement itself shows the effect)
<!-- END: SELECT/REFERENCES -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required (≥90%):**
- Controller: 97% (velocity control, grounded state)
- Input: 95% (button press detection)
- Physics/Friction: 100% (natural deceleration)

**Enables (will become buildable):**
- Air dash variation (future)
- Dash-jump combos (emergent from existing jump)
- Wall-dash interactions (already works with wall sliding)
<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
On button press, if grounded and cooldown ready, apply instant impulse force in input direction. Cooldown timer prevents spam. Existing ground friction naturally decelerates velocity back to normal speed.

**Why a system, not a feature:**
Dash is a primitive movement tool that composes with existing mechanics (jumping, wall sliding, speed states) to create emergent movement combinations. It's not a scripted sequence - it's a force that interacts with the physics foundation.
<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Needs proof:**
- Impulse magnitude must be tunable and produce perceptible burst
- Cooldown duration must prevent spam but allow fluid movement
- Friction deceleration curve must feel natural (already validated in existing movement)

**Known uncertainties:**
- Optimal impulse magnitude (requires tuning)
- Optimal cooldown duration (requires tuning)
- Should impulse be fixed magnitude or scale with input direction magnitude? (Start fixed)
<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: SELECT/GRAYBOX -->
## Graybox

**Simplest implementation:**
Add to controller:
- `dash_impulse` parameter (tunable force magnitude)
- `dash_cooldown` parameter (time between dashes)
- `dash_timer` (tracks cooldown state)
- On input + grounded + timer ready: apply impulse, reset timer
- No special visuals - existing character movement shows the effect

**Validation:**
- Does dash feel responsive? (instant velocity change on input)
- Does friction deceleration feel natural? (should already work)
- Can player chain dash with jump/wall slide? (emergent behavior)
- Does cooldown prevent spam but allow fluid movement?
<!-- END: SELECT/GRAYBOX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Dash applies instant impulse on input
- [ ] Ground-only constraint enforced
- [ ] Cooldown prevents spam
- [ ] Friction naturally decelerates to normal speed
- [ ] Tunable parameters (impulse, cooldown) exposed
- [ ] Composes cleanly with jump, wall slide, speed states
- [ ] Principle scores ≥7.0 average (simplicity, consistency, emergence)
<!-- END: SELECT/SUCCESS -->
