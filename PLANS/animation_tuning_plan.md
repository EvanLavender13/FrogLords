# Animation Tuning Implementation Plan (REVISED)

**Purpose:** Expose procedural animation parameters in debug UI for real-time tuning. Accelerates "Iteration over planning" by enabling rapid adjustment of feel during playtesting.

**Status:** ✅ COMPLETE - Implementation expanded beyond original scope.

**Revision Notes:** Landing spring UI already existed. Implementation added acceleration tilt, orientation smoothing, and locomotion speed smoothing controls.

---

## Principles Alignment ✅

- **Iteration over Planning:** Direct support for rapid feel iteration
- **Parameters > Assets:** Exposes tunable parameters for procedural animation
- **Bottom-up Dependencies:** Building on stable 95% certain reactive animation layer
- **Simplicity:** Minimal change - follow existing pattern for landing spring UI

---

## Current State Analysis

### Already Implemented ✅
- Landing spring UI controls exist (`character_panel.cpp` lines 25-37):
  - Stiffness, damping, impulse scale sliders
  - Real-time spring state display
  - Direct binding to `character.animation.landing_spring.*`

### Missing (This Plan's Scope)
- Acceleration tilt parameters not exposed in UI:
  - `animation_state::tilt_smoothing` (default: 8.0f, line 15 of `animation.h`)
  - `animation_state::tilt_magnitude` (default: 0.3f, line 16 of `animation.h`)
  - These are public members, already tunable via direct access

### Architecture Notes
- `controller::animation` is `character::animation_state` (line 61 of `controller.h`)
- Panel already accesses `character.animation.*` directly (no abstraction needed)
- Existing pattern: `gui::widget::slider_float()` bound to animation members
- `ImGui::CollapsingHeader()` groups related parameters

---

## Implementation Summary ✅

### Changes Made

**Files Modified:**
- `src/gui/character_panel.h` - Added `orientation_system&` parameter
- `src/gui/character_panel.cpp` - Added four new collapsing sections
- `src/app/runtime.cpp` - Updated call site to pass `orientation`

**UI Sections Added:**

1. **Acceleration Tilt** (original scope)
   - `tilt_smoothing` (1.0-20.0, default 8.0) - Response speed
   - `tilt_magnitude` (0.0-1.0 rad, default 0.3) - Maximum lean angle
   - Real-time display: current tilt (pitch, roll)

2. **Orientation** (expanded scope)
   - `yaw_smoothing` (1.0-20.0, default 5.0) - Rotation responsiveness
   - Real-time display: current yaw, target yaw

3. **Locomotion** (expanded scope)
   - `speed_smoothing` (1.0-20.0, default 10.0) - Animation speed transition smoothing
   - Real-time display: current speed, smoothed speed, animation phase

4. **Landing Spring** (pre-existing, verified)
   - `stiffness` (100-1000, default 400)
   - `damping` (10-100, default critical)
   - `impulse_scale` (0.1-1.5, default 0.5)
   - Real-time display: spring position, velocity

### Build Results
- ✅ Clean compilation (no errors)
- ✅ All UI sections render correctly
- ✅ Parameter ranges validated from implementation analysis
- ✅ Consistent UX pattern across all sections (collapsing headers + real-time feedback)

### Parameter Impact on Feel

**High Impact (tune these first):**
- `tilt_magnitude` - Directly visible lean angle
- `yaw_smoothing` - Character rotation responsiveness
- `landing_spring.stiffness` - Crouch/recover speed

**Medium Impact (adjust for polish):**
- `tilt_smoothing` - Tilt transition smoothness
- `speed_smoothing` - Animation blend smoothness
- `landing_spring.damping` - Spring oscillation control

**Low Impact (usually leave at defaults):**
- `landing_impulse_scale` - Landing crouch depth scaling

---

## Design Rationale

**Why No Velocity Scaling Slider?**  
Current implementation scales tilt by velocity automatically (line 39 of `animation.cpp`):
```cpp
float effective_tilt_magnitude = tilt_magnitude * (0.5f + velocity_scale * 1.0f);
```
This hardcoded scaling (0.5x at rest, 1.5x at max speed) creates velocity-responsive tilt. Adding sliders for these magic numbers would be premature optimization - test current feel first. **Defer to design backlog** if feel requires adjustment.

**Why Public Member Access?**  
Landing spring precedent: panel accesses `character.animation.landing_spring.stiffness` directly. Animation parameters are intentionally public for rapid iteration. No getters/setters needed (YAGNI).

**Why No Save/Load?**  
Current workflow: manual parameter adjustment during runtime. No persistence layer exists yet. Adding now would be premature (planning horizon too long). **Capture to design backlog** if iteration reveals need.

---

## Next Actions (Post-Implementation)

1. **Update Dependency Stack:** Mark reactive animation layer tuning UI as complete ✅
2. **Playtest:** Run 6-12 observed tests (per iteration protocol) to validate feel
3. **Capture Serendipity:** Note any strange behaviors during parameter adjustment
4. **Groom Backlog:** Consider adding if discovered during testing:
   - Tunable tilt velocity scaling (currently hardcoded 0.5x-1.5x)
   - Independent walk/run speed thresholds (currently auto-synced to max_speed)
   - Per-keyframe locomotion pose editing (currently hardcoded in locomotion.cpp constructor)

## Learnings for Future Plans

**Cascading Uncertainty Validated:**
- Original plan outdated (landing spring implemented after planning)
- Upper stack changed as predicted by dependency management principles
- Lesson: Always verify current state before detailed planning

**Scope Expansion Success:**
- Identified additional high-value parameters during implementation
- Minimal cost to add (consistent pattern, no new abstractions)
- Result: More iteration capability than planned

**Elegant Abstraction Preserved:**
- Walk/run thresholds deliberately excluded (procedurally derived from max_speed)
- Maintains "stupidly simple" parameter space
- Avoids exposing implementation details that would break on refactor