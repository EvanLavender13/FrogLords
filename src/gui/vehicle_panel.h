#pragma once

#include "vehicle/controller.h"
#include "vehicle/tuning.h"
#include "gui/parameter_command.h"
#include <vector>

namespace gui {

struct vehicle_panel_state {
    bool show = true;
};

std::vector<parameter_command> draw_vehicle_panel(const vehicle_panel_state& state,
                                                  const controller& vehicle,
                                                  const vehicle::tuning_params& params);

} // namespace gui
