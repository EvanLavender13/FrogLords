#include "rendering/debug_visualization.h"

namespace debug_viz {

// Global state for debug visualization toggle
// Start enabled in debug builds by default
#ifdef _DEBUG
static bool s_enabled = true;
#else
static bool s_enabled = false;
#endif

void set_enabled(bool enabled) {
    s_enabled = enabled;
}

bool is_enabled() {
    return s_enabled;
}

void toggle() {
    s_enabled = !s_enabled;
}

} // namespace debug_viz
