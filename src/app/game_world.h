#pragma once

#include "camera/camera.h"
#include "camera/camera_follow.h"
#include "camera/dynamic_fov.h"
#include "vehicle/controller.h"
#include "vehicle/tuning.h"
#include "vehicle/vehicle_reactive_systems.h"
#include "character/character_reactive_systems.h"
#include "rendering/scene.h"
#include "rendering/velocity_trail.h"
#include "foundation/collision_primitives.h"
#include "rendering/debug_primitives.h"
#include <glm/glm.hpp>
#include <vector>

struct game_world {
    camera cam;
    camera_follow cam_follow;
    dynamic_fov_system dynamic_fov;
    scene scn;
    collision_world world_geometry;
    controller character;
    vehicle_reactive_systems vehicle_reactive;
    vehicle::tuning_params vehicle_params;
    velocity_trail_state trail_state;

    debug::debug_primitive_list debug_list;

    void init();
    void update(float dt);

    // Camera input forwarding
    void apply_camera_orbit(float delta_x, float delta_y);
    void apply_camera_zoom(float delta);
};

void setup_test_level(game_world& world);