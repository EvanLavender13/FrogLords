#pragma once

// Forward declarations to avoid including sokol in the header
struct sapp_event;

// Simple Input Handling System
// Provides easy-to-use functions to query keyboard and mouse state

namespace input {
// Maximum number of keys and mouse buttons we track
constexpr int MAX_KEYS = 512;
constexpr int MAX_MOUSE_BUTTONS = 8;

struct cursor_position {
    float x = 0.0f;
    float y = 0.0f;
};

struct cursor_position_i {
    int x = 0;
    int y = 0;
};

// Initialize the input system (call once at startup)
void init();

// Update input state (call at the beginning of each frame)
void update();

// Process Sokol events (call this from your event callback)
void process_event(const sapp_event* event);

// === KEYBOARD ===
// Check if a key is currently pressed (use SAPP_KEYCODE_* constants)
bool is_key_down(int key);

// Check if a key was just pressed this frame
bool is_key_pressed(int key);

// Check if a key was just released this frame
bool is_key_released(int key);

// === MOUSE ===
// Check if a mouse button is currently pressed (use SAPP_MOUSEBUTTON_* constants)
bool is_mouse_button_down(int button);

// Check if a mouse button was just pressed this frame
bool is_mouse_button_pressed(int button);

// Check if a mouse button was just released this frame
bool is_mouse_button_released(int button);

// Get current mouse position
cursor_position get_mouse_position();

// Get mouse position as integers
cursor_position_i get_mouse_position_i();

// Get mouse position (shorthand accessors)
float mouse_x();
float mouse_y();

// Get mouse scroll delta
float mouse_scroll_y();

} // namespace input
