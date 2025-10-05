#pragma once

#include "sokol_gfx.h"
#include "camera/camera.h"
#include "character/controller.h"
#include "character/locomotion.h"
#include "character/orientation.h"
#include "character/tuning.h"
#include "rendering/renderer.h"
#include "rendering/scene.h"
#include "rendering/wireframe.h"
#include "gui/character_panel.h"
#include <glm/glm.hpp>

struct sapp_event;

struct app_runtime {
    void initialize();
    void shutdown();
    void frame();
    void handle_event(const sapp_event* e);

  private:
    void ensure_static_meshes();
    void update_simulation(float dt);
    void render_world();

    bool initialized = false;

    sg_pass_action pass_action{};

    camera cam{};
    scene scn{};
    wireframe_renderer renderer{};
    controller character{};
    orientation_system orientation{};
    locomotion_system locomotion{};
    character::tuning_params character_params{};
    gui::character_panel_state panel_state{};

    float wheel_spin_angle = 0.0f;

    float wireframe_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    wireframe_mesh unit_circle{};
    wireframe_mesh unit_sphere_8{};
    wireframe_mesh unit_sphere_6{};
    wireframe_mesh unit_sphere_4{};
    bool static_meshes_initialized = false;
};

app_runtime& runtime();
