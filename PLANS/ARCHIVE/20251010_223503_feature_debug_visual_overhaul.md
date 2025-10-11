# Debug Visual Overhaul

**Problem Evidence:** Current debug visuals are functional but uninspired—flat circles and spheres that barely communicate the dynamic, physics-driven nature of the character controller. During 2025-10-09 playtest, noticed speed circle overshoot artifact, which revealed opportunity: debug visuals can be **expressive tools** that teach how the system works, not just passive readouts. Visual debugging should feel like watching a living system, not reading a spreadsheet.

**Core Mechanic:** Focused redesign of 3 debug visualizations to make physics/locomotion state **visually readable at a glance**. One new 3D world-space visual, one GUI graphing utility, and one fix for existing speed visualization.

**Pattern Reuse:** Leverages existing wireframe renderer, `debug_draw` infrastructure, and ImGui plotting functions. Proven pattern: spring visualizer already demonstrates expressive debug visuals (compression/color intensity). Extend this philosophy.

**Graybox Scope:**
- **1. Velocity Decay Trail (3D World Space):**
  - Sample character position every 1 second, maintain buffer of last 10 samples
  - Render white spheres at sampled positions that shrink and fade in opacity (oldest = smallest/most transparent)
  - Visualizes momentum and turning radius at a glance
  - **Reusable primitive:** Generic "3D positions over time" visual for future trajectory predictions, dash paths, etc.

- **2. Plot Lines Function (2D GUI):**
  - Reusable wrapper around ImGui's plot lines function
  - Takes value name, current value, time window (5 seconds default)
  - Maintains ring buffer of samples, renders temporal graph in appropriate debug panel
  - **First use case:** Plot FPS over time in debug panel (below current FPS text field)
  - **Future-proof:** Easily plot any value (speed, blend factors, etc.) to catch temporal patterns invisible in instant readouts

- **3. Speed Gradient Ring (3D World Space Fix):**
  - Replaces existing buggy speed circle visualization
  - Ring radius = max speed (visual represents distance traveled in 1 second, tying spatial and temporal meaning)
  - Color gradient blue→green→yellow→red showing current speed position within walk/run range
  - Fixes overshoot artifact noticed in playtest

- Estimated line count: 80-120 lines total

**Minimal Implementation:**
- Files affected:
  - `src/rendering/debug_draw.cpp` (velocity trail rendering, speed gradient ring replacement)
  - `src/rendering/debug_draw.h` (function signatures, trail state struct)
  - `src/gui/debug_panel.cpp` (plot lines wrapper function, FPS graph integration)
  - `src/gui/debug_panel.h` (plot lines helper signature)
  - Character state holder (position sampling state for trail buffer)
- Update order: No changes to game logic; purely additive visualization layer
- Architecture: Modular functions (trail system reusable, plot function generic)

**Unlocks:**
- **Velocity trail primitive:** Reusable for trajectory predictions, dash/dodge paths, projectile motion
- **Plot lines utility:** Foundation for performance monitoring, gameplay telemetry, any temporal debugging
- **Gradient ring system:** Pattern for future radial indicators (cooldowns, ranges, zones)
- Fixes buggy speed visualization with meaningful spatial/temporal design

**Test Loop:**
- **Velocity trail:** Run in circles/figure-8s—does trail clearly show turning radius and momentum?
- **Plot lines:** Observe FPS graph during gameplay—does it reveal frame drops invisible in instant readout?
- **Speed gradient ring:** Walk→run transitions—does color gradient clearly show speed position in range? Ring size meaningful?
- Expected iteration time: 5-10 minutes per visual (implement → build → observe → tweak)

**Certainty:** ~60%—scoped to 3 concrete visualizations. Low risk (isolated to debug visuals, zero gameplay impact), medium learning value (establishes reusable primitives for future features).

**Success Criteria:**
- Ship all 3 visualizations: velocity trail, plot lines utility, speed gradient ring
- Velocity trail clearly shows turning radius/momentum at a glance
- FPS plot reveals temporal patterns invisible in instant readout
- Speed gradient ring fixes overshoot bug and ties spatial/temporal meaning (ring size = distance/second)
- All 3 systems designed for reusability in future features

---

## Completion

**Date Completed:** October 10, 2025

**Final Certainty Score:** 100% (all features validated and proven)

**Code Review:** See `PLANS/ARCHIVE/20251010_223503_code_review_debug_visual_overhaul.md`

**Outcome Summary:** All three debug visualizations successfully implemented with strong systemic reusability. Velocity trail evolved from 1.0s to 0.1s sampling for better fidelity; speed gradient ring changed from static to dynamic expansion for improved visual feedback; plot functions gained axis labels, histogram variant, and configurable buffers through healthy iteration loops. Zero violations of project principles, zero gameplay impact maintained throughout.

---

## Reflection

**What went well:**
- Clear scoping with 3 concrete, non-interlocking visualizations enabled rapid iteration
- Graybox-first approach (white spheres, gradient colors, ImGui defaults) prevented premature polish and maintained focus on functionality
- Systemic thinking yielded high-value reusability: `plot_value()`/`plot_histogram()` are generic primitives ready for any temporal debugging

**What caused friction:**
- Initial 1.0s velocity trail sampling too coarse (discovered through testing, easily adjusted)
- Speed ring static radius less intuitive than dynamic expansion (iteration revealed better design)
- Both issues exemplify healthy iteration: simple first attempt → test → improve

**What would you do differently:**
- Nothing significant. Short planning horizon (3 small features), tight test loops (5-10 min per visual), and willingness to evolve design based on observations worked exactly as intended. Feature complexity estimate (80-120 lines) nearly accurate (~150 actual including enhancements).

---

## Certainty Calibration

**Predicted Certainty:** 60% (from iteration plan Section 3)

**Actual Outcome:**
- [x] Completed on first attempt (no deferrals, no major rework)

**Variance Analysis:**
- Predicted 60%, actual outcome clean completion → underestimated certainty by ~40 points
- Why: All three visualizations built on stable, proven infrastructure (debug_draw, gui/ImGui, game_world state ownership). No novel architecture, no uncertain dependencies.
- Tuning iterations (1.0s→0.1s sampling, static→dynamic ring) were healthy refinements, not architectural rework.
- "Medium uncertainty" from planning likely reflected subjective novelty (first pure debug feature), not technical risk.

**Calibration Adjustment:**
- For similar features in the future (debug visualizations on stable infrastructure), boost certainty estimate by +20-30 points.
- Pattern: "New feature type on proven foundation" ≈ 75-85% certainty if scope is small and non-interlocking.
- Counter-indicator: If debug visual requires new rendering capabilities (e.g., textured sprites, particle systems), maintain lower certainty until renderer extensions proven.

