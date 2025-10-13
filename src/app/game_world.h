#pragma once

#include "camera/camera.h"
#include "character/controller.h"
#include "character/locomotion.h"
#include "character/tuning.h"
#include "character/skeleton.h"
#include "rendering/scene.h"
#include <glm/glm.hpp>
#include <vector>

namespace gui {
struct character_panel_state;
}

struct velocity_trail_state {
    std::vector<glm::vec3> positions;
    std::vector<float> timestamps;
    float sample_interval = 0.1f;
    float time_since_last_sample = 0.0f;
};

struct game_world {
    camera cam;
    scene scn;
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

void setup_test_level(scene& scn);