#include "app/game_world.h"
#include "foundation/math_utils.h"
#include "foundation/debug_assert.h"

#include "rendering/velocity_trail.h"
#include "input/input.h"
#include "input/keycodes.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <algorithm>

void game_world::init() {
    character = controller();
    vehicle_params.apply_to(character);
    cam = camera();
    cam_follow = camera_follow(); // Use default values
    scn = scene();
    setup_test_level(*this);
}

void game_world::update(float dt) {
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

    // A/D input serves two purposes:
    // 1. Turn input (always drives heading integration in controller)
    // 2. Lateral movement (only in FREE_STRAFE mode)
    float lateral_input = 0.0f;
    lateral_input -= input::is_key_down(SAPP_KEYCODE_A) ? 1.0f : 0.0f;
    lateral_input += input::is_key_down(SAPP_KEYCODE_D) ? 1.0f : 0.0f;

    // Always set turn_input (drives heading physics)
    input_params.turn_input = lateral_input;

    // Mode-dependent: lateral movement only in FREE_STRAFE
    input_params.move_direction.x =
        (current_control_scheme == control_scheme::FREE_STRAFE)
            ? lateral_input // FREE_STRAFE: lateral input creates strafing
            : 0.0f;         // CAR_LIKE: no lateral movement (turn only)

    if (glm::length(input_params.move_direction) > 0.0f) {
        input_params.move_direction = glm::normalize(input_params.move_direction);
    }

    // Validate normalized input direction
    float input_length = glm::length(input_params.move_direction);
    FL_POSTCONDITION(input_length == 0.0f || glm::epsilonEqual(input_length, 1.0f, 0.001f),
                     "input direction must be zero or normalized");

    // Construct camera input params (basis selection based on control scheme)
    controller::camera_input_params cam_params;
    if (current_control_scheme == control_scheme::FREE_STRAFE) {
        // Camera-relative basis (existing behavior)
        cam_params.forward = cam.get_forward_horizontal();
        cam_params.right = cam.get_right();
    } else {
        // Heading-relative basis (car-like control)
        float yaw = character.heading_yaw;
        cam_params.forward = math::yaw_to_forward(yaw);
        cam_params.right = math::yaw_to_right(yaw);

        // Validate orthonormal basis from heading
        FL_ASSERT_NORMALIZED(cam_params.forward, "heading-derived forward vector");
        FL_ASSERT_NORMALIZED(cam_params.right, "heading-derived right vector");
        FL_ASSERT_ORTHOGONAL(cam_params.forward, cam_params.right, "heading-derived basis vectors");
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

    // Update camera position based on mode
    glm::vec3 eye_position;
    if (cam_follow.mode == camera_mode::LOCK_TO_ORIENTATION) {
        // Compute forward direction from orientation system
        float yaw = character_visuals.orientation.get_yaw();
        glm::vec3 forward_dir = math::yaw_to_forward(yaw);

        eye_position = camera_follow::compute_locked_eye_position(
            character.position, forward_dir, cam_follow.distance, cam_follow.height_offset);
    } else {
        // Free orbit mode (existing behavior)
        eye_position = cam_follow.compute_eye_position(character.position);
    }

    cam.set_position(eye_position);
    cam.set_target(cam_follow.compute_look_target(character.position));
}

void game_world::apply_camera_orbit(float delta_x, float delta_y) {
    cam_follow.orbit(delta_x, delta_y);
}

void game_world::apply_camera_zoom(float delta) {
    cam_follow.zoom(delta);
}

void setup_test_level(game_world& world) {
    // Platform system geometry
    constexpr float PLATFORM_BASE_HEIGHT = 1.0f;
    constexpr float PLATFORM_HEIGHT_INCREMENT = 1.5f;
    constexpr float PLATFORM_Z_START = -5.0f;
    constexpr float PLATFORM_Z_SPACING = 4.0f;
    constexpr float PLATFORM_HALF_WIDTH = 2.0f;
    constexpr float PLATFORM_HALF_THICKNESS = 0.2f;
    constexpr int PLATFORM_COUNT = 5;

    // Wall geometry
    constexpr float WALL_THICKNESS = 0.2f;

    // Step geometry
    constexpr float STEP_HEIGHT_INCREMENT = 0.15f;
    constexpr float STEP_X_START = -5.0f;
    constexpr float STEP_X_SPACING = 2.0f;
    constexpr float STEP_HALF_EXTENT = 0.8f;
    constexpr int STEP_COUNT = 4;

    foundation::wireframe_mesh floor = foundation::generate_grid_floor(40.0f, 40);
    world.scn.add_object(floor);

    // Ground collision plane (replaces special-case ground at y=0)
    collision_box ground_plane;
    ground_plane.bounds.center = glm::vec3(0.0f, -0.1f, 0.0f);
    ground_plane.bounds.half_extents = glm::vec3(100.0f, 0.1f, 100.0f);
    ground_plane.type = collision_surface_type::FLOOR;
    world.world_geometry.boxes.push_back(ground_plane);

    for (int i = 0; i < PLATFORM_COUNT; ++i) {
        float height = PLATFORM_BASE_HEIGHT + static_cast<float>(i) * PLATFORM_HEIGHT_INCREMENT;
        collision_box platform;
        platform.bounds.center = glm::vec3(0.0f, height, PLATFORM_Z_START - static_cast<float>(i) * PLATFORM_Z_SPACING);
        platform.bounds.half_extents = glm::vec3(PLATFORM_HALF_WIDTH, PLATFORM_HALF_THICKNESS, PLATFORM_HALF_WIDTH);
        platform.type = collision_surface_type::FLOOR;
        world.world_geometry.boxes.push_back(platform);
    }

    collision_box long_wall;
    long_wall.bounds.center = glm::vec3(6.0f, 2.0f, -10.0f);
    long_wall.bounds.half_extents = glm::vec3(WALL_THICKNESS, 2.0f, 8.0f);
    long_wall.type = collision_surface_type::WALL;
    world.world_geometry.boxes.push_back(long_wall);

    collision_box corner_wall_1;
    corner_wall_1.bounds.center = glm::vec3(-6.0f, 1.5f, -8.0f);
    corner_wall_1.bounds.half_extents = glm::vec3(WALL_THICKNESS, 1.5f, 4.0f);
    corner_wall_1.type = collision_surface_type::WALL;
    world.world_geometry.boxes.push_back(corner_wall_1);

    collision_box corner_wall_2;
    corner_wall_2.bounds.center = glm::vec3(-4.0f, 1.5f, -12.0f);
    corner_wall_2.bounds.half_extents = glm::vec3(2.0f, 1.5f, WALL_THICKNESS);
    corner_wall_2.type = collision_surface_type::WALL;
    world.world_geometry.boxes.push_back(corner_wall_2);

    collision_box gap_wall_1;
    gap_wall_1.bounds.center = glm::vec3(3.0f, 1.0f, 2.0f);
    gap_wall_1.bounds.half_extents = glm::vec3(3.0f, 1.0f, WALL_THICKNESS);
    gap_wall_1.type = collision_surface_type::WALL;
    world.world_geometry.boxes.push_back(gap_wall_1);

    collision_box gap_wall_2;
    gap_wall_2.bounds.center = glm::vec3(3.0f, 1.0f, 4.0f);
    gap_wall_2.bounds.half_extents = glm::vec3(3.0f, 1.0f, WALL_THICKNESS);
    gap_wall_2.type = collision_surface_type::WALL;
    world.world_geometry.boxes.push_back(gap_wall_2);

    for (int i = 0; i < STEP_COUNT; ++i) {
        float height = STEP_HEIGHT_INCREMENT * static_cast<float>(i + 1);
        collision_box step;
        step.bounds.center = glm::vec3(STEP_X_START + static_cast<float>(i) * STEP_X_SPACING, height * 0.5f, -8.0f);
        step.bounds.half_extents = glm::vec3(STEP_HALF_EXTENT, height * 0.5f, STEP_HALF_EXTENT);
        step.type = collision_surface_type::FLOOR;
        world.world_geometry.boxes.push_back(step);
    }
}
