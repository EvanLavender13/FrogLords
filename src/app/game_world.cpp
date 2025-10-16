#include "app/game_world.h"
#include "foundation/math_utils.h"

#include "gui/character_panel.h"
#include "rendering/velocity_trail.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <algorithm>

void game_world::init() {
    character = controller();
    character_params.read_from(character);
    cam = camera(character.position, orbit_config{5.0f, 15.0f, 0.0f});
    cam.set_mode(camera_mode::FOLLOW);
    scn = scene();
    setup_test_level(*this);
}

void game_world::update(float dt, const gui::character_panel_state& panel_state) {
    debug_list.clear();

    controller::camera_input_params cam_params;
    cam_params.forward = cam.get_forward_horizontal();
    cam_params.right = cam.get_right();
    character.apply_input(cam_params, dt);

    character.update(&world_geometry, dt);

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

    glm::vec3 horizontal_velocity = math::project_to_horizontal(character.velocity);

    glm::vec3 intended_velocity = character.input_direction * character.max_speed;

    character.orientation.update(intended_velocity, dt);

    character.animation.update_landing_spring(character.just_landed,
                                              character.vertical_velocity_on_land, dt);
    character.just_landed = false;
    character.animation.update_acceleration_tilt(character.last_acceleration, character.velocity,
                                                 character.run_speed,
                                                 character.orientation.get_yaw(), dt);

    character.locomotion.update(horizontal_velocity, dt);

    // Update contact/air weight (smooth transition for phase continuity)
    character.animation.update_contact_weight(character.is_grounded, dt);

    // Calculate wheel spin using dynamic radius from blended stride
    float wheel_radius = character.locomotion.get_blended_stride() / glm::two_pi<float>();
    float angular_speed = 0.0f;
    if (wheel_radius > 0.0001f) {
        angular_speed = character.locomotion.current_speed / wheel_radius;
    }
    wheel_spin_angle += angular_speed * dt;
    if (wheel_spin_angle > glm::two_pi<float>()) {
        wheel_spin_angle = std::fmod(wheel_spin_angle, glm::two_pi<float>());
    }

    if (cam.get_mode() == camera_mode::FOLLOW) {
        cam.follow_update(character.position, dt);
    }

    float walk_factor = 1.0f - character.locomotion.get_run_blend();
    smoothed_walk_factor += (walk_factor - smoothed_walk_factor) * walk_factor_smoothing * dt;


}

void setup_test_level(game_world& world) {
    foundation::wireframe_mesh floor = foundation::generate_grid_floor(40.0f, 40);
    world.scn.add_object(floor);

    for (int i = 0; i < 5; ++i) {
        float height = 1.0f + static_cast<float>(i) * 1.5f;
        aabb platform;
        platform.center = glm::vec3(0.0f, height, -5.0f - static_cast<float>(i) * 4.0f);
        platform.half_extents = glm::vec3(2.0f, 0.2f, 2.0f);
        world.scn.add_collision_box(platform);
        world.world_geometry.boxes.push_back(platform);
    }

    aabb long_wall;
    long_wall.center = glm::vec3(6.0f, 2.0f, -10.0f);
    long_wall.half_extents = glm::vec3(0.2f, 2.0f, 8.0f);
    world.scn.add_collision_box(long_wall);
    world.world_geometry.boxes.push_back(long_wall);

    aabb corner_wall_1;
    corner_wall_1.center = glm::vec3(-6.0f, 1.5f, -8.0f);
    corner_wall_1.half_extents = glm::vec3(0.2f, 1.5f, 4.0f);
    world.scn.add_collision_box(corner_wall_1);
    world.world_geometry.boxes.push_back(corner_wall_1);

    aabb corner_wall_2;
    corner_wall_2.center = glm::vec3(-4.0f, 1.5f, -12.0f);
    corner_wall_2.half_extents = glm::vec3(2.0f, 1.5f, 0.2f);
    world.scn.add_collision_box(corner_wall_2);
    world.world_geometry.boxes.push_back(corner_wall_2);

    aabb gap_wall_1;
    gap_wall_1.center = glm::vec3(3.0f, 1.0f, 2.0f);
    gap_wall_1.half_extents = glm::vec3(3.0f, 1.0f, 0.2f);
    world.scn.add_collision_box(gap_wall_1);
    world.world_geometry.boxes.push_back(gap_wall_1);

    aabb gap_wall_2;
    gap_wall_2.center = glm::vec3(3.0f, 1.0f, 4.0f);
    gap_wall_2.half_extents = glm::vec3(3.0f, 1.0f, 0.2f);
    world.scn.add_collision_box(gap_wall_2);
    world.world_geometry.boxes.push_back(gap_wall_2);

    for (int i = 0; i < 4; ++i) {
        float height = 0.15f * static_cast<float>(i + 1);
        aabb step;
        step.center = glm::vec3(-5.0f + static_cast<float>(i) * 2.0f, height * 0.5f, -8.0f);
        step.half_extents = glm::vec3(0.8f, height * 0.5f, 0.8f);
        world.scn.add_collision_box(step);
        world.world_geometry.boxes.push_back(step);
    }
}
