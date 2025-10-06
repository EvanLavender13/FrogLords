/// @file t_pose.h
/// Hardcoded T-pose skeleton asset for testing.

#pragma once

#include "skeleton.h"

namespace character {

/// Creates a hardcoded humanoid T-pose skeleton.
/// Populates joints with root first; sets parent_index for children.
/// Initializes local_transforms; model_transforms computed by update_global_transforms.
void create_t_pose(skeleton& skel);

} // namespace character