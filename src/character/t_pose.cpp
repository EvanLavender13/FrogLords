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
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(-0.1f, 0.05f, 0.0f)), 2,
                           "left_shoulder"});
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(-0.15f, 0.0f, 0.0f)), 4,
                           "left_elbow"});
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(-0.15f, 0.0f, 0.0f)), 5,
                           "left_wrist"});

    // Right arm (T-pose: horizontal)
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.1f, 0.05f, 0.0f)), 2,
                           "right_shoulder"});
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.15f, 0.0f, 0.0f)), 7,
                           "right_elbow"});
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.15f, 0.0f, 0.0f)), 8,
                           "right_wrist"});

    // Left leg
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(-0.05f, -0.05f, 0.0f)), 0,
                           "left_hip"});
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.2f, 0.0f)), 9,
                           "left_knee"});
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.2f, 0.0f)), 10,
                           "left_ankle"});

    // Right leg
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.05f, -0.05f, 0.0f)), 0,
                           "right_hip"});
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.2f, 0.0f)), 12,
                           "right_knee"});
    skel.joints.push_back({glm::mat4(1.0f),
                           glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.2f, 0.0f)), 13,
                           "right_ankle"});
}

} // namespace character