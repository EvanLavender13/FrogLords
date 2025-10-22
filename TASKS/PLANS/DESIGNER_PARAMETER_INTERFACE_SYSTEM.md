# System: Designer Parameter Interface

Standardized GUI system that exposes designer-friendly tunable parameters with real-time feedback, establishing the pattern for all future debug/tuning interfaces.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-22
**Layer:** 1 (Infrastructure)
**Complexity:** Medium
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/REFERENCES -->
## References

**Technical documentation:**
- `NOTES/DesigningGames/DG_Interface.md` - Visual hierarchy, signal/noise ratio, metaphor vocabulary
- ImGui documentation - PlotLines, PlotHistogram, widget patterns
- Existing command pattern - `parameter_command.h`, `camera_command.h` (unidirectional flow)

**Visual references:**
- Current implementation shows good foundation:
  - `character_panel.cpp` - Collapsing headers, grouped parameters, sliders with units
  - `camera_panel.cpp` - Mode toggles, constrained ranges
  - Mixed read-only displays (`gui::widget::text`) and tunables (`gui::widget::slider_float`)

**Inspiration/Prior art:**
- Unreal Engine's Details Panel - metadata-driven, clear visual distinction between categories
- Unity Inspector - derived values shown in read-only, grouping by component
- Blender's properties - visual hierarchy through indentation and color
- Game debug consoles - live graphs for parameter feedback (frame time, physics metrics)

**Research findings:**
- Visual Hierarchy: More important = more visible; allows players at all skill levels to absorb appropriate information
- Signal vs Noise: Complex visuals obscure meaning; keep designer controls simple and clear (graybox philosophy)
- Metaphor Vocabulary: Establish consistent patterns for "what's tunable" vs "what's read-only" vs "what's derived"
- Feedback Loops: Parameters need validation through visualization (graphs show actual behavior vs intent)

**Constraints/Requirements:**
- MUST maintain unidirectional flow (GUI → commands → game state)
- MUST use vanilla ImGui (no custom rendering, no external libraries)
- MUST be extensible to future systems (animation, physics, AI, etc.)
- MUST clearly distinguish: tunable params, read-only state, derived values
- MUST provide real-time feedback for validating parameter effects
- SHOULD group parameters by what they affect (system-centric organization)
- SHOULD display units explicitly (m/s, m/s², radians, meters, etc.)
- SHOULD show value ranges and constraints visually
<!-- END: SELECT/REFERENCES -->

---

<!-- BEGIN: SELECT/DEPENDENCIES -->
## Dependencies

**Required:**
- GUI System (exists in Layer 1) - `src/gui/gui.{h,cpp}`
- Command Pattern (exists in Layer 1) - `parameter_command.h`, `camera_command.h`
- Runtime orchestration (exists in Layer 1) - `src/app/runtime.{h,cpp}`

**Enables (will become buildable):**
- Standardized tuning for all future systems (animation, IK, ragdoll, etc.)
- Parameter validation and feedback tooling
- Designer workflow improvements (presets, profiles, save/load)
- Automated parameter testing and validation
<!-- END: SELECT/DEPENDENCIES -->

---

<!-- BEGIN: SELECT/CORE -->
## Core

**Irreducible minimum:**
A parameter metadata system that separates designer intent (jump_height) from implementation details (jump_velocity), provides clear visual distinction between tunable/read-only/derived values, and establishes extensible patterns for real-time feedback visualization.

**Why a system, not a feature:**
Every future system needs designer-facing parameters. Without standardization, each system invents its own patterns, creating inconsistent UX and maintenance burden. This system establishes the vocabulary (visual hierarchy, grouping, feedback) that enables coherent growth across all future development.
<!-- END: SELECT/CORE -->

---

<!-- BEGIN: SELECT/MATHEMATICAL_FOUNDATION -->
## Mathematical Foundation

**Needs proof:**
- Derived value calculations must be mathematically correct (e.g., jump_velocity = √(2·|g|·h))
- Parameter constraint enforcement (min ≤ value ≤ max, invariants preserved)
- Time-windowed plot buffers must handle variable frame rates correctly

**Known uncertainties:**
- Optimal time window for different parameter types (position vs velocity vs acceleration)
- Plot update frequency vs performance cost
- Memory footprint for plot buffers across many parameters
<!-- END: SELECT/MATHEMATICAL_FOUNDATION -->

---

<!-- BEGIN: SELECT/GRAYBOX -->
## Graybox

**Simplest implementation:**
Extend existing character_panel with:
1. Parameter metadata struct (name, range, units, type: tunable/readonly/derived)
2. Visual distinction via color or formatting (tunable=editable, readonly=gray, derived=italic)
3. Single plot widget showing one live parameter (e.g., horizontal speed over time)
4. Derived value display (show jump_velocity calculated from jump_height + gravity)

Use existing systems (character movement) for validation. Defer:
- Multi-parameter plots
- Parameter presets/save/load
- Automated validation
- Cross-system parameter viewers

**Validation:**
- Can designer understand parameter purpose from name + units?
- Is visual distinction between parameter types immediately clear?
- Does real-time plot reveal actual behavior vs designer intent?
- Can pattern extend to new system (camera) without code duplication?
<!-- END: SELECT/GRAYBOX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Parameter metadata system implemented (type, range, units, description)
- [ ] Clear visual hierarchy: tunable, read-only, derived values visually distinct
- [ ] Real-time feedback: at least one plot showing parameter effects over time
- [ ] Pattern demonstrated across two systems (character + camera)
- [ ] Derived values shown with calculation transparency (designer sees formula)
- [ ] Units displayed explicitly on all numeric parameters
- [ ] Extensible: adding new parameter requires minimal code
- [ ] Principles upheld: radical simplicity, single source of truth, visual hierarchy
<!-- END: SELECT/SUCCESS -->
