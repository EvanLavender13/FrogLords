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
- Add `target_max_speed` float to controller state
- Convert existing `max_speed` member from static tuning parameter to dynamic smoothed value
- Add smooth transition logic in `controller::update()` (exponential decay or spring-damper)
- Add tuning parameter to animation_tuning UI (1-2 lines)

## Minimal Implementation

**Where it hooks in:**
- `src/character/controller.cpp`: Replace hard clamp with smooth `max_speed` modulation
- Update order: Before velocity application, after input processing
- Files affected: `controller.cpp`, `controller.h`, potentially `gui/character_panel.cpp` for tuning UI

**Implementation approach (Option C from backlog):**
1. Add `float target_max_speed` to `controller` state (new member variable)
2. Set `target_max_speed = shift_held ? walk_speed : run_speed` each frame
3. Smooth existing `max_speed` member toward target using exponential decay: `max_speed = lerp(max_speed, target_max_speed, transition_rate * dt)`
4. Existing velocity cap logic uses smoothed `max_speed` value (no changes to cap application)
5. Expose `transition_rate` in tuning UI

**Key insight:** Reuse existing `max_speed` member as the smoothed value rather than adding separate `current_max_speed`. This makes `max_speed` dynamic (updated each frame) instead of static, preserving all existing references while enabling smooth transitions.

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

---

## Completion

**Date Completed:** 2025-10-11

**Final Certainty Score:** 100% ✅

**Code Review:** [code_review_smoothed_walk_transition.md](code_review_smoothed_walk_transition.md)

**Outcome:** Feature implemented successfully using exponential decay (Option C from backlog). Smooth transitions between walk/run speeds preserve physics-first feel. Critical bug discovered and fixed during implementation: circular dependency in speed state management required separating `run_speed` (immutable reference) from `max_speed` (smoothed state). Debug UI displays enable real-time tuning visualization.

---

## Reflection

### What went well?

- **Architectural audit prevented duplicate systems:** Pre-implementation review of `controller` ownership and all references (gui, game_world, debug_draw, tuning, animation) correctly identified clean single ownership, avoiding migration complexity that plagued earlier features
- **Bug discovery process was systematic:** Circular dependency caught during functional testing, analyzed in implementation notes with clear before/after code examples, and resolved with minimal rework
- **Exponential decay over spring-damper was correct choice:** Simpler implementation (one parameter vs. two), no overshoot risk, sufficient for speed transitions; validated the "simplicity over sophistication" principle

### What caused friction?

- **Initial variable naming confusion:** Feature doc suggested adding `current_max_speed` and `target_max_speed`, but implementation correctly reused existing `max_speed` member as smoothed state; naming inconsistency between documents required clarification
- **Circular dependency not predicted in planning:** Bug only revealed during runtime testing (releasing SHIFT didn't transition back to run); could have been caught with static analysis of state flow during design phase
- **PowerShell script issue with backticks:** Commit script failed on first attempt due to backticks in commit message being interpreted as escape characters; minor tooling friction

### What would you do differently?

- **Add state flow diagram for smoothed variables:** When planning features that modify reference values (like `max_speed`), explicitly diagram which variables are immutable references vs. derived state; would have caught circular dependency before implementation
- **Validate commit message format earlier:** Test commit script with placeholder message during planning to catch parsing issues before final commit
- **Consider "dual reference" pattern upfront:** Any smoothed variable needs both a target reference (immutable) and current state (smoothed); recognizing this pattern would have prevented the `run_speed` bug

---

## Certainty Calibration

**Predicted Certainty:** 60% (from feature plan Section 3)

**Actual Outcome:**
- [x] Completed on first attempt (no deferrals, no major rework)
- [ ] Required feature/rework (specify what changed)
- [ ] Deferred (specify reason and missing prerequisites)

**Variance Analysis:**

Predicted certainty was **40% too low**. Feature completed smoothly despite medium certainty estimate. The implementation approach (exponential decay, max_speed modulation) was architecturally sound and required only a single bug fix (circular dependency) that was resolved within the same session. The "medium certainty (feel)" concern proved unfounded—default transition rate of 10.0 felt natural immediately; no extended tuning iteration required.

**Why was prediction pessimistic?**
- Overestimated tuning risk: Assumed multiple iteration cycles; actual default value worked well
- Undervalued pattern reuse: Exponential smoothing is trivial; treating it as "novel" inflated uncertainty
- Clean system ownership de-risked implementation: Pre-implementation audit confirmed no hidden dependencies

**Calibration Adjustment:**

For similar features in the future:
- **Pattern reuse (exponential decay, lerp toward target) justifies +20% certainty boost:** Industry-standard smoothing techniques with single-owner state require minimal iteration
- **Architectural audit completion justifies +10% certainty boost:** Confirmed single ownership and no migration needs reduces implementation risk significantly
- **Recalibrated certainty for this feature:** 90% (architecture 95%, feel 85%) vs. original 60%
- **Signal for higher confidence:** When state modification is localized (single struct), transformation is simple (one lerp per frame), and debugging UI exists (real-time visualization), implementation risk is low even if feel tuning is uncertain

**Lesson:** Distinguish "requires tuning iteration" from "uncertain outcome." Tuning with immediate feedback (GUI sliders + real-time display) is low-risk iteration, not architectural uncertainty. Reserve <70% certainty for novel patterns or multi-system integration.
