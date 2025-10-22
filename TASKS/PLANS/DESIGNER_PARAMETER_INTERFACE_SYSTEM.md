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
- **Tuning Pattern** (exists in Layer 3) - `src/character/tuning.{h,cpp}`
  - Already implements designer-friendly params (jump_height) → derived values (jump_velocity)
  - Already implements apply_to() derivation logic
  - **Missing:** Metadata layer (this system adds it)

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
A parameter metadata layer that formalizes the existing tuning pattern (`tuning.h` already separates designer intent from implementation). Metadata describes each parameter's presentation (name, units, range, type), enabling generic GUI widgets and establishing consistent visual vocabulary across all systems.

**Why a system, not a feature:**
The tuning pattern already exists but only for character movement. Every future system (camera, animation, IK, ragdoll) will need designer-facing parameters. Without standardized metadata, each system invents its own GUI code, creating inconsistent UX and maintenance burden. This system extracts the pattern from tuning.h, makes it reusable, and establishes the vocabulary (visual hierarchy, grouping, feedback) that enables coherent growth.

**What already exists vs what's new:**
- **Exists:** Designer params → derived values pattern (tuning.h)
- **Exists:** Unidirectional command flow (parameter_command)
- **New:** Metadata layer (param name, units, range, type)
- **New:** Generic metadata-driven GUI widgets
- **New:** Real-time feedback visualization patterns
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

**Phase 1 - Add metadata to tuning.h:**
1. Define param_meta struct (name, units, range, type: tunable/readonly/derived)
2. Add static constexpr metadata for each tuning_params field (max_speed_meta, jump_height_meta, etc.)
3. Add derived_param_meta for jump_velocity (shows formula: √(2·|g|·h))

**Phase 2 - Generic widgets in gui namespace:**
1. `tunable_param(value, meta)` - slider with units, color-coded as tunable
2. `readonly_param(value, meta)` - text display, grayed out
3. `derived_param(value, meta, formula)` - italic text showing calculation
4. `plot_param(value, meta, time_window)` - reuse plot_histogram pattern

**Phase 3 - Refactor character_panel.cpp:**
1. Replace hardcoded sliders with metadata-driven widgets
2. Add one live plot (horizontal speed over time as validation)
3. Show jump_velocity as derived parameter with formula visible

**Phase 4 - Extend pattern to camera:**
1. Add metadata to camera_follow struct (distance_meta, height_offset_meta, etc.)
2. Refactor camera_panel.cpp to use same widgets
3. Verify consistency and extensibility

Use existing character movement system for validation. Defer:
- Multi-parameter correlation plots
- Parameter presets/save/load
- Automated validation tests
- Cross-system parameter comparison

**Validation:**
- Can designer understand parameter purpose from name + units?
- Is visual distinction between parameter types immediately clear?
- Does real-time plot reveal actual behavior vs designer intent?
- Can pattern extend to camera without code duplication?
- Does metadata live with data (tuning.h) not GUI code?
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
