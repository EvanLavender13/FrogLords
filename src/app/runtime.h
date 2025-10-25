#pragma once

#include "sokol_gfx.h"
#include "app/game_world.h"
#include "rendering/renderer.h"
#include "foundation/procedural_mesh.h"
#include "gui/camera_panel.h"
#include "gui/vehicle_panel.h"
#include "gui/fov_panel.h"
#include <glm/glm.hpp>

struct sapp_event;

struct app_runtime {
    void initialize();
    void shutdown();
    void frame();
    void handle_event(const sapp_event* e);

  private:
    void ensure_static_meshes();
    void render_world();
    void apply_parameter_commands(const std::vector<gui::parameter_command>& commands);
    void apply_camera_commands(const std::vector<gui::camera_command>& commands);
    void apply_fov_commands(const std::vector<gui::fov_command>& commands);

    bool initialized = false;

    sg_pass_action pass_action{};

    game_world world;
    wireframe_renderer renderer{};
    gui::camera_panel_state camera_panel_state{};
    gui::vehicle_panel_state vehicle_panel_state{};
    gui::fov_panel_state fov_panel_state{};

    float wireframe_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    foundation::wireframe_mesh unit_circle{};
    foundation::wireframe_mesh unit_sphere_8{};
    foundation::wireframe_mesh unit_sphere_6{};
    foundation::wireframe_mesh unit_sphere_4{};
    bool static_meshes_initialized = false;
};

app_runtime& runtime();
