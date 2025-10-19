#include "gui/camera_panel.h"
#include "gui/gui.h"
#include <imgui.h>

namespace gui {

void draw_camera_panel(camera_panel_state& state, camera& cam, camera_follow& cam_follow) {
    if (!state.show)
        return;

    // Draw as collapsible section (no window wrapper)
    if (!ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
        return;

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

    // Distance slider
    if (ImGui::SliderFloat("Distance", &cam_follow.distance, cam_follow.min_distance,
                           cam_follow.max_distance, "%.1f m")) {
        // Value updated directly via slider
    }

    // Height offset slider
    if (ImGui::SliderFloat("Height Offset", &cam_follow.height_offset, 0.0f, 3.0f, "%.1f m")) {
        // Value updated directly via slider
    }

    ImGui::Spacing();

    // Zoom limits
    ImGui::Text("Zoom Limits");
    ImGui::SliderFloat("Min Distance", &cam_follow.min_distance, 0.5f, 10.0f, "%.1f m");
    ImGui::SliderFloat("Max Distance", &cam_follow.max_distance, 5.0f, 30.0f, "%.1f m");
}

} // namespace gui
