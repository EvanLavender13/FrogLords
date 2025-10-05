#pragma once

#include "rendering/renderer.h"
#include "rendering/wireframe.h"
#include "camera/camera.h"
#include "character/character_controller.h"
#include "character/locomotion.h"
#include "character/orientation.h"
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

void draw_character_state(draw_context& ctx, const character_controller& character,
                          const locomotion_system& locomotion,
                          const orientation_system& orientation);

void draw_physics_springs(draw_context& ctx, const character_controller& character,
                          const locomotion_system& locomotion);

void draw_locomotion_wheel(draw_context& ctx, const character_controller& character,
                           const locomotion_system& locomotion,
                           const orientation_system& orientation, float wheel_spin_angle);

void draw_foot_positions(draw_context& ctx, const character_controller& character,
                         const locomotion_system& locomotion,
                         const orientation_system& orientation);

} // namespace debug
