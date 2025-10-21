#pragma once

#include "camera/camera.h"
#include "camera/camera_follow.h"
#include "gui/camera_command.h"
#include <vector>

enum class camera_mode;

namespace gui {

struct camera_panel_state {
    bool show = true;
    float min_dist = 1.5f;
    float max_dist = 15.0f;
};

struct camera_panel_result {
    std::vector<camera_command> commands;
    std::vector<camera_mode_command> mode_commands;
};

camera_panel_result draw_camera_panel(camera_panel_state& state, const camera& cam,
                                       const camera_follow& cam_follow,
                                       camera_mode current_mode);

} // namespace gui
