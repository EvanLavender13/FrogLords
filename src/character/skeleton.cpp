/// @file skeleton.cpp
/// Implementation of skeleton transform updates.

#include "skeleton.h"

#include <cassert>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace character {

void update_global_transforms(skeleton& skel) {
    assert(skel.joints.size() > 0);
    assert(skel.joints[0].parent_index == NO_PARENT);

    for (size_t i = 0; i < skel.joints.size(); ++i) {
        const joint& j = skel.joints[i];
        if (j.parent_index == NO_PARENT) {
            skel.joints[i].model_transform = j.local_transform;
        } else {
            assert(j.parent_index >= 0 &&
                   static_cast<size_t>(j.parent_index) < i); // Parent before child in array
            skel.joints[i].model_transform =
                skel.joints[j.parent_index].model_transform * j.local_transform;
        }
    }

    // Optional: Simple cycle detection (debug builds only)
#ifndef NDEBUG
    // Basic check: no self-parent, but already covered by parent_index != i implicitly since parent
    // < i
#endif
}

void set_joint_rotation(skeleton& skel, int joint_idx, const glm::quat& rotation) {
    // Extract T-pose position (T-pose rotations are identity)
    glm::vec3 t_pose_pos = glm::vec3(skel.joints[joint_idx].local_transform[3]);
    // Build new transform: translation * rotation (position bone, then apply joint rotation)
    skel.joints[joint_idx].local_transform =
        glm::translate(glm::mat4(1.0f), t_pose_pos) * glm::mat4_cast(rotation);
}

} // namespace character