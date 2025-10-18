# FrogLords Test Suite
cmake_minimum_required(VERSION 3.10)

# Test executable for spring-damper validation
add_executable(test_spring_damper
    foundation/test_spring_damper.cpp
    ${CMAKE_SOURCE_DIR}/src/foundation/spring_damper.cpp
)

target_include_directories(test_spring_damper PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_compile_features(test_spring_damper PRIVATE cxx_std_20)
