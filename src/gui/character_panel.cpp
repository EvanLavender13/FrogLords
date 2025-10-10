#include "gui/character_panel.h"
#include "gui/gui.h"
#include "sokol_app.h"
#include "imgui.h"

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
        changed |= gui::widget::slider_float("Max Speed (m/s)", &character.max_speed, 1.0f, 15.0f);
        changed |= gui::widget::slider_float("Time to Max Speed (s)", &params.time_to_max_speed,
                                             0.1f, 2.0f);
        changed |= gui::widget::slider_float("Jump Height (m)", &params.jump_height, 0.5f, 3.0f);
        changed |= gui::widget::slider_float("Gravity (m/s^2)", &character.gravity, -20.0f, -5.0f);

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

    // Secondary motion parameters
    if (ImGui::CollapsingHeader("Secondary Motion")) {
        gui::widget::checkbox("Enable Secondary Motion", &state.enable_secondary_motion);

        gui::widget::slider_float("Stiffness (catch-up speed)",
                                  &character.animation.secondary_motion.stiffness, 10.0f, 20.0f);
        gui::widget::slider_float("Damping Ratio (bounciness)",
                                  &character.animation.secondary_motion.damping_ratio, 0.5f, 2.0f);
        gui::widget::slider_float("Response Scale (wobble amount)",
                                  &character.animation.secondary_motion.response_scale, 0.0f,
                                  0.04f);

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

        ImGui::Separator();
        gui::widget::slider_float("Cycle Length (m)", &character.animation.cycle_length, 0.5f,
                                  3.0f);

        // Current automatic pose display
        const char* pose_name;
        switch (character.animation.current_automatic_pose) {
        case character::pose_type::T_POSE:
            pose_name = "T-Pose";
            break;
        case character::pose_type::STEP_LEFT:
            pose_name = "Step Left";
            break;
        case character::pose_type::NEUTRAL:
            pose_name = "Neutral";
            break;
        case character::pose_type::STEP_RIGHT:
            pose_name = "Step Right";
            break;
        default:
            pose_name = "Unknown";
            break;
        }
        gui::widget::text("Current Pose: %s", pose_name);
    }

    if (ImGui::CollapsingHeader("Skeleton")) {
        gui::widget::checkbox("Show Skeleton", &state.show_skeleton);
        gui::widget::checkbox("Show Joint Labels", &state.show_joint_labels);

        ImGui::Separator();
        gui::widget::checkbox("Manual Pose Selection", &state.use_manual_pose_selection);

        if (state.use_manual_pose_selection) {
            ImGui::Separator();
            ImGui::Text("Pose Selection:");
            const char* pose_names[] = {"T-Pose", "Step Left", "Neutral", "Step Right"};
            int current_pose = static_cast<int>(state.selected_pose);
            if (ImGui::Combo("Pose", &current_pose, pose_names, 4)) {
                state.selected_pose = static_cast<character::pose_type>(current_pose);
            }

            ImGui::Separator();
            gui::widget::checkbox("Enable Joint Overrides", &state.enable_joint_overrides);
        }

        if (state.enable_joint_overrides && state.use_manual_pose_selection &&
            ImGui::CollapsingHeader("Joint Overrides")) {
            ImGui::PushItemWidth(180.0f);
            ImGui::TextWrapped("Joint Override Offsets (applied on top of selected pose)");
            ImGui::TextWrapped("Euler angles: Pitch=X-axis, Yaw=Y-axis, Roll=Z-axis");
            ImGui::Separator();

            // Left shoulder: bone points along -X axis (parent space = character frame)
            // X-axis = twist (invisible), Y-axis = fwd/back swing, Z-axis = up/down raise
            if (ImGui::TreeNode("Left Shoulder")) {
                gui::widget::slider_float("X-axis (twist)", &state.left_shoulder_angles.x, -180.0f,
                                          180.0f);
                gui::widget::slider_float("Y-axis (fwd/back)", &state.left_shoulder_angles.y,
                                          -180.0f, 180.0f);
                gui::widget::slider_float("Z-axis (up/down)", &state.left_shoulder_angles.z,
                                          -180.0f, 180.0f);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Left Elbow")) {
                gui::widget::slider_float("X-axis (twist)", &state.left_elbow_angles.x, -180.0f,
                                          180.0f);
                gui::widget::slider_float("Y-axis (bend)", &state.left_elbow_angles.y, -180.0f,
                                          180.0f);
                gui::widget::slider_float("Z-axis (rotate)", &state.left_elbow_angles.z, -180.0f,
                                          180.0f);
                ImGui::TreePop();
            }

            // Right shoulder: bone points along +X axis (parent space = character frame)
            // X-axis = twist (invisible), Y-axis = fwd/back swing, Z-axis = up/down raise
            if (ImGui::TreeNode("Right Shoulder")) {
                gui::widget::slider_float("X-axis (twist)", &state.right_shoulder_angles.x, -180.0f,
                                          180.0f);
                gui::widget::slider_float("Y-axis (fwd/back)", &state.right_shoulder_angles.y,
                                          -180.0f, 180.0f);
                gui::widget::slider_float("Z-axis (up/down)", &state.right_shoulder_angles.z,
                                          -180.0f, 180.0f);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Right Elbow")) {
                gui::widget::slider_float("X-axis (twist)", &state.right_elbow_angles.x, -180.0f,
                                          180.0f);
                gui::widget::slider_float("Y-axis (bend)", &state.right_elbow_angles.y, -180.0f,
                                          180.0f);
                gui::widget::slider_float("Z-axis (rotate)", &state.right_elbow_angles.z, -180.0f,
                                          180.0f);
                ImGui::TreePop();
            }

            // Legs point along -Y axis (downward, parent space = character frame)
            // X-axis = fwd/back swing, Y-axis = twist (invisible), Z-axis = in/out spread
            if (ImGui::TreeNode("Left Hip")) {
                gui::widget::slider_float("X-axis (fwd/back)", &state.left_hip_angles.x, -180.0f,
                                          180.0f);
                gui::widget::slider_float("Y-axis (twist)", &state.left_hip_angles.y, -180.0f,
                                          180.0f);
                gui::widget::slider_float("Z-axis (in/out)", &state.left_hip_angles.z, -180.0f,
                                          180.0f);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Left Knee")) {
                gui::widget::slider_float("X-axis (bend)", &state.left_knee_angles.x, -180.0f,
                                          180.0f);
                gui::widget::slider_float("Y-axis (twist)", &state.left_knee_angles.y, -180.0f,
                                          180.0f);
                gui::widget::slider_float("Z-axis (rotate)", &state.left_knee_angles.z, -180.0f,
                                          180.0f);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Right Hip")) {
                gui::widget::slider_float("X-axis (fwd/back)", &state.right_hip_angles.x, -180.0f,
                                          180.0f);
                gui::widget::slider_float("Y-axis (twist)", &state.right_hip_angles.y, -180.0f,
                                          180.0f);
                gui::widget::slider_float("Z-axis (in/out)", &state.right_hip_angles.z, -180.0f,
                                          180.0f);
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Right Knee")) {
                gui::widget::slider_float("X-axis (bend)", &state.right_knee_angles.x, -180.0f,
                                          180.0f);
                gui::widget::slider_float("Y-axis (twist)", &state.right_knee_angles.y, -180.0f,
                                          180.0f);
                gui::widget::slider_float("Z-axis (rotate)", &state.right_knee_angles.z, -180.0f,
                                          180.0f);
                ImGui::TreePop();
            }

            ImGui::PopItemWidth();
        }
    }
}

} // namespace gui
