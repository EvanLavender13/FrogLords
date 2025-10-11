**Velocity-Based Acceleration Tilt Scaling**

- **Core Mechanic:** Anchor the character's acceleration tilt to absolute ground speed so body lean communicates real momentum differences between walk and run states.
- **Pattern Reuse:** Extends the proven acceleration-tilt reactive layer (`animation_state::update_acceleration_tilt`) and existing run-speed tuning parameters in `character::controller`.
- **Graybox Scope:**
    - Swap the tilt scale denominator to a fixed reference speed (controller run speed) and guard against zero-length values.
    - Optional scalar parameter on the animation panel for quick tuning if needed (`tilt_reference_speed` exposed alongside current tilt magnitude).
    - Estimated implementation: 8-12 lines (animation.cpp change + call-site update + optional GUI hook).
- **Minimal Implementation:** Update `src/app/game_world.cpp` so `update_acceleration_tilt()` receives `character.run_speed`; adjust `src/character/animation.cpp` to clamp by the reference speed (fall back to 1.0f if the value is near zero). Keep computation in the same update slot: after landing spring, before locomotion sync.
- **Problem Evidence:** During the 2025-10-06 shift-walk playtest, walk speed (~2 m/s) produced identical tilt magnitude as full run (~8 m/s) because the denominator followed the smoothed `max_speed` transition (`PLANS/DESIGN_BACKLOG.md:70`). The visual leaned body suggested high momentum while moving slowly, reducing motion readability.
- **Unlocks:**
    - Immediate: Restores truthful feedback for the core locomotion loop; enables ongoing tilt tuning without misleading visuals.
    - Future: Provides a stable baseline for backlog items like tunable tilt velocity scaling and any animation telemetry that depends on absolute speed bands.
- **Test Loop:**
    - Self-test: Toggle walk/run repeatedly while watching tilt amplitude; confirm walk lean is subtle and ramp increases smoothly with speed changes.
    - Verify sprinting above reference speed clamps cleanly; observe behaviour while stopping to ensure neutral recovery unchanged. Iteration loop under 2 minutes (build + run + quick movement pass).
- **Certainty:** 85% - builds on validated acceleration tilt system and existing speed parameters; risk limited to ensuring reference speed never reaches zero (covered by guard).

---

**Implementation Notes (2025-10-11)**

- Tilt scaling now clamps the denominator with `glm::max(reference_speed, 0.01f)`; total change ~10 lines across `src/character/animation.h/.cpp` and `src/app/game_world.cpp`.
- `game_world::update` passes `character.run_speed`, so walk/run transitions no longer dilute lean magnitude while the smoothed `max_speed` continues driving the debug speed ring.
- Future backlog candidate: add a non-smoothed speed ring that visualizes raw velocity magnitude alongside an acceleration ring (showing horizontal acceleration or tilt input) to compare control intent vs. body response.

## Completion
- Date: 2025-10-11
- Final Certainty: 100%
- Code Review: [PLANS/ARCHIVE/20251011_170652_code_review_velocity_based_acceleration_tilt_scaling.md](code_review_velocity_based_acceleration_tilt_scaling.md)
- Outcome: Absolute run-speed reference removed the misleading walk lean without introducing regressions; tuning UI still behaves as expected.

## Reflection
- **What went well?**
  - Small, surgical change reused existing run-speed parameter and kept implementation within the planned 10-line budget.
  - Fast build-run loop made it easy to verify walk/run lean differences immediately.
- **What caused friction?**
  - Debug speed ring still eases with `max_speed`, creating a visual mismatch until the underlying variables were reviewed.
- **What would you do differently?**
  - Capture debug-visual alignment questions earlier so visualization adjustments can be planned alongside mechanic tweaks.

## Certainty Calibration
- **Predicted Certainty:** 85%
- **Actual Outcome:**
  - [x] Completed on first attempt (no deferrals, no major rework)
  - [ ] Required rework (specify what changed)
  - [ ] Deferred (specify reason and missing prerequisites)
- **Variance Analysis:** Implementation matched the estimate; the high certainty held because pattern reuse (acceleration tilt) and known parameter source (run_speed) eliminated unknowns.
- **Calibration Adjustment:** Future reactive-layer tweaks that simply re-route existing parameters can stay in the 80-90% certainty range; introduce deductions only when new data sources or animation states are required.

