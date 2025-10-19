cmake_minimum_required(VERSION 3.10)
project(FrogLords)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set debug/release flags
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Add debug definitions for Debug builds
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_definitions(_DEBUG)
endif()

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/external/sokol)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui)
include_directories(${CMAKE_SOURCE_DIR}/external/glm)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/generated)

# sokol-shdc shader compiler
set(SOKOL_SHDC ${CMAKE_SOURCE_DIR}/external/sokol-tools-bin/bin/win32/sokol-shdc.exe)

# Compile wireframe shader
set(WIREFRAME_SHADER_INPUT ${CMAKE_SOURCE_DIR}/shaders/wireframe.glsl)
set(WIREFRAME_SHADER_OUTPUT ${CMAKE_SOURCE_DIR}/generated/wireframe_shader.h)

add_custom_command(
    OUTPUT ${WIREFRAME_SHADER_OUTPUT}
    COMMAND ${SOKOL_SHDC} --input ${WIREFRAME_SHADER_INPUT} --output ${WIREFRAME_SHADER_OUTPUT} --slang glsl410:hlsl5:metal_macos
    DEPENDS ${WIREFRAME_SHADER_INPUT}
    COMMENT "Compiling wireframe shader"
)

# Dear ImGui sources (required for compilation)
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
)

add_executable(FrogLords
    src/sokol_impl.cpp
    src/main.cpp
    src/app/runtime.cpp
    src/app/game_world.cpp
    src/app/debug_generation.cpp
    src/camera/camera.cpp
    src/camera/camera_follow.cpp
    src/character/controller.cpp
    src/character/character_reactive_systems.cpp
    src/character/orientation.cpp
    src/character/animation.cpp
    src/character/tuning.cpp
    src/foundation/easing.cpp
    src/foundation/collision.cpp
    src/foundation/spring_damper.cpp
    src/foundation/procedural_mesh.cpp
    src/rendering/scene.cpp
    src/rendering/renderer.cpp
    src/rendering/debug_draw.cpp
    src/rendering/debug_visualization.cpp
    src/rendering/debug_validation.cpp
    src/input/input.cpp
    src/gui/gui.cpp
    src/gui/character_panel.cpp
    src/gui/camera_panel.cpp
    ${WIREFRAME_SHADER_OUTPUT}
    ${IMGUI_SOURCES}
)

if (WIN32)
    target_link_libraries(FrogLords PRIVATE d3d11 dxgi dxguid)
endif()

# Add test subdirectory
add_subdirectory(tests)
