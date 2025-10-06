# Implementation Plan: Refactor Game World Separation

### Graybox Implementation Checklist

#### 1. Create Game World Header

- [ ] **File:** `src/app/game_world.h`
    - [ ] Add pragma once guard
    - [ ] Include necessary headers:
        - [ ] `camera/camera.h`
        - [ ] `character/controller.h`
        - [ ] `character/locomotion.h`
        - [ ] `character/orientation.h`
        - [ ] `character/tuning.h`
        - [ ] `character/skeleton.h`
        - [ ] `rendering/scene.h`
        - [ ] `gui/character_panel.h` (forward declare `gui::character_panel_state`)
    - [ ] Define `game_world` struct with public members:
        - [ ] `camera cam;`
        - [ ] `scene scn;`
        - [ ] `controller character;`
        - [ ] `orientation_system orientation;`
        - [ ] `locomotion_system locomotion;`
        - [ ] `character::tuning_params character_params;`
        - [ ] `character::skeleton t_pose_skeleton;`
        - [ ] `float wheel_spin_angle = 0.0f;`
        - [ ] `float skeleton_animation_time = 0.0f;`
    - [ ] Declare public methods:
        - [ ] `void init();` — Initialize all simulation systems
        - [ ] `void update(float dt, const gui::character_panel_state& panel_state);` — Update simulation state
    - [ ] Declare standalone function: `void setup_test_level(scene& scn);`

#### 2. Create Game World Implementation

- [ ] **File:** `src/app/game_world.cpp`
    - [ ] Include `app/game_world.h`
    - [ ] Include additional headers needed for implementation:
        - [ ] `character/t_pose.h`
        - [ ] `character/skeleton.h`
        - [ ] `rendering/wireframe.h`
        - [ ] `<glm/gtc/matrix_transform.hpp>`
        - [ ] `<cmath>`
        - [ ] `<algorithm>`
    - [ ] Add anonymous namespace constants:
        - [ ] `constexpr float WHEEL_RADIUS = 0.45f;`
        - [ ] `constexpr float TWO_PI = 6.28318530718f;`
    - [ ] Implement `void game_world::init()`:
        - [ ] Initialize character: `character = controller();`
        - [ ] Initialize orientation: `orientation = orientation_system();`
        - [ ] Initialize locomotion: `locomotion = locomotion_system();`
        - [ ] Read character params: `character_params.read_from(character);`
        - [ ] Sync locomotion targets: `character::sync_locomotion_targets(character, locomotion);`
        - [ ] Initialize camera: `cam = camera(character.position, orbit_config{5.0f, 15.0f, 0.0f});`
        - [ ] Set camera mode: `cam.set_mode(camera_mode::follow);`
        - [ ] Initialize scene: `scn = scene();`
        - [ ] Call `setup_test_level(scn);` to populate scene
        - [ ] Create T-pose skeleton: `create_t_pose(t_pose_skeleton);`
        - [ ] Update skeleton transforms: `character::update_global_transforms(t_pose_skeleton);`
    - [ ] Implement `void game_world::update(float dt, const gui::character_panel_state& panel_state)`:
        - [ ] Apply character input: `character.apply_input(cam, dt);`
        - [ ] Update character physics: `character.update(&scn, dt);`
        - [ ] Calculate horizontal velocity: `glm::vec3 horizontal_velocity = character.velocity; horizontal_velocity.y = 0.0f;`
        - [ ] Calculate intended velocity: `glm::vec3 intended_velocity = character.input_direction * character.max_speed;`
        - [ ] Update orientation: `orientation.update(intended_velocity, dt);`
        - [ ] Update landing spring animation: `character.animation.update_landing_spring(character.just_landed, character.vertical_velocity_on_land, dt);`
        - [ ] Clear landed flag: `character.just_landed = false;`
        - [ ] Update acceleration tilt animation: `character.animation.update_acceleration_tilt(character.last_acceleration, character.velocity, character.max_speed, orientation.get_yaw(), dt);`
        - [ ] Sync locomotion targets: `character::sync_locomotion_targets(character, locomotion);`
        - [ ] Update locomotion: `locomotion.update(horizontal_velocity, dt, character.is_grounded);`
        - [ ] Calculate wheel angular speed and update angle (with wraparound)
        - [ ] Update camera follow if in follow mode: `if (cam.get_mode() == camera_mode::follow) { cam.follow_update(character.position, dt); }`
        - [ ] Animate skeleton if enabled (check `panel_state.animate_skeleton`, update `skeleton_animation_time`, apply elbow rotation, update transforms)
    - [ ] Implement `void setup_test_level(scene& scn)`:
        - [ ] Generate and add grid floor: `wireframe_mesh floor = generate_grid_floor(40.0f, 40); scn.add_object(floor);`
        - [ ] Add 5 platforms in loop (heights 1.0 to 7.0, z positions -5 to -21, size 2x0.2x2)
        - [ ] Add long wall for sustained sliding (center 6,2,-10, extents 0.2x2x8)
        - [ ] Add corner wall 1 (center -6,1.5,-8, extents 0.2x1.5x4)
        - [ ] Add corner wall 2 (center -4,1.5,-12, extents 2x1.5x0.2)
        - [ ] Add gap wall 1 (center 3,1,2, extents 3x1x0.2)
        - [ ] Add gap wall 2 (center 3,1,4, extents 3x1x0.2)
        - [ ] Add 4 graduated steps in loop (heights 0.15, 0.30, 0.45, 0.60, x positions -5 to -1, z -8, size 0.8x(height/2)x0.8)

#### 3. Update CMakeLists.txt

- [ ] **File:** `CMakeLists.txt`
    - [ ] Add `src/app/game_world.cpp` to the `add_executable(FrogLords ...)` source list (after `src/app/runtime.cpp`)

#### 4. Refactor App Runtime Header

- [ ] **File:** `src/app/runtime.h`
    - [ ] Add include: `#include "app/game_world.h"`
    - [ ] Remove includes (now in game_world.h):
        - [ ] `camera/camera.h`
        - [ ] `character/controller.h`
        - [ ] `character/locomotion.h`
        - [ ] `character/orientation.h`
        - [ ] `character/tuning.h`
        - [ ] `rendering/scene.h`
        - [ ] `gui/character_panel.h`
        - [ ] `character/skeleton.h`
    - [ ] Keep includes:
        - [ ] `sokol_gfx.h`
        - [ ] `rendering/renderer.h`
        - [ ] `rendering/wireframe.h`
        - [ ] `<glm/glm.hpp>`
    - [ ] In `app_runtime` private members, replace simulation state members with:
        - [ ] Remove: `camera cam;`
        - [ ] Remove: `scene scn;`
        - [ ] Remove: `controller character;`
        - [ ] Remove: `orientation_system orientation;`
        - [ ] Remove: `locomotion_system locomotion;`
        - [ ] Remove: `character::tuning_params character_params;`
        - [ ] Remove: `gui::character_panel_state panel_state;`
        - [ ] Remove: `float wheel_spin_angle;`
        - [ ] Remove: `float skeleton_animation_time;`
        - [ ] Remove: `character::skeleton t_pose_skeleton;`
        - [ ] Add: `game_world world;`
        - [ ] Keep: `gui::character_panel_state panel_state;` (needed for frame loop)

#### 5. Refactor App Runtime Implementation

- [ ] **File:** `src/app/runtime.cpp`
    - [ ] Remove includes (now in game_world.cpp):
        - [ ] `character/tuning.h`
        - [ ] `character/t_pose.h`
        - [ ] `character/skeleton.h`
        - [ ] `<glm/gtc/matrix_transform.hpp>`
        - [ ] `<cmath>`
        - [ ] `<algorithm>`
    - [ ] Remove anonymous namespace constants (moved to game_world.cpp):
        - [ ] Remove `WHEEL_RADIUS` and `TWO_PI`
    - [ ] In `app_runtime::initialize()`:
        - [ ] Remove all character/orientation/locomotion/camera initialization (lines 47-57)
        - [ ] Remove all scene initialization and test level geometry (lines 59-118)
        - [ ] Remove T-pose skeleton creation (lines 120-121)
        - [ ] Add after `renderer.init();`: `world.init();`
    - [ ] In `app_runtime::frame()`:
        - [ ] Update camera orbit control to use `world.cam` instead of `cam`
        - [ ] Update camera zoom to use `world.cam` instead of `cam`
        - [ ] Update GUI call: `gui::draw_character_panel(panel_state, world.character, world.locomotion, world.orientation, world.character_params);`
    - [ ] In `app_runtime::update_simulation()`:
        - [ ] Replace entire function body with: `world.update(dt, panel_state);`
    - [ ] In `app_runtime::render_world()`:
        - [ ] Update `renderer.draw()` loop to use `world.scn.objects()` instead of `scn.objects()`
        - [ ] Update `debug::draw_context` to use `world.cam` instead of `cam`
        - [ ] Update all debug draw calls to use `world.character`, `world.locomotion`, `world.orientation`, `world.scn`, `world.wheel_spin_angle`
        - [ ] Update skeleton rendering to use `world.t_pose_skeleton` and check `panel_state.show_skeleton`/`panel_state.animate_skeleton`/`panel_state.show_joint_labels`

#### 6. Quality Gates

- [ ] **Build:** Run task "CMake: Build (Debug)" — must complete with no errors or warnings
- [ ] **Format:** Run task "Format: clang-format src" — must complete cleanly
- [ ] **Lint:** Run task "Lint: clang-tidy src" — must complete with no new warnings
- [ ] **Smoke Test:** Run task "Run (Debug)" — application launches successfully
- [ ] **Visual Verification:** 
    - [ ] Character moves identically to before refactor
    - [ ] Camera controls (orbit, zoom, follow) work identically
    - [ ] Collision with all test geometry works identically
    - [ ] GUI character panel displays all values correctly
    - [ ] Skeleton debug visualization works identically
    - [ ] All debug draw overlays render correctly

#### 7. Commit

- [ ] **Git:** Stage all changes and commit with message: `refactor: separate game_world from app_runtime`
