#include "gui/fov_panel.h"
#include "gui/gui.h"
#include "camera/dynamic_fov.h"
#include <imgui.h>

namespace gui {

std::vector<fov_command> draw_fov_panel(const fov_panel_state& state,
                                        const dynamic_fov_system& fov_system) {
    std::vector<fov_command> commands;

    if (!state.show)
        return commands;

    // Draw as collapsible section (no window wrapper)
    if (!ImGui::CollapsingHeader("Dynamic FOV", ImGuiTreeNodeFlags_DefaultOpen))
        return commands;

    // Local copies for slider interaction (GUI needs mutable values)
    float base_fov = fov_system.base_fov;
    float max_fov_range = fov_system.max_fov_range;
    float g_multiplier = fov_system.g_multiplier;

    // Metadata-driven tunable parameters
    if (gui::widget::tunable_param(&base_fov, dynamic_fov_system::base_fov_meta)) {
        commands.push_back({fov_parameter_type::BASE_FOV, base_fov});
    }

    if (gui::widget::tunable_param(&max_fov_range, dynamic_fov_system::max_fov_range_meta)) {
        commands.push_back({fov_parameter_type::MAX_FOV_RANGE, max_fov_range});
    }

    if (gui::widget::tunable_param(&g_multiplier, dynamic_fov_system::g_multiplier_meta)) {
        commands.push_back({fov_parameter_type::G_MULTIPLIER, g_multiplier});
    }

    return commands;
}

} // namespace gui
