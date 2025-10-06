/// @file t_pose.cpp
/// Implementation of hardcoded T-pose skeleton asset.

#include "t_pose.h"
#include <glm/gtc/matrix_transform.hpp>

namespace character {

void create_t_pose(skeleton& skel) {
    skel.joints.clear();

    // Root (hips) at origin
    skel.joints.push_back({glm::mat4(1.0f), glm::mat4(1.0f), NO_PARENT, "root"});

    // Spine
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.1f, 0.0f)), 0,
                           "spine_lower"});
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.1f, 0.0f)), 1,
                           "spine_upper"});

    // Head
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.05f, 0.0f)), 2,
                           "neck"});
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.05f, 0.0f)), 3,
                           "head"});

    // Left arm (T-pose: horizontal)
    // indices: 0=root,1=spine_lower,2=spine_upper,3=neck,4=head
    // left_shoulder -> parent spine_upper (2)
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(-0.1f, 0.05f, 0.0f)), 2,
                           "left_shoulder"}); // index 5
    // left_elbow -> parent left_shoulder (5)
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(-0.15f, 0.0f, 0.0f)), 5,
                           "left_elbow"}); // index 6
    // left_wrist -> parent left_elbow (6)
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(-0.15f, 0.0f, 0.0f)), 6,
                           "left_wrist"}); // index 7

    // Right arm (T-pose: horizontal)
    // right_shoulder -> parent spine_upper (2)
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, 0.05f, 0.0f)), 2,
                           "right_shoulder"}); // index 8
    // right_elbow -> parent right_shoulder (8)
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.15f, 0.0f, 0.0f)), 8,
                           "right_elbow"}); // index 9
    // right_wrist -> parent right_elbow (9)
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.15f, 0.0f, 0.0f)), 9,
                           "right_wrist"}); // index 10

    // Left leg
    // left_hip -> parent root (0)
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(-0.05f, -0.05f, 0.0f)), 0,
                           "left_hip"}); // index 11
    // left_knee -> parent left_hip (11)
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.2f, 0.0f)), 11,
                           "left_knee"}); // index 12
    // left_ankle -> parent left_knee (12)
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.2f, 0.0f)), 12,
                           "left_ankle"}); // index 13

    // Right leg
    // right_hip -> parent root (0)
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.05f, -0.05f, 0.0f)), 0,
                           "right_hip"}); // index 14
    // right_knee -> parent right_hip (14)
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.2f, 0.0f)), 14,
                           "right_knee"}); // index 15
    // right_ankle -> parent right_knee (15)
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.2f, 0.0f)), 15,
                           "right_ankle"}); // index 16
}

} // namespace character