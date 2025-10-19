#include "app/game_world.h"
#include "foundation/math_utils.h"

#include "gui/character_panel.h"
#include "rendering/velocity_trail.h"
#include "input/input.h"
#include "input/keycodes.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <algorithm>

void game_world::init() {
    character = controller();
    character_params.apply_to(character);
    cam = camera();
    cam_follow = camera_follow();  // Use default values
    scn = scene();
    setup_test_level(*this);
}

void game_world::update(float dt, const gui::character_panel_state& panel_state) {
    debug_list.clear();

    // Poll input and construct controller input params
    controller::controller_input_params input_params;
    input_params.move_direction = glm::vec2(0.0f, 0.0f);
    input_params.move_direction.y += input::is_key_down(SAPP_KEYCODE_W) ? 1.0f : 0.0f;
    input_params.move_direction.y -= input::is_key_down(SAPP_KEYCODE_S) ? 1.0f : 0.0f;
    input_params.move_direction.x -= input::is_key_down(SAPP_KEYCODE_A) ? 1.0f : 0.0f;
    input_params.move_direction.x += input::is_key_down(SAPP_KEYCODE_D) ? 1.0f : 0.0f;
    if (glm::length(input_params.move_direction) > 0.0f) {
        input_params.move_direction = glm::normalize(input_params.move_direction);
    }
    input_params.jump_pressed = input::is_key_pressed(SAPP_KEYCODE_SPACE);

    // Construct camera input params
    controller::camera_input_params cam_params;
    cam_params.forward = cam.get_forward_horizontal();
    cam_params.right = cam.get_right();

    character.apply_input(input_params, cam_params, dt);

    character.update(&world_geometry, dt);

    // Update reactive visual systems after physics
    character_visuals.update(character, dt);

    // Sample velocity trail
    trail_state.time_since_last_sample += dt;
    if (trail_state.time_since_last_sample >= trail_state.sample_interval) {
        bool has_positions = !trail_state.positions.empty();
        bool position_changed = !has_positions || glm::distance(trail_state.positions.back(),
                                                                character.position) > 1e-4f;

        if (position_changed) {
            if (trail_state.positions.size() >= MAX_TRAIL_SAMPLES) {
                // Remove oldest sample
                trail_state.positions.erase(trail_state.positions.begin());
                trail_state.timestamps.erase(trail_state.timestamps.begin());
            }

            trail_state.positions.push_back(character.position);

            // Use relative timestamp (seconds since first sample)
            float current_time = trail_state.timestamps.empty()
                                     ? 0.0f
                                     : trail_state.timestamps.back() + trail_state.sample_interval;
            trail_state.timestamps.push_back(current_time);
            trail_state.time_since_last_sample = 0.0f;
        }
    }

    // Update camera position from follow controller
    cam.set_position(cam_follow.compute_eye_position(character.position));
    cam.set_target(cam_follow.compute_look_target(character.position));
}

void game_world::apply_camera_orbit(float delta_x, float delta_y) {
    cam_follow.orbit(delta_x, delta_y);
}

void game_world::apply_camera_zoom(float delta) {
    cam_follow.zoom(delta);
}

void setup_test_level(game_world& world) {
    foundation::wireframe_mesh floor = foundation::generate_grid_floor(40.0f, 40);
    world.scn.add_object(floor);

    // Ground collision plane (replaces special-case ground at y=0)
    aabb ground_plane;
    ground_plane.center = glm::vec3(0.0f, -0.1f, 0.0f);
    ground_plane.half_extents = glm::vec3(100.0f, 0.1f, 100.0f);
    world.world_geometry.boxes.push_back(ground_plane);

    for (int i = 0; i < 5; ++i) {
        float height = 1.0f + static_cast<float>(i) * 1.5f;
        aabb platform;
        platform.center = glm::vec3(0.0f, height, -5.0f - static_cast<float>(i) * 4.0f);
        platform.half_extents = glm::vec3(2.0f, 0.2f, 2.0f);
        world.world_geometry.boxes.push_back(platform);
    }

    aabb long_wall;
    long_wall.center = glm::vec3(6.0f, 2.0f, -10.0f);
    long_wall.half_extents = glm::vec3(0.2f, 2.0f, 8.0f);
    world.world_geometry.boxes.push_back(long_wall);

    aabb corner_wall_1;
    corner_wall_1.center = glm::vec3(-6.0f, 1.5f, -8.0f);
    corner_wall_1.half_extents = glm::vec3(0.2f, 1.5f, 4.0f);
    world.world_geometry.boxes.push_back(corner_wall_1);

    aabb corner_wall_2;
    corner_wall_2.center = glm::vec3(-4.0f, 1.5f, -12.0f);
    corner_wall_2.half_extents = glm::vec3(2.0f, 1.5f, 0.2f);
    world.world_geometry.boxes.push_back(corner_wall_2);

    aabb gap_wall_1;
    gap_wall_1.center = glm::vec3(3.0f, 1.0f, 2.0f);
    gap_wall_1.half_extents = glm::vec3(3.0f, 1.0f, 0.2f);
    world.world_geometry.boxes.push_back(gap_wall_1);

    aabb gap_wall_2;
    gap_wall_2.center = glm::vec3(3.0f, 1.0f, 4.0f);
    gap_wall_2.half_extents = glm::vec3(3.0f, 1.0f, 0.2f);
    world.world_geometry.boxes.push_back(gap_wall_2);

    for (int i = 0; i < 4; ++i) {
        float height = 0.15f * static_cast<float>(i + 1);
        aabb step;
        step.center = glm::vec3(-5.0f + static_cast<float>(i) * 2.0f, height * 0.5f, -8.0f);
        step.half_extents = glm::vec3(0.8f, height * 0.5f, 0.8f);
        world.world_geometry.boxes.push_back(step);
    }
}
