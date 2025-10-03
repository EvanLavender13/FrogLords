// Sokol implementation file
// All SOKOL_IMPL definitions should be here and only here
// to avoid redefinition errors across multiple translation units

#define SOKOL_IMPL
#define SOKOL_WIN32
#define SOKOL_D3D11
#define SOKOL_WIN32_FORCE_MAIN

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#include "sokol_time.h"

// sokol_imgui implementation
#include "imgui.h"
#define SOKOL_IMGUI_IMPL
#include "sokol_imgui.h"
