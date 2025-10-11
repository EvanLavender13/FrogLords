# Implementation Plan: Debug Visual Overhaul

## Feature Summary
Add 3 focused debug visualizations: velocity decay trail (3D world-space breadcrumbs showing momentum/turning), reusable plot lines function (2D GUI temporal graphs), and speed gradient ring (fixes buggy speed circle with meaningful spatial/temporal design).

## Foundation Architecture Audit

**Systems Involved:** debug_draw (3D rendering), gui (2D panels), game_world (state ownership)

**Current State Audit:**
- **debug_draw:**
  - Location: `src/rendering/debug_draw.h`, `src/rendering/debug_draw.cpp`
  - Namespace: `debug::`
  - Current functions: `draw_character_state()`, `draw_physics_springs()`, `draw_locomotion_wheel()`, etc.
  - Called from: `runtime.cpp` in `render_world()` (lines 144-153)
  - Already has speed circle visualization (lines 46-54 in debug_draw.cpp) — **will replace**

- **gui:**
  - Location: `src/gui/gui.h`, `src/gui/gui.cpp`
  - Namespace: `gui::`
  - Current architecture: Simple wrapper around ImGui with panel/widget helpers
  - Character panel: `src/gui/character_panel.cpp` (uses collapsing headers pattern)
  - FPS display: Currently in runtime.cpp (line 103) — **will add plot below this**

- **game_world:**
  - Location: `src/app/game_world.h`
  - Current state: `controller character`, `locomotion_system locomotion`, `camera cam`, etc.
  - Position sampling state: **needs to be added here** (velocity trail buffer)

**Architecture Issues Found:**
- None — clean separation. Debug visuals are isolated and don't affect gameplay.
- Speed circle rendering exists in `draw_character_state()` and needs replacement (not removal, just update in place).

**No Migration Required:**
- All systems are stable and in correct locations
- Purely additive changes (new functions, new state, update one existing visualization)

---

## Graybox Implementation Checklist

### 1. Velocity Decay Trail (3D World Space)

**Goal:** Sample character position every 1 second, render white spheres that shrink and fade (oldest = smallest/most transparent).

#### Data Structures & State

- [x] **File:** `src/app/game_world.h`
  - [x] Add `velocity_trail_state` struct with ring buffer for position samples
    - `std::vector<glm::vec3> positions` (max 10 samples)
    - `std::vector<float> timestamps` (max 10 samples)
    - `float sample_interval = 1.0f` (time between samples)
    - `float time_since_last_sample = 0.0f`
  - [x] Add `velocity_trail_state trail_state;` member to `game_world` struct

**Files Changed:**
- `src/app/game_world.h` — Added velocity_trail_state struct and trail_state member

#### Update Logic

- [x] **File:** `src/app/game_world.cpp`
  - [x] In `game_world::update()`, add velocity trail sampling logic
    - Increment `trail_state.time_since_last_sample` by `dt`
    - When `time_since_last_sample >= sample_interval`, sample current character position
    - Add position to ring buffer (if buffer full, remove oldest sample)
    - Add current timestamp to ring buffer
    - Reset `time_since_last_sample`

**Files Changed:**
- `src/app/game_world.cpp` — Added velocity trail sampling logic in update()

**Pseudocode:**
```
trail_state.time_since_last_sample += dt;
if (trail_state.time_since_last_sample >= trail_state.sample_interval) {
    // Add new sample
    if (trail_state.positions.size() >= 10) {
        // Remove oldest (shift or use circular buffer index)
        trail_state.positions.erase(trail_state.positions.begin());
        trail_state.timestamps.erase(trail_state.timestamps.begin());
    }
    trail_state.positions.push_back(character.position);
    trail_state.timestamps.push_back(current_time);
    trail_state.time_since_last_sample = 0.0f;
}
```

#### Rendering

- [x] **File:** `src/rendering/debug_draw.h`
  - [x] Add function signature: `void draw_velocity_trail(draw_context& ctx, const velocity_trail_state& trail);`
  - [x] Forward declare `velocity_trail_state` struct (or include game_world.h)

- [x] **File:** `src/rendering/debug_draw.cpp`
  - [x] Implement `draw_velocity_trail()` function
    - Iterate through trail positions (oldest to newest)
    - For each position, calculate age factor (0.0 = oldest, 1.0 = newest)
    - Render white sphere at position using `ctx.unit_sphere_4`
    - Scale: lerp from 0.05f (oldest) to 0.15f (newest)
    - Alpha: lerp from 0.2f (oldest) to 0.8f (newest)

**Pseudocode:**
```
for (int i = 0; i < trail.positions.size(); ++i) {
    float age_factor = static_cast<float>(i) / static_cast<float>(trail.positions.size() - 1);
    float scale = lerp(0.05f, 0.15f, age_factor);
    float alpha = lerp(0.2f, 0.8f, age_factor);

    wireframe_mesh sphere = ctx.unit_sphere_4;
    sphere.position = trail.positions[i];
    sphere.scale = glm::vec3(scale);
    ctx.renderer.draw(sphere, ctx.cam, ctx.aspect, glm::vec4(1, 1, 1, alpha));
}
```

- [x] **File:** `src/app/runtime.cpp`
  - [x] In `render_world()`, add call to `debug::draw_velocity_trail(debug_ctx, world.trail_state);` after existing debug draw calls (around line 153)

**Files Changed:**
- `src/rendering/debug_draw.h` — Added draw_velocity_trail() signature and forward declaration
- `src/rendering/debug_draw.cpp` — Implemented draw_velocity_trail() with age-based size/alpha fade
- `src/app/runtime.cpp` — Added draw_velocity_trail() call in render_world()

---

### 2. Plot Lines Function (2D GUI)

**Goal:** Reusable wrapper around ImGui's `PlotLines()` for temporal graphing. First use case: FPS over time.

#### Data Structures & State

- [x] **File:** `src/gui/gui.h`
  - [x] Add function signature to `gui::` namespace: `void plot_value(const char* label, float current_value, float time_window = 5.0f);`

- [x] **File:** `src/gui/gui.cpp`
  - [x] Define internal state struct for plot buffers:
    - `struct plot_buffer { std::vector<float> values; std::vector<float> timestamps; float time_window; }`
  - [x] Create static map to store per-label buffers: `static std::map<std::string, plot_buffer> plot_buffers;`

**Files Changed:**
- `src/gui/gui.h` — Added plot_value() function signature
- `src/gui/gui.cpp` — Added plot_buffer struct and static plot_buffers map

#### Implementation

- [x] **File:** `src/gui/gui.cpp`
  - [x] Implement `plot_value()` function
    - Get or create plot buffer for the given label
    - Add current value and timestamp to buffer
    - Remove samples older than `time_window` seconds (prune old data from front)
    - Call `ImGui::PlotLines()` with buffer data

**Files Changed:**
- `src/gui/gui.cpp` — Implemented plot_value() with buffer management and ImGui::PlotLines rendering

**Pseudocode:**
```
void plot_value(const char* label, float current_value, float time_window) {
    auto& buffer = plot_buffers[label];
    if (buffer.time_window == 0.0f) buffer.time_window = time_window;

    float current_time = ImGui::GetTime();
    buffer.values.push_back(current_value);
    buffer.timestamps.push_back(current_time);

    // Prune old samples
    while (!buffer.timestamps.empty() &&
           current_time - buffer.timestamps[0] > buffer.time_window) {
        buffer.timestamps.erase(buffer.timestamps.begin());
        buffer.values.erase(buffer.values.begin());
    }

    // Render plot
    if (!buffer.values.empty()) {
        ImGui::PlotLines(label, buffer.values.data(), buffer.values.size(),
                         0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0, 60));
    }
}
```

#### Integration

- [x] **File:** `src/app/runtime.cpp`
  - [x] In `frame()`, after FPS text display (line 103), add call to `gui::plot_value("FPS", 1.0f / sapp_frame_duration(), 5.0f);`

**Files Changed:**
- `src/app/runtime.cpp` — Added gui::plot_value() call for FPS graphing after FPS text display

---

### 3. Speed Gradient Ring (3D World Space Fix)

**Goal:** Replace existing speed circle with gradient ring (blue → green → yellow → red based on speed position in range). Ring radius = max speed.

#### Update Existing Visualization

- [x] **File:** `src/rendering/debug_draw.cpp`
  - [x] In `draw_character_state()`, replace current speed circle rendering (lines 46-54)
  - [x] Calculate speed ratio: `float speed_ratio = current_speed / max_speed` (clamp to [0, 1])
  - [x] Calculate gradient color based on speed ratio:
    - 0.0 → blue (0, 0, 1)
    - 0.33 → green (0, 1, 0)
    - 0.66 → yellow (1, 1, 0)
    - 1.0 → red (1, 0, 0)
  - [x] Render circle at radius = max_speed (represents 1 second travel distance)
  - [x] Use gradient color for ring

**Files Changed:**
- `src/rendering/debug_draw.cpp` — Replaced speed circle with gradient ring using character.max_speed

**Pseudocode:**
```
// Replace lines 46-54 with:
glm::vec3 horiz_vel = math::project_to_horizontal(character.velocity);
float current_speed = glm::length(horiz_vel);
float max_speed = character.max_speed;  // Need to pass max_speed or get from character

if (current_speed > 0.05f) {
    float speed_ratio = glm::clamp(current_speed / max_speed, 0.0f, 1.0f);

    // Gradient color: blue → green → yellow → red
    glm::vec4 color;
    if (speed_ratio < 0.33f) {
        // Blue to green
        float t = speed_ratio / 0.33f;
        color = glm::vec4(0, t, 1 - t, 0.8f);
    } else if (speed_ratio < 0.66f) {
        // Green to yellow
        float t = (speed_ratio - 0.33f) / 0.33f;
        color = glm::vec4(t, 1, 0, 0.8f);
    } else {
        // Yellow to red
        float t = (speed_ratio - 0.66f) / 0.34f;
        color = glm::vec4(1, 1 - t, 0, 0.8f);
    }

    wireframe_mesh speed_ring = ctx.unit_circle;
    speed_ring.position = character.position;
    speed_ring.scale = glm::vec3(max_speed, 1.0f, max_speed);  // Ring shows max travel distance
    ctx.renderer.draw(speed_ring, ctx.cam, ctx.aspect, color);
}
```

**Files Changed:**
- `src/rendering/debug_draw.cpp` — Replaced speed circle with gradient ring using character.max_speed

---

### 4. Quality Gates

- [x] Build passes (`CMake: Build (Debug)`)
- [x] Lint/format clean (run clang-format/clang-tidy tasks)
- [x] Smoke run: Launch and verify all 3 visualizations render correctly
  - [x] Velocity trail: Shows fine-grained path at 0.1s intervals (changed from 1.0s)
  - [x] FPS plot/histogram: Both temporal graphs with axis labels appear below FPS text
  - [x] Speed gradient ring: Expands with current speed, color shifts blue → green → yellow → red

---

## Post-Implementation Changes

**Velocity Trail Tuning:**
- Sample interval reduced from 1.0s to 0.1s for higher fidelity path visualization
- File: `src/app/game_world.h` (line 19)

**Speed Gradient Ring Refinement:**
- Removed fixed threshold circles (walk/run) - only dynamic gradient ring remains
- Ring now expands with current_speed (was fixed at max_speed)
- File: `src/rendering/debug_draw.cpp`

**Plot Function Enhancements:**
- Added axis labels: Y-axis min/max, X-axis time window
- Added current value overlay on graph
- Added `plot_histogram()` companion function (same interface, histogram rendering)
- Made `max_samples` an optional parameter (default 500)
- Fixed narrowing conversion warnings (ImGui::GetTime(), sapp_frame_duration())
- Files: `src/gui/gui.h`, `src/gui/gui.cpp`, `src/app/runtime.cpp`

**Final Implementation Stats:**
- Lines added: ~150 (including enhancements)
- Files modified: 7 total
  - `src/app/game_world.h` (velocity trail state)
  - `src/app/game_world.cpp` (sampling logic)
  - `src/rendering/debug_draw.h` (trail function signature)
  - `src/rendering/debug_draw.cpp` (trail rendering, gradient ring)
  - `src/gui/gui.h` (plot functions)
  - `src/gui/gui.cpp` (plot implementation)
  - `src/app/runtime.cpp` (integration calls)

---

## Notes

**Architecture:**
- Velocity trail state lives in `game_world` (state ownership)
- Trail sampling happens in `game_world::update()` (time-based logic)
- Trail rendering happens in `debug_draw` (pure visualization layer)
- Plot functions live in `gui::` namespace (reusable utility)
- Speed gradient ring updates existing visualization (no new state needed)

**Reusability:**
- Velocity trail: Generic "positions over time" visual (future use: dash paths, trajectory predictions)
- Plot functions: Generic temporal graphing with both line and histogram modes (future use: speed, blend factors, any float value)
- Speed gradient ring: Pattern for radial indicators (future use: cooldowns, ranges, zones)

**Deliverables:**
✓ Velocity decay trail: 3D world-space breadcrumb trail showing character path with age-based fade
✓ Reusable plot functions: `plot_value()` and `plot_histogram()` with axis labels and configurable buffers
✓ Speed gradient ring: Dynamic expanding ring with color gradient (blue → red) indicating speed ratio

**Implementation Complete:** All features tested and quality gates passed.

---

## Approval

**Status:** Approved
**Reviewer:** GitHub Copilot (AI Code Review Agent)
**Date:** October 10, 2025

**Summary:** Implementation complete with zero violations. All code adheres to project principles (snake_case naming, dependency flow, graybox-first, systemic reusability). Feature evolved appropriately through iteration—velocity trail tuning (1.0s → 0.1s), speed ring refinement (static → dynamic), and plot enhancements (axis labels, histogram variant) demonstrate healthy iteration loops. Ready for archival and merge.

**Review Document:** See `PLANS/ARCHIVE/20251010_223503_code_review_debug_visual_overhaul.md` for detailed analysis.
