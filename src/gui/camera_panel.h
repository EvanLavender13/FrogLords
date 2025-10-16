#pragma once

#include "camera/camera.h"

namespace gui {

struct camera_panel_state {
    bool show = true;
    float min_dist = 1.5f;
    float max_dist = 15.0f;
};

void draw_camera_panel(camera_panel_state& state, camera& cam);

} // namespace gui
