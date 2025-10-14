#include "gui/character_panel.h"
#include "gui/gui.h"
#include "sokol_app.h"
#include "imgui.h"
#include <glm/common.hpp>

namespace {

/// Helper to draw joint angle controls with three sliders (X/Y/Z Euler angles)
void draw_joint_angles_widget(const char* label, glm::vec3& angles, const char* x_label,
                              const char* y_label, const char* z_label) {
    if (ImGui::TreeNode(label)) {
        gui::widget::slider_float(x_label, &angles.x, -180.0f, 180.0f);
        gui::widget::slider_float(y_label, &angles.y, -180.0f, 180.0f);
        gui::widget::slider_float(z_label, &angles.z, -180.0f, 180.0f);
        ImGui::TreePop();
    }
}

float compute_base_walk_factor(const locomotion_system& locomotion) {
    float threshold_span = locomotion.run_speed_threshold - locomotion.walk_speed_threshold;
    float run_blend = 0.0f;
    if (threshold_span > 0.0f) {
        float normalized =
            (locomotion.smoothed_speed - locomotion.walk_speed_threshold) / threshold_span;
        run_blend = glm::clamp(normalized, 0.0f, 1.0f);
    } else {
        run_blend = locomotion.smoothed_speed > locomotion.walk_speed_threshold ? 1.0f : 0.0f;
    }
    return 1.0f - run_blend;
}

} // namespace

namespace gui {

void draw_character_panel(character_panel_state& state, controller& character,
                          locomotion_system& locomotion, orientation_system& orientation,
                          character::tuning_params& params) {
    if (!state.show)
        return;

    // Draw as collapsing sections (no window wrapper)
    // Main tuning parameters
    if (ImGui::CollapsingHeader("Character Tuning", ImGuiTreeNodeFlags_DefaultOpen)) {
        bool changed = false;
        changed |= gui::widget::slider_float("Run Speed (m/s)", &character.run_speed, 1.0f, 15.0f);
        changed |= gui::widget::slider_float("Time to Max Speed (s)", &params.time_to_max_speed,
                                             0.1f, 2.0f);
        changed |= gui::widget::slider_float("Jump Height (m)", &params.jump_height, 0.5f, 3.0f);
        changed |= gui::widget::slider_float("Gravity (m/s^2)", &character.gravity, -20.0f, -5.0f);
        gui::widget::slider_float("Walk Transition Rate", &character.walk_transition_rate, 1.0f,
                                  30.0f);

        // Debug displays for walk/run transition
        gui::widget::text("Target Max Speed: %.2f m/s", character.target_max_speed);
        gui::widget::text("Current Max Speed: %.2f m/s", character.max_speed);

        if (changed) {
            params.apply_to(character);
            character::sync_locomotion_targets(character, locomotion);
        }
    }

    // Landing spring parameters
    if (ImGui::CollapsingHeader("Landing Spring")) {
        gui::widget::slider_float("Stiffness", &character.animation.landing_spring.stiffness,
                                  100.0f, 1000.0f);
        gui::widget::slider_float("Damping", &character.animation.landing_spring.damping, 10.0f,
                                  100.0f);
        gui::widget::slider_float("Impulse Scale", &character.animation.landing_impulse_scale, 0.1f,
                                  1.5f);

        // Read-only spring state display
        gui::widget::text("Spring Position: %.3f", character.animation.get_vertical_offset());
        gui::widget::text("Spring Velocity: %.3f",
                          character.animation.landing_spring.get_velocity());
    }

    // Acceleration tilt parameters
    if (ImGui::CollapsingHeader("Acceleration Tilt")) {
        gui::widget::slider_float("Tilt Smoothing", &character.animation.tilt_smoothing, 1.0f,
                                  20.0f);
        gui::widget::slider_float("Tilt Magnitude (rad)", &character.animation.tilt_magnitude, 0.0f,
                                  1.0f);

        // Read-only tilt state display
        gui::widget::text("Current Tilt (pitch, roll): (%.2f, %.2f)",
                          character.animation.tilt_angles.x, character.animation.tilt_angles.z);
    }

    // Air locomotion weights (phase continuity)
    if (ImGui::CollapsingHeader("Air Locomotion Weights")) {
        gui::widget::slider_float("Contact Weight Frequency (Hz)",
                                  &character.animation.contact_weight_frequency, 3.0f, 12.0f);

        // Read-only weight state display
        gui::widget::text("Contact Weight Target: %.3f", character.animation.contact_weight_target);
        gui::widget::text("Contact Weight (smoothed): %.3f",
                          character.animation.contact_weight_spring.get_position());
        gui::widget::text("Air Weight: %.3f",
                          1.0f - character.animation.contact_weight_spring.get_position());
        gui::widget::text("Spring Velocity: %.3f",
                          character.animation.contact_weight_spring.get_velocity());
    }

    // Secondary motion parameters
    if (ImGui::CollapsingHeader("Secondary Motion")) {
        gui::widget::checkbox("Enable Secondary Motion", &state.enable_secondary_motion);

        gui::widget::slider_float("Stiffness (catch-up speed)",
                                  &character.animation.secondary_motion.stiffness, 10.0f, 20.0f);
        gui::widget::slider_float("Damping Ratio (bounciness)",
                                  &character.animation.secondary_motion.damping_ratio, 0.5f, 2.0f);
        gui::widget::slider_float("Response Scale (wobble amount)",
                                  &character.animation.secondary_motion.response_scale, 0.0f, 0.1f);

        // Read-only spring state display
        gui::widget::text("Left Elbow: %.3f rad (vel: %.3f)",
                          character.animation.secondary_motion.left_elbow_offset,
                          character.animation.secondary_motion.left_elbow_velocity);
        gui::widget::text("Right Elbow: %.3f rad (vel: %.3f)",
                          character.animation.secondary_motion.right_elbow_offset,
                          character.animation.secondary_motion.right_elbow_velocity);
        gui::widget::text("Left Knee: %.3f rad (vel: %.3f)",
                          character.animation.secondary_motion.left_knee_offset,
                          character.animation.secondary_motion.left_knee_velocity);
        gui::widget::text("Right Knee: %.3f rad (vel: %.3f)",
                          character.animation.secondary_motion.right_knee_offset,
                          character.animation.secondary_motion.right_knee_velocity);
    }

    // Orientation parameters
    if (ImGui::CollapsingHeader("Orientation")) {
        gui::widget::slider_float("Yaw Smoothing", &orientation.yaw_smoothing, 1.0f, 20.0f);

        // Read-only orientation state display
        gui::widget::text("Current Yaw (rad): %.2f", orientation.current_yaw);
        gui::widget::text("Target Yaw (rad): %.2f", orientation.target_yaw);
    }

    // Locomotion parameters
    if (ImGui::CollapsingHeader("Locomotion")) {
        gui::widget::slider_float("Speed Smoothing", &locomotion.speed_smoothing, 1.0f, 20.0f);

        // Read-only locomotion state display
        gui::widget::text("Current Speed: %.2f m/s", locomotion.current_speed);
        gui::widget::text("Smoothed Speed: %.2f m/s", locomotion.smoothed_speed);
        gui::widget::text("Animation Phase: %.2f", locomotion.phase);
        gui::widget::text("Stride: %.2fm (auto from locomotion)", locomotion.get_blended_stride());

        ImGui::Separator();

        // Current automatic pose display
        const char* pose_name;
        switch (character.animation.current_automatic_pose) {
        case character::pose_type::T_POSE:
            pose_name = "T-Pose";
            break;
        case character::pose_type::WALK_REACH_LEFT:
            pose_name = "Walk - Reach Left";
            break;
        case character::pose_type::WALK_PASS_RIGHT:
            pose_name = "Walk - Pass Right";
            break;
        case character::pose_type::WALK_REACH_RIGHT:
            pose_name = "Walk - Reach Right";
            break;
        case character::pose_type::WALK_PASS_LEFT:
            pose_name = "Walk - Pass Left";
            break;
        case character::pose_type::RUN_REACH_LEFT:
            pose_name = "Run - Reach Left";
            break;
        case character::pose_type::RUN_PASS_RIGHT:
            pose_name = "Run - Pass Right";
            break;
        case character::pose_type::RUN_REACH_RIGHT:
            pose_name = "Run - Reach Right";
            break;
        case character::pose_type::RUN_PASS_LEFT:
            pose_name = "Run - Pass Left";
            break;
        default:
            pose_name = "Unknown";
            break;
        }
        gui::widget::text("Current Pose: %s", pose_name);
    }

    if (ImGui::CollapsingHeader("Locomotion Blending", ImGuiTreeNodeFlags_DefaultOpen)) {
        const char* mode_labels[] = {"Walk Only", "Mixed (Speed)", "Run Only"};
        int current_mode = static_cast<int>(state.gait_blend_mode);
        if (ImGui::Combo("Blend Mode", &current_mode, mode_labels, 3)) {
            state.gait_blend_mode = static_cast<character_panel_state::blend_mode>(current_mode);
        }
        float base_walk_factor = compute_base_walk_factor(locomotion);
        float applied_walk_factor = compute_walk_factor_override(state, base_walk_factor);
        gui::widget::text("Walk Factor (locomotion): %.3f", base_walk_factor);
        gui::widget::text("Walk Factor (applied): %.3f", applied_walk_factor);
        gui::widget::text("Contact Weight: %.3f",
                          character.animation.contact_weight_spring.get_position());
    }

    if (ImGui::CollapsingHeader("Skeleton")) {
        gui::widget::checkbox("Show Skeleton", &state.show_skeleton);
        gui::widget::checkbox("Show Joint Labels", &state.show_joint_labels);
        gui::widget::checkbox("Show Velocity Trail", &state.show_velocity_trail);
        gui::widget::checkbox("Show Axis Gizmo", &state.show_axis_gizmo);

        bool manual_pose_toggled =
            gui::widget::checkbox("Manual Pose Selection", &state.use_manual_pose_selection);

        if (state.use_manual_pose_selection) {
            ImGui::Separator();
            ImGui::Text("Pose Selection:");
            const char* pose_names[] = {
                "T-Pose",           "Walk - Reach Left", "Walk - Pass Right", "Walk - Reach Right",
                "Walk - Pass Left", "Run - Reach Left",  "Run - Pass Right",  "Run - Reach Right",
                "Run - Pass Left"};
            int current_pose = static_cast<int>(state.selected_pose);
            if (ImGui::Combo("Pose", &current_pose, pose_names, 9)) {
                state.selected_pose = static_cast<character::pose_type>(current_pose);
                state.reset_joint_overrides(); // Reset on pose change
                state.gait_blend_mode = character_panel_state::blend_mode::MIXED;
            }

            ImGui::Separator();
            bool joint_overrides_toggled =
                gui::widget::checkbox("Enable Joint Overrides", &state.enable_joint_overrides);

            if (joint_overrides_toggled) {
                state.reset_joint_overrides();
            }
        }

        if (manual_pose_toggled) {
            state.reset_joint_overrides();
            state.gait_blend_mode = character_panel_state::blend_mode::MIXED;
        }

        if (state.enable_joint_overrides && state.use_manual_pose_selection &&
            ImGui::CollapsingHeader("Joint Overrides")) {
            ImGui::PushItemWidth(180.0f);
            ImGui::TextWrapped("Joint Override Offsets (applied on top of selected pose)");
            ImGui::TextWrapped("Euler angles: Pitch=X-axis, Yaw=Y-axis, Roll=Z-axis");
            ImGui::Separator();

            // Left shoulder: bone points along -X axis (parent space = character frame)
            // X-axis = twist (invisible), Y-axis = fwd/back swing, Z-axis = up/down raise
            draw_joint_angles_widget("Left Shoulder", state.left_shoulder_angles, "X-axis (twist)",
                                     "Y-axis (fwd/back)", "Z-axis (up/down)");

            draw_joint_angles_widget("Left Elbow", state.left_elbow_angles, "X-axis (twist)",
                                     "Y-axis (bend)", "Z-axis (rotate)");

            // Right shoulder: bone points along +X axis (parent space = character frame)
            // X-axis = twist (invisible), Y-axis = fwd/back swing, Z-axis = up/down raise
            draw_joint_angles_widget("Right Shoulder", state.right_shoulder_angles,
                                     "X-axis (twist)", "Y-axis (fwd/back)", "Z-axis (up/down)");

            draw_joint_angles_widget("Right Elbow", state.right_elbow_angles, "X-axis (twist)",
                                     "Y-axis (bend)", "Z-axis (rotate)");

            // Legs point along -Y axis (downward, parent space = character frame)
            // X-axis = fwd/back swing, Y-axis = twist (invisible), Z-axis = in/out spread
            draw_joint_angles_widget("Left Hip", state.left_hip_angles, "X-axis (fwd/back)",
                                     "Y-axis (twist)", "Z-axis (in/out)");

            draw_joint_angles_widget("Left Knee", state.left_knee_angles, "X-axis (bend)",
                                     "Y-axis (twist)", "Z-axis (rotate)");

            draw_joint_angles_widget("Right Hip", state.right_hip_angles, "X-axis (fwd/back)",
                                     "Y-axis (twist)", "Z-axis (in/out)");

            draw_joint_angles_widget("Right Knee", state.right_knee_angles, "X-axis (bend)",
                                     "Y-axis (twist)", "Z-axis (rotate)");

            ImGui::PopItemWidth();
        }
    }
}

float compute_walk_factor_override(const character_panel_state& state, float base_walk_factor) {
    switch (state.gait_blend_mode) {
    case character_panel_state::blend_mode::WALK_ONLY:
        return 1.0f;
    case character_panel_state::blend_mode::RUN_ONLY:
        return 0.0f;
    case character_panel_state::blend_mode::MIXED:
    default:
        return base_walk_factor;
    }
}

} // namespace gui
