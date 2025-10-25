#pragma once

#include "camera/camera.h"
#include "camera/camera_follow.h"
#include "vehicle/controller.h"
#include "vehicle/tuning.h"
#include "vehicle/vehicle_visual_systems.h"
#include "character/character_reactive_systems.h"
#include "rendering/scene.h"
#include "rendering/velocity_trail.h"
#include "foundation/collision_primitives.h"
#include "rendering/debug_primitives.h"
#include <glm/glm.hpp>
#include <vector>

enum class control_scheme { FREE_STRAFE, CAR_LIKE };

struct game_world {
    camera cam;
    camera_follow cam_follow;
    scene scn;
    collision_world world_geometry;
    controller character;
    vehicle_visual_systems vehicle_visuals;
    vehicle::tuning_params vehicle_params;
    velocity_trail_state trail_state;

    control_scheme current_control_scheme = control_scheme::FREE_STRAFE;

    debug::debug_primitive_list debug_list;

    void init();
    void update(float dt);

    // Camera input forwarding
    void apply_camera_orbit(float delta_x, float delta_y);
    void apply_camera_zoom(float delta);
};

void setup_test_level(game_world& world);