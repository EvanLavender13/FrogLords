#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include <vector>

namespace debug {

struct debug_sphere {
    glm::vec3 center;
    float radius;
    glm::vec4 color;
    int segments = 8;
};

struct debug_line {
    glm::vec3 start;
    glm::vec3 end;
    glm::vec4 color;
};

struct debug_arrow {
    glm::vec3 start;
    glm::vec3 end;
    glm::vec4 color;
    float head_size = 0.1f;
};

struct debug_box {
    glm::mat4 transform;
    glm::vec3 half_extents;
    glm::vec4 color;
};

struct debug_text {
    std::string text;
    glm::vec3 position;
    glm::vec4 color;
    float font_size = 18.0f;
};

struct debug_primitive_list {
    std::vector<debug_sphere> spheres;
    std::vector<debug_line> lines;
    std::vector<debug_arrow> arrows;
    std::vector<debug_box> boxes;
    std::vector<debug_text> texts;

    void clear() {
        spheres.clear();
        lines.clear();
        arrows.clear();
        boxes.clear();
        texts.clear();
    }
};

} // namespace debug
