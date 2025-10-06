#include "app/game_world.h"
#include "character/t_pose.h"
#include "character/skeleton.h"
#include "rendering/wireframe.h"
#include "gui/character_panel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <algorithm>

namespace {
constexpr float WHEEL_RADIUS = 0.45f;
constexpr float TWO_PI = 6.28318530718f;
} // namespace

void game_world::init() {
    character = controller();
    orientation = orientation_system();
    locomotion = locomotion_system();
    character_params.read_from(character);
    character::sync_locomotion_targets(character, locomotion);
    cam = camera(character.position, orbit_config{5.0f, 15.0f, 0.0f});
    cam.set_mode(camera_mode::follow);
    scn = scene();
    setup_test_level(scn);
    create_t_pose(t_pose_skeleton);
    character::update_global_transforms(t_pose_skeleton);
}

void game_world::update(float dt, const gui::character_panel_state& panel_state) {
    character.apply_input(cam, dt);
    character.update(&scn, dt);

    glm::vec3 horizontal_velocity = character.velocity;
    horizontal_velocity.y = 0.0f;

    glm::vec3 intended_velocity = character.input_direction * character.max_speed;

    orientation.update(intended_velocity, dt);

    character.animation.update_landing_spring(character.just_landed,
                                              character.vertical_velocity_on_land, dt);
    character.just_landed = false;
    character.animation.update_acceleration_tilt(character.last_acceleration, character.velocity,
                                                 character.max_speed, orientation.get_yaw(), dt);

    character::sync_locomotion_targets(character, locomotion);
    locomotion.update(horizontal_velocity, dt, character.is_grounded);

    float angular_speed = 0.0f;
    if (WHEEL_RADIUS > 0.0001f) {
        angular_speed = locomotion.current_speed / WHEEL_RADIUS;
    }
    wheel_spin_angle += angular_speed * dt;
    if (wheel_spin_angle > TWO_PI) {
        wheel_spin_angle = std::fmod(wheel_spin_angle, TWO_PI);
    }

    if (cam.get_mode() == camera_mode::follow) {
        cam.follow_update(character.position, dt);
    }

    if (panel_state.animate_skeleton) {
        skeleton_animation_time += dt;
        float angle = 0.5f * std::sin(skeleton_animation_time * 2.0f);
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
        t_pose_skeleton.joints[6].local_transform =
            rotation * glm::translate(glm::mat4(1.0f), glm::vec3(-0.15f, 0.0f, 0.0f));
        character::update_global_transforms(t_pose_skeleton);
    }
}

void setup_test_level(scene& scn) {
    wireframe_mesh floor = generate_grid_floor(40.0f, 40);
    scn.add_object(floor);

    for (int i = 0; i < 5; ++i) {
        float height = 1.0f + static_cast<float>(i) * 1.5f;
        aabb platform;
        platform.center = glm::vec3(0.0f, height, -5.0f - static_cast<float>(i) * 4.0f);
        platform.half_extents = glm::vec3(2.0f, 0.2f, 2.0f);
        scn.add_collision_box(platform);
    }

    aabb long_wall;
    long_wall.center = glm::vec3(6.0f, 2.0f, -10.0f);
    long_wall.half_extents = glm::vec3(0.2f, 2.0f, 8.0f);
    scn.add_collision_box(long_wall);

    aabb corner_wall_1;
    corner_wall_1.center = glm::vec3(-6.0f, 1.5f, -8.0f);
    corner_wall_1.half_extents = glm::vec3(0.2f, 1.5f, 4.0f);
    scn.add_collision_box(corner_wall_1);

    aabb corner_wall_2;
    corner_wall_2.center = glm::vec3(-4.0f, 1.5f, -12.0f);
    corner_wall_2.half_extents = glm::vec3(2.0f, 1.5f, 0.2f);
    scn.add_collision_box(corner_wall_2);

    aabb gap_wall_1;
    gap_wall_1.center = glm::vec3(3.0f, 1.0f, 2.0f);
    gap_wall_1.half_extents = glm::vec3(3.0f, 1.0f, 0.2f);
    scn.add_collision_box(gap_wall_1);

    aabb gap_wall_2;
    gap_wall_2.center = glm::vec3(3.0f, 1.0f, 4.0f);
    gap_wall_2.half_extents = glm::vec3(3.0f, 1.0f, 0.2f);
    scn.add_collision_box(gap_wall_2);

    for (int i = 0; i < 4; ++i) {
        float height = 0.15f * static_cast<float>(i + 1);
        aabb step;
        step.center = glm::vec3(-5.0f + static_cast<float>(i) * 2.0f, height * 0.5f, -8.0f);
        step.half_extents = glm::vec3(0.8f, height * 0.5f, 0.8f);
        scn.add_collision_box(step);
    }
}