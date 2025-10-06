/// @file skeleton.cpp
/// Implementation of skeleton transform updates.

#include "skeleton.h"

#include <cassert>

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

} // namespace character