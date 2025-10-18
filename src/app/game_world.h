#pragma once

#include "camera/camera.h"
#include "character/controller.h"
#include "character/character_reactive_systems.h"
#include "character/tuning.h"
#include "rendering/scene.h"
#include "rendering/velocity_trail.h"
#include "foundation/collision_primitives.h"
#include "rendering/debug_primitives.h"
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
    character_reactive_systems character_visuals;
    character::tuning_params character_params;
    velocity_trail_state trail_state;

    debug::debug_primitive_list debug_list;

    void init();
    void update(float dt, const gui::character_panel_state& panel_state);
};

void setup_test_level(game_world& world);