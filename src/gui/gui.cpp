#include "gui.h"
#include "foundation/debug_assert.h"
#include "sokol_gfx.h"
#include "sokol_log.h"
#include "imgui.h"
#include "sokol_imgui.h"
#include <cfloat>
#include <cmath>
#include <cstdarg>
#include <cstdio>
#include <map>
#include <string>
#include <vector>

namespace gui {

// Internal state for plot buffers
struct plot_buffer {
    std::vector<float> values;
    std::vector<float> timestamps;
    float time_window = 0.0f;
};

static std::map<std::string, plot_buffer> plot_buffers;

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
    const ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureMouse;
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

bool slider_float(const char* label, float* value, float min, float max) {
    // Set fixed width for consistency with tunable_param
    ImGui::SetNextItemWidth(250.0f);
    return ImGui::SliderFloat(label, value, min, max);
}

bool tunable_param(float* value, const param_meta& meta) {
    // Validate metadata configuration
    FL_PRECONDITION(meta.min < meta.max, "param_meta min must be less than max");
    FL_PRECONDITION(std::isfinite(meta.min) && std::isfinite(meta.max),
                    "param_meta min/max must be finite");

    // Format label with units
    char label[128];
    if (meta.units[0] != '\0') {
        snprintf(label, sizeof(label), "%s (%s)", meta.name, meta.units);
    } else {
        snprintf(label, sizeof(label), "%s", meta.name);
    }

    // Set fixed width to prevent panel resizing when values change digit count
    ImGui::SetNextItemWidth(250.0f);

    // Slider with metadata-defined range
    return ImGui::SliderFloat(label, value, meta.min, meta.max);
}

void readonly_param(float value, const param_meta& meta) {
    // Format display with units (use meta.name for label)
    char display[128];
    if (meta.units[0] != '\0') {
        snprintf(display, sizeof(display), "%s: %.3f %s", meta.name, value, meta.units);
    } else {
        snprintf(display, sizeof(display), "%s: %.3f", meta.name, value);
    }

    // Grayed out text to indicate read-only
    ImGui::TextDisabled("%s", display);
}

void derived_param(float value, const param_meta& meta, const char* formula) {
    // Format display with value, units, and formula (use meta.name for label)
    char display[256];
    if (meta.units[0] != '\0') {
        snprintf(display, sizeof(display), "%s: %.3f %s = %s", meta.name, value, meta.units,
                 formula);
    } else {
        snprintf(display, sizeof(display), "%s: %.3f = %s", meta.name, value, formula);
    }

    // Grayed out italic text to indicate derived value
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
    ImGui::TextWrapped("%s", display);
    ImGui::PopStyleColor();
}

} // namespace widget

static void prune_plot_buffer(plot_buffer& buffer, float current_time, size_t max_samples) {
    // Cap buffer size to prevent unbounded growth at high sample rates
    if (buffer.values.size() > max_samples) {
        buffer.timestamps.erase(buffer.timestamps.begin());
        buffer.values.erase(buffer.values.begin());
    }

    // Prune old samples outside time window
    while (!buffer.timestamps.empty() && current_time - buffer.timestamps[0] > buffer.time_window) {
        buffer.timestamps.erase(buffer.timestamps.begin());
        buffer.values.erase(buffer.values.begin());
    }
}

static void update_plot_buffer(plot_buffer& buffer, float current_value, float time_window,
                               size_t max_samples) {
    if (buffer.time_window == 0.0f) {
        buffer.time_window = time_window;
    }

    float current_time = static_cast<float>(ImGui::GetTime());
    buffer.values.push_back(current_value);
    buffer.timestamps.push_back(current_time);

    prune_plot_buffer(buffer, current_time, max_samples);
}

void plot_histogram(const char* label, float current_value, float time_window, float min_value,
                    float max_value, size_t max_samples) {
    auto& buffer = plot_buffers[label];
    update_plot_buffer(buffer, current_value, time_window, max_samples);

    // Render histogram with axis labels
    if (!buffer.values.empty()) {
        // Create overlay label with current value
        char overlay[64];
        snprintf(overlay, sizeof(overlay), "%.1f", current_value);

        // Y-axis max label (top-left)
        if (max_value != FLT_MAX) {
            ImGui::Text("%.0f", max_value);
            ImGui::SameLine();
        }

        ImGui::PlotHistogram(label, buffer.values.data(), static_cast<int>(buffer.values.size()), 0,
                             overlay, min_value, max_value, ImVec2(0, 60));

        // Y-axis min and X-axis time range labels (bottom row)
        if (min_value != FLT_MAX) {
            ImGui::Text("%.0f", min_value);
        } else {
            ImGui::Text(" ");
        }
        ImGui::SameLine();
        ImGui::Text("Time: %.1fs", time_window);
    }
}

} // namespace gui
