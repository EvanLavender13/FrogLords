# Animation Cycle Stride Consolidation

**Core Mechanic:** Unify animation cycle length with locomotion's blended stride length, completing the GDC "surveyor wheel" principle. Animation phase advances based on locomotion's actual stride size, creating proper stride/cadence coupling where walk (1.2m stride) yields quicker leg turnover and run (2.0m stride) yields longer, powerful strides.

**Pattern Reuse:** Extends proven surveyor wheel pattern from locomotion (distance-phased bounce/gait) to animation system. Locomotion computes `blended_stride` from walk (1.2m) and run (2.0m) based on speed—animation will consume this as cycle length. Dual-reference spring pattern for smooth stride transitions.

**Problem Evidence:** During run gait blending (2025-10-13), animation phase progression observed identical for walk and run despite different stride concepts. Current: locomotion uses variable surveyor wheel (`walk_stride = 1.2m`, `run_stride = 2.0m`) but animation divides `distance_traveled` by fixed `cycle_length = 2.0f`. Violates GDC principle: "blend between their stride size... blended 'surveyor wheel' size." Missing cadence variation—walk needs quicker steps, run needs powerful strides.

**Graybox Scope:**
- Pass `blended_stride` from locomotion to animation system (10-15 lines)
- Replace fixed `cycle_length` with parameter in `update_skeletal_animation()`
- Remove debug UI slider for cycle length (consolidated with locomotion strides)
- Expected visual: cadence frequency scales as `speed / blended_stride`
  - Walk @ 1.5 m/s with 1.2m stride → ~1.25 Hz leg turnover (small, quicker steps)
  - Run @ 5.0 m/s with 2.0m stride → ~2.5 Hz leg turnover (long, powerful strides)
- Key parameters: None new (uses existing `walk_stride`, `run_stride` from locomotion)
- Estimated line count: ~10-20 lines

**Minimal Implementation:**
- Hook: `game_world.cpp` update loop, after locomotion update, before animation update
- Files affected:
  - `src/character/animation.h`: Add `cycle_length` parameter to `update_skeletal_animation()`
  - `src/character/animation.cpp`: Replace hardcoded `2.0f` with parameter
  - `src/character/locomotion.h`: Add `get_blended_stride()` accessor (or make member public)
  - `src/app/game_world.cpp`: Fetch blended stride and pass to animation
- Implementation approach:
  ```cpp
  // In game_world.cpp:
  float blended_stride = character.locomotion.get_blended_stride();
  character.animation.update_skeletal_animation(
      t_pose_skeleton,
      character.locomotion.distance_traveled,
      blended_stride,  // NEW: pass as cycle length (surveyor wheel size)
      applied_walk_factor,
      panel_state.selected_pose,
      panel_state.use_manual_pose_selection,
      dt
  );
  ```

**Unlocks:**
- **Immediate:** Completes surveyor wheel principle (single source of truth for stride); validates GDC animation philosophy
- **Future:** Unblocks bounce gravity footfall timing (needs blended stride for phase sync); sets pattern for phase-synchronized systems (footstep audio, particles)

**Test Loop:**
- Visual verification: Walk→run transitions show leg turnover frequency increasing with speed
- Debug check: Verify stride distance and cadence feel naturally coupled
- Edge case: No animation pops during blend transitions; phase continuity maintained
- Self-test in minutes: tune walk_stride/run_stride in locomotion UI, immediately see cadence changes
- Expected iteration: 1-2 tuning passes to validate stride lengths feel correct with new cadence

**Certainty:** ~60% - straightforward consolidation of proven systems (locomotion 95%, animation 95%). Risk: tuning time if current animation feel depends on fixed 2.0m cycle; may require retuning stride lengths. Low implementation risk (single parameter pass), medium validation risk (subjective feel). Could expose phase discontinuity if stride blending doesn't match pose blending smoothly.

**Deferred Until Now:** Run gait blending needed completion and playtesting. Required data on whether fixed cycle length feels wrong. Run gait integration (2025-10-13) confirmed pose extremity differences insufficient—missing cadence variation apparent.

**When to Revisit (if deferred again):** If stride/cadence coupling feels correct as-is during extended playtesting, may not be necessary. Monitor for player feedback about walk/run distinction clarity.

## Completion

**Date Completed:** 2025-10-14

**Final Certainty Score:** 100% ✅

**Code Review:** [PLANS/ARCHIVE/20251014_010421_animation_cycle_stride_consolidation_PLAN.md](animation_cycle_stride_consolidation_PLAN.md) - Approval section

**Outcome:** Successfully unified surveyor wheel principle across locomotion and animation. Animation now reuses locomotion's pre-calculated phase instead of recalculating from distance/stride, eliminating phase discontinuities during stride transitions. Bonus: Dynamic surveyor wheel debug visual provides accurate stride feedback.

## Reflection

**What went well?**
- Architecture audit in PLAN caught that `blended_stride` was a local variable, not exposed—prevented implementation confusion
- Testing revealed vibration issue immediately, leading to rapid iteration on the fix
- Pattern recognition: Recalculation instability similar to previous issues → tried phase reuse, which solved it elegantly
- The final solution (pass phase directly) was cleaner than the original plan (pass distance + stride)

**What caused friction?**
- Initial implementation passed `blended_stride` as parameter but still recalculated phase in animation, causing vibration
- Attempted dual-reference smoothing fix addressed symptoms, not root cause—wasted iteration
- Didn't initially recognize that locomotion already computed phase correctly; focused on stride instead of phase

**What would you do differently?**
- During planning, explicitly check if dependent systems already provide computed values (phase) vs primitives (distance/stride)
- When testing reveals instability, analyze the mathematical operation (division by changing value) before adding complexity (smoothing)
- Consider: "What does the upstream system already give us correctly?" before deciding what to pass as parameters

## Certainty Calibration

**Predicted Certainty:** 60% (from feature description)

**Actual Outcome:**
- [x] Completed on first attempt (no deferrals, no major rework)
- [x] Required refinement (vibration fix during testing, but same session)
- [ ] Deferred (N/A)

**Variance Analysis:**
- Predicted 60% due to "may require retuning stride lengths" and "could expose phase discontinuity" risks
- Feature completed smoothly, but did expose phase discontinuity (as predicted)
- The discontinuity was fixable within the same session via better architecture (phase reuse)
- Implementation risk was actually lower than predicted—the fix improved the design

**Calibration Adjustment:**
- For similar "consolidation of computed values" features: Check if upstream system already provides the target value correctly
- Pattern consolidation on proven systems (90%+ foundation) should get +10-20% certainty boost
- When risks are "may expose X," if X is debuggable/fixable (like phase discontinuity), it shouldn't reduce certainty as much
- This feature could have been 70-75% certainty: straightforward consolidation on stable foundation with debuggable risks
