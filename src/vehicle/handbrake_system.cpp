#include "vehicle/handbrake_system.h"
#include "foundation/debug_assert.h"

void handbrake_system::update(bool input) {
    // Store handbrake state
    // Drag contribution applied via friction_model::compute_total_drag()
    active = input;
}
