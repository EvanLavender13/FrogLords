#pragma once
#include "sokol_app.h"

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

// Check if GUI wants to capture mouse/keyboard input
bool wants_mouse();
bool wants_keyboard();

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
bool button(const char* label);
bool checkbox(const char* label, bool* value);
bool slider_float(const char* label, float* value, float min, float max);
void color_edit(const char* label, float* color); // RGB or RGBA
} // namespace widget
} // namespace gui
