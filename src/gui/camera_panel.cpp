#include "gui/camera_panel.h"
#include "gui/gui.h"
#include "app/game_world.h"
#include <imgui.h>
#include <vector>

namespace gui {

camera_panel_result draw_camera_panel(camera_panel_state& state, const camera& cam,
                                       const camera_follow& cam_follow,
                                       camera_mode current_mode) {
    camera_panel_result result;

    if (!state.show)
        return result;

    // Draw as collapsible section (no window wrapper)
    if (!ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
        return result;

    // Camera mode selection
    ImGui::Text("Mode:");
    int mode_index = static_cast<int>(current_mode);
    if (ImGui::RadioButton("Free Orbit", &mode_index, 0)) {
        result.mode_commands.push_back({camera_mode::FREE_ORBIT});
    }
    if (ImGui::RadioButton("Lock to Orientation", &mode_index, 1)) {
        result.mode_commands.push_back({camera_mode::LOCK_ORIENTATION});
    }
    if (ImGui::RadioButton("Lock to Velocity", &mode_index, 2)) {
        result.mode_commands.push_back({camera_mode::LOCK_VELOCITY});
    }
    ImGui::Spacing();

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
        result.commands.push_back({camera_parameter_type::DISTANCE, distance});
    }

    // Height offset slider
    if (ImGui::SliderFloat("Height Offset", &height_offset, 0.0f, 3.0f, "%.1f m")) {
        result.commands.push_back({camera_parameter_type::HEIGHT_OFFSET, height_offset});
    }

    ImGui::Spacing();

    // Zoom limits
    ImGui::Text("Zoom Limits");
    if (ImGui::SliderFloat("Min Distance", &min_distance, 0.5f, 10.0f, "%.1f m")) {
        result.commands.push_back({camera_parameter_type::MIN_DISTANCE, min_distance});
    }
    if (ImGui::SliderFloat("Max Distance", &max_distance, 5.0f, 30.0f, "%.1f m")) {
        result.commands.push_back({camera_parameter_type::MAX_DISTANCE, max_distance});
    }

    return result;
}

} // namespace gui
