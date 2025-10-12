#pragma once

#include "camera/camera.h"

namespace gui {

struct camera_panel_state {
    bool show = true;
};

void draw_camera_panel(const camera_panel_state& state, camera& cam);

} // namespace gui
