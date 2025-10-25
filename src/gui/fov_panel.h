#pragma once
#include <vector>

// Forward declarations
struct dynamic_fov_system;

namespace gui {

struct fov_panel_state {
    bool show = true;
};

enum class fov_parameter_type { BASE_FOV, MAX_FOV_RANGE, G_MULTIPLIER, SPRING_STIFFNESS };

struct fov_command {
    fov_parameter_type type;
    float value;
};

std::vector<fov_command> draw_fov_panel(const fov_panel_state& state,
                                        const dynamic_fov_system& fov_system);

} // namespace gui
