#include "gui/camera_panel.h"
#include "gui/gui.h"
#include <imgui.h>
#include <vector>

namespace gui {

std::vector<camera_command> draw_camera_panel(const camera_panel_state& state, const camera& cam,
                                              const camera_follow& cam_follow) {
    std::vector<camera_command> commands;

    if (!state.show)
        return commands;

    // Draw as collapsible section (no window wrapper)
    if (!ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
        return commands;

    // Camera mode
    ImGui::Text("Mode");
    if (ImGui::RadioButton("Free Orbit", cam_follow.mode == camera_mode::FREE_ORBIT)) {
        commands.push_back({camera_parameter_type::MODE, 0.0f, camera_mode::FREE_ORBIT});
    }
    ImGui::SameLine();
    if (ImGui::RadioButton("Lock to Orientation", cam_follow.mode == camera_mode::LOCK_TO_ORIENTATION)) {
        commands.push_back({camera_parameter_type::MODE, 0.0f, camera_mode::LOCK_TO_ORIENTATION});
    }

    ImGui::Spacing();

    // Current state (read-only)
    gui::widget::readonly_param(cam_follow.distance, camera_follow::make_distance_meta(cam_follow.min_distance, cam_follow.max_distance));
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

    // Metadata-driven tunable parameters
    if (gui::widget::tunable_param(&distance, camera_follow::make_distance_meta(min_distance, max_distance))) {
        commands.push_back({camera_parameter_type::DISTANCE, distance});
    }

    if (gui::widget::tunable_param(&height_offset, camera_follow::height_offset_meta)) {
        commands.push_back({camera_parameter_type::HEIGHT_OFFSET, height_offset});
    }

    ImGui::Spacing();

    // Zoom limits
    ImGui::Text("Zoom Limits");

    bool min_changed = gui::widget::tunable_param(&min_distance, camera_follow::min_distance_meta);
    bool max_changed = gui::widget::tunable_param(&max_distance, camera_follow::max_distance_meta);

    // Validate min_distance <= max_distance invariant before submitting commands
    bool invariant_valid = (min_distance <= max_distance);

    if (min_changed && invariant_valid) {
        commands.push_back({camera_parameter_type::MIN_DISTANCE, min_distance});
    }
    if (max_changed && invariant_valid) {
        commands.push_back({camera_parameter_type::MAX_DISTANCE, max_distance});
    }

    // Warn user if invariant violated
    if ((min_changed || max_changed) && !invariant_valid) {
        ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f),
                          "Warning: Min Distance must be <= Max Distance");
    }

    return commands;
}

} // namespace gui
