# Implementation Plan: Freeze Velocity Trail on Stop

### Graybox Implementation Checklist

#### 1. Foundation Architecture Review

- [x] Systems involved identified
  - Sampling site: `game_world::update()` trail sampling block
  - Data: `velocity_trail_state` (positions, timestamps, interval, accumulator)
  - Render: `debug::draw_velocity_trail()`
  - UI: `gui::character_panel_state` / `draw_character_panel()` (no trail toggle yet)
- [x] Ownership audit
  - `game_world.trail_state` is the sole instance; updated in `game_world::update`, rendered via `app_runtime::render_world()` → `debug_draw` call
  - No duplicates; no migration needed
  - GUI currently has no toggle; to be added in `character_panel_state`

#### 2. Data Structures & State

- [x] File: `src/gui/character_panel.h`
  - [x] In `gui::character_panel_state`, add `bool show_velocity_trail = true;`

Files touched in this step: `src/gui/character_panel.h`

#### 3. Core Logic (Sampling Gate)

- [x] File: `src/app/game_world.cpp`
  - [x] In `game_world::update()`, in the velocity trail sampling block:
    - [x] After the interval check, compute distance from `character.position` to `trail_state.positions.back()` when not empty
    - [x] Only append a new sample when `positions.empty()` OR `glm::distance(current, last) > 1e-4f`
    - [x] Keep existing capacity cap (25), timestamp progression, and reset `time_since_last_sample` only when a sample is appended

Pseudocode:
```
trail_state.time_since_last_sample += dt
if time_since_last_sample >= sample_interval:
    can_sample = positions.empty() or distance(character.position, positions.back()) > 1e-4
    if can_sample:
        if positions.size() >= 25: pop_front oldest (positions, timestamps)
        positions.push_back(character.position)
        timestamps.push_back(timestamps.empty() ? 0 : timestamps.back() + sample_interval)
        time_since_last_sample = 0
```

Files touched in this step: `src/app/game_world.cpp`

#### 4. Debug UI (Toggle)

- [x] File: `src/gui/character_panel.cpp`
  - [x] Add a checkbox to control `state.show_velocity_trail` (e.g., under Locomotion or a small "Debug Visuals" subsection)
  - [x] Label: "Show Velocity Trail"

Files touched in this step: `src/gui/character_panel.cpp`

#### 5. Rendering Hook

- [x] File: `src/app/runtime.cpp`
  - [x] In `app_runtime::render_world()`, wrap `debug::draw_velocity_trail(debug_ctx, world.trail_state);` with `if (panel_state.show_velocity_trail) { ... }`

Files touched in this step: `src/app/runtime.cpp`

#### 6. Quality Gates

- [x] Build passes (Debug)
- [x] Format/tidy clean (existing scripts)
- [x] Smoke run: trail remains static while idle; resumes on movement; toggle hides/shows as expected

## Approval

- Reviewer: Codex (2025-10-11) — See `PLANS/freeze_velocity_trail_on_stop_CODE_REVIEW.md`

---

Changelog:
- 2025-10-12: Initial plan created from CLARIFICATIONS (Resolved) for freeze-on-stop + GUI toggle
