**Freeze Velocity Trail on Stop**

- Core Mechanic: Pause velocity-trail sampling while character speed is below a small threshold; resume sampling immediately when speed rises above it. Keeps the trail as a “motion breadcrumb” rather than accumulating points while idle.
- Pattern Reuse: Extends the proven debug visualization system. Reactive layer reads motion source (velocity magnitude) and interprets it; no changes to core physics or controller logic.
- Graybox Scope:
  - Minimum: Only append a new trail sample when the current position differs from the last recorded position (use tiny float tolerance, e.g., glm::distance < 1e-4 treated as equal).
  - Controls: Add a GUI checkbox to show/hide the velocity trail. Freezing is always enabled (no toggle).
  - Estimated lines: ~10–20 (sampling gate + GUI toggle + render conditional).
- Minimal Implementation:
  - Sampling gate: At the sampling site (src/app/game_world.cpp:29), after interval elapses, only push a new sample if `positions.empty()` or `glm::distance(character.position, positions.back()) > 1e-4f`.
  - GUI: Add `bool show_velocity_trail = true;` to `gui::character_panel_state` (src/gui/character_panel.h) and a checkbox under an appropriate section in `draw_character_panel` (src/gui/character_panel.cpp).
  - Rendering: In `app_runtime::render_world()` (src/app/runtime.cpp), wrap `debug::draw_velocity_trail(...)` with `if (panel_state.show_velocity_trail) { ... }`.
- Problem Evidence: During routine playtesting on 2025‑10‑11, the velocity trail continued updating while stationary, cluttering the character with spheres and obscuring recent path. The trail should represent “where the character has been while moving,” not “time passing at one spot.”
- Unlocks:
  - Immediate: Clearer motion analysis during tuning sessions; easier to read stop/start behavior and pathing.
  - Future: Cleaner baseline for any time‑based trail effects (e.g., decay, freeze/unfreeze visuals) without fighting idle noise.
- Test Loop:
  - Steps: Run, stop, wait 2–3 seconds, then run again. Verify no new spheres appear while idle; verify new samples appear immediately on movement; verify slow intentional walking still produces samples (any jitter still advances because position differs).
  - Iteration time: Seconds.
- Certainty: 95% base (trivial debug‑only change) + 25% debug‑tool boost on stable infrastructure → 100% (cap). Risk confined to visualization; revert cost is negligible.

## CLARIFICATIONS (Resolved)

- Visual/Behavioral
  - Idle behavior: Strictly static; no time-based fade while stopped (renderer already fades by index only).
  - Stop sample: Last periodic sample is sufficient; no extra "pin" sample needed.

- Mechanical
  - Gating rule: Freeze sampling when the next sample position equals the previous sample position. Implement as "no new sample if glm::distance(current_position, last_position) <= 1e-4f". No velocity math, no thresholds, no hysteresis.
  - Sampling accumulator: Acceptable that idle time does not "catch up" samples; sampling resumes normally on movement.

- Scope/Controls
  - GUI: Add a toggle to show/hide the velocity trail entirely. Freezing is always enabled (no toggle).

- Success Criteria
  - No samples added while stationary; new samples appear immediately on movement; slow walking still produces samples; visual remains static while idle.

## Completion

**Date Completed:** 2025-10-12

**Final Certainty Score:** 100% (trivial debug feature, completed on first attempt with zero rework)

**Code Review:** See `PLANS/freeze_velocity_trail_on_stop_CODE_REVIEW.md`

**Outcome Summary:** Position-delta gating successfully prevents trail sampling while idle. Trail freezes perfectly on stop and resumes instantly on movement. GUI toggle allows show/hide control. Implementation was simpler than velocity-threshold approach and has no edge cases.

## Reflection

### What went well?
- **Clarifications process caught complexity early:** Initial velocity-threshold approach would have introduced hysteresis edge cases. Clarifications dialogue redirected to simpler position-delta gating (geometric truth > threshold tuning).
- **Minimal scope discipline:** Resisted adding unnecessary features (freeze toggle, decay timers). Kept implementation to 3 small edits across 3 files.
- **Pattern reuse:** GUI toggle followed established debug panel conventions; sampling gate fit naturally into existing trail update logic.

### What caused friction?
- **Initial velocity-threshold assumption:** Started planning with velocity magnitude checks before questioning if that was the simplest solution. Position-delta is architecturally simpler (no state, no tuning).
- **Clarifications latency:** Required explicit back-and-forth to settle on position-delta approach. Could have been caught in feature planning if "simplest gate mechanism" was explicitly explored.

### What would you do differently?
- **Gate mechanism exploration in planning:** When adding conditional logic, explicitly compare approaches (velocity-threshold vs position-delta vs time-delta) before committing to one. Ask: "What's the simplest check that satisfies the requirement?"
- **Question velocity assumptions:** For debug visualizations responding to motion, position-delta (geometric) may be simpler than velocity-threshold (dynamic). Check both before planning.
- **Defer toggle complexity:** Almost added a "freeze on/off toggle" before realizing it was unnecessary. Default to simplest controls; wait for user request before adding options.

## Certainty Calibration

**Predicted Certainty:** 95% base + 25% debug-tool boost → 100% (capped) — from feature plan Section 3

**Actual Outcome:**
- [x] Completed on first attempt (no deferrals, no major rework)
- [ ] Required feature/rework (specify what changed)
- [ ] Deferred (specify reason and missing prerequisites)

**Variance Analysis:**
- Prediction was accurate: trivial debug-only feature with zero architectural changes completed smoothly.
- Clarifications process caught the velocity-threshold complexity early, redirecting to simpler position-delta approach before implementation began.
- No surprises during implementation; all 3 files edited as planned with no unexpected dependencies.

**Calibration Adjustment:**
- **Debug visualization pattern validated:** Features that add conditional logic to existing debug systems without touching core gameplay deserve high certainty (90-100%) when:
  - Infrastructure already exists (trail sampling, debug draw, GUI panels)
  - Changes are purely reactive (interpret state, don't modify it)
  - Scope is minimal (single conditional check, simple toggle)
- **Position-delta > velocity-threshold for debug gates:** When freezing/gating debug visualizations based on "motion," geometric checks (position delta) are architecturally simpler than dynamic checks (velocity threshold). No state, no hysteresis, no tuning. Prefer position-delta for future similar features.
- **Certainty boost justified:** Debug-tool boost (+25%) was appropriate here. Reactive debug features on stable infrastructure have lower risk than gameplay-affecting features at same base certainty.