#include "gui/camera_panel.h"
#include "gui/gui.h"
#include <imgui.h>

namespace gui {

void draw_camera_panel(camera_panel_state& state, camera& cam) {
    if (!state.show)
        return;

    // Draw as collapsible section (no window wrapper)
    if (!ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
        return;

    // Camera mode display
    const char* mode_name = (cam.get_mode() == camera_mode::ORBIT) ? "ORBIT" : "FOLLOW";
    ImGui::Text("Mode: %s", mode_name);

    // Current state (read-only)
    ImGui::Text("Distance: %.2f m",
                cam.get_mode() == camera_mode::FOLLOW ? cam.get_follow_distance() : 0.0f);
    ImGui::Text("FOV: %.1f degrees", cam.get_fov());

    glm::vec3 pos = cam.get_position();
    ImGui::Text("Position: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);

    ImGui::Spacing();

    // Follow mode settings (only editable in FOLLOW mode)
    if (cam.get_mode() == camera_mode::FOLLOW) {
        ImGui::Text("Follow Settings");

        // Get current distance for slider
        float current_distance = cam.get_follow_distance();
        if (ImGui::SliderFloat("Distance", &current_distance, 1.5f, 15.0f, "%.1f m")) {
            cam.set_follow_distance(current_distance);
        }

        // Height offset slider
        float height_offset = cam.get_follow_height();
        if (ImGui::SliderFloat("Height Offset", &height_offset, 0.0f, 3.0f, "%.1f m")) {
            cam.set_follow_height(height_offset);
        }

        ImGui::Spacing();

        // Zoom limits
        ImGui::Text("Zoom Limits");
        static float min_dist = 1.5f;
        static float max_dist = 15.0f;

        if (ImGui::SliderFloat("Min Distance", &min_dist, 0.5f, 10.0f, "%.1f m")) {
            cam.set_follow_distance_limits(min_dist, max_dist);
        }
        if (ImGui::SliderFloat("Max Distance", &max_dist, 5.0f, 30.0f, "%.1f m")) {
            cam.set_follow_distance_limits(min_dist, max_dist);
        }
    } else {
        ImGui::TextDisabled("Follow mode settings disabled (camera in ORBIT mode)");
    }
}

} // namespace gui
