# Implementation Plan: Refactor Game World Separation

### Graybox Implementation Checklist

#### 1. Create Game World Header

- [x] **File:** `src/app/game_world.h`
    - [x] Add pragma once guard
    - [x] Include necessary headers:
        - [x] `camera/camera.h`
        - [x] `character/controller.h`
        - [x] `character/locomotion.h`
        - [x] `character/orientation.h`
        - [x] `character/tuning.h`
        - [x] `character/skeleton.h`
        - [x] `rendering/scene.h`
        - [x] `gui/character_panel.h` (forward declare `gui::character_panel_state`)
    - [x] Define `game_world` struct with public members:
        - [x] `camera cam;`
        - [x] `scene scn;`
        - [x] `controller character;`
        - [x] `orientation_system orientation;`
        - [x] `locomotion_system locomotion;`
        - [x] `character::tuning_params character_params;`
        - [x] `character::skeleton t_pose_skeleton;`
        - [x] `float wheel_spin_angle = 0.0f;`
        - [x] `float skeleton_animation_time = 0.0f;`
    - [x] Declare public methods:
        - [x] `void init();` — Initialize all simulation systems
        - [x] `void update(float dt, const gui::character_panel_state& panel_state);` — Update simulation state
    - [x] Declare standalone function: `void setup_test_level(scene& scn);`

#### 2. Create Game World Implementation

- [x] **File:** `src/app/game_world.cpp`
    - [x] Include `app/game_world.h`
    - [x] Include additional headers needed for implementation:
        - [x] `character/t_pose.h`
        - [x] `character/skeleton.h`
        - [x] `rendering/wireframe.h`
        - [x] `<glm/gtc/matrix_transform.hpp>`
        - [x] `<cmath>`
        - [x] `<algorithm>`
    - [x] Add anonymous namespace constants:
        - [x] `constexpr float WHEEL_RADIUS = 0.45f;`
        - [x] `constexpr float TWO_PI = 6.28318530718f;`
    - [x] Implement `void game_world::init()`:
        - [x] Initialize character: `character = controller();`
        - [x] Initialize orientation: `orientation = orientation_system();`
        - [x] Initialize locomotion: `locomotion = locomotion_system();`
        - [x] Read character params: `character_params.read_from(character);`
        - [x] Sync locomotion targets: `character::sync_locomotion_targets(character, locomotion);`
        - [x] Initialize camera: `cam = camera(character.position, orbit_config{5.0f, 15.0f, 0.0f});`
        - [x] Set camera mode: `cam.set_mode(camera_mode::follow);`
        - [x] Initialize scene: `scn = scene();`
        - [x] Call `setup_test_level(scn);` to populate scene
        - [x] Create T-pose skeleton: `create_t_pose(t_pose_skeleton);`
        - [x] Update skeleton transforms: `character::update_global_transforms(t_pose_skeleton);`
    - [x] Implement `void game_world::update(float dt, const gui::character_panel_state& panel_state)`:
        - [x] Apply character input: `character.apply_input(cam, dt);`
        - [x] Update character physics: `character.update(&scn, dt);`
        - [x] Calculate horizontal velocity: `glm::vec3 horizontal_velocity = character.velocity; horizontal_velocity.y = 0.0f;`
        - [x] Calculate intended velocity: `glm::vec3 intended_velocity = character.input_direction * character.max_speed;`
        - [x] Update orientation: `orientation.update(intended_velocity, dt);`
        - [x] Update landing spring animation: `character.animation.update_landing_spring(character.just_landed, character.vertical_velocity_on_land, dt);`
        - [x] Clear landed flag: `character.just_landed = false;`
        - [x] Update acceleration tilt animation: `character.animation.update_acceleration_tilt(character.last_acceleration, character.velocity, character.max_speed, orientation.get_yaw(), dt);`
        - [x] Sync locomotion targets: `character::sync_locomotion_targets(character, locomotion);`
        - [x] Update locomotion: `locomotion.update(horizontal_velocity, dt, character.is_grounded);`
        - [x] Calculate wheel angular speed and update angle (with wraparound)
        - [x] Update camera follow if in follow mode: `if (cam.get_mode() == camera_mode::follow) { cam.follow_update(character.position, dt); }`
        - [x] Animate skeleton if enabled (check `panel_state.animate_skeleton`, update `skeleton_animation_time`, apply elbow rotation, update transforms)
    - [x] Implement `void setup_test_level(scene& scn)`:
        - [x] Generate and add grid floor: `wireframe_mesh floor = generate_grid_floor(40.0f, 40); scn.add_object(floor);`
        - [x] Add 5 platforms in loop (heights 1.0 to 7.0, z positions -5 to -21, size 2x0.2x2)
        - [x] Add long wall for sustained sliding (center 6,2,-10, extents 0.2x2x8)
        - [x] Add corner wall 1 (center -6,1.5,-8, extents 0.2x1.5x4)
        - [x] Add corner wall 2 (center -4,1.5,-12, extents 2x1.5x0.2)
        - [x] Add gap wall 1 (center 3,1,2, extents 3x1x0.2)
        - [x] Add gap wall 2 (center 3,1,4, extents 3x1x0.2)
        - [x] Add 4 graduated steps in loop (heights 0.15, 0.30, 0.45, 0.60, x positions -5 to -1, z -8, size 0.8x(height/2)x0.8)

#### 3. Update CMakeLists.txt

- [x] **File:** `CMakeLists.txt`
    - [x] Add `src/app/game_world.cpp` to the `add_executable(FrogLords ...)` source list (after `src/app/runtime.cpp`)

#### 4. Refactor App Runtime Header

- [x] **File:** `src/app/runtime.h`
    - [x] Add include: `#include "app/game_world.h"`
    - [x] Remove includes (now in game_world.h):
        - [x] `camera/camera.h`
        - [x] `character/controller.h`
        - [x] `character/locomotion.h`
        - [x] `character/orientation.h`
        - [x] `character/tuning.h`
        - [x] `rendering/scene.h`
        - [x] `character/skeleton.h`
    - [x] Keep includes:
        - [x] `sokol_gfx.h`
        - [x] `rendering/renderer.h`
        - [x] `rendering/wireframe.h`
        - [x] `<glm/glm.hpp>`
    - [x] In `app_runtime` private members, replace simulation state members with:
        - [x] Remove: `camera cam;`
        - [x] Remove: `scene scn;`
        - [x] Remove: `controller character;`
        - [x] Remove: `orientation_system orientation;`
        - [x] Remove: `locomotion_system locomotion;`
        - [x] Remove: `character::tuning_params character_params;`
        - [x] Remove: `gui::character_panel_state panel_state;`
        - [x] Remove: `float wheel_spin_angle;`
        - [x] Remove: `float skeleton_animation_time;`
        - [x] Remove: `character::skeleton t_pose_skeleton;`
        - [x] Add: `game_world world;`
        - [x] Keep: `gui::character_panel_state panel_state;` (needed for frame loop)

#### 5. Refactor App Runtime Implementation

- [x] **File:** `src/app/runtime.cpp`
    - [x] Remove includes (now in game_world.cpp):
        - [x] `character/tuning.h`
        - [x] `character/t_pose.h`
        - [x] `character/skeleton.h`
        - [x] `<glm/gtc/matrix_transform.hpp>`
        - [x] `<cmath>`
        - [x] `<algorithm>`
    - [x] Remove anonymous namespace constants (moved to game_world.cpp):
        - [x] Remove `WHEEL_RADIUS` and `TWO_PI`
    - [x] In `app_runtime::initialize()`:
        - [x] Remove all character/orientation/locomotion/camera initialization (lines 47-57)
        - [x] Remove all scene initialization and test level geometry (lines 59-118)
        - [x] Remove T-pose skeleton creation (lines 120-121)
        - [x] Add after `renderer.init();`: `world.init();`
    - [x] In `app_runtime::frame()`:
        - [x] Update camera orbit control to use `world.cam` instead of `cam`
        - [x] Update camera zoom to use `world.cam` instead of `cam`
        - [x] Update GUI call: `gui::draw_character_panel(panel_state, world.character, world.locomotion, world.orientation, world.character_params);`
    - [x] In `app_runtime::update_simulation()`:
        - [x] Replace entire function body with: `world.update(dt, panel_state);`
    - [x] In `app_runtime::render_world()`:
        - [x] Update `renderer.draw()` loop to use `world.scn.objects()` instead of `scn.objects()`
        - [x] Update `debug::draw_context` to use `world.cam` instead of `cam`
        - [x] Update all debug draw calls to use `world.character`, `world.locomotion`, `world.orientation`, `world.scn`, `world.wheel_spin_angle`
        - [x] Update skeleton rendering to use `world.t_pose_skeleton` and check `panel_state.show_skeleton`/`panel_state.animate_skeleton`/`panel_state.show_joint_labels`

#### 6. Quality Gates

- [x] **Build:** Run task "CMake: Build (Debug)" — must complete with no errors or warnings
- [x] **Format:** Run task "Format: clang-format src" — must complete cleanly
- [x] **Lint:** Run task "Lint: clang-tidy src" — must complete with no new warnings
- [x] **Smoke Test:** Run task "Run (Debug)" — application launches successfully
- [x] **Visual Verification:** 
    - [x] Character moves identically to before refactor
    - [x] Camera controls (orbit, zoom, follow) work identically
    - [x] Collision with all test geometry works identically
    - [x] GUI character panel displays all values correctly
    - [x] Skeleton debug visualization works identically
    - [x] All debug draw overlays render correctly

## Approval

- **Reviewer:** Gemini
- **Date:** 2025-10-06

## Completion

- **Date:** 2025-10-06
- **Checklist:** All items confirmed complete.
- **Code Review:** `PLANS/ARCHIVE/20251006_120000_code_review_refactor_game_world.md`
