# Feature: Smoothed Walk Speed Transition

## Core Mechanic

Replace hard velocity clamp with smooth deceleration to walk speed when SHIFT pressed. Player expects natural momentum decay, not instant speed lock. When SHIFT released, smoothly accelerate back to run speed.

## Pattern Reuse

Extends proven spring-damper foundation (currently unused in dependency stack). Reuses existing `tuning_params` pattern from reactive animation systems. Aligns with "input maps to acceleration" principle (AGENTS.md) - modulates `max_speed` target rather than violating physics.

## Graybox Scope

**Minimum testable implementation:**
- Smooth `max_speed` between `run_speed` (4.0 m/s) and `walk_speed` (2.0 m/s) based on SHIFT state
- Physics system naturally decelerates/accelerates to new cap via existing friction
- Single tuning parameter: transition time (~0.2-0.5s estimate)

**Key parameters to expose:**
- `walk_transition_time`: How quickly max_speed lerps between walk/run
- Optional: Spring stiffness/damping if lerp feels too linear

**Estimated line count:** 20-40 lines
- Remove hard velocity clamp at controller.cpp:92-99 (~8 lines deleted)
- Add `target_max_speed` and `current_max_speed` floats to controller state
- Add smooth transition logic in `controller::update()` (exponential decay or spring-damper)
- Add tuning parameter to animation_tuning UI (1-2 lines)

## Minimal Implementation

**Where it hooks in:**
- `src/character/controller.cpp`: Replace hard clamp with smooth `max_speed` modulation
- Update order: Before velocity application, after input processing
- Files affected: `controller.cpp`, `controller.h`, potentially `gui/character_panel.cpp` for tuning UI

**Implementation approach (Option C from backlog):**
1. Add `float target_max_speed` and `float current_max_speed` to `controller` state
2. Set `target_max_speed = shift_held ? walk_speed : run_speed` each frame
3. Smooth `current_max_speed` toward target using exponential decay: `current_max_speed = lerp(current_max_speed, target_max_speed, transition_rate * dt)`
4. Replace existing `max_speed` constant usage with `current_max_speed`
5. Expose `transition_rate` in tuning UI

## Problem Evidence

**Documented observations:**
- User request 2025-10-10: "Smoothed Walk Speed Transition"
- Code review 2025-10-09: Identified hard velocity clamp as "Walk Speed Lock Refactor" backlog item
- Current implementation: `controller.cpp:92-99` hard-caps velocity at 2.0 m/s when SHIFT pressed
- Violation: Breaks "input maps to acceleration" principle - instant lock overrides physics momentum

**Specific gameplay moment:**
- Player running at max speed (4.0 m/s) presses SHIFT → velocity instantly locks to 2.0 m/s
- Expected: Smooth deceleration over ~0.3s (feels natural, preserves momentum)
- Actual: Abrupt stop (feels like hitting invisible wall)

## Unlocks

**Immediate:**
- Validates spring-damper foundation for non-animation uses (currently unused)
- Completes physics-first input feel (aligns with procedural animation philosophy)

**Future:**
- Foundation for crouch/sprint transitions (same pattern)
- Stamina/fatigue systems (modulate max_speed smoothly)
- Momentum-based abilities (dash/dodge can use same transition logic)

**Backlog items unblocked:**
- None explicitly, but completes control feel foundation before adding movement abilities (dash, dodge, wall jump)

## Test Loop

**How to verify success:**
1. Run at max speed, press SHIFT → should smoothly decelerate to walk (no instant lock)
2. Hold SHIFT from standstill → should accelerate directly to walk (no overshoot to run first)
3. Release SHIFT while walking → should smoothly accelerate to run
4. Rapid SHIFT tapping → should handle state changes gracefully (no oscillation)

**What to test first:**
- Transition feel at max speed (most obvious case)
- Visual coherence: Does animation blend match velocity change? (walk_factor already synced to velocity)

**Expected iteration time:**
- Initial implementation: 30-60 minutes (simple exponential decay)
- Tuning iteration: 15-30 minutes per adjustment (real-time via tuning UI)
- Total: 1-2 hours to production-quality feel

**Self-test loop:**
- Immediate feedback (press SHIFT, observe feel)
- No compilation/scene load delays
- Tuning parameters exposed in GUI (change `transition_rate` live)

## Certainty

**60% - Medium certainty**

**Justification:**
- **High certainty (implementation):** Exponential decay is trivial math; integration point is clear (controller update loop)
- **Medium certainty (feel):** Transition time requires tuning iteration. May need spring-damper instead of lerp for better overshoot control.
- **Low risk (architecture):** No new systems; removes hard clamp rather than adding complexity. Worst case: feels bad, revert to instant lock (8 lines restored).

**Dependencies:**
- ✅ Controller physics stable (95% certain per dependency stack)
- ✅ Animation tuning UI pattern proven (reactive animation systems)
- ✅ Spring-damper foundation exists (100% certain, currently unused)

**Novelty:** Low - exponential smoothing is industry-standard for target tracking

**Risk:** Tuning time only. No architectural unknowns.
