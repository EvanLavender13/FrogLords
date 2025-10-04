#pragma once

#include "sokol_gfx.h"
#include "camera/camera.h"
#include "character/character_controller.h"
#include "character/locomotion.h"
#include "character/orientation.h"
#include "rendering/renderer.h"
#include "rendering/scene.h"
#include "rendering/wireframe.h"
#include <glm/glm.hpp>

struct sapp_event;

struct intuitive_character_params {
    float time_to_max_speed = 0.4f;
    float jump_height = 1.3f;

    void apply_to(character_controller& c) const;
    void read_from(const character_controller& c);
};

struct app_runtime {
    void initialize();
    void shutdown();
    void frame();
    void handle_event(const sapp_event* e);

  private:
    void sync_locomotion_speed_targets();
    void ensure_static_meshes();
    void update_simulation(float dt);
    void begin_gui_frame();
    void build_character_panel();
    void render_world();
    void update_spring_history();

    bool initialized = false;

    sg_pass_action pass_action{};

    camera cam{};
    scene scn{};
    wireframe_renderer renderer{};
    character_controller character{};
    orientation_system orientation{};
    locomotion_system locomotion{};
    intuitive_character_params character_params{};

    float wireframe_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    bool show_character_panel = true;

    wireframe_mesh unit_circle{};
    wireframe_mesh unit_sphere_8{};
    wireframe_mesh unit_sphere_6{};
    wireframe_mesh unit_sphere_4{};
    bool static_meshes_initialized = false;

    static constexpr int spring_history_size = 200;
    float spring_position_history[spring_history_size] = {};
    float spring_velocity_history[spring_history_size] = {};
    int spring_history_index = 0;
};

app_runtime& runtime();
