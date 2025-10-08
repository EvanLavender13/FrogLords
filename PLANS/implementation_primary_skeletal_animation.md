# Implementation Plan: Primary Skeletal Animation

### Foundation Audit Snapshot
- **locomotion_system** — Owned by `game_world`, instantiated/reset in `game_world::init`, updated each frame inside `game_world::update`, tuned via `character::sync_locomotion_targets`. Referenced for debugging in `rendering/debug_draw.cpp` and for GUI readouts in `gui/character_panel.cpp`. No duplicate instances exist outside the world.
- **character::skeleton (t_pose_skeleton)** — Single instance stored on `game_world`, created via `character::create_t_pose` and attached to the controller root in `game_world::update`. Render path pulls it through `runtime.cpp` → `debug::draw_skeleton`. No other systems mutate joint locals.
- **Skeletal animation stub** — `game_world` tracks `skeleton_animation_time` and, when `panel_state.animate_skeleton` is true, applies a sine-wave swing directly to joint index 6 before calling `character::update_global_transforms`. This placeholder must be removed in favor of the run-cycle system.
- **GUI ownership** — `gui::character_panel_state` only exposes booleans (show skeleton, animate, etc.). `draw_character_panel` receives the world’s controller, locomotion, orientation, and tuning structs by reference; it has no access to a skeletal animation module yet.
- **Build wiring** — `CMakeLists.txt` enumerates every source file explicitly. Any new module under `src/character/` must be added to the executable target to compile.

### Graybox Implementation Checklist

#### 1. Module Scaffolding & Baseline Data
- [ ] **File:** `src/character/skeletal_animation.h` *(new)*
    - [ ] Declare a `run_keyframe` struct with six named joint angles (left/right shoulders, elbows, hips, knees) stored in radians.
    - [ ] Define a `skeletal_animation_system` that holds:
        - Baseline copies of the T-pose local transforms for every animated joint
        - An array of four `run_keyframe` entries ordered REACH-RIGHT → PASS-LEFT → REACH-LEFT → PASS-RIGHT
        - Runtime state (current phase, selected frame index, enable/lock flags, frequency multiplier, manual phase override)
        - Utility accessors for GUI (e.g., fetch/edit keyframe angles in degrees)
    - [ ] Expose public methods `init_defaults(const character::skeleton&)`, `update_phase(const locomotion_system&, const controller&, const gui::character_panel_state&)`, and `apply(character::skeleton&)`.
    - [ ] Introduce either a small enum or constants that map semantic joint names (`left_shoulder`, `right_knee`, etc.) to indices in the T-pose skeleton so the rest of the plan avoids hard-coded numbers.
- [ ] **File:** `src/character/skeletal_animation.cpp` *(new)*
    - [ ] Implement the struct defaults using the feature brief’s initial angle estimates (convert degrees to radians here and keep the constants in one place for easy tweaking).
    - [ ] Cache the baseline local transforms from the supplied skeleton so the system can restore untouched joints when disabled.
    - [ ] Convert GUI edits (degrees) into stored radians while clamping to sensible ranges (e.g., ±120° shoulders, knees 0°–180°) and ensure elbow locks stay fixed at 90°.
    - [ ] Add helper functions to rebuild joint-local transforms by composing the cached translation with axis rotations (shoulders/hips rotate about local Z for sagittal swing; knees/elbows hinge about local X).

#### 2. Keyframe Selection & Pose Application
- [ ] Encode the locomotion-driven phase logic inside `update_phase`:
    - [ ] Compute `$phase = \text{fmod}(\max(0, locomotion.distance\_traveled \times frequency), 1.0)$` whenever run-cycle driving is enabled (no interpolation—frame selection is discrete).
    - [ ] Freeze `phase` when `controller.velocity` magnitudes fall below a small epsilon (limbs stop instantly with the body).
    - [ ] Support manual scrubbing: when "lock to locomotion" is false, honor the panel’s manual phase slider and selected keyframe index instead of the computed phase, but keep playback discrete (nearest frame wins).
    - [ ] Track the `current_frame` index (`static_cast<int>(phase * 4.0f) % 4`) for GUI readbacks.
- [ ] Inside `apply`:
    - [ ] Reset animated joints to their cached baseline transforms before applying pose data.
    - [ ] Apply elbow rotations first (constant 90° flex) so they remain locked regardless of keyframe selection.
    - [ ] Apply the current keyframe’s shoulder/hip/knee angles by composing rotations with the cached translation, ensuring opposite-limb coordination matches the stored keyframe data.
    - [ ] Early-out to a pure T-pose when the feature toggle is disabled.

#### 3. Runtime Integration
- [ ] **File:** `src/app/game_world.h`
    - [ ] Include `character/skeletal_animation.h` and add a `character::skeletal_animation_system run_cycle;` member.
    - [ ] Remove `skeleton_animation_time` (no longer needed).
- [ ] **File:** `src/app/game_world.cpp`
    - [ ] In `init()`, after creating the T-pose skeleton and computing globals, call `run_cycle.init_defaults(t_pose_skeleton)`.
    - [ ] Replace the legacy sine-wave block with a call sequence:
        1. Update the skeletal animation phase via `run_cycle.update_phase(locomotion, character, panel_state)`
        2. Apply the resulting pose to `t_pose_skeleton`
        3. Call `character::update_global_transforms(t_pose_skeleton)` exactly once per frame.
    - [ ] Preserve the existing attachment of joint 0 to `character.get_world_transform()` so reactive animation layers continue to stack.
- [ ] Ensure `game_world::update` keeps locomotion/orientation logic untouched (no new dependencies flowing backwards into the controller) and that the skeletal system runs after locomotion/orientation to respect the layering described in the feature brief.

#### 4. GUI State & Controls
- [ ] **File:** `src/gui/character_panel.h`
    - [ ] Extend `character_panel_state` with a nested `skeletal_controls` struct containing:
        - `bool run_cycle_enabled`
        - `bool lock_to_locomotion`
        - `float frequency_multiplier` (default 0.5f strides per meter)
        - `float manual_phase` (0–1)
        - `int selected_keyframe`
        - Per-keyframe angle buffers stored in degrees for six joints
        - Optional debug flags (highlight current frame, show phase readout)
    - [ ] Update the function signature of `draw_character_panel` to accept `character::skeletal_animation_system&` so the GUI can read/write live data.
- [ ] **File:** `src/gui/character_panel.cpp`
    - [ ] Add a "Primary Skeletal Animation" collapsing section that presents:
        - Toggle for enabling the run cycle
        - Frequency multiplier slider with live readout of computed cycles per meter
        - Lock-to-locomotion checkbox and manual phase slider (0..1) when unlocked
    - Keyframe combo box (`REACH-RIGHT`, `PASS-LEFT`, `REACH-LEFT`, `PASS-RIGHT`)
        - Six sliders (degrees) for the selected keyframe’s joint angles (left/right shoulders, hips, knees)
        - Read-only telemetry for locomotion distance, computed phase, and active frame index
    - [ ] Wire sliders to the skeletal system by translating between degrees (UI) and radians (system storage).
    - [ ] Remove the legacy "Animate Skeleton" checkbox or repurpose it to gate `run_cycle_enabled`.

#### 5. Runtime/GUI Wiring
- [ ] **File:** `src/app/runtime.cpp`
    - [ ] Update `world.update` calls and the GUI invocation to pass the skeletal animation system reference.
    - [ ] Drop the conditional `character::update_global_transforms` call in `render_world`; rely on `game_world::update` having already applied the pose while still maintaining the fallback when the skeleton is shown but animation disabled.
    - [ ] Surface the current frame index or phase from `run_cycle` back into the GUI readout (either via return data or by querying the system directly).

#### 6. Debug Rendering Enhancements *(Optional but valuable)*
- [ ] **File:** `src/rendering/debug_draw.cpp`
    - [ ] Accept an optional highlight index (e.g., tint joints red when the GUI locks a keyframe) driven by the panel state.
    - [ ] Display the current locomotion phase and frame name alongside the skeleton when the run cycle is enabled (text overlay using ImGui foreground draw list).

#### 7. Cleanup & Migrations
- [ ] Audit the codebase for references to `skeleton_animation_time` and `panel_state.animate_skeleton`; remove or rename them to reflect the new system.
- [ ] Verify no other modules cache joint transforms—`skeletal_animation_system` should remain the sole writer for limb joints.
- [ ] Update `CMakeLists.txt` to compile the new `skeletal_animation.cpp` source.

#### 8. Quality Gates
- [ ] Configure & build (`CMake: Configure (Debug)` → `CMake: Build (Debug)`).
- [ ] Run clang-format/clang-tidy tasks to ensure style compliance.
- [ ] Launch the debug build (`Run (Debug)`) and confirm the GUI controls manipulate the run cycle as expected during a quick smoke test.
