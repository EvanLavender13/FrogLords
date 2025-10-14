#include "app/debug_generation.h"
#include "app/game_world.h"
#include "gui/character_panel.h"
#include "rendering/debug_primitives.h"
#include "foundation/math_utils.h"
#include "character/locomotion.h"
#include "character/skeleton.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace {

auto safe_normalize = [](const glm::vec3& v, const glm::vec3& fallback) {
    float len = glm::length(v);
    if (len > 0.0001f) {
        return v / len;
    }
    return fallback;
};

// Anonymous namespace for helper functions that translate game state into debug primitives.

void generate_character_state_primitives(debug::debug_primitive_list& list,
                                         const controller& character,
                                         const locomotion_system& locomotion,
                                         const orientation_system& orientation) {
    // Collision sphere
    list.spheres.push_back(debug::debug_sphere{
        .center = character.collision_sphere.center,
        .radius = character.collision_sphere.radius,
        .color = {0, 1, 1, 1},
        .segments = 12, // Smoother than the default 8
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
    float yaw = orientation.get_yaw();
    glm::vec3 forward_dir = math::yaw_to_forward(yaw);
    list.spheres.push_back(debug::debug_sphere{
        .center = character.position + forward_dir * 0.8f,
        .radius = 0.1f,
        .color = {0, 1, 0, 1},
    });
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

void generate_skeleton_primitives(debug::debug_primitive_list& list,
                                  const character::skeleton& skel, bool show_labels) {
    if (skel.joints.empty())
        return;

    std::vector<glm::vec3> joint_positions;
    joint_positions.reserve(skel.joints.size());

    for (const auto& joint : skel.joints) {
        glm::vec3 joint_pos = glm::vec3(joint.model_transform[3]);
        joint_positions.push_back(joint_pos);

        list.spheres.push_back(debug::debug_sphere{
            .center = joint_pos,
            .radius = 0.05f,
            .color = {1.0f, 1.0f, 0.0f, 1.0f},
        });

        if (show_labels) {
            list.texts.push_back(debug::debug_text{
                .text = joint.name,
                .position = joint_pos,
                .color = {1.0f, 1.0f, 0.0f, 1.0f},
            });
        }
    }

    for (size_t i = 0; i < skel.joints.size(); ++i) {
        if (skel.joints[i].parent_index != character::NO_PARENT) {
            list.lines.push_back(debug::debug_line{
                .start = joint_positions[i],
                .end = joint_positions[skel.joints[i].parent_index],
                .color = {1.0f, 1.0f, 1.0f, 1.0f},
            });
        }
    }
}

void generate_axis_gizmo_primitives(debug::debug_primitive_list& list,
                                    const character::skeleton& skel, float axis_length) {
    if (skel.joints.empty()) {
        return;
    }

    const glm::mat4& root_transform = skel.joints[0].model_transform;
    glm::vec3 root_position = glm::vec3(root_transform[3]);

    glm::vec3 basis_x = safe_normalize(glm::vec3(root_transform[0]), {1, 0, 0});
    glm::vec3 basis_y = safe_normalize(glm::vec3(root_transform[1]), {0, 1, 0});
    glm::vec3 basis_z = safe_normalize(glm::vec3(root_transform[2]), {0, 0, 1});

    list.arrows.push_back({root_position, root_position + basis_x * axis_length, {1, 0, 0, 1}});
    list.arrows.push_back({root_position, root_position + basis_y * axis_length, {0, 1, 0, 1}});
    list.arrows.push_back({root_position, root_position + basis_z * axis_length, {0, 0, 1, 1}});

    list.texts.push_back({"X", root_position + basis_x * (axis_length + 0.1f), {1, 0, 0, 1}});
    list.texts.push_back({"Y", root_position + basis_y * (axis_length + 0.1f), {0, 1, 0, 1}});
    list.texts.push_back({"Z", root_position + basis_z * (axis_length + 0.1f), {0, 0, 1, 1}});
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
    generate_character_state_primitives(list, world.character, world.locomotion,
                                        world.character.orientation);

    if (panel_state.show_velocity_trail) {
        generate_velocity_trail_primitives(list, world.trail_state);
    }

    if (panel_state.show_skeleton) {
        generate_skeleton_primitives(list, world.t_pose_skeleton, panel_state.show_joint_labels);
    }

    if (panel_state.show_axis_gizmo) {
        generate_axis_gizmo_primitives(list, world.t_pose_skeleton, 0.6f);
    }
}

} // namespace app
