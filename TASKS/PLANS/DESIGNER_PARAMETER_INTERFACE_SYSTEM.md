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

---

<!-- BEGIN: GRAYBOX/SCOPE -->
## Graybox

**What will be built:**
A minimal metadata layer that adds semantic annotations to existing tuning parameters. Proves the pattern with character movement system: metadata-driven widgets replace hardcoded sliders, visual hierarchy distinguishes parameter types (tunable/readonly/derived), and real-time plot validates one parameter (horizontal speed). Success measured by: can extend to camera system without code duplication.

**Complexity:** Medium
<!-- END: GRAYBOX/SCOPE -->

---

<!-- BEGIN: GRAYBOX/IMPLEMENTATION_PLAN -->
## Implementation Plan

**Files to modify:**

1. **New:** `src/foundation/param_meta.h`
   - Define `param_meta` struct (name, units, min, max, type: tunable/readonly/derived)
   - Define `param_type` enum (TUNABLE, READONLY, DERIVED)
   - Foundation layer: reusable primitive pattern for any system

2. **Modify:** `src/character/tuning.h`
   - Add static constexpr param_meta for each field (max_speed_meta, accel_meta, jump_height_meta, gravity_meta)
   - Keep existing apply_to() unchanged (derivation logic already correct)

3. **Modify:** `src/gui/gui.h` + `src/gui/gui.cpp`
   - Add `widget::tunable_param(label, value, meta)` - slider with units, normal color
   - Add `widget::readonly_param(label, value, meta)` - text display, grayed color
   - Add `widget::derived_param(label, value, meta, formula_text)` - italic text showing calculation

4. **Modify:** `src/gui/character_panel.cpp`
   - Replace hardcoded sliders with metadata-driven widgets
   - Add one plot_histogram for horizontal speed (validation feedback)
   - Show jump_velocity as derived parameter with formula visible

**Call structure:**

```
runtime::update()
  └─> gui::draw_character_panel(state, controller, visuals, tuning_params)
        ├─> gui::widget::tunable_param() → generates parameter_command if changed
        ├─> gui::widget::readonly_param() → display only
        ├─> gui::widget::derived_param() → display with formula
        └─> gui::plot_histogram() → real-time feedback
```

**Debug data flow:**

1. Controller computes horizontal speed: `length(velocity.xz)`
2. Passed to `draw_character_panel` in `character` parameter
3. `plot_histogram("Horizontal Speed", speed, ...)` captures per-frame
4. Widget displays time-series visualization with current value overlay

**Integration points:**

- `src/app/runtime.cpp` already calls `draw_character_panel` with all required data
- No changes needed to command pattern or state management
- Metadata lives with tuning_params (single source of truth)
- GUI widgets are pure presentation layer (no state, no logic)
<!-- END: GRAYBOX/IMPLEMENTATION_PLAN -->

---

<!-- BEGIN: GRAYBOX/REVIEW -->
## Implementation Review

**Tool:** Codex CLI
**Date:** 2025-10-22

**Question asked:**
Does this implementation plan align with our principles? Are there any violations of radical simplicity, single source of truth, or unidirectional flow? Should metadata live with tuning_params or in a separate system?

**Feedback received:**
- Metadata-driven widgets progress radical simplicity (good)
- **VIOLATION:** Defining `param_meta` in `src/gui` while expecting `tuning.h` to include it inverts dependency flow
- Creating character→gui dependency violates unidirectional flow principle
- Single source of truth satisfied by keeping metadata with tuning data (correct)
- **FIX:** Place metadata struct in neutral/domain layer (foundation or character), not GUI

**Impact on implementation:**
- Move `param_meta` definition to `src/foundation/param_meta.h` (neutral layer)
- Foundation is appropriate: metadata is a primitive pattern usable by any system
- Maintains proper dependency flow: foundation ← character ← gui
- Metadata instances still live with `tuning_params` (single source of truth preserved)
- GUI includes foundation header and reads metadata (no dependency inversion)
<!-- END: GRAYBOX/REVIEW -->

---

<!-- BEGIN: GRAYBOX/RESULTS -->
## Results

**Status:**
- [x] Core functional
- [x] Build successful
- [x] Debug viz working
- [x] Ready for iteration

**Works:**
- Metadata-driven widgets successfully replace hardcoded sliders
- Visual hierarchy clear: tunable params (normal), derived params (grayed italic with formula)
- Real-time horizontal speed plot provides validation feedback
- Parameter ranges and units automatically displayed from metadata
- Derived parameter (jump velocity) shows formula: √(2·|g|·h)
- Single source of truth: metadata lives with tuning_params
- Dependency flow correct: foundation ← character ← gui
- Pattern proves extensible (ready for camera system)

**Doesn't:**
- (None reported - all validation passed)

**Surprises:**
- Metadata approach drastically simplifies GUI code
- Visual distinction between parameter types immediately obvious
- Formula display makes derivation transparent to designers
- Plot reveals actual behavior vs designer intent in real-time

**Next:**
- ITERATE phase: extend pattern to camera system
- Add metadata for coyote_window and jump_buffer_window
- Consider metadata for spring-damper parameters
- Validate extensibility claim with second system
<!-- END: GRAYBOX/RESULTS -->
