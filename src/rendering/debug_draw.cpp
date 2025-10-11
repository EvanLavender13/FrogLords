#include "rendering/debug_draw.h"
#include "rendering/scene.h"
#include "character/skeleton.h"
#include "foundation/math_utils.h"
#include "app/game_world.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include "imgui.h"

namespace debug {

void draw_character_state(draw_context& ctx, const controller& character,
                          const locomotion_system& locomotion,
                          const orientation_system& orientation) {
    simple_pose current_pose = locomotion.get_current_pose();
    glm::vec3 pose_offset = current_pose.root_offset;

    // Collision sphere (cyan = physics sphere)
    wireframe_mesh collision_vis = ctx.unit_sphere_8;
    collision_vis.position = character.collision_sphere.center;
    collision_vis.scale = glm::vec3(character.collision_sphere.radius);
    ctx.renderer.draw(collision_vis, ctx.cam, ctx.aspect, glm::vec4(0, 1, 1, 1));

    // Velocity indicator (no pose offset - attached to physics position)
    glm::vec3 vel = character.velocity;
    if (glm::length(vel) > 0.1f) {
        wireframe_mesh velocity_indicator = ctx.unit_sphere_4;
        velocity_indicator.position = character.position + vel;
        velocity_indicator.scale = glm::vec3(0.1f);
        ctx.renderer.draw(velocity_indicator, ctx.cam, ctx.aspect, glm::vec4(1, 0, 0, 1));
    }

    // Speed gradient ring (blue → green → yellow → red)
    glm::vec3 horiz_vel = math::project_to_horizontal(character.velocity);
    float current_speed = glm::length(horiz_vel);
    float max_speed = character.max_speed;

    if (current_speed > 0.05f) {
        float speed_ratio = glm::clamp(current_speed / max_speed, 0.0f, 1.0f);

        // Gradient color: blue → green → yellow → red
        glm::vec4 color;
        if (speed_ratio < 0.33f) {
            // Blue to green
            float t = speed_ratio / 0.33f;
            color = glm::vec4(0.0f, t, 1.0f - t, 0.8f);
        } else if (speed_ratio < 0.66f) {
            // Green to yellow
            float t = (speed_ratio - 0.33f) / 0.33f;
            color = glm::vec4(t, 1.0f, 0.0f, 0.8f);
        } else {
            // Yellow to red
            float t = (speed_ratio - 0.66f) / 0.34f;
            color = glm::vec4(1.0f, 1.0f - t, 0.0f, 0.8f);
        }

        wireframe_mesh speed_ring = ctx.unit_circle;
        speed_ring.position = character.position;
        speed_ring.scale = glm::vec3(current_speed, 1.0f, current_speed);
        ctx.renderer.draw(speed_ring, ctx.cam, ctx.aspect, color);
    }

    // Orientation indicator (attached to physics position)
    float yaw = orientation.get_yaw();
    glm::vec3 forward_dir = math::yaw_to_forward(yaw);
    wireframe_mesh yaw_indicator = ctx.unit_sphere_4;
    yaw_indicator.position = character.position + forward_dir * 0.8f;
    yaw_indicator.scale = glm::vec3(0.1f);
    ctx.renderer.draw(yaw_indicator, ctx.cam, ctx.aspect, glm::vec4(0, 1, 0, 1));
}

void draw_physics_springs(draw_context& ctx, const controller& character,
                          [[maybe_unused]] const locomotion_system& locomotion) {

    // Landing spring visualization: collision sphere bottom to character body center
    float spring_offset = character.animation.get_vertical_offset();

    // Spring bottom: bottom of collision sphere (acts as "feet")
    glm::vec3 spring_bottom = character.collision_sphere.center;
    spring_bottom.y -= character.collision_sphere.radius;

    // Spring top: character body center (affected by spring compression)
    glm::vec3 spring_top = character.position;
    spring_top.y += spring_offset; // Negative when compressed

    // Generate spring mesh (coils compress as spring compresses)
    int coil_count = 8;
    float spring_radius = 0.2f;
    wireframe_mesh spring = generate_spring(spring_bottom, spring_top, coil_count, spring_radius);

    // Color: bright yellow/orange when compressed, dim when at rest
    glm::vec4 spring_color;
    if (spring_offset < -0.01f) {
        // Compressed: bright yellow with intensity based on compression
        float compression_factor = glm::min(1.0f, -spring_offset / 0.3f);
        spring_color = glm::vec4(1.0f, 0.8f, 0.1f, 0.7f + compression_factor * 0.3f);
    } else {
        // At rest: subtle gray
        spring_color = glm::vec4(0.6f, 0.6f, 0.6f, 0.25f);
    }

    ctx.renderer.draw(spring, ctx.cam, ctx.aspect, spring_color);
}

void draw_locomotion_wheel(draw_context& ctx, const controller& character,
                           [[maybe_unused]] const locomotion_system& locomotion,
                           const orientation_system& orientation, float wheel_spin_angle) {
    // Phase information used for foot placement, not wheel center

    float yaw = orientation.get_yaw();
    glm::vec3 forward_dir = math::yaw_to_forward(yaw);

    float wheel_ground_y = character.collision_sphere.center.y - character.collision_sphere.radius;
    glm::vec3 wheel_center = character.position;
    wheel_center.y = wheel_ground_y + WHEEL_RADIUS;

    const int RIM_SEGMENTS = 24;
    const int SPOKE_COUNT = 4;

    wireframe_mesh wheel_mesh;
    wheel_mesh.vertices.reserve(RIM_SEGMENTS + 1);
    wheel_mesh.edges.reserve(RIM_SEGMENTS + SPOKE_COUNT);

    wheel_mesh.vertices.push_back(wheel_center);
    for (int i = 0; i < RIM_SEGMENTS; ++i) {
        float base_angle = static_cast<float>(i) / static_cast<float>(RIM_SEGMENTS) * TWO_PI;
        float rim_angle = base_angle - wheel_spin_angle;
        glm::vec3 offset = std::cos(rim_angle) * forward_dir + std::sin(rim_angle) * math::UP;
        wheel_mesh.vertices.push_back(wheel_center + offset * WHEEL_RADIUS);
    }

    for (int i = 0; i < RIM_SEGMENTS; ++i) {
        int current = 1 + i;
        int next = 1 + ((i + 1) % RIM_SEGMENTS);
        wheel_mesh.edges.emplace_back(current, next);
    }

    for (int i = 0; i < SPOKE_COUNT; ++i) {
        int rim_index = 1 + (i * RIM_SEGMENTS) / SPOKE_COUNT;
        wheel_mesh.edges.emplace_back(0, rim_index);
    }

    ctx.renderer.draw(wheel_mesh, ctx.cam, ctx.aspect, glm::vec4(0.85f, 0.85f, 0.85f, 1.0f));
}

void draw_foot_positions(draw_context& ctx, const controller& character,
                         const locomotion_system& locomotion,
                         const orientation_system& orientation) {
    float yaw = orientation.get_yaw();
    glm::vec3 forward_dir = math::yaw_to_forward(yaw);
    glm::vec3 right_dir = math::yaw_to_right(yaw);

    float wheel_ground_y = character.collision_sphere.center.y - character.collision_sphere.radius;
    float ground_contact_y = wheel_ground_y;

    // Left foot
    float left_foot_offset = std::sin(locomotion.phase * 2.0f * 3.14159f) * 0.4f;
    glm::vec3 left_foot_pos =
        character.position + forward_dir * left_foot_offset + right_dir * 0.2f;
    left_foot_pos.y = ground_contact_y;
    wireframe_mesh left_foot = ctx.unit_sphere_4;
    left_foot.position = left_foot_pos;
    left_foot.scale = glm::vec3(0.08f);
    ctx.renderer.draw(left_foot, ctx.cam, ctx.aspect, glm::vec4(1, 0, 1, 1));

    // Right foot
    float right_phase = std::fmod(locomotion.phase + 0.5f, 1.0f);
    float right_foot_offset = std::sin(right_phase * 2.0f * 3.14159f) * 0.4f;
    glm::vec3 right_foot_pos =
        character.position + forward_dir * right_foot_offset + right_dir * -0.2f;
    right_foot_pos.y = ground_contact_y;
    wireframe_mesh right_foot = ctx.unit_sphere_4;
    right_foot.position = right_foot_pos;
    right_foot.scale = glm::vec3(0.08f);
    ctx.renderer.draw(right_foot, ctx.cam, ctx.aspect, glm::vec4(1, 0.5f, 0, 1));
}

void draw_collision_state(draw_context& ctx, const controller& character, const scene& scn) {
    // Draw all collision boxes with distinct colors based on type
    for (const auto& box : scn.collision_boxes()) {
        box_dimensions dims{box.half_extents.x * 2.0f, box.half_extents.y * 2.0f,
                            box.half_extents.z * 2.0f};
        wireframe_mesh box_mesh = generate_box(dims);
        box_mesh.position = box.center;

        // Color based on box type (heuristic classification)
        glm::vec4 color;

        // Steps (small, square-ish boxes near ground)
        if (box.half_extents.y < 0.4f && box.half_extents.x < 1.2f && box.half_extents.z < 1.2f &&
            box.center.y < 1.0f) {
            // Graduated color for steps: cyan to blue
            float height_ratio = box.half_extents.y / 0.3f;
            color = glm::vec4(0.2f, 0.5f + height_ratio * 0.5f, 1.0f, 1.0f); // Cyan to bright blue
        }
        // Tall walls (height > width)
        else if (box.half_extents.y > box.half_extents.x &&
                 box.half_extents.y > box.half_extents.z) {
            color = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f); // Bright orange
        }
        // Long walls (one horizontal dimension much larger)
        else if (box.half_extents.x > 3.0f || box.half_extents.z > 3.0f) {
            color = glm::vec4(0.9f, 0.9f, 0.2f, 1.0f); // Bright yellow
        }
        // Platforms (flat, wide boxes)
        else if (box.half_extents.y < 0.5f &&
                 (box.half_extents.x > 1.5f || box.half_extents.z > 1.5f)) {
            color = glm::vec4(0.3f, 1.0f, 0.3f, 1.0f); // Bright green
        }
        // Default (miscellaneous)
        else {
            color = glm::vec4(1.0f, 0.3f, 0.9f, 1.0f); // Bright magenta
        }

        ctx.renderer.draw(box_mesh, ctx.cam, ctx.aspect, color);
    }

    // Draw ground contact point (if grounded)
    if (character.is_grounded) {
        wireframe_mesh contact = ctx.unit_sphere_4;
        contact.position =
            character.collision_sphere.center - glm::vec3(0, character.collision_sphere.radius, 0);
        contact.scale = glm::vec3(0.05f);
        ctx.renderer.draw(contact, ctx.cam, ctx.aspect, glm::vec4(0, 1, 0, 1));
    }
}

void draw_character_body(draw_context& ctx, const controller& character,
                         const orientation_system& orientation) {
    // Generate tall box as character body stand-in
    wireframe_mesh body = generate_box({0.4f, 0.8f, 0.3f}); // Width, height, depth

    // Build transform: translate → orient → landing offset → tilt → scale
    glm::mat4 transform = glm::mat4(1.0f);

    // Translate to character position
    transform = glm::translate(transform, character.position);

    // Apply orientation (yaw rotation around Y axis)
    float yaw = orientation.get_yaw();
    transform = glm::rotate(transform, yaw, math::UP);

    // Apply landing spring vertical offset (crouch effect)
    transform *= character.animation.get_vertical_offset_matrix();

    // Apply acceleration tilt
    transform *= character.animation.get_tilt_matrix();

    // Apply body vertices to transform
    for (auto& vertex : body.vertices) {
        glm::vec4 transformed = transform * glm::vec4(vertex, 1.0f);
        vertex = glm::vec3(transformed);
    }

    // Draw with distinctive color (magenta for character body)
    ctx.renderer.draw(body, ctx.cam, ctx.aspect, glm::vec4(1.0f, 0.2f, 1.0f, 1.0f));
}

void draw_skeleton(draw_context& ctx, const character::skeleton& skel, bool show_labels) {
    // Draw joints as small spheres
    for (const auto& joint : skel.joints) {
        // Extract position from model transform (translation component)
        glm::vec3 joint_pos = glm::vec3(joint.model_transform[3]);

        wireframe_mesh joint_sphere = ctx.unit_sphere_4;
        joint_sphere.position = joint_pos;
        joint_sphere.scale = glm::vec3(0.05f); // Small joint spheres
        ctx.renderer.draw(joint_sphere, ctx.cam, ctx.aspect,
                          glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)); // Yellow joints
    }

    // Draw bones as lines between parent and child joints
    wireframe_mesh bones;
    bones.vertices.reserve(skel.joints.size());
    bones.edges.reserve(skel.joints.size() - 1); // At least one edge per non-root joint

    // Add all joint positions as vertices
    for (const auto& joint : skel.joints) {
        glm::vec3 joint_pos = glm::vec3(joint.model_transform[3]);
        bones.vertices.push_back(joint_pos);
    }

    // Add edges between parent and child joints
    for (size_t i = 0; i < skel.joints.size(); ++i) {
        int parent_idx = skel.joints[i].parent_index;
        if (parent_idx != character::NO_PARENT) {
            bones.edges.emplace_back(parent_idx, i);
        }
    }

    // Draw bones with white color
    ctx.renderer.draw(bones, ctx.cam, ctx.aspect, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    // Draw joint labels if requested
    if (show_labels) {
        glm::mat4 view_proj = ctx.cam.get_projection_matrix(ctx.aspect) * ctx.cam.get_view_matrix();
        ImDrawList* draw_list = ImGui::GetForegroundDrawList();
        for (const auto& joint : skel.joints) {
            glm::vec3 world_pos = glm::vec3(joint.model_transform[3]);
            glm::vec4 clip = view_proj * glm::vec4(world_pos, 1.0f);
            if (clip.w > 0.0f) {
                glm::vec3 ndc = glm::vec3(clip) / clip.w;
                if (ndc.z >= 0.0f && ndc.z <= 1.0f) { // In front of camera
                    ImVec2 screen_pos((ndc.x + 1.0f) * 0.5f * ImGui::GetIO().DisplaySize.x,
                                      (1.0f - ndc.y) * 0.5f * ImGui::GetIO().DisplaySize.y);
                    draw_list->AddText(screen_pos, IM_COL32(255, 255, 0, 255), joint.name);
                }
            }
        }
    }
}

void draw_velocity_trail(draw_context& ctx, const velocity_trail_state& trail) {
    if (trail.positions.empty()) {
        return;
    }

    // Render trail positions from oldest to newest with size/alpha fade
    for (size_t i = 0; i < trail.positions.size(); ++i) {
        // Age factor: 0.0 = oldest, 1.0 = newest
        float age_factor =
            trail.positions.size() > 1
                ? static_cast<float>(i) / static_cast<float>(trail.positions.size() - 1)
                : 1.0f;

        // Scale: lerp from 0.05 (oldest) to 0.15 (newest)
        float scale = 0.05f + (0.15f - 0.05f) * age_factor;

        // Alpha: lerp from 0.2 (oldest) to 0.8 (newest)
        float alpha = 0.2f + (0.8f - 0.2f) * age_factor;

        wireframe_mesh sphere = ctx.unit_sphere_4;
        sphere.position = trail.positions[i];
        sphere.scale = glm::vec3(scale);
        ctx.renderer.draw(sphere, ctx.cam, ctx.aspect, glm::vec4(1.0f, 1.0f, 1.0f, alpha));
    }
}

} // namespace debug
