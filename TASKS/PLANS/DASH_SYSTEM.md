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

---

<!-- BEGIN: GRAYBOX/SCOPE -->
## Graybox

**What will be built:**
Dash mechanic as instant impulse force applied to grounded controller. Button press triggers velocity boost in input direction. Cooldown timer prevents spam. Existing friction system naturally decelerates. Debug overlay shows dash state (ready/cooldown) and cooldown timer.

**Core validation:**
- Impulse creates perceptible burst movement
- Cooldown prevents spam without feeling restrictive
- Friction deceleration curve feels natural
- Composes cleanly with jump and wall slide
- Tunable parameters allow feel adjustment

**What will NOT be built:**
- Air dash (deferred - adds complexity)
- Direction lock during dash (violates control principle)
- Special animation states (physics drives animation)
- Dash-specific particles/effects (polish, not core)

**Complexity:** Small (3-5 points)
<!-- END: GRAYBOX/SCOPE -->

---

<!-- BEGIN: GRAYBOX/IMPLEMENTATION_PLAN -->
## Implementation Plan

**Files to modify:**

1. `src/character/controller.h`
   - Add `dash_pressed` bool to `controller_input_params` struct
   - Add dash state: `dash_impulse` (tunable), `dash_cooldown` (tunable), `dash_timer` (state)
   - Add `can_dash()` const method (query helper for debug viz)

2. `src/character/controller.cpp`
   - Constructor: Initialize `dash_timer = 0.0f`
   - `apply_input()`: Check dash input + grounded + timer ready, apply impulse to velocity
   - `update()`: Decay dash_timer toward zero (cooldown tracking)

3. `src/app/game_world.cpp`
   - Add dash input polling: `input_params.dash_pressed = input::is_key_pressed(SAPP_KEYCODE_LEFT_SHIFT)`

4. `src/app/debug_generation.cpp`
   - Add dash state visualization function
   - Show dash ready/cooldown state
   - Show cooldown timer progress

**Call structure:**

```
game_world::update()
  → Poll input (SHIFT key for dash)
  → controller::apply_input(input_params, cam_params, dt)
      → If dash_pressed && is_grounded && dash_timer <= 0
          → Apply impulse: velocity += input_direction * dash_impulse
          → Reset dash_timer = dash_cooldown
  → controller::update(world, dt)
      → Decay dash_timer: dash_timer = max(0, dash_timer - dt)
      → (existing friction naturally decelerates dash velocity)
  → app::generate_debug_primitives()
      → Visualize dash state (ready/cooldown)
```

**Debug data flow:**

- Controller exposes: `dash_timer`, `dash_cooldown`, `can_dash()` method
- Debug viz reads state and renders:
  - Green indicator when ready (`dash_timer <= 0 && is_grounded`)
  - Red/orange indicator during cooldown
  - Timer bar showing cooldown progress (`dash_timer / dash_cooldown`)

**Integration points:**

- `controller::apply_input()`: Add dash logic after jump handling (line ~106)
- `controller::update()`: Add timer decay after jump buffer timer (line ~190)
- `game_world.cpp`: Add dash input polling after jump input (line ~34)
- `debug_generation.cpp`: Add dash viz function, call from `generate_debug_primitives()`

**Mathematical validation:**

- Impulse magnitude: `dash_impulse` in m/s (direct velocity addition)
- Cooldown duration: `dash_cooldown` in seconds (frame-rate independent)
- Timer decay: `dash_timer -= dt` (linear countdown to zero)
- Friction deceleration: Existing system handles velocity reduction (no changes needed)

**Edge cases:**

- Dash during jump buffering: Allowed (orthogonal systems)
- Dash allows exceeding max_speed: Clamp logic modified to only apply when speed ≤ max_speed. When dash pushes velocity above max_speed, friction naturally decelerates back to cap.
- Dash with no input: If `input_direction` is zero (no stick input), dash impulse adds zero velocity (no-op). Natural behavior - can't dash without direction.
- Dash while airborne: Blocked by `is_grounded` check
- Division by zero: Precondition validates `dash_cooldown > 0.0f` in constructor

**Design decisions from Codex review:**

1. **Speed cap modification**: Change clamp logic in `controller::update()` to only apply when `current_speed <= max_speed`. This allows dash impulse to temporarily exceed cap, with friction providing natural deceleration back to max_speed.

2. **Zero input direction**: No special handling needed. If player presses dash with no directional input, `input_direction` is zero, impulse adds zero velocity. Simple and consistent.

3. **Input semantics**: Uses `is_key_pressed()` (frame-based), matching jump behavior. Holding key does not auto-fire after cooldown expires.

4. **Parameter validation**: Constructor validates `dash_cooldown > 0.0f` to prevent division by zero in debug viz.
<!-- END: GRAYBOX/IMPLEMENTATION_PLAN -->

---

<!-- BEGIN: GRAYBOX/REVIEW -->
## Implementation Review

**Tool:** Codex CLI
**Date:** 2025-10-19

**Question asked:**
Validate this dash system implementation plan. Does it follow principles? Is the mathematical approach sound? Are there edge cases or concerns I should address?

**Feedback received:**
- Plan follows Radical Simplicity and Solid Mathematical Foundations principles
- Speed cap interaction: Original plan would clamp dash impulse immediately, disabling the mechanic
- Directionless dash: input_direction is recalculated each frame, so zero input = zero impulse (acceptable)
- Division by zero: Need to validate dash_cooldown > 0 to protect debug viz
- Input semantics: is_key_pressed() confirmed as frame-based (correct)

**Impact on implementation:**
- Modified speed clamp logic: Only clamp when current_speed <= max_speed
- This allows dash to temporarily exceed max_speed, with friction providing natural deceleration
- Added precondition validation for dash_cooldown > 0.0f
- Kept zero-input behavior simple (no special handling)
<!-- END: GRAYBOX/REVIEW -->

---

<!-- BEGIN: GRAYBOX/RESULTS -->
## Results

**Status:**
- [x] Core functional
- [x] Build successful
- [x] Debug viz implemented
- [ ] Manual testing required (user validation)

**Implementation complete:**
- Dash impulse applies instant velocity boost in input direction (controller.cpp:117-120)
- Ground-only constraint enforced via is_grounded check (controller.cpp:115)
- Cooldown prevents spam, decays frame-independently (controller.cpp:207)
- Speed clamp modified to allow temporary overspeed (controller.cpp:162-168)
- Debug visualization shows ready/cooldown state + timer bar (debug_generation.cpp:309-351)
- Input binding: LEFT_SHIFT key (game_world.cpp:35)

**Parameters (tunable):**
- dash_impulse: 6.0 m/s (velocity boost magnitude)
- dash_cooldown: 0.8s (time between dashes)

**Mathematical correctness:**
- Impulse applied directly to velocity (instant force)
- Cooldown timer decays linearly: timer -= dt
- Friction deceleration unchanged (existing system)
- Speed clamp conditional: only when speed <= max_speed

**User testing required:**
1. Does dash feel responsive? (instant velocity change on SHIFT press)
2. Does 6.0 m/s impulse feel perceptible? (may need tuning)
3. Does 0.8s cooldown prevent spam without feeling restrictive? (may need tuning)
4. Does friction deceleration feel natural from overspeed?
5. Can player chain dash with jump/wall slide? (emergent behavior)
6. Debug viz clear? (green = ready, red = cooldown, orange bar = timer)

**Known limitations (defer to ITERATE):**
- No air dash (ground-only by design)
- No dash-specific particles/sound (polish, not core)
- Parameters need playtesting to tune feel

**Surprises:**
- CRITICAL: Speed clamp modification breaks normal movement
- Skipping clamp when speed > max_speed allows infinite acceleration from normal input
- Even dash-only clamping would fail: after dash exceeds max_speed, next frame allows continued acceleration above cap
- Root cause: Hard software clamp vs natural physical limit

**Critical Issue - BLOCKER:**

The speed cap is implemented as a hard clamp, not a physical equilibrium. This creates fundamental conflict with dash:

1. **Current system:** `clamp_horizontal_speed(velocity, max_speed)` truncates any velocity above cap
2. **Problem:** Dash must exceed cap temporarily, but ANY exception to clamp allows infinite acceleration
3. **Root cause:** Acceleration (20 m/s²) exceeds friction (8.82 m/s²), so without clamp, speed grows unbounded

**Required fix (ITERATE phase):**

Replace hard clamp with speed-dependent friction that creates natural equilibrium at max_speed:
- When speed < max_speed: Low friction, net acceleration positive
- When speed = max_speed: Friction balances acceleration, net zero
- When speed > max_speed: High friction, net deceleration back toward max_speed

This allows dash to temporarily exceed max_speed while friction naturally brings it back down, and prevents normal movement from accelerating infinitely.

**Status:** Dash core implemented but non-functional until speed cap system refactored

**Next:**
- ITERATE phase: Refactor speed cap to use physical friction curve instead of hard clamp
- Then validate dash mechanic works with natural deceleration
- Defer if friction refactor too complex
<!-- END: GRAYBOX/RESULTS -->

---

## Iterations

- [DASH_ITERATION_1.md](DASH_ITERATION_1.md) - REVISE
- [DASH_ITERATION_2.md](DASH_ITERATION_2.md) - Ready for VALIDATE

