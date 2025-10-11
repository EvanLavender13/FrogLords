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
