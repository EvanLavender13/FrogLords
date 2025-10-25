#pragma once

// Parameter metadata for designer-facing tuning interfaces
// Separates presentation metadata (how to display) from data (what to display)
// Enables metadata-driven GUI widgets without hardcoding presentation logic
//
// Visual distinction (tunable vs readonly vs derived) comes from widget choice,
// not from metadata flags. Widgets enforce radical simplicity through function selection:
//   - gui::widget::tunable_param() for interactive sliders
//   - gui::widget::readonly_param() for display-only values
//   - gui::widget::derived_param() for calculated values with formulas

struct param_meta {
    const char* name = "";  // Display label
    const char* units = ""; // Physical units (m/s, m/s², meters, radians, etc.)
    float min = 0.0f;       // Minimum value (for sliders)
    float max = 1.0f;       // Maximum value (for sliders)
};
