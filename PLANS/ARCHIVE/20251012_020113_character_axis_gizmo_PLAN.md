# Implementation Plan: Character Axis Gizmo

### Foundation Architecture Review (Audit)

Systems involved: debug draw, skeleton, runtime render loop, character debug UI, wireframe generation, ImGui overlay labels.

Current ownership/state
- skeleton: `game_world.t_pose_skeleton` (updated each frame in `game_world::update`, rendered from `app_runtime::render_world()`)
- debug draw: functions in `src/rendering/debug_draw.{h,cpp}`; called from `app_runtime::render_world()`
- GUI state: `gui::character_panel_state` in `src/gui/character_panel.{h,cpp}`; toggles like `show_skeleton`, `show_joint_labels`, `show_velocity_trail`
- renderer: `wireframe_renderer` draws depth-tested lines via a single pipeline; no "always-on-top" path exposed
- wireframe primitives: `generate_arrow()` available in `src/rendering/wireframe.cpp`

Conflicts/duplicates: None. Axis gizmo is a new debug draw utility that references existing `t_pose_skeleton` and renderer. Optional always-on-top would require extending `wireframe_renderer` with a no-depth pipeline; not trivial for v1 -- defer.

---

### Graybox Implementation Checklist

#### 1) Debug Draw API

- [x] File: `src/rendering/debug_draw.h`
  - [x] Declare `void draw_axis_gizmo(draw_context& ctx, const character::skeleton& skel, float axis_length = 0.6f);`

- [x] File: `src/rendering/debug_draw.cpp`
  - [x] Implement `draw_axis_gizmo(...)`:
    - [x] Extract root joint world transform: `skel.joints[0].model_transform`
    - [x] Root position = translation component; basis = normalized column vectors (X, Y, Z)
    - [x] Build three arrows using `generate_arrow(start, end, head_size)`
      - [x] `start = root_pos`, `end = root_pos + axis_dir * axis_length`
      - [x] `head_size` reasonable default (~0.08 m)
      - [x] Draw with colors: X=red, Y=green, Z=blue via `ctx.renderer.draw(...)`
    - [x] Draw labels always on using ImGui foreground overlay (reuse projection approach from `draw_skeleton`)
      - [x] Project label points (slightly beyond arrow tips) to screen and `AddText` "X", "Y", "Z" with matching colors (font scaled for readability)
    - [x] Depth: render via normal renderer path (depth-tested). Do not add special pipeline in v1.
  - [x] Increase label font size ~1.4x for clarity at common camera distances.

#### 2) Character Panel Toggle

- [x] File: `src/gui/character_panel.h`
  - [x] Add `bool show_axis_gizmo = false;` to `character_panel_state`.

- [x] File: `src/gui/character_panel.cpp`
  - [x] In the Skeleton section, add a checkbox: `Show Axis Gizmo` bound to `state.show_axis_gizmo` (default OFF). No extra parameters.

#### 3) Render Hook

- [x] File: `src/app/runtime.cpp`
  - [x] After existing skeleton draw, call: `if (panel_state.show_axis_gizmo) debug::draw_axis_gizmo(debug_ctx, world.t_pose_skeleton, 0.6f);`

#### 4) Optional (Defer unless trivial)

- [ ] File: `src/rendering/renderer.{h,cpp}`
  - [ ] Add an alternate no-depth pipeline and an overload `draw_no_depth(...)` for always-on-top gizmos.
  - [ ] Wire `draw_axis_gizmo` to use `draw_no_depth` if available; otherwise fall back to normal `draw`.
  - Note: Deferred -- requires renderer pipeline changes beyond v1 scope.

#### 5) Quality Gates

- [x] Build (Debug) succeeds.
- [x] Format/lint passes (`scripts/run_clang_format.ps1`, optional tidy).
- [x] Smoke run: app launches; in Debug Panel + Skeleton section, enable `Show Axis Gizmo`; arrows + labels render at root and track full root orientation; colors/labels unambiguous.

---

### Notes & Pseudocode

Label projection (reuse pattern from `draw_skeleton`):
```
view_proj = cam.projection(aspect) * cam.view()
for each axis (dir, text, color):
  world = root_pos + dir * (axis_length + head_size * 0.2)
  clip = view_proj * vec4(world, 1)
  if clip.w > 0:
    ndc = clip.xyz / clip.w
    if 0 <= ndc.z <= 1:
      screen = ((ndc.x+1)/2 * width, (1-ndc.y)/2 * height)
      draw_list->AddText(screen, color, text)
```

Changed files (expected):
- `src/rendering/debug_draw.h`
- `src/rendering/debug_draw.cpp`
- `src/gui/character_panel.h`
- `src/gui/character_panel.cpp`
- `src/app/runtime.cpp`

Completed step files:
- Step 1 (Debug Draw API): `src/rendering/debug_draw.h`, `src/rendering/debug_draw.cpp`
- Step 2 (Character Panel Toggle): `src/gui/character_panel.h`, `src/gui/character_panel.cpp`
- Step 3 (Render Hook): `src/app/runtime.cpp`
- Step 5 (Quality Gates): build, clang-format, manual smoke run

## Approval

**Reviewer:** GitHub Copilot  
**Date:** 2025-10-12  
**Status:** APPROVED

Implementation complete and verified. All checklist items completed successfully. Code review passed with no violations. Feature meets all specified requirements and adheres to FrogLords development principles.

See: `PLANS/ARCHIVE/20251012_020113_character_axis_gizmo_CODE_REVIEW.md` for detailed review findings.
