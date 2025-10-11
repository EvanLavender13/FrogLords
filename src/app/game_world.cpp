#include "app/game_world.h"
#include "character/t_pose.h"
#include "character/skeleton.h"
#include "character/keyframe.h"
#include "foundation/math_utils.h"
#include "rendering/wireframe.h"
#include "gui/character_panel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <algorithm>

void game_world::init() {
    character = controller();
    locomotion = locomotion_system();
    character_params.read_from(character);
    character::sync_locomotion_targets(character, locomotion);
    cam = camera(character.position, orbit_config{5.0f, 15.0f, 0.0f});
    cam.set_mode(camera_mode::FOLLOW);
    scn = scene();
    setup_test_level(scn);
    create_t_pose(t_pose_skeleton);
    character::update_global_transforms(t_pose_skeleton);
}

void game_world::update(float dt, const gui::character_panel_state& panel_state) {
    character.apply_input(cam, dt);
    character.update(&scn, dt);

    // Sample velocity trail
    trail_state.time_since_last_sample += dt;
    if (trail_state.time_since_last_sample >= trail_state.sample_interval) {
        // Add new sample
        if (trail_state.positions.size() >= 25) {
            // Remove oldest
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

    t_pose_skeleton.joints[0].local_transform = character.get_world_transform();

    glm::vec3 horizontal_velocity = math::project_to_horizontal(character.velocity);

    glm::vec3 intended_velocity = character.input_direction * character.max_speed;

    character.orientation.update(intended_velocity, dt);

    character.animation.update_landing_spring(character.just_landed,
                                              character.vertical_velocity_on_land, dt);
    character.just_landed = false;
    character.animation.update_acceleration_tilt(character.last_acceleration, character.velocity,
                                                 character.max_speed,
                                                 character.orientation.get_yaw(), dt);

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

    if (cam.get_mode() == camera_mode::FOLLOW) {
        cam.follow_update(character.position, dt);
    }

    // Update skeletal animation (automatic distance-phased or manual pose selection)
    character.animation.update_skeletal_animation(
        t_pose_skeleton, locomotion.distance_traveled, panel_state.selected_pose,
        panel_state.use_manual_pose_selection, panel_state.enable_secondary_motion, dt);

    // Apply joint overrides if enabled (debug feature)
    // Use the currently active pose (automatic or manual) as the base
    if (panel_state.enable_joint_overrides) {
        character::pose_type current_pose = panel_state.use_manual_pose_selection
                                                ? panel_state.selected_pose
                                                : character.animation.current_automatic_pose;

        character::apply_pose_with_overrides(
            t_pose_skeleton, current_pose, panel_state.left_shoulder_angles,
            panel_state.left_elbow_angles, panel_state.right_shoulder_angles,
            panel_state.right_elbow_angles, panel_state.left_hip_angles,
            panel_state.left_knee_angles, panel_state.right_hip_angles,
            panel_state.right_knee_angles);
    }

    // Propagate all local transform changes (pose + secondary motion + overrides) to global
    // transforms
    character::update_global_transforms(t_pose_skeleton);
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