# Principle Review: Freeze Velocity Trail on Stop

Changelog:
- 2025-10-12: Doc-only updates — backlog entry aligned to position-delta gating; renderer fade verification added; no code changes (review-only).

**Date:** 2025-10-12
**Status:** APPROVED

### Summary

Plan aligns with principles: simple, reactive, and non-invasive. Sampling is gated by position change, keeping this a graybox-level debug improvement without touching core physics. Scope is minimal and atomic; ownership is clear (single trail state, updated in `game_world`, rendered via debug). Documentation drift resolved in backlog (now matches position-delta gating). Renderer fade verified as index-based. Recommendation: proceed.

### Violations (Must Fix)

No violations found.

### Misalignments & Risks

- **Backlog mismatch:** Resolved. Backlog now reflects position-delta gating.

- **UI state availability:** `app_runtime::render_world()` must have access to `panel_state.show_velocity_trail` to guard `debug::draw_velocity_trail(...)`.
  - **Mitigation:** Verify existing wiring; if absent, pass `panel_state` (or the specific flag) into render path or move the conditional to a layer where it is available.

- **Timestamp semantics:** Idle time does not "catch up" timestamps; renderer fades by index, not time (verified).
  - **Verification:** `draw_velocity_trail` computes age by position index, no timestamp usage (src/rendering/debug_draw.cpp:314).

- **Tolerance choice:** `1e-4f` is extremely small; floating jitter could still produce samples in some setups or, conversely, very slow motion might be skipped depending on sample interval.
  - **Mitigation:** Keep as a constant for now; expose as a tunable in `velocity_trail_state` if field data shows issues.

### Actionable Items

- [x] Update `PLANS/DESIGN_BACKLOG.md` to match position-delta gating (done 2025-10-12).
- [ ] Ensure `panel_state.show_velocity_trail` is accessible where `debug::draw_velocity_trail` is called; adjust wiring if needed.
- [x] Sanity-check trail fade behavior is index-based (verified at src/rendering/debug_draw.cpp:314).
- [ ] Optional: Define `const float trail_min_step = 1e-4f;` near trail state for quick tuning.

### Recommendation

**Reasoning:** Minimal, debug-only change; reactive layer interprets motion without altering core logic; clear ownership; small, reversible scope.

**Next Steps:**
- If APPROVED: Implement per `PLANS/ARCHIVE/20251011_235818_freeze_velocity_trail_on_stop_PLAN.md`.
