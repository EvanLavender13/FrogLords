# Iteration Plan: Debug Visual Overhaul

### 1. Feature Overview

*   **Description:** Add 3 focused debug visualizations: velocity decay trail (3D world-space breadcrumbs), reusable plot lines function (2D GUI temporal graphs), and speed gradient ring (fixes buggy speed circle with meaningful spatial/temporal design).
*   **Core Principle Alignment:** Purely additive debug layer (zero gameplay impact). Builds reusable primitives (trail system, plot utility, gradient rings) for future features. Graybox-first approach—white spheres, simple graphs, no premature polish.

### 2. Design Rationale

*   **Problem/Goal:** Current debug visuals are flat and uninspired. Speed circle has overshoot artifact. Temporal patterns (FPS drops, speed fluctuations) invisible in instant readouts. No visualization of momentum/turning radius.
*   **Hypothesis:** Velocity trail makes turning behavior instantly readable. FPS plot catches frame drops invisible in text. Speed gradient ring fixes bug while tying spatial (ring size) and temporal (1 second travel distance) meaning together.

### 3. Dependencies & Uncertainty

*   **Placement in Stack:** Bottom layer—pure debug visualization with zero dependencies on uncertain systems. Does not block or depend on any gameplay features.
*   **Certainty Score:** 60%—scoped to 3 concrete visualizations with clear specs. Low technical risk (leverages existing debug_draw and ImGui infrastructure). Main uncertainty: tuning visual parameters (fade rates, colors, graph scales) for readability.

### 4. Implementation & Testing Loop

*   **Graybox Scope (The "Stupid Simple Core"):**
    *   **Velocity Decay Trail:**
        *   Sample character position every 1 second (time-based sampling)
        *   Maintain ring buffer of last 10 positions
        *   Render white spheres at each position, shrinking and fading (oldest = smallest/most transparent)
        *   Use existing `unit_sphere_4` or `unit_sphere_6` mesh from `debug_draw_context`
        *   Parameters to expose: sphere base size, fade curve, maybe toggle on/off
    *   **Plot Lines Function:**
        *   Reusable wrapper around `ImGui::PlotLines()`
        *   Function signature: `plot_value(const char* label, float current_value, float time_window = 5.0f)`
        *   Maintains per-label ring buffer of (timestamp, value) pairs
        *   **First use case:** Plot FPS in Debug Panel, below existing FPS text field
        *   Stored in static/global map keyed by label string
        *   Parameters: time window (default 5 seconds), min/max value range (auto or manual)
    *   **Speed Gradient Ring:**
        *   Replace existing speed circle visualization in `draw_character_state()`
        *   Ring radius = max speed (represents distance traveled in 1 second)
        *   Color gradient: blue (0% speed) → green (33%) → yellow (66%) → red (100% speed)
        *   Use existing circle mesh, render with interpolated color based on current speed / max speed ratio
        *   Parameters: maybe expose color stops, gradient curve
*   **Testing Protocol:**
    *   **Self-test first:** Build → run → observe each visual in isolation
    *   **Velocity trail test:** Run in circles, figure-8 patterns—does trail clearly show turning radius? Is 10-sample buffer enough or too cluttered?
    *   **Plot lines test:** Watch FPS graph during gameplay—does it reveal frame drops? Are 5 seconds enough to spot patterns?
    *   **Speed gradient ring test:** Walk → run transitions—is color shift readable? Does ring size feel meaningful (1 second travel distance)?
    *   Iteration time: 5-10 minutes per visual (tweak parameters, rebuild, observe)
*   **Success Metrics (Definition of "Proven"):**
    *   All 3 visualizations rendering correctly and toggleable
    *   Velocity trail clearly shows momentum/turning at a glance (no confusion)
    *   FPS plot reveals temporal patterns invisible in instant readout (catches drops)
    *   Speed gradient ring fixes overshoot bug and spatial/temporal meaning is intuitive
    *   Plot function documented and ready for reuse (speed, blend factors, etc.)
*   **Iteration Cadence & Horizon:**
    *   **Estimated complexity:** 3 points (1 per visualization, each simple and isolated)
    *   **Horizon:** Complete all 3 in single iteration (80-120 lines total, no dependencies)
    *   Criteria for extension: if velocity trail proves valuable, consider generalizing for trajectory prediction

### 5. System Integration

*   **System Ownership:**
    *   `debug_draw` lives in `src/rendering/debug_draw.cpp`, called from `runtime.cpp`—no conflicts
    *   `gui` wrapper in `src/gui/gui.cpp`—will add plot function here
    *   Character position sampling: add state to `controller` or `game_world` for trail buffer
*   **Mechanic Interactions:** Zero gameplay interaction—purely debug visualization layer. Future reuse: velocity trail for dash paths, plot function for any temporal debugging, gradient rings for cooldown indicators.
*   **Interface & Feedback:**
    *   Velocity trail: white spheres are simple, proven visual language (already used elsewhere)
    *   FPS plot: labeled graph in Debug Panel, clear temporal axis
    *   Speed gradient ring: color gradient is intuitive (blue = slow, red = fast), ring size ties space + time

### 6. Risks & Future Work

*   **Potential Risks:**
    *   Velocity trail sampling every 1 second might miss fast direction changes (can adjust sampling rate if needed)
    *   Plot function stored in static map—might need cleanup if labels change frequently (low risk for debug use)
    *   Speed gradient ring color scheme might not be readable on all backgrounds (can adjust saturation/brightness)
*   **Content Restrictions:** None—purely debug visualizations with zero impact on gameplay or level design.
*   **Decision Impacts (Checklist):**
    *   Implementation cost: Low (80-120 lines, isolated changes)
    *   Immaturity burden: None (no other systems depend on debug visuals)
    *   Critical failure risk: None (debug-only, can toggle off)
    *   Process burden: None (no coordination needed)
    *   Political/cultural effects: Neutral
    *   Decision cost: Low (well-scoped, clear specs)
*   **Backlog Candidates:**
    *   Generalize velocity trail for trajectory prediction (dash, jump arcs)
    *   Plot additional values (speed, blend factors, input magnitude)
    *   Gradient rings for gameplay feedback (cooldowns, ranges, zones)
    *   Ground contact ripple visualization (landing feedback)
    *   Phase wheel for locomotion cycle visualization

---

## Approval

**Status:** Approved
**Reviewer:** GitHub Copilot (AI Code Review Agent)
**Date:** October 10, 2025

**Summary:** Iteration complete and successful. All three debug visualizations implemented and tested. Code adheres to development principles with no violations. Feature evolved appropriately through iteration (velocity trail sample rate tuning, speed ring dynamic expansion, plot function enhancements). Zero gameplay impact maintained throughout. Ready for archival and merge.

**Review Document:** See `PLANS/code_review_debug_visual_overhaul.md` for detailed code analysis.
