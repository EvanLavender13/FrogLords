#pragma once
#include "sokol_app.h"
#include "foundation/param_meta.h"
#include <cfloat>

// GUI System Framework using Dear ImGui with Sokol
//
// Simple wrapper around Dear ImGui to provide a clean interface
// for creating and managing GUI panels/windows.

namespace gui {

// Initialize the GUI system
// Must be called after sokol_gfx is initialized
void init();

// Begin a new GUI frame
// Call at the start of each frame before any GUI code
void begin_frame();

// Render the GUI
// Call after all GUI drawing, but before sg_end_pass()
void render();

// Shutdown the GUI system
void shutdown();

// Handle input events
// Forward sokol app events to the GUI system
void handle_event(const sapp_event* event);

// Check if GUI wants to capture mouse input
bool wants_mouse();

// GUI Panel/Window helpers
namespace panel {
// Begin a new window/panel
// Returns false if window is collapsed
bool begin(const char* title, bool* open = nullptr);

// End the current window/panel
void end();
} // namespace panel

// Common GUI widgets
namespace widget {
void text(const char* fmt, ...);
bool slider_float(const char* label, float* value, float min, float max);

// Metadata-driven parameter widgets
bool tunable_param(float* value, const param_meta& meta);
void readonly_param(const char* label, float value, const param_meta& meta);
void derived_param(float value, const param_meta& meta, const char* formula);
} // namespace widget

// Plot temporal data as histogram
// Same interface as plot_value but renders as histogram instead of line graph
void plot_histogram(const char* label, float current_value, float time_window = 5.0f,
                    float min_value = FLT_MAX, float max_value = FLT_MAX, size_t max_samples = 500);

} // namespace gui
