#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#include "app/runtime.h"

static void init() {
    runtime().initialize();
}

static void frame() {
    runtime().frame();
}

static void cleanup() {
    runtime().shutdown();
}

static void event(const sapp_event* e) {
    runtime().handle_event(e);
}

sapp_desc sokol_main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {

    sapp_desc desc = {};
    desc.init_cb = init;
    desc.frame_cb = frame;
    desc.cleanup_cb = cleanup;
    desc.event_cb = event;
    // Enable multisampling (MSAA) to reduce edge aliasing on thin wireframes
    desc.sample_count = 8;
    desc.width = 1920;
    desc.height = 1080;
    desc.window_title = "FrogLords - GUI Demo";
    desc.icon.sokol_default = true;
    desc.enable_clipboard = true;
    desc.logger.func = slog_func;
    return desc;
}
