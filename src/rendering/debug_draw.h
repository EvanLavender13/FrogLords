#pragma once

#include "rendering/renderer.h"
#include "rendering/wireframe.h"
#include "camera/camera.h"
#include "rendering/debug_primitives.h"
#include <glm/glm.hpp>

namespace debug {

// The context now primarily provides access to the renderer and camera,
// as well as pre-made unit meshes for efficiency.
struct draw_context {
    wireframe_renderer& renderer;
    camera& cam;
    float aspect;

    const wireframe_mesh& unit_circle;
    const wireframe_mesh& unit_sphere_8;
    const wireframe_mesh& unit_sphere_6;
    const wireframe_mesh& unit_sphere_4;
};

// The single entry point for all debug drawing.
// It takes a list of primitives and renders them.
void draw_primitives(draw_context& ctx, const debug_primitive_list& list);

} // namespace debug