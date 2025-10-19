#pragma once

#include "camera/camera.h"
#include "camera/camera_follow.h"
#include "gui/camera_command.h"
#include <vector>

namespace gui {

struct camera_panel_state {
    bool show = true;
    float min_dist = 1.5f;
    float max_dist = 15.0f;
};

std::vector<camera_command> draw_camera_panel(camera_panel_state& state,
                                               const camera& cam,
                                               const camera_follow& cam_follow);

} // namespace gui
