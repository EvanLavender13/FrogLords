**Character Axis Gizmo**

- **Core Mechanic:** Draw RGB world-space axes (X=red, Y=green, Z=blue) at the character’s skeleton root to make parent-space vs. bone-local rotations visually obvious while tuning poses/animation.
- **Pattern Reuse:** Uses existing wireframe rendering and debug draw patterns (`wireframe_renderer`, `generate_arrow`, ImGui foreground text used in `draw_skeleton`). Attaches to `t_pose_skeleton.joints[0]` model transform (already updated each frame).
- **Graybox Scope:**
  - Render three arrows from root in local X/Y/Z using existing arrow primitive; default axis length ~0.6 m; reasonable arrowhead size (e.g., ~0.08 m). Always show XYZ labels at endpoints.
  - Toggle via Character panel checkbox (default: off). No additional UI parameters in v1.
  - No special performance handling in v1; draw each frame when enabled.
  - Estimated line count: ~50–70 total (function + UI + call site).
- **Minimal Implementation:**
  - Add `debug::draw_axis_gizmo(draw_context&, const character::skeleton&, float length)` in `src/rendering/debug_draw.{h,cpp}` using `generate_arrow` and ImGui for labels (labels always on; no toggle).
  - Add `bool show_axis_gizmo` to `gui::character_panel_state` and a checkbox in `src/gui/character_panel.cpp` (under “Skeleton”).
  - Call from `app_runtime::render_world()` after skeleton draw: `if (panel_state.show_axis_gizmo) debug::draw_axis_gizmo(debug_ctx, world.t_pose_skeleton, 0.6f);`
  - Depth handling: attempt always-on-top only if trivial; otherwise render as normal depth-tested wireframe.
  - Files touched: `src/rendering/debug_draw.h`, `src/rendering/debug_draw.cpp`, `src/gui/character_panel.h`, `src/gui/character_panel.cpp`, `src/app/runtime.cpp`.
- **Problem Evidence:** During 2025-10-08 Static Keyframe Preview, repeated confusion about which parent-space axis controlled each joint (shoulder twist vs. swing vs. raise). Visualizing axes at the character frame will remove guesswork when manipulating Euler sliders and validating quaternion-based pose application.
- **Unlocks:**
  - Immediate: Faster, mistake-free pose authoring and animation tuning; clear mapping of joint controls to motion.
  - Future: Easier debugging for IK alignment, foot placement, and extended joint sets; consistent frame for locomotion-driven limb motion.
- **Test Loop:**
  - Enable checkbox, scrub walk/run poses and joint override sliders, verify limb motion aligns with expected axes; labels confirm orientation.
  - Expect 1–3 minute iterations; zero gameplay coupling; no perf sensitivity.
- **Certainty:** 90% — Pure debug tool on stable rendering/debug infrastructure (certainty boost applicable). No gameplay risk; integration points are straightforward and already used (skeleton/joint labels, wireframe arrows).

## CLARIFIED REQUIREMENTS (Resolved)

- Visual/Behavioral
  - Use existing arrow primitive with reasonable head size; start with length ~0.6 m; tune later if needed.
  - Always show endpoint labels "X", "Y", "Z" using the same ImGui overlay technique as joint labels; no separate toggle.
  - If making axes always-on-top is trivial, do so; otherwise render like other wireframes (depth-tested).

- Mechanical
  - Anchor at skeleton root world transform: `t_pose_skeleton.joints[0].model_transform`.
  - Use full root orientation (not yaw-only) when determining axis directions.
  - Axis conventions confirmed: X=right (red), Y=up (green), Z=forward (blue).

- Scope
  - No GUI parameters beyond a single enable checkbox; default OFF.
  - No special performance optimizations in v1.

- Success Criteria
  - With gizmo enabled, rotating a shoulder around Y reads as forward/back swing; around Z reads as up/down raise.
  - Labels remain legible at common camera distances; colors unambiguously map to axes (R=X, G=Y, B=Z).

## Completion

**Date Completed:** 2025-10-12  
**Final Certainty:** 100%  
**Code Review:** See `PLANS/ARCHIVE/20251012_020113_character_axis_gizmo_CODE_REVIEW.md`

**Outcome:** Feature implemented successfully with zero violations. Pure debug visualization tool cleanly extends existing debug draw infrastructure. Reuses established patterns (`generate_arrow`, ImGui label projection) with minimal new code (~90 lines). Provides immediate value for pose authoring and joint rotation debugging. Default-off toggle ensures zero gameplay impact.

## Reflection

**What went well?**
- **Pattern reuse maximized clarity:** Leveraging existing `generate_arrow` primitive and skeleton label projection technique meant zero new architecture—just composition of proven components.
- **Minimal scope prevented scope creep:** Deferring always-on-top rendering and extra parameters kept implementation tight and focused on core value (visible axes with labels).
- **Clear requirements eliminated ambiguity:** Explicit clarification of axis conventions (X=right/red, Y=up/green, Z=forward/blue) and label behavior (always-on, no toggle) prevented implementation questions.

**What caused friction?**
- **None significant:** Feature was straightforward—foundation systems (wireframe rendering, debug draw, ImGui labels) were all stable and well-documented. Only minor consideration was safe normalization of basis vectors (trivial guard with fallback).

**What would you do differently?**
- **Nothing major:** Scope was appropriately minimal for v1. Only potential improvement: extracting font scaling constant (1.4x) earlier if multiple debug labels adopt same scaling, but premature without evidence of pattern reuse.

## Certainty Calibration

**Predicted Certainty:** 90% (from feature plan)

**Actual Outcome:**
- [x] Completed on first attempt (no deferrals, no major rework)
- [ ] Required feature/rework (specify what changed)
- [ ] Deferred (specify reason and missing prerequisites)

**Variance Analysis:**

Predicted certainty (90%) was accurate—feature completed smoothly with zero rework. The 10% uncertainty buffer accounted for potential unknowns in label projection or arrow primitive usage, but both patterns were well-established from prior work (`draw_skeleton`, wireframe generation). No surprises encountered.

**Calibration Adjustment:**

For similar debug visualization features in the future, maintain 90%+ certainty when:
- Reusing established rendering patterns (wireframe primitives, ImGui overlay projection)
- Zero gameplay coupling (pure reactive layer)
- Minimal scope with no architectural changes
- Stable foundation systems (rendering, debug draw)

Pattern validation: Debug visualization pattern proven reliable—toggle-driven, depth-tested wireframe + viewport-projected labels = low-risk, high-value tooling. Certainty boost (+10%) applicable for similar tools (e.g., collision normal visualizer, IK target gizmo).
