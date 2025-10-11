# Dependency Stack Snapshot - October 10, 2025

**Purpose:** Historical archive of completed layer details before backlog grooming collapse.

**Context:** First backlog grooming session after completing 6 major feature iterations (Skeleton Debug → Debug Visual Overhaul).

---

## Reactive Systems Layer (Implemented, ~100% certain) ✅

**Status:** Architecture proven, tuning UI complete, ready for iteration

**Completed: Acceleration Tilt ✅**
- Character-local space transformation of world acceleration
- Exponential smoothing for natural response
- Pure reactive layer ("do no harm" principle verified)
- 45 lines of code, 2 tunable parameters
- UI controls: tilt_smoothing, tilt_magnitude with real-time pitch/roll display

**Completed: Landing Spring ✅**
- Spring-damper driven crouch/recover on landing
- Impulse proportional to fall velocity
- Critically damped for natural feel (no bounce)
- Visual debug spring (collision sphere to body center)
- UI controls: stiffness, damping, impulse_scale with spring state display

**Completed: Animation Tuning UI ✅**
- Comprehensive parameter exposure across animation/orientation/locomotion systems
- Real-time adjustment during gameplay (zero rebuild iteration)
- Consistent UX pattern: collapsing headers + live state feedback
- See `animation_tuning_plan.md` for implementation details and learnings

**Architecture Validated:**
- `character::animation_state` pattern works well (proven twice)
- Update timing: after physics, after orientation, before render
- Transform injection: position → orient → landing offset → tilt → render
- Parameters over assets paradigm successful
- Flag-based event communication (just_landed) pattern works
- Public member access for tuning (no getters/setters needed)
- Direct UI binding to system parameters (no intermediate abstractions)

**Next Candidates:**
- Additional reactive systems as gameplay needs emerge

**Certainty:** ~100% for current systems. Architecture solid, tuning enabled, ready for feel iteration.

**Dependencies:** Requires stable physics core. Changes in controller would force redesign here.

---

## Skeleton Debug Layer (Implemented, 100% certain) ✅

**Status:** Minimum skeleton system delivered and integrated

- `character::skeleton` data model with transform propagation
- Hardcoded humanoid T-pose asset for graybox visualization
- Debug draw support with GUI toggles and optional animation probe
- Build + lint + runtime smoke test verified (see implementation checklist)

**Certainty:** 100% — matches plan scope and validated in runtime.

**Dependencies:** Requires rendering + runtime scaffolding; now unblocks secondary motion, IK, and pose tooling work.

---

## Static Keyframe Preview (Implemented, 100% certain) ✅

**Status:** Quaternion keyframe architecture validated and proven

- `character::keyframe` module with quaternion-based pose storage
- 8-joint minimum set (shoulders, elbows, hips, knees) sufficient for validation
- 4 hardcoded poses (T_POSE, STEP_LEFT, NEUTRAL, STEP_RIGHT) visually distinct
- Manual GUI selection with instant pose switching (no blending)
- No visual artifacts, crashes, or stability issues

**Certainty:** 100% — core hypothesis proven through implementation and testing.

**Key Learning:** Quaternion composition with T-pose baseline (`local_transform = translate(t_pose_pos) * mat4_cast(pose_quat)`) works correctly. Ready for locomotion-driven animation.

**Dependencies:** Built on Skeleton Debug System and Attach Skeleton to Body. Unblocks primary skeletal animation (locomotion-driven) for next iteration.

**Code Standards Improvements:**
- Established UPPER_CASE convention for enum constants in `AGENTS.md`
- Updated `.clang-tidy` to enforce `EnumConstantCase: UPPER_CASE`
- Pattern validated and ready for reuse

---

## Primary Skeletal Animation (Implemented, 100% certain) ✅

**Status:** Distance-phased pose switching validated and proven

- Distance-phase calculation: `phase = fmod(distance_traveled, cycle_length) / cycle_length`
- Threshold-based pose selection (STEP_LEFT → NEUTRAL → STEP_RIGHT → NEUTRAL cycle)
- Surveyor-wheel synchronization with locomotion system
- Manual pose override mode for debugging
- Real-time cycle length tuning via GUI slider
- Walk speed lock (SHIFT key) for precise tuning observation

**Certainty:** 100% — core hypothesis proven. Distance-phased triggering works cleanly, poses cycle correctly at all speeds, no visual artifacts or crashes.

**Key Learning:** Surveyor-wheel pattern extends naturally to skeletal animation. Cumulative `distance_traveled` with modulo-based phase calculation provides stable, speed-independent cycling. Threshold-based pose selection sufficient for graybox validation before adding interpolation.

**Dependencies:** Built on Static Keyframe Preview and Procedural Locomotion. Unblocks pose blending (lerp/slerp) and speed-based gait switching for next iteration.

**Architecture Validated:**
- `animation_state` pattern continues to work well (cycle_length parameter, current_automatic_pose state)
- `update_skeletal_animation()` encapsulation clean (single call site in game loop)
- Manual override parameter preserves debug UI functionality
- Stop behavior correct (pose freezes when distance stops, no special handling needed)

---

## Secondary Motion (Implemented, 100% certain) ✅

**Status:** Per-bone spring-damper lag validated and proven

- 4 spring states (left_elbow, right_elbow, left_knee, right_knee) track offset and velocity
- Velocity-injection approach: spring drives joint angular velocity, not direct offset
- Integration parameters: stiffness (15.0 Hz), damping_ratio (1.0), response_scale (0.02)
- Clean reactive layer: applied after primary pose, before transform propagation
- UI controls: stiffness, damping, response_scale, enable/disable toggle

**Certainty:** 100% — hypothesis proven. Spring-damper lag creates natural follow-through on limbs during pose transitions and speed changes without affecting gameplay.

**Key Learning:** Velocity-injection approach (discovered during iteration) superior to direct offset manipulation. Prevents overshoot artifacts and maintains natural joint motion. Spring-damper pattern successfully scaled from root transform (landing spring, acceleration tilt) to individual skeletal joints.

**Dependencies:** Built on Primary Skeletal Animation and Spring-Damper foundation. Pattern proven for skeletal application; ready for expansion to additional joints (head bobble, spine flex) or cloth/appendage physics.

**Architecture Validated:**
- Per-bone spring state pattern scales cleanly (4 joints, 8 floats total state)
- Axis-specific lag (elbows Y-axis, knees X-axis) provides natural swing motion
- Update timing preserved: after `apply_pose()`, before skeleton propagation
- "Do no harm" principle maintained (pure reactive layer, zero gameplay impact)
- Parameter exposure pattern consistent with existing tuning UI

---

## Debug Visual Overhaul (Implemented, 100% certain) ✅

**Status:** Three focused debug visualizations validated and proven

- **Velocity Decay Trail:** Time-sampled position breadcrumbs (0.1s intervals, 25 samples) with age-based size/alpha fade
- **Plot Functions:** Reusable `plot_value()` and `plot_histogram()` with configurable time windows, value ranges, axis labels
- **Speed Gradient Ring:** Dynamic expanding ring with blue → green → yellow → red gradient based on speed ratio

**Certainty:** 100% — all visualizations rendering correctly, zero gameplay impact verified, systemic reusability achieved.

**Key Learning:**
- Velocity trail tuning: 1.0s sampling too coarse, 0.1s provides readable turning radius visualization
- Speed ring evolution: Fixed radius (max_speed) less intuitive than dynamic expansion (current_speed)
- Plot functions: Generic temporal graphing unlocks future debugging (speed, blend factors, any float value)
- Buffer management: Simple vector erase sufficient for debug use (<500 samples)

**Dependencies:** Built on debug_draw and gui infrastructure. Pure additive layer, no dependencies on gameplay systems. Pattern proven for future debug visualizations (trajectory prediction, cooldown rings, temporal graphs).

**Architecture Validated:**
- State ownership pattern: `velocity_trail_state` in `game_world` (app layer), visualization in `debug_draw` (rendering layer)
- Forward declaration pattern prevents circular dependencies (rendering → app)
- Static map storage for plot buffers scales well (per-label isolation, automatic cleanup not required for debug)
- Graybox discipline: white spheres, gradient colors, ImGui defaults (no premature polish)

---

## Summary

**Completed Iterations (Oct 7-10, 2025):**
1. Skeleton Debug System → Static Keyframe Preview
2. Static Keyframe Preview → Primary Skeletal Animation
3. Primary Skeletal Animation → Secondary Motion
4. Camera Zoom + Debug Panel Unification
5. Debug Visual Overhaul

**Total Certainty Growth:** 6 layers moved from planning → 100% validated

**Key Patterns Proven:**
- Surveyor-wheel pattern (locomotion → skeletal animation)
- Spring-damper pattern (root transform → individual joints)
- Animation state encapsulation
- Debug visualization without gameplay impact
- Velocity-injection for reactive animation
- Distance-phased triggering
- Graybox discipline (parameters over polish)

**Architecture Decisions Validated:**
- Quaternion keyframe composition
- Transform propagation timing (physics → orientation → animation → render)
- State ownership separation (app vs rendering)
- Public parameter access for tuning UI
- Forward declarations for dependency management

---

**Historical Context:** This snapshot preserves detailed retrospectives and architecture validations that informed the foundation. Future work should reference implementation docs (e.g., `PLANS/implementation_*.md`, `PLANS/code_review_*.md`) for specifics rather than relying on inline notes in DEPENDENCY_STACK.md.
