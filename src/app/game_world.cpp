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
    cam_follow = camera_follow(); // Use default values
    scn = scene();
    setup_test_level(*this);
}

void game_world::update(float dt, const gui::character_panel_state& panel_state) {
    debug_list.clear();

    // Toggle control scheme
    if (input::is_key_pressed(SAPP_KEYCODE_T)) {
        current_control_scheme = (current_control_scheme == control_scheme::FREE_STRAFE)
                                      ? control_scheme::CAR_LIKE
                                      : control_scheme::FREE_STRAFE;
    }

    // Poll input and construct controller input params
    controller::controller_input_params input_params;
    input_params.move_direction = glm::vec2(0.0f, 0.0f);
    input_params.move_direction.y += input::is_key_down(SAPP_KEYCODE_W) ? 1.0f : 0.0f;
    input_params.move_direction.y -= input::is_key_down(SAPP_KEYCODE_S) ? 1.0f : 0.0f;
    input_params.move_direction.x -= input::is_key_down(SAPP_KEYCODE_A) ? 1.0f : 0.0f;
    input_params.move_direction.x += input::is_key_down(SAPP_KEYCODE_D) ? 1.0f : 0.0f;

    // Update heading in CAR_LIKE mode (composition layer responsibility)
    if (current_control_scheme == control_scheme::CAR_LIKE) {
        // Integrate heading from turn input (A/D)
        // Negated: A (negative x) turns left (increase yaw), D (positive x) turns right (decrease yaw)
        character.heading_yaw += -input_params.move_direction.x * character.turn_rate * dt;
        character.heading_yaw = math::wrap_angle_radians(character.heading_yaw);

        // Zero lateral input for controller (car control = no strafing)
        input_params.move_direction.x = 0.0f;
    }

    if (glm::length(input_params.move_direction) > 0.0f) {
        input_params.move_direction = glm::normalize(input_params.move_direction);
    }

    input_params.jump_pressed = input::is_key_pressed(SAPP_KEYCODE_SPACE);

    // Construct camera input params (basis selection based on control scheme)
    controller::camera_input_params cam_params;
    if (current_control_scheme == control_scheme::FREE_STRAFE) {
        // Camera-relative basis (existing behavior)
        cam_params.forward = cam.get_forward_horizontal();
        cam_params.right = cam.get_right();
    } else {
        // Heading-relative basis (car-like control)
        float yaw = character.heading_yaw;
        cam_params.forward = glm::vec3(glm::sin(yaw), 0.0f, glm::cos(yaw));
        cam_params.right = glm::vec3(glm::cos(yaw), 0.0f, -glm::sin(yaw));
    }

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
    collision_box ground_plane;
    ground_plane.bounds.center = glm::vec3(0.0f, -0.1f, 0.0f);
    ground_plane.bounds.half_extents = glm::vec3(100.0f, 0.1f, 100.0f);
    ground_plane.type = collision_surface_type::FLOOR;
    world.world_geometry.boxes.push_back(ground_plane);

    for (int i = 0; i < 5; ++i) {
        float height = 1.0f + static_cast<float>(i) * 1.5f;
        collision_box platform;
        platform.bounds.center = glm::vec3(0.0f, height, -5.0f - static_cast<float>(i) * 4.0f);
        platform.bounds.half_extents = glm::vec3(2.0f, 0.2f, 2.0f);
        platform.type = collision_surface_type::FLOOR;
        world.world_geometry.boxes.push_back(platform);
    }

    collision_box long_wall;
    long_wall.bounds.center = glm::vec3(6.0f, 2.0f, -10.0f);
    long_wall.bounds.half_extents = glm::vec3(0.2f, 2.0f, 8.0f);
    long_wall.type = collision_surface_type::WALL;
    world.world_geometry.boxes.push_back(long_wall);

    collision_box corner_wall_1;
    corner_wall_1.bounds.center = glm::vec3(-6.0f, 1.5f, -8.0f);
    corner_wall_1.bounds.half_extents = glm::vec3(0.2f, 1.5f, 4.0f);
    corner_wall_1.type = collision_surface_type::WALL;
    world.world_geometry.boxes.push_back(corner_wall_1);

    collision_box corner_wall_2;
    corner_wall_2.bounds.center = glm::vec3(-4.0f, 1.5f, -12.0f);
    corner_wall_2.bounds.half_extents = glm::vec3(2.0f, 1.5f, 0.2f);
    corner_wall_2.type = collision_surface_type::WALL;
    world.world_geometry.boxes.push_back(corner_wall_2);

    collision_box gap_wall_1;
    gap_wall_1.bounds.center = glm::vec3(3.0f, 1.0f, 2.0f);
    gap_wall_1.bounds.half_extents = glm::vec3(3.0f, 1.0f, 0.2f);
    gap_wall_1.type = collision_surface_type::WALL;
    world.world_geometry.boxes.push_back(gap_wall_1);

    collision_box gap_wall_2;
    gap_wall_2.bounds.center = glm::vec3(3.0f, 1.0f, 4.0f);
    gap_wall_2.bounds.half_extents = glm::vec3(3.0f, 1.0f, 0.2f);
    gap_wall_2.type = collision_surface_type::WALL;
    world.world_geometry.boxes.push_back(gap_wall_2);

    for (int i = 0; i < 4; ++i) {
        float height = 0.15f * static_cast<float>(i + 1);
        collision_box step;
        step.bounds.center = glm::vec3(-5.0f + static_cast<float>(i) * 2.0f, height * 0.5f, -8.0f);
        step.bounds.half_extents = glm::vec3(0.8f, height * 0.5f, 0.8f);
        step.type = collision_surface_type::FLOOR;
        world.world_geometry.boxes.push_back(step);
    }
}
