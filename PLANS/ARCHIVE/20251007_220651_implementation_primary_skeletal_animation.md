# Implementation Plan: Primary Skeletal Animation

## CRITICAL ARCHITECTURAL ISSUE DISCOVERED

**Status:** DEFERRED - Feature requires quaternion-based keyframes

**Problem Identified:**
The current single-float-per-joint keyframe structure (`run_keyframe { float left_shoulder; ... }`) is fundamentally insufficient for 3D skeletal animation. Attempting to encode full 3D rotation into a single scalar angle creates unsolvable transform composition issues:

1. **Insufficient degrees of freedom:** Real limb motion requires pitch/yaw/roll (3 axes), not a single rotation angle
2. **Transform ambiguity:** No consistent way to map single float → correct 3D rotation across different joint orientations
3. **Unworkable baseline composition:** Attempting to compose single-axis rotations with T-pose baselines produces incorrect results depending on baseline orientation

**Attempted Solutions (all failed):**
- Custom shoulder transform with swing direction vectors → mirroring issues
- Direct axis rotation (X/Y/Z) → different joints need different axes
- Ignoring T-pose baseline → loses joint positioning
- Simplified rotation-only transforms → still requires knowing correct rotation axis per joint type

**Root Cause:**
The data structure itself (`float` per joint) cannot represent the required information. This is not a code bug but an architectural limitation.

**Required Change:**
Keyframes must store **quaternions** (or Euler angles minimum) per joint to enable arbitrary 3D rotations:
```cpp
struct run_keyframe {
    glm::quat left_shoulder;   // Full 3D rotation
    glm::quat right_shoulder;
    // ... etc
};
```

**Deferral Decision:**
Per development principles ("short plans → build → test → repeat", "graybox first"), implementing quaternion-based animation is a **separate feature** that should be planned independently. The current implementation attempted to shortcut proper keyframe representation and failed.

**Scope Reduction - New Minimal Feature:**
Instead of full locomotion-driven animation, implement **static keyframe preview only**:
1. Store 4 hardcoded T-pose variations (REACH-RIGHT, PASS-LEFT, REACH-LEFT, PASS-RIGHT)
2. GUI allows manual selection between poses (dropdown/buttons)
3. No locomotion integration, no phase computation
4. Validates quaternion keyframe storage works before adding animation layer

This separates keyframe authoring from animation playback per user directive.

---

## ORIGINAL PLAN (INVALIDATED - kept for reference)

### Foundation Audit Snapshot
- **locomotion_system** — Owned by `game_world`, instantiated/reset in `game_world::init`, updated each frame inside `game_world::update`, tuned via `character::sync_locomotion_targets`. Referenced for debugging in `rendering/debug_draw.cpp` and for GUI readouts in `gui/character_panel.cpp`. No duplicate instances exist outside the world.
- **character::skeleton (t_pose_skeleton)** — Single instance stored on `game_world`, created via `character::create_t_pose` and attached to the controller root in `game_world::update`. Render path pulls it through `runtime.cpp` → `debug::draw_skeleton`. No other systems mutate joint locals.
- **Skeletal animation stub** — `game_world` tracks `skeleton_animation_time` and, when `panel_state.animate_skeleton` is true, applies a sine-wave swing directly to joint index 6 before calling `character::update_global_transforms`. This placeholder must be removed in favor of the run-cycle system.
- **GUI ownership** — `gui::character_panel_state` only exposes booleans (show skeleton, animate, etc.). `draw_character_panel` receives the world’s controller, locomotion, orientation, and tuning structs by reference; it has no access to a skeletal animation module yet.
- **Build wiring** — `CMakeLists.txt` enumerates every source file explicitly. Any new module under `src/character/` must be added to the executable target to compile.

### Graybox Implementation Checklist

#### 1. Module Scaffolding & Baseline Data
- [x] **File:** `src/character/skeletal_animation.h` *(new)*
    - [x] Declare a `run_keyframe` struct with six named joint angles (left/right shoulders, elbows, hips, knees) stored in radians.
    - [x] Define a `skeletal_animation_system` that holds:
        - Baseline copies of the T-pose local transforms for every animated joint
        - An array of four `run_keyframe` entries ordered REACH-RIGHT → PASS-LEFT → REACH-LEFT → PASS-RIGHT (canonical source of truth)
        - Runtime state (current phase, selected frame index, enable/lock flags, frequency multiplier, manual phase override)
        - Utility accessors for GUI (e.g., fetch/edit keyframe angles in degrees) so the UI never caches duplicate limb data
    - [x] Expose public methods `init_defaults(const character::skeleton&)`, `update_phase(const locomotion_system&, const controller&, const gui::character_panel_state&)`, `apply(character::skeleton&)`, and angle accessors `get_keyframe_degrees(int, joint)` / `set_keyframe_degrees(int, joint, float)`.
    - [x] Introduce either a small enum or constants that map semantic joint names (`left_shoulder`, `right_knee`, etc.) to indices in the T-pose skeleton so the rest of the plan avoids hard-coded numbers.
- [x] **File:** `src/character/skeletal_animation.cpp` *(new)*
    - [x] Implement the struct defaults using the feature brief’s initial angle estimates (convert degrees to radians here and keep the constants in one place for easy tweaking).
    - [x] Cache the baseline local transforms from the supplied skeleton so the system can restore untouched joints when disabled.
    - [x] Convert GUI edits (degrees) into stored radians while clamping to sensible ranges (e.g., ±120° shoulders, knees 0°–180°) and ensure elbow locks stay fixed at 90°.
    - [x] Add helper functions to rebuild joint-local transforms by composing the cached translation with axis rotations (shoulders/hips rotate about local Z for sagittal swing; knees/elbows hinge about local X).
    - **Files touched (Step 1):**
        - `src/character/skeletal_animation.h`
        - `src/character/skeletal_animation.cpp`

#### 2. Keyframe Selection & Pose Application
- [x] Encode the locomotion-driven phase logic inside `update_phase`:
    - [x] Compute `$phase = \text{fmod}(\max(0, locomotion.distance_traveled \times frequency), 1.0)$` whenever run-cycle driving is enabled (no interpolation—frame selection is discrete).
    - [x] Freeze `phase` when `controller.velocity` magnitudes fall below a small epsilon (limbs stop instantly with the body).
    - [x] Support manual scrubbing: when "lock to locomotion" is false, honor the panel’s manual phase slider and selected keyframe index instead of the computed phase, but keep playback discrete (nearest frame wins).
    - [x] Track the `current_frame` index (`static_cast<int>(phase * 4.0f) % 4`) for GUI readbacks.
- [x] Inside `apply`:
    - [x] Reset animated joints to their cached baseline transforms before applying pose data.
    - [x] Apply elbow rotations first (constant 90° flex) so they remain locked regardless of keyframe selection.
    - [x] Apply the current keyframe’s shoulder/hip/knee angles by composing rotations with the cached translation, ensuring opposite-limb coordination matches the stored keyframe data.
    - [x] Early-out to a pure T-pose when the feature toggle is disabled.

    - **Files touched (Step 2):**
        - `src/character/skeletal_animation.cpp`
#### 3. Runtime Integration
- [x] **File:** `src/app/game_world.h`
    - [x] Include `character/skeletal_animation.h` and add a `character::skeletal_animation_system run_cycle;` member.
    - [x] Remove `skeleton_animation_time` (no longer needed).
- [x] **File:** `src/app/game_world.cpp`
    - [x] In `init()`, after creating the T-pose skeleton and computing globals, call `run_cycle.init_defaults(t_pose_skeleton)`.
    - [x] Replace the legacy sine-wave block with a call sequence:
        1. Update the skeletal animation phase via `run_cycle.update_phase(locomotion, character, panel_state)`
        2. Apply the resulting pose to `t_pose_skeleton`
        3. Call `character::update_global_transforms(t_pose_skeleton)` exactly once per frame.
    - [x] Preserve the existing attachment of joint 0 to `character.get_world_transform()` so reactive animation layers continue to stack.
- [x] Ensure `game_world::update` keeps locomotion/orientation logic untouched (no new dependencies flowing backwards into the controller) and that the skeletal system runs after locomotion/orientation to respect the layering described in the feature brief.

    - **Files touched (Step 3):**
        - `src/app/game_world.h`
        - `src/app/game_world.cpp`

#### 4. GUI State & Controls
- [x] **File:** `src/gui/character_panel.h`
    - [x] Extend `character_panel_state` with a nested `skeletal_controls` struct containing:
        - `bool run_cycle_enabled`
        - `bool lock_to_locomotion`
        - `float frequency_multiplier` (default 0.5f strides per meter)
        - `float manual_phase` (0–1)
        - `int selected_keyframe`
        - Optional debug flags (highlight current frame, show phase readout)
    - [x] Update the function signature of `draw_character_panel` to accept `character::skeletal_animation_system&` so the GUI can read/write live data.
- [x] **File:** `src/gui/character_panel.cpp`
    - [x] Add a "Primary Skeletal Animation" collapsing section that presents:
        - Toggle for enabling the run cycle
        - Frequency multiplier slider with live readout of computed cycles per meter
        - Lock-to-locomotion checkbox and manual phase slider (0..1) when unlocked
    - Keyframe combo box (`REACH-RIGHT`, `PASS-LEFT`, `REACH-LEFT`, `PASS-RIGHT`)
        - Six sliders (degrees) for the selected keyframe’s joint angles (left/right shoulders, hips, knees)
        - Read-only telemetry for locomotion distance, computed phase, and active frame index
    - [x] Wire sliders to the skeletal system by translating between degrees (UI) and radians (system storage) via the system accessors (no per-frame copies stored in panel state).
    - [x] Remove the legacy "Animate Skeleton" checkbox or repurpose it to gate `run_cycle_enabled`.

    - **Files touched (Step 4):**
        - `src/gui/character_panel.h`
        - `src/gui/character_panel.cpp`
        - `src/app/runtime.cpp`

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
- [x] Update `CMakeLists.txt` to compile the new `skeletal_animation.cpp` source.

    - **Files touched (Step 7 so far):**
        - `CMakeLists.txt`

#### 8. Quality Gates
- [ ] Configure & build (`CMake: Configure (Debug)` → `CMake: Build (Debug)`).
- [ ] Run clang-format/clang-tidy tasks to ensure style compliance.
- [ ] Launch the debug build (`Run (Debug)`) and confirm the GUI controls manipulate the run cycle as expected during a quick smoke test.

---

## Deferral

**Date:** 2025-10-07

**Reason:** Fundamental data structure limitation - single-float-per-joint cannot represent 3D rotation

### Implementation Attempts

**Approach 1: Custom shoulder transform with swing direction vectors**
- Created hardcoded forward/back swing vectors to determine rotation direction
- Failed: Mirroring issues - right shoulder behavior didn't correctly mirror left shoulder
- Issue: Single float angle still ambiguous about rotation axis and composition order

**Approach 2: Direct axis rotation (X/Y/Z)**
- Attempted to apply rotations around specific axes (e.g., X for shoulder swing)
- Failed: Different joints need different rotation axes based on their orientation in the T-pose hierarchy
- Issue: No consistent mapping from joint type → correct rotation axis without multi-axis representation

**Approach 3: Ignoring T-pose baseline transforms**
- Tried building transforms from scratch using only keyframe angles
- Failed: Lost joint positioning - limbs detached from body or positioned incorrectly
- Issue: Need baseline transforms for translation/parent attachment, but can't compose with single-float rotations

**Approach 4: Simplified rotation-only transforms**
- Attempted to build minimal rotation matrices from single angle
- Failed: Still requires knowing which axis to rotate around, which varies per joint type and orientation
- Issue: Single scalar cannot encode "rotate shoulder forward 30°" without additional axis information

### Key Technical Insights

1. **Degrees of freedom mismatch:** Human joints require 3 rotational degrees of freedom (pitch/yaw/roll), but single float provides only 1
2. **Transform composition requires quaternions:** Composing multiple rotations cleanly requires quaternion math or full 3×3 rotation matrices, not scalar angles
3. **Axis dependency:** Each joint type (shoulder, elbow, hip, knee) has different rotation constraints and primary axes that must be encoded in the keyframe data structure itself
4. **Baseline dependency:** Must compose keyframe rotations with T-pose baseline transforms to preserve skeletal hierarchy and positioning

### Missing Prerequisites

- Quaternion-based (or Euler-angle-based) keyframe data structure:
  ```cpp
  struct run_keyframe {
      glm::quat left_shoulder;   // Full 3D rotation (4 floats)
      glm::quat right_shoulder;
      glm::quat left_elbow;
      glm::quat right_elbow;
      glm::quat left_hip;
      glm::quat right_hip;
      glm::quat left_knee;
      glm::quat right_knee;
  };
  ```
- Clear separation of keyframe authoring from animation playback
- Understanding of per-joint rotation axes and composition rules (which quaternions handle automatically)

### Reconsideration Criteria

After implementing reduced-scope **Static Keyframe Preview** feature:
1. Upgrade keyframe struct to quaternions (or 3-component Euler angles minimum)
2. Store 4 hardcoded poses (no authoring UI initially)
3. GUI manual selection only (no locomotion phase computation)
4. Validates quaternion storage/application works before re-adding locomotion integration

### Principle Reinforcement

- **"Clarity over cleverness"** - attempting to encode 3D rotation in 1D was too clever by half
- **"Work bottom-up. Stabilize core gameplay before adding layers"** - should have validated keyframe representation works before adding locomotion integration  
- **"Short plans → build → test → repeat"** - plan attempted too much in single iteration (keyframes + locomotion + GUI + phase computation)
- **"Paradox of quality"** - multiple failed implementation attempts consumed time that could have been spent on simpler, validated approaches

### Code State

Implementation steps 1-4 completed but non-functional due to data structure limitation. Code preserved on branch `iteration/primary_skeletal_animation` for reference:
- `src/character/skeletal_animation.h/cpp` - module scaffolding with `run_keyframe` struct (float-based)
- `src/gui/character_panel.h/cpp` - GUI controls for keyframe editing
- `src/app/game_world.h/cpp` - runtime integration
- `CMakeLists.txt` - build system updates

All code compiles and runs without crashes, but joint rotations produce incorrect/unstable visual results due to insufficient rotation representation.

**Next Steps:** Revert or preserve as reference, implement Static Keyframe Preview with quaternion-based keyframes as separate minimal feature.
