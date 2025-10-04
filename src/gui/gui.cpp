#include "gui.h"
#include "sokol_gfx.h"
#include "sokol_log.h"
#include "imgui.h"
#include "sokol_imgui.h"
#include <cstdarg>

namespace gui {

void init() {
    simgui_desc_t desc = {};
    desc.logger.func = slog_func;
    simgui_setup(&desc);
}

void begin_frame() {
    simgui_new_frame({sapp_width(), sapp_height(), sapp_frame_duration(), sapp_dpi_scale()});
}

void render() {
    simgui_render();
}

void shutdown() {
    simgui_shutdown();
}

void handle_event(const sapp_event* event) {
    simgui_handle_event(event);
}

bool wants_mouse() {
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureMouse;
}

bool wants_keyboard() {
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureKeyboard;
}

namespace panel {

bool begin(const char* title, bool* open) {
    if (open != nullptr) {
        return ImGui::Begin(title, open);
    }
    return ImGui::Begin(title);
}

void end() {
    ImGui::End();
}

} // namespace panel

namespace widget {

void text(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    ImGui::TextV(fmt, args);
    va_end(args);
}

bool button(const char* label) {
    return ImGui::Button(label);
}

bool checkbox(const char* label, bool* value) {
    return ImGui::Checkbox(label, value);
}

bool slider_float(const char* label, float* value, float min, float max) {
    return ImGui::SliderFloat(label, value, min, max);
}

void color_edit(const char* label, float* color) {
    // Assume RGB by default, but ImGui will auto-detect if 4 floats
    ImGui::ColorEdit3(label, color);
}

} // namespace widget

} // namespace gui
