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

    // Velocity indicator (replaced by cyan/yellow arrows showing intent vs constrained)
    // Red sphere removed - arrows show the same information with more detail

    // Orientation indicator
    float yaw = visuals.orientation.get_yaw();
    glm::vec3 forward_dir = math::yaw_to_forward(yaw);
    list.spheres.push_back(debug::debug_sphere{
        .center = character.position + forward_dir * 0.8f,
        .radius = 0.1f,
        .color = {0, 1, 0, 1},
    });

    // Speed gradient ring - continuous interpolation
    float current_speed = glm::length(math::project_to_horizontal(character.velocity));
    if (current_speed > 0.05f) {
        float speed_ratio = glm::clamp(current_speed / character.max_speed, 0.0f, 1.0f);

        // Gradient: blue → cyan → yellow → red
        constexpr glm::vec3 gradient[] = {
            {0.0f, 0.0f, 1.0f}, // Blue
            {0.0f, 1.0f, 1.0f}, // Cyan
            {1.0f, 1.0f, 0.0f}, // Yellow
            {1.0f, 0.0f, 0.0f}, // Red
        };
        constexpr int num_stops = sizeof(gradient) / sizeof(gradient[0]);

        // Map speed_ratio to continuous position in gradient
        float position = speed_ratio * (num_stops - 1);
        int index = glm::clamp(static_cast<int>(position), 0, num_stops - 2);
        float t = position - index;

        // Interpolate between adjacent stops
        glm::vec3 rgb = glm::mix(gradient[index], gradient[index + 1], t);
        glm::vec4 color = glm::vec4(rgb, 0.8f);

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

    // Color-code character body by locomotion state
    glm::vec4 body_color;
    switch (character.locomotion.state) {
    case controller::locomotion_speed_state::walk:
        body_color = {0.2f, 1.0f, 0.2f, 1.0f}; // Green = walk
        break;
    case controller::locomotion_speed_state::run:
        body_color = {1.0f, 1.0f, 0.2f, 1.0f}; // Yellow = run
        break;
    case controller::locomotion_speed_state::sprint:
        body_color = {1.0f, 0.2f, 0.2f, 1.0f}; // Red = sprint
        break;
    }

    // Manually transform the vertices and add them as debug lines

    for (const auto& edge : body_mesh.edges) {

        glm::vec3 v0 = glm::vec3(transform * glm::vec4(body_mesh.vertices[edge.v0], 1.0f));

        glm::vec3 v1 = glm::vec3(transform * glm::vec4(body_mesh.vertices[edge.v1], 1.0f));

        list.lines.push_back(debug::debug_line{v0, v1, body_color});
    }
}

void generate_locomotion_surveyor_wheel(debug::debug_primitive_list& list,
                                        const controller& character,
                                        const character_reactive_systems& visuals) {
    // Surveyor wheel visualization (David Rosen GDC technique)
    // Shows phase (rotation) and cycle length (size) for distance-based cyclic motion
    // Wheel is vertical like a bicycle wheel, axis perpendicular to movement direction

    glm::vec3 wheel_center = character.position; // Centered on character

    // Wheel radius scales with cycle length (from locomotion_state output)
    float cycle_length = character.locomotion.cycle_length;
    float wheel_radius = cycle_length * 0.25f;

    // Wheel rotation based on distance traveled (not phase) for visual continuity
    // Phase recalculates on state change, but distance is continuous
    // rotation_angle = distance / radius (standard wheel physics)
    // Negate for correct forward roll direction
    float rotation_angle = -(character.distance_traveled / wheel_radius);

    // Get character facing direction from orientation system
    float yaw = visuals.orientation.get_yaw();
    glm::vec3 forward_dir = math::yaw_to_forward(yaw);

    // Draw circle (vertical, in forward-up plane like a bicycle wheel)
    constexpr int circle_segments = 24;
    glm::vec4 circle_color = {1.0f, 1.0f, 1.0f, 0.8f}; // White

    for (int i = 0; i < circle_segments; ++i) {
        float angle0 = (static_cast<float>(i) / circle_segments) * 2.0f * glm::pi<float>();
        float angle1 = (static_cast<float>(i + 1) / circle_segments) * 2.0f * glm::pi<float>();

        // Circle in forward-up plane (rotates around right_dir axis)
        glm::vec3 p0 = wheel_center + forward_dir * std::cos(angle0) * wheel_radius +
                       math::UP * std::sin(angle0) * wheel_radius;
        glm::vec3 p1 = wheel_center + forward_dir * std::cos(angle1) * wheel_radius +
                       math::UP * std::sin(angle1) * wheel_radius;

        list.lines.push_back(debug::debug_line{p0, p1, circle_color});
    }

    // Draw "+" cross rotated by phase (rolling around right_dir axis)
    glm::vec4 cross_color;
    switch (character.locomotion.state) {
    case controller::locomotion_speed_state::walk:
        cross_color = {0.2f, 1.0f, 0.2f, 1.0f}; // Green = walk
        break;
    case controller::locomotion_speed_state::run:
        cross_color = {1.0f, 1.0f, 0.2f, 1.0f}; // Yellow = run
        break;
    case controller::locomotion_speed_state::sprint:
        cross_color = {1.0f, 0.2f, 0.2f, 1.0f}; // Red = sprint
        break;
    }

    // First spoke (rotated by phase around right_dir axis, in forward-up plane)
    glm::vec3 spoke1_dir =
        forward_dir * std::cos(rotation_angle) + math::UP * std::sin(rotation_angle);
    glm::vec3 s1_0 = wheel_center + spoke1_dir * wheel_radius;
    glm::vec3 s1_1 = wheel_center - spoke1_dir * wheel_radius;
    list.lines.push_back(debug::debug_line{s1_0, s1_1, cross_color});

    // Perpendicular spoke (90° offset)
    glm::vec3 spoke2_dir = forward_dir * std::cos(rotation_angle + glm::pi<float>() / 2.0f) +
                           math::UP * std::sin(rotation_angle + glm::pi<float>() / 2.0f);
    glm::vec3 s2_0 = wheel_center + spoke2_dir * wheel_radius;
    glm::vec3 s2_1 = wheel_center - spoke2_dir * wheel_radius;
    list.lines.push_back(debug::debug_line{s2_0, s2_1, cross_color});
}

void generate_collision_state_primitives(debug::debug_primitive_list& list,
                                         const controller& character,
                                         const collision_world& world) {
    // Type-based colors for semantic collision boxes
    constexpr glm::vec4 FLOOR_COLOR = {0.3f, 1.0f, 0.3f, 1.0f};    // Green
    constexpr glm::vec4 WALL_COLOR = {1.0f, 0.0f, 1.0f, 1.0f};     // Magenta
    constexpr glm::vec4 PLATFORM_COLOR = {1.0f, 1.0f, 0.3f, 1.0f}; // Yellow
    constexpr glm::vec4 GENERIC_COLOR = {0.5f, 0.5f, 0.5f, 1.0f};  // Gray

    // World geometry boxes
    for (const auto& box : world.boxes) {
        glm::vec4 color;
        switch (box.type) {
        case collision_surface_type::FLOOR:
            color = FLOOR_COLOR;
            break;
        case collision_surface_type::WALL:
            color = WALL_COLOR;
            break;
        case collision_surface_type::PLATFORM:
            color = PLATFORM_COLOR;
            break;
        default:
            color = GENERIC_COLOR;
            break;
        }

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), box.bounds.center);
        list.boxes.push_back(debug::debug_box{
            .transform = transform,
            .half_extents = box.bounds.half_extents,
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

    // Wall sliding debug visualization
    if (character.collision_contact_debug.active) {
        const auto& debug = character.collision_contact_debug;

        // Draw collision normal (color-coded by surface type)
        glm::vec4 normal_color;
        if (debug.is_wall) {
            normal_color = {1, 0, 0, 1}; // Red = Wall
        } else if (debug.normal.y > 0.5f) {
            normal_color = {0, 1, 0, 1}; // Green = Floor
        } else {
            normal_color = {0, 0, 1, 1}; // Blue = Ceiling
        }

        list.arrows.push_back(debug::debug_arrow{
            .start = character.position,
            .end = character.position + debug.normal * 1.0f,
            .color = normal_color,
            .head_size = 0.15f,
        });

        // Velocity arrow (replaces red velocity sphere - shows ground truth)
        if (glm::length(character.velocity) > 0.01f) {
            list.arrows.push_back(debug::debug_arrow{
                .start = character.position,
                .end = character.position + character.velocity,
                .color = {1.0f, 0.0f, 0.0f, 1.0f}, // Red = actual velocity (ground truth)
                .head_size = 0.15f,
            });
        }
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
    generate_locomotion_surveyor_wheel(list, world.character, world.character_visuals);

    if (panel_state.show_velocity_trail) {
        generate_velocity_trail_primitives(list, world.trail_state);
    }
}

} // namespace app