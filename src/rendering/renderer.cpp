#include "renderer.h"
#include <wireframe_shader.h>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

wireframe_renderer::wireframe_renderer()
    : pipeline({0})
    , shader({0}) {}

wireframe_renderer::~wireframe_renderer() {
    shutdown();
}

void wireframe_renderer::init() {
    if (initialized)
        return;

    shader = sg_make_shader(wireframe_shader_desc(sg_query_backend()));

    // Create pipeline for line rendering
    sg_pipeline_desc pipeline_desc = {};
    pipeline_desc.shader = shader;
    pipeline_desc.primitive_type = SG_PRIMITIVETYPE_LINES;
    pipeline_desc.index_type = SG_INDEXTYPE_UINT16;

    // Vertex layout
    pipeline_desc.layout.attrs[ATTR_wireframe_position].format = SG_VERTEXFORMAT_FLOAT3;

    // Depth test
    pipeline_desc.depth.compare = SG_COMPAREFUNC_LESS_EQUAL;
    pipeline_desc.depth.write_enabled = true;

    pipeline = sg_make_pipeline(&pipeline_desc);

    // Create persistent dynamic buffers for streaming geometry
    sg_buffer_desc vbuf_desc = {};
    vbuf_desc.size = 65536; // 64KB - generous size for dynamic vertex data
    vbuf_desc.usage.stream_update = true;
    vbuf_desc.usage.vertex_buffer = true;
    dynamic_vertex_buffer = sg_make_buffer(&vbuf_desc);

    sg_buffer_desc ibuf_desc = {};
    ibuf_desc.size = 65536; // 64KB - generous size for dynamic index data
    ibuf_desc.usage.stream_update = true;
    ibuf_desc.usage.index_buffer = true;
    dynamic_index_buffer = sg_make_buffer(&ibuf_desc);

    initialized = true;
}

void wireframe_renderer::shutdown() {
    if (!initialized)
        return;

    sg_destroy_buffer(dynamic_index_buffer);
    sg_destroy_buffer(dynamic_vertex_buffer);
    sg_destroy_pipeline(pipeline);
    sg_destroy_shader(shader);

    initialized = false;
}

void wireframe_renderer::draw(const foundation::wireframe_mesh& mesh, const camera& cam,
                              float aspect_ratio, const glm::vec4& color) {
    if (!initialized)
        return;
    if (mesh.vertices.empty() || mesh.edges.empty())
        return;

    // Build MVP matrix
    glm::mat4 model = mesh.get_model_matrix();
    glm::mat4 view = cam.get_view_matrix();
    glm::mat4 projection = cam.get_projection_matrix(aspect_ratio);
    glm::mat4 mvp = projection * view * model;

    // Convert edges to line indices
    std::vector<uint16_t> indices;
    indices.reserve(mesh.edges.size() * 2);
    for (const foundation::edge& e : mesh.edges) {
        indices.push_back(static_cast<uint16_t>(e.v0));
        indices.push_back(static_cast<uint16_t>(e.v1));
    }

    // Update persistent dynamic buffers with this mesh's data
    sg_update_buffer(dynamic_vertex_buffer, &(sg_range){
        .ptr = mesh.vertices.data(),
        .size = mesh.vertices.size() * sizeof(glm::vec3)
    });

    sg_update_buffer(dynamic_index_buffer, &(sg_range){
        .ptr = indices.data(),
        .size = indices.size() * sizeof(uint16_t)
    });

    // Bind persistent buffers
    sg_bindings draw_bindings = {};
    draw_bindings.vertex_buffers[0] = dynamic_vertex_buffer;
    draw_bindings.index_buffer = dynamic_index_buffer;

    sg_apply_pipeline(pipeline);
    sg_apply_bindings(&draw_bindings);

    // Set uniforms
    vs_params_t vs_params = {};
    memcpy(&vs_params.mvp, glm::value_ptr(mvp), sizeof(glm::mat4));
    sg_range vs_range = SG_RANGE(vs_params);
    sg_apply_uniforms(UB_vs_params, &vs_range);

    fs_params_t fs_params = {};
    fs_params.color[0] = color.r;
    fs_params.color[1] = color.g;
    fs_params.color[2] = color.b;
    fs_params.color[3] = color.a;
    sg_range fs_range = SG_RANGE(fs_params);
    sg_apply_uniforms(UB_fs_params, &fs_range);

    // Draw
    sg_draw(0, static_cast<int>(indices.size()), 1);
}
