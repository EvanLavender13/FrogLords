# Implementation Plan: Minimum Viable Skeleton System

### Graybox Implementation Checklist

#### 1. Data Structures & State

- [x] **File:** `src/character/skeleton.h`
    - [x] Create a new file `src/character/skeleton.h`.
    - [x] Document transform conventions at the top of the file:
      - Units: meters; Up-axis: Y (project default).
      - Column-major glm; model = parent.model * local.
      - `constexpr int NO_PARENT = -1;` root at index 0 uses NO_PARENT.
    - [x] Define a `joint` struct containing:
        - `glm::mat4 model_transform`
        - `glm::mat4 local_transform`
        - `int parent_index`
        - `const char* name`
    - [x] Define a `skeleton` struct containing:
        - `std::vector<joint> joints`

#### 2. Transforms

- [x] **File:** `src/character/skeleton.cpp`
    - [x] Create a new file.
    - [x] Implement `void update_global_transforms(skeleton& skel)`:
        - [x] Assert `joints[0].parent_index == NO_PARENT`.
        - [x] For each joint in parent-first order, set `model_transform = (parent_index == NO_PARENT) ? local_transform : skel.joints[parent_index].model_transform * local_transform;`
        - [x] Add debug asserts: valid parent indices for non-root; no cycles (optional simple guard via visited set in debug builds).

#### 3. T-Pose Asset

- [x] **File:** `src/character/t_pose.h`
    - [x] Create a new file `src/character/t_pose.h`.
    - [x] Declare `void create_t_pose(skeleton& skel);`.
- [x] **File:** `src/character/t_pose.cpp`
    - [x] Implement `create_t_pose` to construct a hardcoded humanoid T-pose.
        - [x] Populate `joints` with root first; set `parent_index` for children.
        - [x] Initialize each joint `local_transform`; leave `model_transform` to be computed by `update_global_transforms`.
- [x] Add `src/character/t_pose.cpp` to CMakeLists.txt build sources.

#### 4. Rendering

- [x] **File:** `src/rendering/debug_draw.h`
    - [x] Add the include `#include "character/skeleton.h"`.
    - [x] In `namespace debug { }`, declare: `void draw_skeleton(draw_context& ctx, const skeleton& skel);`.
- [x] **File:** `src/rendering/debug_draw.cpp`
    - [x] Implement `debug::draw_skeleton`.
    - [x] Iterate joints; compute joint positions from `model_transform`.
    - [x] Draw joints as small spheres or points; draw bones as lines between parent and child. (Fallback if capsules/boxes are unavailable.)

#### 5. Integration

- [x] **File:** `src/app/runtime.h`
    - [x] Include `character/skeleton.h`.
    - [x] Add `skeleton t_pose_skeleton;` to `app_runtime`.
- [x] **File:** `src/app/runtime.cpp`
    - [x] Include `character/t_pose.h` and `character/skeleton.h`.
    - [x] In `app_runtime::initialize()`, call `create_t_pose(t_pose_skeleton);` then `update_global_transforms(t_pose_skeleton);`.
    - [x] In `app_runtime::render_world()`, call `debug::draw_skeleton(debug_ctx, t_pose_skeleton);`.

Notes:
- Joint parent indices in `src/character/t_pose.cpp` ensure a valid humanoid hierarchy (shoulders/elbows/wrists and hips/knees/ankles parent correctly).

#### 6. Debug UI (Toggle & Labels)

- [x] **File:** `src/gui/character_panel.h/.cpp`
    - [x] Add `bool show_skeleton = true;` and `bool show_joint_labels = false;` to `character_panel_state`.
    - [x] Add checkboxes in the Character panel's "Debug Info" section: "Show Skeleton", "Show Joint Labels".
    - [x] Skeleton rendering is controlled by the GUI toggles.

#### 7. Minimal Animated-Joint Verification

- [x] **File:** `src/app/runtime.cpp`
    - [x] In a temporary test path (guarded by a debug flag), apply a small rotation to a forearm joint's `local_transform` each frame; call `update_global_transforms(t_pose_skeleton)`; verify expected motion on screen.

Notes: Added GUI toggle "Animate Skeleton" in Character panel. Animates left_elbow with sinusoidal rotation around Z-axis. Verified build passes and runs.

#### 8. Quality Gates

- [x] Build passes (`CMake: Build (Debug)`).
- [x] Lint/format clean (run clang-format/clang-tidy tasks).
- [x] Smoke run compiles and launches (`Run (Debug)`), UI toggles work, and animated-joint verification behaves correctly.
