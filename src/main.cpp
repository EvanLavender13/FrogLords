#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#include "app/runtime.h"

static void init(void) {
    runtime().initialize();
}

static void frame(void) {
    runtime().frame();
}

static void cleanup(void) {
    runtime().shutdown();
}

static void event(const sapp_event* e) {
    runtime().handle_event(e);
}

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    sapp_desc desc = {};
    desc.init_cb = init;
    desc.frame_cb = frame;
    desc.cleanup_cb = cleanup;
    desc.event_cb = event;
    desc.width = 1920;
    desc.height = 1080;
    desc.window_title = "FrogLords - GUI Demo";
    desc.icon.sokol_default = true;
    desc.enable_clipboard = true;
    desc.logger.func = slog_func;
    return desc;
}
