#pragma once

#include "character/controller.h"
#include "character/keyframe.h"
#include "character/locomotion.h"
#include "character/orientation.h"
#include "character/tuning.h"
#include <glm/glm.hpp>

namespace gui {

struct character_panel_state {
    bool show = true;
    bool show_skeleton = true;
    bool show_joint_labels = false;
    character::pose_type selected_pose = character::pose_type::T_POSE;

    // Animation control
    bool use_manual_pose_selection = false;
    bool enable_secondary_motion = true;

    // Real-time joint override (Euler angles in degrees)
    // vec3 format: (X-rotation, Y-rotation, Z-rotation)
    // Applied in order: Y -> X -> Z (yaw -> pitch -> roll)
    bool enable_joint_overrides = false;
    glm::vec3 left_shoulder_angles = glm::vec3(0.0f);
    glm::vec3 left_elbow_angles = glm::vec3(0.0f);
    glm::vec3 right_shoulder_angles = glm::vec3(0.0f);
    glm::vec3 right_elbow_angles = glm::vec3(0.0f);
    glm::vec3 left_hip_angles = glm::vec3(0.0f);
    glm::vec3 left_knee_angles = glm::vec3(0.0f);
    glm::vec3 right_hip_angles = glm::vec3(0.0f);
    glm::vec3 right_knee_angles = glm::vec3(0.0f);

    void reset_joint_overrides() {
        left_shoulder_angles = glm::vec3(0.0f);
        left_elbow_angles = glm::vec3(0.0f);
        right_shoulder_angles = glm::vec3(0.0f);
        right_elbow_angles = glm::vec3(0.0f);
        left_hip_angles = glm::vec3(0.0f);
        left_knee_angles = glm::vec3(0.0f);
        right_hip_angles = glm::vec3(0.0f);
        right_knee_angles = glm::vec3(0.0f);
    }
};

void draw_character_panel(character_panel_state& state, controller& character,
                          locomotion_system& locomotion, orientation_system& orientation,
                          character::tuning_params& params);

} // namespace gui
