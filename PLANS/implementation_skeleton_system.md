# Implementation Plan: Minimum Viable Skeleton System

### Graybox Implementation Checklist

#### 1. Data Structures & State

- [ ] **File:** `src/character/skeleton.h`
    - [ ] Create a new file `src/character/skeleton.h`.
    - [ ] Document transform conventions at the top of the file:
      - Units: meters; Up-axis: Y (project default).
      - Column-major glm; model = parent.model * local.
      - `constexpr int NO_PARENT = -1;` root at index 0 uses NO_PARENT.
    - [ ] Define a `joint` struct containing:
        - `glm::mat4 model_transform`
        - `glm::mat4 local_transform`
        - `int parent_index`
        - `const char* name`
    - [ ] Define a `skeleton` struct containing:
        - `std::vector<joint> joints`

#### 2. Transforms

- [ ] **File:** `src/character/skeleton.cpp`
    - [ ] Create a new file.
    - [ ] Implement `void update_global_transforms(skeleton& skel)`:
        - [ ] Assert `joints[0].parent_index == NO_PARENT`.
        - [ ] For each joint in parent-first order, set `model_transform = (parent_index == NO_PARENT) ? local_transform : skel.joints[parent_index].model_transform * local_transform;`
        - [ ] Add debug asserts: valid parent indices for non-root; no cycles (optional simple guard via visited set in debug builds).

#### 3. T-Pose Asset

- [ ] **File:** `src/character/t_pose.h`
    - [ ] Create a new file `src/character/t_pose.h`.
    - [ ] Declare `void create_t_pose(skeleton& skel);`.
- [ ] **File:** `src/character/t_pose.cpp`
    - [ ] Implement `create_t_pose` to construct a hardcoded humanoid T-pose.
        - [ ] Populate `joints` with root first; set `parent_index` for children.
        - [ ] Initialize each joint `local_transform`; leave `model_transform` to be computed by `update_global_transforms`.

#### 4. Rendering

- [ ] **File:** `src/rendering/debug_draw.h`
    - [ ] Add the include `#include "character/skeleton.h"`.
    - [ ] In `namespace debug { }`, declare: `void draw_skeleton(draw_context& ctx, const skeleton& skel);`.
- [ ] **File:** `src/rendering/debug_draw.cpp`
    - [ ] Implement `debug::draw_skeleton`.
    - [ ] Iterate joints; compute joint positions from `model_transform`.
    - [ ] Draw joints as small spheres or points; draw bones as lines between parent and child. (Fallback if capsules/boxes are unavailable.)

#### 5. Integration

- [ ] **File:** `src/app/runtime.h`
    - [ ] Include `character/skeleton.h`.
    - [ ] Add `skeleton t_pose_skeleton;` to `app_runtime`.
- [ ] **File:** `src/app/runtime.cpp`
    - [ ] Include `character/t_pose.h` and `character/skeleton.h`.
    - [ ] In `app_runtime::initialize()`, call `create_t_pose(t_pose_skeleton);` then `update_global_transforms(t_pose_skeleton);`.
    - [ ] In `app_runtime::render_world()`, call `debug::draw_skeleton(debug_ctx, t_pose_skeleton);`.

#### 6. Debug UI (Toggle & Labels)

- [ ] **File:** `src/gui/character_panel.h/.cpp`
    - [ ] Add a `bool show_skeleton` and `bool show_joint_labels` in a suitable GUI state/config.
    - [ ] Add checkboxes in the Character panel: “Show Skeleton”, “Show Joint Labels”.
    - [ ] When `show_skeleton` is true, call `debug::draw_skeleton`; when `show_joint_labels` is true, draw joint names near joint positions.

#### 7. Minimal Animated-Joint Verification

- [ ] **File:** `src/app/runtime.cpp`
    - [ ] In a temporary test path (guarded by a debug flag), apply a small rotation to a forearm joint’s `local_transform` each frame; call `update_global_transforms(t_pose_skeleton)`; verify expected motion on screen.

#### 8. Quality Gates

- [ ] Build passes (`CMake: Build (Debug)`).
- [ ] Lint/format clean (run clang-format/clang-tidy tasks).
- [ ] Smoke run compiles and launches (`Run (Debug)`), UI toggles work, and animated-joint verification behaves correctly.

