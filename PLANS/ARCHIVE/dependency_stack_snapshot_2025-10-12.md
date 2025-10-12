# Dependency Stack Snapshot — 2025-10-12

Purpose: Archive of detailed “Completed Features” writeups prior to backlog grooming collapse on 2025-10-12. Use this snapshot for historical context and retrospectives. The live planning docs now keep only concise summaries that guide future work.

---

## Completed Features (pre-collapse details)

These features are built on the stable foundation below. They represent completed experiments that successfully solved specific problems.

**Reactive Animation Systems:**
- **Acceleration Tilt:** Character leans into acceleration (character-local space, tilt matrix)
- **Landing Spring:** Spring-damper crouch/recover on landing
- **Animation Tuning UI:** Real-time parameter adjustment panel
- **Walk/Run Transition:** Smoothed max_speed blending based on SHIFT input
  - Fixed circular dependency: separated `run_speed` (reference) from `max_speed` (state)
  - Pattern validated: `max_speed = lerp(max_speed, target, rate * dt)`
  - See [implementation_smoothed_walk_transition.md](../implementation_smoothed_walk_transition.md)

**Skeletal Animation Stack:**
- **Skeleton Debug System:** Hierarchical joints, T-pose asset, debug visualization
- **Attach Skeleton to Body:** Skeleton follows character transform
- **Static Keyframe Preview:** Quaternion keyframe architecture validated in isolation
  - Critical learning: validate data representation before building complex features on it
  - See [ARCHIVE/dependency_stack_snapshot_2025-10-10.md](dependency_stack_snapshot_2025-10-10.md)
- **Primary Skeletal Animation:** Distance-phased pose switching (surveyor wheel pattern)
- **Secondary Motion:** Per-bone spring-damper lag for natural follow-through
- **Pose Blending:** Quaternion slerp between keyframes
  - Eliminates pops at phase boundaries
  - Hemisphere correction for shortest-path interpolation
  - See [iteration_pose_blending.md](../iteration_pose_blending.md)

**Debug Tooling:**
- **Debug Visual Overhaul:** Velocity trail, plot functions, speed gradient ring
- **Camera Zoom:** Mouse wheel zoom in orbit + follow modes
- **Unified Panel Architecture:** Collapsing headers, real-time displays
- See [implementation_debug_visual_overhaul.md](../implementation_debug_visual_overhaul.md)

---

End of snapshot.

