#pragma once

#include "camera/camera.h"
#include "character/controller.h"
#include "character/locomotion.h"
#include "character/tuning.h"
#include "character/skeleton.h"
#include "rendering/scene.h"
#include "rendering/velocity_trail.h"
#include "foundation/collision_primitives.h"
#include <glm/glm.hpp>
#include <vector>

namespace gui {
struct character_panel_state;
}

struct game_world {
    camera cam;
    scene scn;
    collision_world world_geometry;
    controller character;
    locomotion_system locomotion;
    character::tuning_params character_params;
    character::skeleton t_pose_skeleton;
    float wheel_spin_angle = 0.0f;
    float smoothed_walk_factor = 1.0f;
    float walk_factor_smoothing = 15.0f;
    velocity_trail_state trail_state;

    void init();
    void update(float dt, const gui::character_panel_state& panel_state);
};

void setup_test_level(game_world& world);