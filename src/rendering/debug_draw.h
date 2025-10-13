#pragma once

#include "rendering/renderer.h"
#include "rendering/wireframe.h"
#include "rendering/velocity_trail.h"
#include "camera/camera.h"
#include "character/controller.h"
#include "character/locomotion.h"
#include "character/orientation.h"
#include "character/skeleton.h"
#include <glm/glm.hpp>

namespace debug {

struct draw_context {
    wireframe_renderer& renderer;
    camera& cam;
    float aspect;

    const wireframe_mesh& unit_circle;
    const wireframe_mesh& unit_sphere_8;
    const wireframe_mesh& unit_sphere_6;
    const wireframe_mesh& unit_sphere_4;
};

void draw_character_state(draw_context& ctx, const controller& character,
                          const locomotion_system& locomotion,
                          const orientation_system& orientation);

void draw_physics_springs(draw_context& ctx, const controller& character,
                          const locomotion_system& locomotion);

void draw_locomotion_wheel(draw_context& ctx, const controller& character,
                           const locomotion_system& locomotion,
                           const orientation_system& orientation, float wheel_spin_angle);

void draw_foot_positions(draw_context& ctx, const controller& character,
                         const locomotion_system& locomotion,
                         const orientation_system& orientation);

void draw_collision_state(draw_context& ctx, const controller& character, const scene& scn);

void draw_character_body(draw_context& ctx, const controller& character,
                         const orientation_system& orientation);

void draw_skeleton(draw_context& ctx, const character::skeleton& skel, bool show_labels = false);

void draw_axis_gizmo(draw_context& ctx, const character::skeleton& skel, float axis_length = 0.6f);

void draw_velocity_trail(draw_context& ctx, const velocity_trail_state& trail);

} // namespace debug
