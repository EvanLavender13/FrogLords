#include "gui/camera_panel.h"
#include "gui/gui.h"
#include <imgui.h>
#include <vector>

namespace gui {

std::vector<camera_command> draw_camera_panel(camera_panel_state& state, const camera& cam,
                                              const camera_follow& cam_follow) {
    std::vector<camera_command> commands;

    if (!state.show)
        return commands;

    // Draw as collapsible section (no window wrapper)
    if (!ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
        return commands;

    // Camera mode display (always FOLLOW now)
    ImGui::Text("Mode: FOLLOW");

    // Current state (read-only)
    ImGui::Text("Distance: %.2f m", cam_follow.distance);
    ImGui::Text("FOV: %.1f degrees", cam.get_fov());

    glm::vec3 pos = cam.get_position();
    ImGui::Text("Position: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);

    ImGui::Spacing();

    // Follow settings
    ImGui::Text("Follow Settings");

    // Local copies for slider interaction (GUI needs mutable values)
    float distance = cam_follow.distance;
    float height_offset = cam_follow.height_offset;
    float min_distance = cam_follow.min_distance;
    float max_distance = cam_follow.max_distance;

    // Distance slider
    if (ImGui::SliderFloat("Distance", &distance, min_distance, max_distance, "%.1f m")) {
        commands.push_back({camera_parameter_type::DISTANCE, distance});
    }

    // Height offset slider
    if (ImGui::SliderFloat("Height Offset", &height_offset, 0.0f, 3.0f, "%.1f m")) {
        commands.push_back({camera_parameter_type::HEIGHT_OFFSET, height_offset});
    }

    ImGui::Spacing();

    // Zoom limits
    ImGui::Text("Zoom Limits");
    if (ImGui::SliderFloat("Min Distance", &min_distance, 0.5f, 10.0f, "%.1f m")) {
        commands.push_back({camera_parameter_type::MIN_DISTANCE, min_distance});
    }
    if (ImGui::SliderFloat("Max Distance", &max_distance, 5.0f, 30.0f, "%.1f m")) {
        commands.push_back({camera_parameter_type::MAX_DISTANCE, max_distance});
    }

    return commands;
}

} // namespace gui
