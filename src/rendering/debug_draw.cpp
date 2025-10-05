#include "rendering/debug_draw.h"
#include <glm/gtc/constants.hpp>
#include <cmath>

namespace {
constexpr float WHEEL_RADIUS = 0.45f;
constexpr float TWO_PI = 6.28318530718f;
} // namespace

namespace debug {

void draw_character_state(draw_context& ctx, const controller& character,
                          const locomotion_system& locomotion,
                          const orientation_system& orientation) {
    simple_pose current_pose = locomotion.get_current_pose();
    glm::vec3 pose_offset = current_pose.root_offset;

    // Bumper sphere (no pose offset - follows physics position)
    wireframe_mesh bumper_vis = ctx.unit_sphere_8;
    bumper_vis.position = character.bumper.center;
    bumper_vis.scale = glm::vec3(character.bumper.radius);
    ctx.renderer.draw(bumper_vis, ctx.cam, ctx.aspect, glm::vec4(0, 1, 1, 1));

    // Weightlifter sphere
    wireframe_mesh weightlifter_vis = ctx.unit_sphere_6;
    weightlifter_vis.position = character.weightlifter.center;
    weightlifter_vis.scale = glm::vec3(character.weightlifter.radius);
    ctx.renderer.draw(weightlifter_vis, ctx.cam, ctx.aspect, glm::vec4(1, 1, 0, 1));

    // Velocity indicator (no pose offset - attached to physics position)
    glm::vec3 vel = character.velocity;
    if (glm::length(vel) > 0.1f) {
        wireframe_mesh velocity_indicator = ctx.unit_sphere_4;
        velocity_indicator.position = character.position + vel;
        velocity_indicator.scale = glm::vec3(0.1f);
        ctx.renderer.draw(velocity_indicator, ctx.cam, ctx.aspect, glm::vec4(1, 0, 0, 1));
    }

    // Speed threshold circles
    wireframe_mesh walk_circle = ctx.unit_circle;
    walk_circle.position = character.position;
    walk_circle.scale =
        glm::vec3(locomotion.walk_speed_threshold, 1.0f, locomotion.walk_speed_threshold);
    ctx.renderer.draw(walk_circle, ctx.cam, ctx.aspect, glm::vec4(0, 1, 0, 0.5f));

    wireframe_mesh run_circle = ctx.unit_circle;
    run_circle.position = character.position;
    run_circle.scale =
        glm::vec3(locomotion.run_speed_threshold, 1.0f, locomotion.run_speed_threshold);
    ctx.renderer.draw(run_circle, ctx.cam, ctx.aspect, glm::vec4(1, 1, 0, 0.5f));

    // Current speed circle
    glm::vec3 horiz_vel = character.velocity;
    horiz_vel.y = 0.0f;
    float current_speed = glm::length(horiz_vel);
    if (current_speed > 0.05f) {
        wireframe_mesh speed_circle = ctx.unit_circle;
        speed_circle.position = character.position;
        speed_circle.scale = glm::vec3(current_speed, 1.0f, current_speed);
        ctx.renderer.draw(speed_circle, ctx.cam, ctx.aspect, glm::vec4(1, 0, 0, 0.8f));
    }

    // Orientation indicator (attached to physics position)
    float yaw = orientation.get_yaw();
    glm::vec3 forward_dir(std::sin(yaw), 0, std::cos(yaw));
    wireframe_mesh yaw_indicator = ctx.unit_sphere_4;
    yaw_indicator.position = character.position + forward_dir * 0.8f;
    yaw_indicator.scale = glm::vec3(0.1f);
    ctx.renderer.draw(yaw_indicator, ctx.cam, ctx.aspect, glm::vec4(0, 1, 0, 1));
}

void draw_physics_springs(draw_context& ctx, const controller& character,
                          const locomotion_system& locomotion) {
    (void) ctx;
    (void) character;
    (void) locomotion;
    // Phase 2: No spring visualization - vertical motion comes from keyframe interpolation
    // Spring visualization will be added in Phase 3
}

void draw_locomotion_wheel(draw_context& ctx, const controller& character,
                           const locomotion_system& locomotion,
                           const orientation_system& orientation, float wheel_spin_angle) {
    (void) locomotion; // Phase information used for foot placement, not wheel center

    float yaw = orientation.get_yaw();
    glm::vec3 forward_dir(std::sin(yaw), 0, std::cos(yaw));
    glm::vec3 up_axis(0.0f, 1.0f, 0.0f);

    float wheel_ground_y = character.weightlifter.center.y - character.weightlifter.radius;
    glm::vec3 wheel_center = character.position;
    wheel_center.y = wheel_ground_y + WHEEL_RADIUS;

    const int RIM_SEGMENTS = 24;
    const int SPOKE_COUNT = 6;

    wireframe_mesh wheel_mesh;
    wheel_mesh.vertices.reserve(RIM_SEGMENTS + 1);
    wheel_mesh.edges.reserve(RIM_SEGMENTS + SPOKE_COUNT);

    wheel_mesh.vertices.push_back(wheel_center);
    for (int i = 0; i < RIM_SEGMENTS; ++i) {
        float base_angle = static_cast<float>(i) / static_cast<float>(RIM_SEGMENTS) * TWO_PI;
        float rim_angle = base_angle - wheel_spin_angle;
        glm::vec3 offset = std::cos(rim_angle) * forward_dir + std::sin(rim_angle) * up_axis;
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
    glm::vec3 forward_dir(std::sin(yaw), 0, std::cos(yaw));
    glm::vec3 right_dir(-std::cos(yaw), 0, std::sin(yaw));

    float wheel_ground_y = character.weightlifter.center.y - character.weightlifter.radius;
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

} // namespace debug
