#include "input.h"
#include "sokol_app.h"
#include <cstring>
#include <cstdio>

namespace input {
    // Internal state tracking
    namespace {
        // Current and previous frame states
        bool key_state[MAX_KEYS] = {};
        bool prev_key_state[MAX_KEYS] = {};

        bool mouse_button_state[MAX_MOUSE_BUTTONS] = {};
        bool prev_mouse_button_state[MAX_MOUSE_BUTTONS] = {};

        float mouse_pos_x = 0.0f;
        float mouse_pos_y = 0.0f;
        float mouse_scroll_delta_y = 0.0f;

        // TODO: For future mouse delta tracking
        // float prev_mouse_x = 0.0f;
        // float prev_mouse_y = 0.0f;
    }

    void init() {
        // Clear all state
        memset(key_state, 0, sizeof(key_state));
        memset(prev_key_state, 0, sizeof(prev_key_state));
        memset(mouse_button_state, 0, sizeof(mouse_button_state));
        memset(prev_mouse_button_state, 0, sizeof(prev_mouse_button_state));
        mouse_pos_x = 0.0f;
        mouse_pos_y = 0.0f;
        mouse_scroll_delta_y = 0.0f;
    }

    void update() {
        // Copy current state to previous state for edge detection
        memcpy(prev_key_state, key_state, sizeof(key_state));
        memcpy(prev_mouse_button_state, mouse_button_state, sizeof(mouse_button_state));

        // Reset scroll delta
        mouse_scroll_delta_y = 0.0f;

        // TODO: For mouse delta tracking
        // prev_mouse_x = mouse_x;
        // prev_mouse_y = mouse_y;
    }

    void process_event(const sapp_event* event) {
        switch (event->type) {
            case SAPP_EVENTTYPE_KEY_DOWN:
                if (event->key_code < MAX_KEYS) {
                    key_state[event->key_code] = true;
                    printf("[INPUT] Key pressed: %d\n", event->key_code);
                }
                break;

            case SAPP_EVENTTYPE_KEY_UP:
                if (event->key_code < MAX_KEYS) {
                    key_state[event->key_code] = false;
                    printf("[INPUT] Key released: %d\n", event->key_code);
                }
                break;

            case SAPP_EVENTTYPE_MOUSE_DOWN:
                if (event->mouse_button < MAX_MOUSE_BUTTONS) {
                    mouse_button_state[event->mouse_button] = true;
                    printf("[INPUT] Mouse button pressed: %d at (%.1f, %.1f)\n",
                           event->mouse_button, event->mouse_x, event->mouse_y);
                }
                break;

            case SAPP_EVENTTYPE_MOUSE_UP:
                if (event->mouse_button < MAX_MOUSE_BUTTONS) {
                    mouse_button_state[event->mouse_button] = false;
                    printf("[INPUT] Mouse button released: %d at (%.1f, %.1f)\n",
                           event->mouse_button, event->mouse_x, event->mouse_y);
                }
                break;

            case SAPP_EVENTTYPE_MOUSE_MOVE:
                // Note: Mouse move events are NOT logged by default as they are very frequent
                // Uncomment the line below if you need to debug mouse movement
                // printf("[INPUT] Mouse move: (%.1f, %.1f)\n", event->mouse_x, event->mouse_y);
                mouse_pos_x = event->mouse_x;
                mouse_pos_y = event->mouse_y;
                break;

            case SAPP_EVENTTYPE_MOUSE_SCROLL:
                mouse_scroll_delta_y = event->scroll_y;
                break;

            default:
                break;
        }
    }

    // === KEYBOARD IMPLEMENTATION ===
    bool is_key_down(int key) {
        if (key < 0 || key >= MAX_KEYS) return false;
        return key_state[key];
    }

    bool is_key_pressed(int key) {
        if (key < 0 || key >= MAX_KEYS) return false;
        return key_state[key] && !prev_key_state[key];
    }

    bool is_key_released(int key) {
        if (key < 0 || key >= MAX_KEYS) return false;
        return !key_state[key] && prev_key_state[key];
    }

    // === MOUSE IMPLEMENTATION ===
    bool is_mouse_button_down(int button) {
        if (button < 0 || button >= MAX_MOUSE_BUTTONS) return false;
        return mouse_button_state[button];
    }

    bool is_mouse_button_pressed(int button) {
        if (button < 0 || button >= MAX_MOUSE_BUTTONS) return false;
        return mouse_button_state[button] && !prev_mouse_button_state[button];
    }

    bool is_mouse_button_released(int button) {
        if (button < 0 || button >= MAX_MOUSE_BUTTONS) return false;
        return !mouse_button_state[button] && prev_mouse_button_state[button];
    }

    void get_mouse_position(float& x, float& y) {
        x = mouse_pos_x;
        y = mouse_pos_y;
    }

    void get_mouse_position_i(int& x, int& y) {
        x = static_cast<int>(mouse_pos_x);
        y = static_cast<int>(mouse_pos_y);
    }

    float mouse_x() {
        return mouse_pos_x;
    }

    float mouse_y() {
        return mouse_pos_y;
    }

    float mouse_scroll_y() {
        return mouse_scroll_delta_y;
    }

    // TODO: Mouse delta implementation
    // float get_mouse_delta_x() {
    //     return mouse_x - prev_mouse_x;
    // }
    //
    // float get_mouse_delta_y() {
    //     return mouse_y - prev_mouse_y;
    // }
    //
    // void get_mouse_delta(float& dx, float& dy) {
    //     dx = get_mouse_delta_x();
    //     dy = get_mouse_delta_y();
    // }
}
