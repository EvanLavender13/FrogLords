cmake_minimum_required(VERSION 3.10)
project(FrogLords)

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
    src/camera/camera.cpp
    src/character/character_controller.cpp
    src/character/orientation.cpp
    src/character/locomotion.cpp
    src/foundation/easing.cpp
    src/foundation/collision.cpp
    src/foundation/spring_damper.cpp
    src/rendering/scene.cpp
    src/rendering/test_terrain.cpp
    src/rendering/wireframe.cpp
    src/rendering/renderer.cpp
    src/input/input.cpp
    src/gui/gui.cpp
    ${WIREFRAME_SHADER_OUTPUT}
    ${IMGUI_SOURCES}
)

