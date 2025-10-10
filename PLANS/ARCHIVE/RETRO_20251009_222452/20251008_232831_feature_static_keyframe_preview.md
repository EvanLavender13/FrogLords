# Static Keyframe Preview (Keyframe Foundation Validation)

**Core Mechanic:** Manual GUI selection between hardcoded skeletal poses to validate quaternion-based keyframe data structure before adding locomotion-driven animation. Proves fundamental storage/application/composition works correctly without the complexity of distance-phased triggering.

**Simplified Scope:** This iteration validates the quaternion architecture using **3 walking keyframes only** (STEP_LEFT, NEUTRAL, STEP_RIGHT) derived from walk gait reference images. Manual GUI selection proves the foundation works before adding automatic cycle playback or locomotion triggering.

**Pattern Reuse:** Extends existing skeleton transform propagation and GUI tuning patterns. Follows same architecture as reactive animation layers: pure transformation of existing data (skeleton local transforms) with parameter exposure via GUI. Reuses proven quaternion composition from GLM.

---

## CLARIFICATION (Resolved)

### Visual References Established

**Source Material:** Walk gait images from NOTES/Images/ directory (walk1.png, walk2.png, walk3.png, walk4.png)

The walk cycle uses **3 keyframes** that cycle as: STEP_LEFT → NEUTRAL → STEP_RIGHT → NEUTRAL (repeat).

**Visual Analysis from Images:**

1. **STEP_LEFT** (left leg forward):
   - Left hip flexed forward, left knee slightly bent
   - Right leg extended back, right knee near-straight
   - Right arm swings forward (counter-rotation), left arm back
   - Torso upright with subtle forward lean

2. **NEUTRAL** (passing pose):
   - Both legs near-vertical, feet approximately together
   - Arms at sides or transitioning through neutral position
   - Torso upright, minimal lean
   - This is the transition keyframe between step poses

3. **STEP_RIGHT** (right leg forward):
   - Right hip flexed forward, right knee slightly bent
   - Left leg extended back, left knee near-straight
   - Left arm swings forward (counter-rotation), right arm back
   - Torso upright with subtle forward lean
   - Mirror of STEP_LEFT

**Cycle Pattern:** The walk animates as STEP_LEFT → NEUTRAL → STEP_RIGHT → NEUTRAL, creating a 4-phase cycle from 3 unique keyframes (NEUTRAL is reused).

### Skeleton Structure (Current Codebase)

From `t_pose.cpp`, the skeleton has **17 joints** in this hierarchy:

```
0:  root (hips)
1:  spine_lower
2:  spine_upper
3:  neck
4:  head
5:  left_shoulder
6:  left_elbow
7:  left_wrist
8:  right_shoulder
9:  right_elbow
10: right_wrist
11: left_hip
12: left_knee
13: left_ankle
14: right_hip
15: right_knee
16: right_ankle
```

**Joint Set for Walk Validation (Iteration 1):** Shoulders (5,8), elbows (6,9), hips (11,14), knees (12,15). Total: 8 joints.

**Future Extensions (Deferred):** Add spine_upper (2) for torso lean; add ankles (13,16) for grounded foot placement. Defer until 8-joint validation succeeds and reveals specific need.

### Quaternion Specification Method

**Input Format:** Euler angles (degrees) → converted to quaternions via `glm::quat(glm::radians(vec3(pitch, yaw, roll)))`.

**Rationale:** Human-readable for hand-authoring initial poses. Matches GDC approach: "You could always do a hybrid... didn't have to take it quite this far." Quaternions internally prevent gimbal lock; Euler externally for legibility.

**Composition:** Pose quaternions **multiply with** T-pose rotations (not replace). `final_rotation = t_pose_quat * pose_quat`. This allows poses to be deltas from T-pose baseline, matching procedural layering principle ("animation on top of physics/base").

### Scope Boundaries (Iteration 1)

**In Scope:**
- 3 hardcoded walking keyframes (STEP_LEFT, NEUTRAL, STEP_RIGHT)
- GUI dropdown for manual pose selection (4 options: STEP_LEFT, NEUTRAL, STEP_RIGHT, T_POSE)
- 8 joints only: shoulders (5,8), elbows (6,9), hips (11,14), knees (12,15)
- Instant pose switching (no blending/interpolation)
- Visual validation via skeleton wireframe

**Deferred to Later Iterations:**
- Automatic walk cycle playback (STEP_LEFT → NEUTRAL → STEP_RIGHT → NEUTRAL loop)
- Distance-phased triggering (surveyor-wheel integration)
- Blend weight slider (quaternion slerp)
- Pose authoring UI (hardcoded Euler values only)
- Secondary motion/softness layers
- Gravity-aware bounce

### Success Criteria (Concrete)

**Primary Test (STEP_LEFT vs T_POSE):**
- STEP_LEFT displays visibly different from T_POSE (highest visual contrast)
- Left arm swings back, right arm swings forward (counter-rotation visible)
- Left leg extends forward, right leg extends back (stride visible)
- No gimbal lock artifacts (limbs don't flip/twist unnaturally)
- Hierarchical integrity: child joints follow parent rotations correctly

**Full Validation (3 Walk Poses + T_POSE):**
- All 4 poses (T_POSE, STEP_LEFT, NEUTRAL, STEP_RIGHT) are selectable and visually distinct
- Switching between poses is instant and stable (no frame-delay glitches)
- STEP_RIGHT mirrors STEP_LEFT visually (symmetry check)
- NEUTRAL shows transitional pose (legs near-vertical, arms transitioning)
- Skeleton maintains visual coherence across all switches

**Acceptance:** All 4 selectable poses display correctly; switching is instant/stable. Validates quaternion keyframe foundation → proceed to automatic cycle playback or locomotion-driven animation.

**Note:** Angle values mentioned in implementation (e.g., ~30° shoulder rotation) are targets for achieving visual correctness, not numeric acceptance criteria. The test is visual quality.

---

## Implementation Specification

**Keyframe Structure:**
```cpp
struct keyframe {
    glm::quat left_shoulder, right_shoulder;   // Arms
    glm::quat left_elbow, right_elbow;
    glm::quat left_hip, right_hip;             // Legs
    glm::quat left_knee, right_knee;
    // Optional (extended): glm::quat spine_upper, left_ankle, right_ankle;
};
```

**Pose Storage:** 3 hardcoded walking keyframes (STEP_LEFT, NEUTRAL, STEP_RIGHT) + T_POSE enum (4 total selectable).

**GUI Control:** Dropdown in Animation Tuning panel with 4 options: "T-Pose", "Step Left", "Neutral", "Step Right".

**Application:** Each frame, compose selected pose quaternions with T-pose: `final_quat = t_pose_quat * pose_quat`. Inject into `skeleton.joints[i].local_transform` before `update_global_transforms()`.

**Files Modified:**
- `animation.h/.cpp` – keyframe struct, 3 walk poses + T_POSE enum, application logic (~45 lines)
- `character_panel.cpp` – GUI dropdown (~15 lines)

**Integration Point:** `character::animation::update()` → `apply_keyframe_pose(skeleton, selected_pose)`.

---

## Validation Metrics

**Primary Test:** STEP_LEFT vs T_POSE (highest visual contrast).
- Left shoulder: ~30° back, left elbow: near-straight
- Right shoulder: ~45° forward, right elbow: ~20° bend
- Left hip: ~30° forward flex, left knee: ~15° bend
- Right hip: ~20° back extension, right knee: near-straight

**Correctness Checks:**
- No gimbal lock artifacts (limbs don't flip/twist)
- Hierarchical integrity (child joints follow parents)
- Symmetry: STEP_RIGHT mirrors STEP_LEFT
- NEUTRAL shows near-vertical legs, arms at sides
- Stability: instant switching, no frame glitches

**Iteration Time:** Minutes (pose selection instant, quaternion tweaks real-time).

---

## Deferred Features

- Automatic cycle playback (STEP_LEFT → NEUTRAL → STEP_RIGHT → NEUTRAL loop)
- Blend weight slider (quaternion slerp)
- Distance-phased triggering (surveyor-wheel)
- Pose authoring UI
- Secondary motion layers
- Gravity-aware bounce

---

## Risk Assessment

**Certainty:** ~70%

**Validated:**
- ✅ Skeleton system proven
- ✅ Reduced scope (separated keyframe validation from locomotion)
- ✅ GLM quaternion math (library-provided)

**Risks:**
- ⚠️ Hand-authored Euler→quaternion values (no visual tooling; expect iteration)
- ⚠️ First multi-DOF rotation implementation (quaternion composition well-understood, but untested in this codebase)

**Unlocks:**
- Validates quaternion keyframe architecture (critical prerequisite)
- Re-enables locomotion-driven skeletal animation
- Foundational for IK, secondary motion, blend trees, authoring tools

---

## Origin

Identified during Primary Skeletal Animation deferral (2025-10-07) as critical missing prerequisite. Single-float keyframes failed; quaternion-based multi-DOF required. See `DEFERRAL_LOG.md`.
