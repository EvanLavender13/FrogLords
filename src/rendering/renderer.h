#pragma once

#include "sokol_gfx.h"
#include "foundation/procedural_mesh.h"
#include "camera/camera.h"
#include <glm/glm.hpp>

class wireframe_renderer {
  public:
    wireframe_renderer();
    ~wireframe_renderer();

    /// Initialize renderer resources (call after sokol_gfx init)
    void init();

    /// Release renderer resources
    void shutdown();

    /// Render wireframe mesh using camera transform
    /// @param mesh Wireframe mesh to render
    /// @param cam Camera for view/projection matrices
    /// @param aspect_ratio Viewport width/height ratio
    /// @param color Line color (RGBA, defaults to white)
    void draw(const foundation::wireframe_mesh& mesh, const camera& cam, float aspect_ratio,
              const glm::vec4& color = glm::vec4(1.0f));

  private:
    sg_pipeline pipeline;
    sg_shader shader;
    sg_buffer dynamic_vertex_buffer;
    sg_buffer dynamic_index_buffer;

    bool initialized = false;
};
