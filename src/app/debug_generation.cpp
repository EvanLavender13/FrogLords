#include "app/debug_generation.h"
#include "app/game_world.h"
#include "gui/character_panel.h"
#include "rendering/debug_primitives.h"
#include "foundation/procedural_mesh.h"
#include "foundation/math_utils.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace {

void mesh_to_debug_lines(debug::debug_primitive_list& list, const foundation::wireframe_mesh& mesh,
                         const glm::vec4& color) {
    glm::mat4 transform = mesh.get_model_matrix();
    for (const auto& edge : mesh.edges) {
        glm::vec3 v0 = glm::vec3(transform * glm::vec4(mesh.vertices[edge.v0], 1.0f));
        glm::vec3 v1 = glm::vec3(transform * glm::vec4(mesh.vertices[edge.v1], 1.0f));
        list.lines.push_back(debug::debug_line{v0, v1, color});
    }
}

// Anonymous namespace for helper functions that translate game state into debug primitives.

void generate_character_state_primitives(debug::debug_primitive_list& list,
                                         const controller& character,
                                         const character_reactive_systems& visuals) {
    // Collision sphere
    list.spheres.push_back(debug::debug_sphere{
        .center = character.collision_sphere.center,
        .radius = character.collision_sphere.radius,
        .color = {0, 1, 1, 1},
        .segments = 12,
    });

    // Velocity indicator
    if (glm::length(character.velocity) > 0.1f) {
        list.spheres.push_back(debug::debug_sphere{
            .center = character.position + character.velocity,
            .radius = 0.1f,
            .color = {1, 0, 0, 1},
        });
    }

    // Orientation indicator
    float yaw = visuals.orientation.get_yaw();
    glm::vec3 forward_dir = math::yaw_to_forward(yaw);
    list.spheres.push_back(debug::debug_sphere{
        .center = character.position + forward_dir * 0.8f,
        .radius = 0.1f,
        .color = {0, 1, 0, 1},
    });

    // Speed gradient ring
    float current_speed = glm::length(math::project_to_horizontal(character.velocity));
    if (current_speed > 0.05f) {
        float speed_ratio = glm::clamp(current_speed / character.max_speed, 0.0f, 1.0f);
        glm::vec4 color;
        if (speed_ratio < 0.33f) {
            float t = speed_ratio / 0.33f;
            color = glm::vec4(0.0f, t, 1.0f - t, 0.8f);
        } else if (speed_ratio < 0.66f) {
            float t = (speed_ratio - 0.33f) / 0.33f;
            color = glm::vec4(t, 1.0f, 0.0f, 0.8f);
        } else {
            float t = (speed_ratio - 0.66f) / 0.34f;
            color = glm::vec4(1.0f, 1.0f - t, 0.0f, 0.8f);
        }
        foundation::wireframe_mesh speed_ring =
            foundation::generate_circle(character.position, {current_speed});
        mesh_to_debug_lines(list, speed_ring, color);
    }
}

void generate_physics_springs_primitives(debug::debug_primitive_list& list,
                                         const controller& character,
                                         const character_reactive_systems& visuals) {
    float spring_offset = visuals.animation.get_vertical_offset();
    glm::vec3 spring_bottom = character.collision_sphere.center;
    spring_bottom.y -= character.collision_sphere.radius;
    glm::vec3 spring_top = character.position;
    spring_top.y += spring_offset;

    foundation::wireframe_mesh spring_mesh =
        foundation::generate_spring(spring_bottom, spring_top, 8, 0.2f);

    glm::vec4 spring_color;
    if (spring_offset < -0.01f) {
        float compression_factor = glm::min(1.0f, -spring_offset / 0.3f);
        spring_color = glm::vec4(1.0f, 0.8f, 0.1f, 0.7f + compression_factor * 0.3f);
    } else {
        spring_color = glm::vec4(0.6f, 0.6f, 0.6f, 0.25f);
    }
    mesh_to_debug_lines(list, spring_mesh, spring_color);
}

void generate_character_body_primitives(debug::debug_primitive_list& list,
                                        const controller& character,
                                        const character_reactive_systems& visuals) {

    // Get the full world transform from the reactive systems, which includes tilt and offset

    glm::mat4 transform = visuals.get_visual_transform(character);

    // Generate the character body box in local space

    foundation::wireframe_mesh body_mesh = foundation::generate_box({0.4f, 0.8f, 0.3f});

    // Manually transform the vertices and add them as debug lines

    for (const auto& edge : body_mesh.edges) {

        glm::vec3 v0 = glm::vec3(transform * glm::vec4(body_mesh.vertices[edge.v0], 1.0f));

        glm::vec3 v1 = glm::vec3(transform * glm::vec4(body_mesh.vertices[edge.v1], 1.0f));

        list.lines.push_back(debug::debug_line{v0, v1, {1.0f, 0.2f, 1.0f, 1.0f}});
    }
}

void generate_collision_state_primitives(debug::debug_primitive_list& list,
                                         const controller& character,
                                         const collision_world& world) {
    // World geometry boxes
    for (const auto& box : world.boxes) {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), box.center);
        glm::vec4 color;
        if (box.half_extents.y < 0.4f && box.half_extents.x < 1.2f && box.half_extents.z < 1.2f &&
            box.center.y < 1.0f) {
            float height_ratio = box.half_extents.y / 0.3f;
            color = {0.2f, 0.5f + height_ratio * 0.5f, 1.0f, 1.0f};
        } else if (box.half_extents.y > box.half_extents.x &&
                   box.half_extents.y > box.half_extents.z) {
            color = {1.0f, 0.5f, 0.2f, 1.0f};
        } else {
            color = {0.3f, 1.0f, 0.3f, 1.0f};
        }
        list.boxes.push_back(debug::debug_box{
            .transform = transform,
            .half_extents = box.half_extents,
            .color = color,
        });
    }

    // Ground contact point
    if (character.is_grounded) {
        list.spheres.push_back(debug::debug_sphere{
            .center = character.collision_sphere.center -
                      glm::vec3(0, character.collision_sphere.radius, 0),
            .radius = 0.05f,
            .color = {0, 1, 0, 1},
        });
    }
}

void generate_velocity_trail_primitives(debug::debug_primitive_list& list,
                                        const velocity_trail_state& trail) {
    if (trail.positions.empty()) {
        return;
    }

    for (size_t i = 0; i < trail.positions.size(); ++i) {
        float age_factor =
            trail.positions.size() > 1
                ? static_cast<float>(i) / static_cast<float>(trail.positions.size() - 1)
                : 1.0f;

        float radius = 0.05f + (0.15f - 0.05f) * age_factor;
        float alpha = 0.2f + (0.8f - 0.2f) * age_factor;

        list.spheres.push_back(debug::debug_sphere{
            .center = trail.positions[i],
            .radius = radius,
            .color = {1.0f, 1.0f, 1.0f, alpha},
            .segments = 4,
        });
    }
}

} // namespace

namespace app {

void generate_debug_primitives(debug::debug_primitive_list& list, const game_world& world,
                               const gui::character_panel_state& panel_state) {
    // This function orchestrates calls to the various generation helpers.
    generate_collision_state_primitives(list, world.character, world.world_geometry);
    generate_character_state_primitives(list, world.character, world.character_visuals);
    generate_physics_springs_primitives(list, world.character, world.character_visuals);
    generate_character_body_primitives(list, world.character, world.character_visuals);

    if (panel_state.show_velocity_trail) {
        generate_velocity_trail_primitives(list, world.trail_state);
    }
}

} // namespace app