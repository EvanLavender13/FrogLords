#pragma once

#include "camera/camera.h"
#include "character/controller.h"
#include "character/locomotion.h"
#include "character/tuning.h"
#include "character/skeleton.h"
#include "rendering/scene.h"

namespace gui {
struct character_panel_state;
}

struct game_world {
    camera cam;
    scene scn;
    controller character;
    locomotion_system locomotion;
    character::tuning_params character_params;
    character::skeleton t_pose_skeleton;
    float wheel_spin_angle = 0.0f;
    float skeleton_animation_time = 0.0f;

    void init();
    void update(float dt, const gui::character_panel_state& panel_state);
};

void setup_test_level(scene& scn);