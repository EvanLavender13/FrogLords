#include "app/debug_generation.h"
#include "app/game_world.h"
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
                                         const vehicle_reactive_systems& visuals) {
    // Collision sphere
    list.spheres.push_back(debug::debug_sphere{
        .center = character.collision_sphere.center,
        .radius = character.collision_sphere.radius,
        .color = {0, 1, 1, 1},
        .segments = 12,
    });

    // Velocity indicator (replaced by cyan/yellow arrows showing intent vs constrained)
    // Red sphere removed - arrows show the same information with more detail

    // Calculate horizontal speed once (used by both orientation arrow and speed ring)
    float current_speed = glm::length(math::project_to_horizontal(character.velocity));

    // Orientation indicator (scales with horizontal speed)
    if (current_speed > 0.05f) {
        float yaw = visuals.orientation.get_yaw();
        glm::vec3 forward_dir = math::yaw_to_forward(yaw);
        list.arrows.push_back(debug::debug_arrow{
            .start = character.position,
            .end = character.position + forward_dir * current_speed,
            .color = {0.0f, 1.0f, 0.0f, 1.0f}, // Green = orientation
            .head_size = 0.15f,
        });
    }

    // Speed gradient ring - continuous interpolation
    if (current_speed > 0.05f) {
        float speed_ratio = glm::clamp(current_speed / character.max_speed, 0.0f, 1.0f);

        // Gradient: blue → cyan → yellow → red
        constexpr glm::vec3 GRADIENT[] = {
            {0.0f, 0.0f, 1.0f}, // Blue
            {0.0f, 1.0f, 1.0f}, // Cyan
            {1.0f, 1.0f, 0.0f}, // Yellow
            {1.0f, 0.0f, 0.0f}, // Red
        };
        constexpr int NUM_STOPS = sizeof(GRADIENT) / sizeof(GRADIENT[0]);

        // Map speed_ratio to continuous position in gradient
        float position = speed_ratio * (NUM_STOPS - 1);
        int index = glm::clamp(static_cast<int>(position), 0, NUM_STOPS - 2);
        float t = position - index;

        // Interpolate between adjacent stops
        glm::vec3 rgb = glm::mix(GRADIENT[index], GRADIENT[index + 1], t);
        glm::vec4 color = glm::vec4(rgb, 0.8f);

        foundation::wireframe_mesh speed_ring =
            foundation::generate_circle(character.position, {current_speed});
        mesh_to_debug_lines(list, speed_ring, color);
    }

    // Lateral g-force indicator (centripetal acceleration)
    float g_force = character.calculate_lateral_g_force();
    constexpr float G_FORCE_THRESHOLD = 0.05f; // Show arrow above 0.05g
    if (std::abs(g_force) > G_FORCE_THRESHOLD) {
        // Arrow direction: perpendicular to heading (right vector)
        // Negative g-force = right turn = arrow points right (toward center)
        // Positive g-force = left turn = arrow points left (toward center)
        glm::vec3 right_dir = math::yaw_to_right(character.heading_yaw);

        // Scale arrow for visibility (3.0 meters per g), direction encodes sign
        constexpr float SCALE = 3.0f;
        glm::vec3 arrow_vector = right_dir * (g_force * SCALE);

        list.arrows.push_back(debug::debug_arrow{
            .start = character.position,
            .end = character.position + arrow_vector,
            .color = {1.0f, 0.0f, 1.0f, 1.0f}, // Magenta = lateral g-force
            .head_size = 0.15f,
        });
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
        list.lines.push_back(debug::debug_line{v0, v1, {0.2f, 1.0f, 0.2f, 1.0f}});
    }
}

void generate_vehicle_body_primitives(debug::debug_primitive_list& list,
                                      const controller& character,
                                      const vehicle_reactive_systems& visuals) {

    // Get the full world transform from the vehicle visual systems, which includes tilt
    glm::mat4 transform = visuals.get_visual_transform(character);

    // Generate the vehicle body box in local space (long vehicle proportions)
    // X=width, Y=height, Z=length (forward is +Z)
    foundation::wireframe_mesh body_mesh = foundation::generate_box({0.6f, 0.4f, 1.2f});

    // Manually transform the vertices and add them as debug lines
    for (const auto& edge : body_mesh.edges) {
        glm::vec3 v0 = glm::vec3(transform * glm::vec4(body_mesh.vertices[edge.v0], 1.0f));
        glm::vec3 v1 = glm::vec3(transform * glm::vec4(body_mesh.vertices[edge.v1], 1.0f));
        list.lines.push_back(debug::debug_line{v0, v1, {0.2f, 1.0f, 0.2f, 1.0f}});
    }
}

// Removed: generate_locomotion_surveyor_wheel (used locomotion-specific fields)

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
        if (debug.normal.y > 0.5f) {
            normal_color = {0, 1, 0, 1}; // Green = Floor
        } else if (debug.normal.y < -0.5f) {
            normal_color = {0, 0, 1, 1}; // Blue = Ceiling
        } else {
            normal_color = {1, 0, 0, 1}; // Red = Wall/Slope
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

void generate_car_control_primitives(debug::debug_primitive_list& list, const controller& character,
                                     control_scheme scheme) {
    if (scheme == control_scheme::CAR_LIKE) {
        // Draw heading yaw arrow (yellow for car control, scales with speed like green arrow)
        float current_speed = glm::length(math::project_to_horizontal(character.velocity));
        if (current_speed > 0.05f) {
            float yaw = character.heading_yaw;
            glm::vec3 forward = math::yaw_to_forward(yaw);
            list.arrows.push_back(debug::debug_arrow{
                .start = character.position,
                .end = character.position + forward * current_speed,
                .color = {1.0f, 1.0f, 0.0f, 1.0f}, // Yellow = car heading
                .head_size = 0.2f,
            });

            // VISUAL VALIDATION: Steering authority cone
            // Shows maximum possible turn angle at current speed
            // Cone width shrinks at high speeds, making steering reduction obvious
            float steering_multiplier = character.compute_steering_multiplier(current_speed);
            float max_turn_angle =
                character.turn_rate * steering_multiplier * 0.5f; // 0.5s lookahead

            // Left turn limit (CCW from heading)
            glm::vec3 left_limit = math::yaw_to_forward(yaw - max_turn_angle);
            list.arrows.push_back(debug::debug_arrow{
                .start = character.position,
                .end = character.position + left_limit * current_speed * 0.8f,
                .color = {1.0f, 0.5f, 0.0f, 0.6f}, // Orange, semi-transparent
                .head_size = 0.15f,
            });

            // Right turn limit (CW from heading)
            glm::vec3 right_limit = math::yaw_to_forward(yaw + max_turn_angle);
            list.arrows.push_back(debug::debug_arrow{
                .start = character.position,
                .end = character.position + right_limit * current_speed * 0.8f,
                .color = {1.0f, 0.5f, 0.0f, 0.6f}, // Orange, semi-transparent
                .head_size = 0.15f,
            });
        }
    }
}

} // namespace

namespace app {

void generate_debug_primitives(debug::debug_primitive_list& list, const game_world& world) {
    // This function orchestrates calls to the various generation helpers.
    generate_collision_state_primitives(list, world.character, world.world_geometry);
    generate_character_state_primitives(list, world.character, world.vehicle_reactive);
    generate_vehicle_body_primitives(list, world.character, world.vehicle_reactive);
    generate_car_control_primitives(list, world.character, world.current_control_scheme);
    generate_velocity_trail_primitives(list, world.trail_state);
}

} // namespace app