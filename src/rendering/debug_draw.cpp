#include "rendering/debug_draw.h"
#include "foundation/math_utils.h"
#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"

namespace debug {

void draw_primitives(draw_context& ctx, const debug_primitive_list& list) {
    // Draw Spheres
    for (const auto& sphere : list.spheres) {
        wireframe_mesh mesh;
        if (sphere.segments <= 4) {
            mesh = ctx.unit_sphere_4;
        } else if (sphere.segments <= 6) {
            mesh = ctx.unit_sphere_6;
        } else {
            mesh = ctx.unit_sphere_8;
        }
        mesh.position = sphere.center;
        mesh.scale = glm::vec3(sphere.radius);
        ctx.renderer.draw(mesh, ctx.cam, ctx.aspect, sphere.color);
    }

    // Draw Lines
    if (!list.lines.empty()) {
        wireframe_mesh line_mesh;
        for (const auto& line : list.lines) {
            int v0 = line_mesh.vertices.size();
            int v1 = v0 + 1;
            line_mesh.vertices.push_back(line.start);
            line_mesh.vertices.push_back(line.end);
            line_mesh.edges.emplace_back(v0, v1);
        }
        // For now, all lines are drawn with the color of the first line.
        // A more advanced implementation would batch lines by color.
        ctx.renderer.draw(line_mesh, ctx.cam, ctx.aspect, list.lines[0].color);
    }

    // Draw Boxes
    for (const auto& box : list.boxes) {
        wireframe_mesh mesh = generate_box(
            {box.half_extents.x * 2.0f, box.half_extents.y * 2.0f, box.half_extents.z * 2.0f});
        // Apply the transform to the vertices manually
        for (auto& vertex : mesh.vertices) {
            vertex = glm::vec3(box.transform * glm::vec4(vertex, 1.0f));
        }
        ctx.renderer.draw(mesh, ctx.cam, ctx.aspect, box.color);
    }

    // Draw Arrows
    for (const auto& arrow : list.arrows) {
        wireframe_mesh mesh = generate_arrow(arrow.start, arrow.end, arrow.head_size);
        ctx.renderer.draw(mesh, ctx.cam, ctx.aspect, arrow.color);
    }

    // Draw Texts (using ImGui)
    if (!list.texts.empty()) {
        ImDrawList* draw_list = ImGui::GetForegroundDrawList();
        glm::mat4 view_proj = ctx.cam.get_projection_matrix(ctx.aspect) * ctx.cam.get_view_matrix();

        for (const auto& text : list.texts) {
            glm::vec4 clip = view_proj * glm::vec4(text.position, 1.0f);
            if (clip.w > 0.0f) {
                glm::vec3 ndc = glm::vec3(clip) / clip.w;
                if (ndc.z >= 0.0f && ndc.z <= 1.0f) { // In front of camera
                    ImVec2 screen_pos((ndc.x + 1.0f) * 0.5f * ImGui::GetIO().DisplaySize.x,
                                      (1.0f - ndc.y) * 0.5f * ImGui::GetIO().DisplaySize.y);
                    draw_list->AddText(screen_pos,
                                       ImGui::ColorConvertFloat4ToU32({text.color.r, text.color.g,
                                                                       text.color.b, text.color.a}),
                                       text.text.c_str());
                }
            }
        }
    }
}

} // namespace debug