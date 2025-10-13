# Implementation Plan: Air Locomotion Weights

**Complexity:** 3 points (feature)

**Foundation Architecture Audit:**

Systems Involved: `animation_state`, `locomotion_system`

Current State:
- `animation_state`: Lives in `src/character/animation.h/cpp`
  - Instantiated in `controller.animation` (`src/character/controller.h`)
  - Updated in `game_world::update()` (`src/app/game_world.cpp`)
  - Contains: acceleration tilt, landing spring, skeletal animation, secondary motion
  - Exposed in GUI via `src/gui/character_panel.cpp` (separate collapsing headers)

- `locomotion_system`: Lives in `src/character/locomotion.h/cpp`
  - Instantiated in `game_world.locomotion` (`src/app/game_world.h`)
  - Updated in `game_world::update()` (`src/app/game_world.cpp`)
  - Uses `is_grounded` flag from `controller.is_grounded`
  - Currently **freezes phase advancement when airborne** (line 19: `if (is_grounded)`)

- `skeleton`: Lives in `src/character/skeleton.h/cpp`
  - Instantiated as `game_world.t_pose_skeleton`
  - Modified by `animation.update_skeletal_animation()` and `animation.update_secondary_motion()`
  - Uses quaternion slerp blending (validated pattern)

Architecture Issues: None. Clean separation; feature integrates into existing update pipeline.

Update Flow:
1. `controller.update()` → sets `is_grounded` via collision detection
2. `locomotion.update()` → advances phase/distance only when grounded
3. `animation.update_skeletal_animation()` → applies keyframe blending
4. NEW: Contact/air weight system sits between steps 2-3

---

## Graybox Implementation Checklist

### 0. Planning Checks

- [x] **Foundation Audit Complete:** No architectural conflicts; feature integrates into existing pipeline
- [x] **Gate Mechanism:** Using dual-reference spring-damper pattern (contact_weight target vs smoothed state)
- [x] **Multi-Mode Review:** Feature applies uniformly (no mode-specific behavior to verify)

### 1. Data Structures & State

- [x] **File:** `src/character/animation.h`
  - [x] Add `spring_damper contact_weight_spring;` to `animation_state` struct
  - [x] Add `float contact_weight_target = 1.0f;` to `animation_state` struct (dual-reference pattern)
  - [x] Add `float contact_weight_frequency = 8.0f;` to `animation_state` struct (tuning parameter, Hz)

**Files changed:**
- `src/character/animation.h` (added contact weight fields)
- `src/character/animation.cpp` (initialized spring position in constructor)

### 2. Weight State Management

- [x] **File:** `src/character/animation.cpp`
  - [x] In `animation_state::animation_state()` constructor:
    - [x] Initialize `contact_weight_spring.position = 1.0f;` (start grounded)
    - [x] Set spring stiffness/damping based on `contact_weight_frequency` (use critical damping pattern from landing spring)

- [x] **File:** `src/character/animation.h`
  - [x] Add new method signature: `void update_contact_weight(bool is_grounded, float dt);`

- [x] **File:** `src/character/animation.cpp`
  - [x] Implement `update_contact_weight()`:
    - [x] Set `contact_weight_target` based on `is_grounded` (1.0 if grounded, 0.0 if airborne)
    - [x] Update spring stiffness/damping from frequency: `stiffness = (contact_weight_frequency * 2.0f * PI)^2`, `damping = critical_damping(stiffness)`
    - [x] Update `contact_weight_spring` toward `contact_weight_target` using `spring_step`

**Pseudocode:**
```
void update_contact_weight(bool is_grounded, float dt):
    contact_weight_target = is_grounded ? 1.0f : 0.0f
    
    // Convert frequency (Hz) to spring stiffness (API consistency: use glm::two_pi)
    float omega = contact_weight_frequency * glm::two_pi<float>()
    contact_weight_spring.stiffness = omega * omega
    contact_weight_spring.damping = critical_damping(contact_weight_spring.stiffness)
    
    // Update spring toward target
    spring_step step = { contact_weight_target, dt }
    contact_weight_spring.update(step)
```

**Note:** Requires `#include <glm/gtc/constants.hpp>` for `glm::two_pi<float>()`

- [x] **File:** `src/app/game_world.cpp`
  - [x] In `game_world::update()`, after `locomotion.update()` and before `update_skeletal_animation()`:
    - [x] Call `character.animation.update_contact_weight(character.is_grounded, dt);`

**Files changed:**
- `src/character/animation.h` (added method signature)
- `src/character/animation.cpp` (constructor initialization, method implementation, added glm/gtc/constants.hpp)
- `src/app/game_world.cpp` (added update call)

### 3. Airborne Phase Advancement

- [x] **File:** `src/character/locomotion.cpp`
  - [x] Modify `locomotion_system::update()`:
    - [x] **Remove** the outer `if (is_grounded)` gate (line 19)
    - [x] Keep speed calculations and phase advancement logic **always active**
    - [x] Keep distance accumulation **always active** (surveyor wheel continues in air)

**Files changed:**
- `src/character/locomotion.cpp` (removed grounded gate, phase now advances in air)

**Pseudocode (modified update logic):**
```
void locomotion_system::update(ground_velocity, dt, is_grounded):
    // Calculate current speed from horizontal velocity (XZ plane)
    current_speed = length(ground_velocity)
    
    // Smooth speed (keep existing logic)
    smoothed_speed += (current_speed - smoothed_speed) * speed_smoothing * dt
    
    // Calculate blend factor (keep existing logic)
    blend = calculate_walk_run_blend(smoothed_speed)
    
    // Blend stride length (keep existing logic)
    blended_stride = smooth_mix(walk_stride, run_stride, blend)
    
    // Phase advancement (NOW ALWAYS ACTIVE - was gated by is_grounded)
    if blended_stride > 0:
        // Use horizontal speed (XZ magnitude) for phase drive
        drive_speed = max(0.0, current_speed)
        phase += (drive_speed / blended_stride) * dt
        phase = fmod(phase, 1.0)
        
        // Distance accumulation (surveyor wheel)
        distance_traveled += drive_speed * dt
```

### 4. Amplitude Suppression (Pose Blending)

- [x] **File:** `src/character/animation.cpp`
  - [x] In `update_skeletal_animation()`, after calculating blended quaternions from keyframes:
    - [x] Get `contact_weight = contact_weight_spring.get_position()`
    - [x] Calculate `air_weight = 1.0f - contact_weight`
    - [x] For each joint, blend from T_POSE to gait blend using contact_weight:
      - [x] `final_rotation = slerp(T_POSE_rotation, gait_blended_rotation, contact_weight)`
    - [x] Apply final rotations to skeleton joints

**Files changed:**
- `src/character/animation.cpp` (added contact weight blending in update_skeletal_animation)

**Pseudocode:**
```
// After existing keyframe blending (source_pose → target_pose with factor t)
glm::quat gait_blended_rotation = slerp(source_kf.joint, target_kf.joint, t)

// Get contact weight (1.0 grounded, 0.0 airborne, smoothed transition)
float contact_weight = contact_weight_spring.get_position()

// T_POSE uses identity quaternion for all joints (arms out 90°, legs straight)
glm::quat t_pose_rotation = glm::quat(1, 0, 0, 0)  // identity

// Blend from T-pose (neutral) to gait animation based on ground contact
glm::quat final_rotation = slerp(t_pose_rotation, gait_blended_rotation, contact_weight)

// Apply to joint (existing pattern)
apply_blended_joint(joint_idx, final_rotation)
```

### 5. Acceleration Tilt Scaling

- [x] **File:** `src/character/animation.cpp`
  - [x] In `update_acceleration_tilt()`, after calculating `target_pitch` and `target_roll`:
    - [x] Get `contact_weight = contact_weight_spring.get_position()`
    - [x] Scale tilt targets by contact_weight: `target_pitch *= contact_weight`, `target_roll *= contact_weight`
    - [x] Continue with existing smoothing logic

**Files changed:**
- `src/character/animation.cpp` (added contact weight scaling in update_acceleration_tilt)

**Pseudocode:**
```
// After calculating target tilt angles (existing logic)
float target_pitch = local_forward * effective_tilt_magnitude
float target_roll = local_right * effective_tilt_magnitude

// Scale by contact weight (no tilt when airborne)
float contact_weight = contact_weight_spring.get_position()
target_pitch *= contact_weight
target_roll *= contact_weight

// Continue with existing smoothing (blend toward scaled targets)
```

### 6. Debug UI Integration

- [x] **File:** `src/gui/character_panel.cpp`
  - [x] Add new collapsing header `"Air Locomotion Weights"` after "Acceleration Tilt" section
  - [x] Add `ImGui::SliderFloat("Contact Weight Frequency (Hz)", &character.animation.contact_weight_frequency, 3.0f, 12.0f);`
  - [x] Add read-only displays:
    - [x] `ImGui::Text("Contact Weight Target: %.3f", character.animation.contact_weight_target);`
    - [x] `ImGui::Text("Contact Weight (smoothed): %.3f", character.animation.contact_weight_spring.get_position());`
    - [x] `ImGui::Text("Air Weight: %.3f", 1.0f - character.animation.contact_weight_spring.get_position());`
    - [x] `ImGui::Text("Spring Velocity: %.3f", character.animation.contact_weight_spring.get_velocity());`

**Files changed:**
- `src/gui/character_panel.cpp` (added Air Locomotion Weights collapsing header)

### 7. Quality Gates

- [x] Build passes (`CMake: Build (Debug)`).
- [x] Lint/format clean (run `clang-format`/`clang-tidy` tasks).
- [x] Smoke run compiles and launches (`Run (Debug)`).
- [x] **T_POSE validation:** Verify `get_keyframe_data(pose_type::T_POSE)` returns identity quaternions (1,0,0,0) for all 8 joints.
- [x] **Visual validation:** Jump mid-stride → limbs continue moving smoothly in air (no freeze).
- [x] **Landing coherence:** Land at different phases → no pops, smooth transition back to contact.
- [x] **Weight visualization:** Debug UI shows smooth 0→1 transitions during jumps.
- [x] **Edge case test:** Vertical jumps (near-zero XZ velocity) → acceptable if phase advance minimizes; suppression still hides pops.

**Build/Format Status:** All automated checks pass. ✅

**Visual Validation:** All manual tests pass. ✅

**Known Polish Item (deferred to backlog):** Secondary motion can cause elbows to hyperextend during rapid pose transitions (e.g., jump with T-pose blending). Logged in `DESIGN_BACKLOG.md` as "Secondary motion joint limits" - constraint enforcement would clamp spring offsets to anatomical ranges.

**Files changed (complete feature):**
- `src/character/animation.h` (contact weight fields, method signature)
- `src/character/animation.cpp` (constructor, update_contact_weight, skeletal blending, tilt scaling)
- `src/character/locomotion.cpp` (removed grounded gate for phase advancement)
- `src/app/game_world.cpp` (added update_contact_weight call)
- `src/gui/character_panel.cpp` (added Air Locomotion Weights UI)

---

## Implementation Notes

**Dual-Reference Pattern Application:**
- **Immutable target:** `contact_weight_target` (set by grounded state, instant 0/1 flip)
- **Smoothed state:** `contact_weight_spring.position` (drives all blending/scaling, smooth transitions)
- **Rationale:** Prevents circular dependency where smoothed value references itself; allows instant target changes with smooth follow-through

**Grounded Detection:**
- Already exists: `controller.is_grounded` (set by collision detection in `controller.cpp`)
- No changes needed to grounded detection logic

**Phase Continuity:**
- Horizontal distance (XZ velocity magnitude * dt) drives phase in air
- Matches grounded surveyor-wheel semantics (vertical velocity ignored)
- Distance accumulation continues (odometer never stops)
- **Behavior change:** Removing `if (is_grounded)` gate means `wheel_spin_angle` and `distance_traveled` update during air time
  - This is **intended** (odometer semantics: wheel continues to "spin" based on horizontal motion)
  - **Validation:** Confirm no gameplay systems depend on grounded-only distance accumulation
  - Debug wheel visualization will spin during jumps (accurate to horizontal velocity)

**T_POSE Blending:**
- T_POSE keyframe uses identity quaternions (no rotation from bind pose)
- Blend from identity to gait animation via contact_weight
- Result: limbs "deflate" toward neutral when airborne, "inflate" when grounded

**Spring Frequency Tuning:**
- Start at 8.0 Hz (visible transitions for debugging)
- Tune toward 8-12 Hz range for production feel
- Lower = slower/floatier transition, Higher = snappier transition

**Integration Points:**
- `update_contact_weight()` called in `game_world::update()` after locomotion, before skeletal animation
- Weight system sits cleanly between locomotion (phase) and animation (pose blending)
- No changes to controller, collision, or orientation systems

---

## Changelog

- **2025-10-13:** Plan reviewed and approved (see `20251013_010455_air_locomotion_weights_PLAN_REVIEW.md`)
  - Updated pseudocode to use `glm::two_pi<float>()` for API consistency
  - Added T_POSE identity quaternion validation to quality gates
  - Added edge case test for vertical jumps (minimal XZ phase advance acceptable)
  - Documented locomotion gating removal impact (wheel spin/distance continue in air)
- **2025-10-12:** Initial plan created after feature description finalized and clarifications resolved
