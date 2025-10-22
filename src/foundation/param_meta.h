#pragma once

// Parameter metadata for designer-facing tuning interfaces
// Separates presentation metadata (how to display) from data (what to display)
// Enables metadata-driven GUI widgets without hardcoding presentation logic

enum class param_type {
    TUNABLE,  // Designer can modify via slider/input
    READONLY, // Display only, not editable
    DERIVED   // Calculated from other parameters, shows formula
};

struct param_meta {
    const char* name = "";    // Display label
    const char* units = "";   // Physical units (m/s, m/s², meters, radians, etc.)
    float min = 0.0f;         // Minimum value (for sliders)
    float max = 1.0f;         // Maximum value (for sliders)
    param_type type = param_type::TUNABLE;
};
