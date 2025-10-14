#include "rendering/debug_draw.h"
#include "foundation/math_utils.h"
#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"
#include <algorithm>

namespace debug {

void draw_primitives(draw_context& ctx, const debug_primitive_list& list) {
    // Draw Spheres
    for (const auto& sphere : list.spheres) {
        foundation::wireframe_mesh mesh;
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

    // Draw Lines (with color batching)
    if (!list.lines.empty()) {
        auto lines_copy = list.lines; // Make a mutable copy

        // Custom comparator for glm::vec4
        auto color_less = [](const glm::vec4& a, const glm::vec4& b) {
            if (a.r != b.r)
                return a.r < b.r;
            if (a.g != b.g)
                return a.g < b.g;
            if (a.b != b.b)
                return a.b < b.b;
            return a.a < b.a;
        };

        std::sort(lines_copy.begin(), lines_copy.end(),
                  [&](const debug::debug_line& a, const debug::debug_line& b) {
                      return color_less(a.color, b.color);
                  });

        foundation::wireframe_mesh current_batch;
        glm::vec4 current_color = lines_copy[0].color;

        for (const auto& line : lines_copy) {
            bool color_changed =
                (line.color.r != current_color.r || line.color.g != current_color.g ||
                 line.color.b != current_color.b || line.color.a != current_color.a);

            if (color_changed) {
                if (!current_batch.edges.empty()) {
                    ctx.renderer.draw(current_batch, ctx.cam, ctx.aspect, current_color);
                }
                current_batch = {};
                current_color = line.color;
            }

            int v0_idx = current_batch.vertices.size();
            current_batch.vertices.push_back(line.start);
            int v1_idx = current_batch.vertices.size();
            current_batch.vertices.push_back(line.end);
            current_batch.edges.emplace_back(v0_idx, v1_idx);
        }

        // Draw the final batch
        if (!current_batch.edges.empty()) {
            ctx.renderer.draw(current_batch, ctx.cam, ctx.aspect, current_color);
        }
    }

    // Draw Boxes
    for (const auto& box : list.boxes) {
        foundation::wireframe_mesh mesh = foundation::generate_box(
            {box.half_extents.x * 2.0f, box.half_extents.y * 2.0f, box.half_extents.z * 2.0f});
        // Apply the transform to the vertices manually
        for (auto& vertex : mesh.vertices) {
            vertex = glm::vec3(box.transform * glm::vec4(vertex, 1.0f));
        }
        ctx.renderer.draw(mesh, ctx.cam, ctx.aspect, box.color);
    }

    // Draw Arrows
    for (const auto& arrow : list.arrows) {
        foundation::wireframe_mesh mesh =
            foundation::generate_arrow(arrow.start, arrow.end, arrow.head_size);
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