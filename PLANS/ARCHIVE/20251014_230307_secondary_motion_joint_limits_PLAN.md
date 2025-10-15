# Implementation Plan: Secondary Motion Joint Limits

**Complexity:** 1-2 points (feature)

**Feature Description:** Add anatomical bend constraints (min/max offset clamps) to elbow/knee secondary motion springs to prevent unnatural hyperextension during rapid pose transitions.

---

## Foundation Architecture Review

**Systems Involved:** `secondary_motion_state` (animation.h/cpp)

**Current State Audit:**
- `secondary_motion_state`: Defined in `src/character/animation.h` lines 9-30
  - Per-joint spring states: offset + velocity for elbows/knees
  - Tuning parameters: stiffness, damping_ratio, response_scale
  - Updated in `animation_state::update_secondary_motion()` at line 239
  - Spring update logic in lambda `update_spring()` at lines 240-295
  
- Current spring flow (per joint):
  1. Detect parent rotation change (lines 243-252)
  2. Inject parent velocity into child spring (lines 254-271)
  3. Apply spring force toward zero offset (lines 273-282)
  4. **← CLAMP LOCATION:** After offset integration (line 282), before rotation application (lines 284-291)
  5. Apply offset rotation to child joint (lines 284-291)

- GUI exposure: `src/gui/character_panel.cpp` lines 106-129
  - Tuning sliders: stiffness, damping_ratio, response_scale
  - Read-only state: per-joint offset + velocity displays

**Architecture Issues Found:** None. Clear insertion point for clamp operation.

**Multi-Mode Feature Comparison:** N/A (single update path for all joints)

---

## Graybox Implementation Checklist

### 0. Planning Checks

- [x] Gate Mechanism Review: Clamp operation (geometric delta) - standard pattern ✅
- [ ] UI Ordering Verified: N/A (no enums/cycles)
- [ ] Parameter Cleanup Review: No conditional logic changes

### 1. Data Structures & State

- [x] **File:** `src/character/animation.h`
  - [x] In `secondary_motion_state` struct (after line 18), add per-joint limit fields:
    - [x] `float left_elbow_min_offset = 0.0f;` (radians, no backward bend)
    - [x] `float left_elbow_max_offset = 2.618f;` (150° in radians)
    - [x] `float right_elbow_min_offset = 0.0f;`
    - [x] `float right_elbow_max_offset = 2.618f;`
    - [x] `float left_knee_min_offset = 0.0f;`
    - [x] `float left_knee_max_offset = 2.618f;`
    - [x] `float right_knee_min_offset = 0.0f;`
    - [x] `float right_knee_max_offset = 2.618f;`
  - [x] Brief comment: "Anatomical hinge constraints (min/max radians)"

**Rationale:** Per-joint limits allow future asymmetry or joint-specific tuning (e.g., tighter knee limits than elbows). Initialize to anatomically sensible ranges (0° = straight, 150° = typical hinge max).

**Files Changed:** `src/character/animation.h`

### 2. Core Logic

- [x] **File:** `src/character/animation.cpp`
  - [x] In `update_spring()` lambda (line 240), after offset integration (line 282):
    - [x] Clamp offset: `float clamped_offset = glm::clamp(offset, min_limit, max_limit);`
    - [x] Zero outward velocity at boundary to prevent jitter:
      ```cpp
      if (clamped_offset != offset) {
          // Hit limit - check if velocity is pushing further outward
          bool at_min = (clamped_offset == min_limit && velocity < 0.0f);
          bool at_max = (clamped_offset == max_limit && velocity > 0.0f);
          if (at_min || at_max) {
              velocity = 0.0f;  // Stop pushing against limit
          }
      }
      offset = clamped_offset;
      ```
    - [x] Pass min/max as lambda parameters (add to lambda signature after `rotation_axis`)
  - [x] Update all four `update_spring()` calls (lines 298-309):
    - [x] Left elbow call: Add `secondary_motion.left_elbow_min_offset, secondary_motion.left_elbow_max_offset`
      - [x] Add comment: `// Elbow hinge: bend axis = math::UP (Y+)`
    - [x] Right elbow call: Add `secondary_motion.right_elbow_min_offset, secondary_motion.right_elbow_max_offset`
      - [x] Add comment: `// Elbow hinge: bend axis = math::UP (Y+)`
    - [x] Left knee call: Add `secondary_motion.left_knee_min_offset, secondary_motion.left_knee_max_offset`
      - [x] Add comment: `// Knee hinge: bend axis = X+ (side-to-side)`
    - [x] Right knee call: Add `secondary_motion.right_knee_min_offset, secondary_motion.right_knee_max_offset`
      - [x] Add comment: `// Knee hinge: bend axis = X+ (side-to-side)`

**Pseudocode:**
```
// In update_spring lambda, after line 282:
float clamped_offset = glm::clamp(offset, min_limit, max_limit);
if (clamped_offset != offset) {
    bool at_min = (clamped_offset == min_limit && velocity < 0.0f);
    bool at_max = (clamped_offset == max_limit && velocity > 0.0f);
    if (at_min || at_max) velocity = 0.0f;
}
offset = clamped_offset;

// Lambda signature becomes:
auto update_spring = [&](int parent_idx, int joint_idx, float& offset, float& velocity,
                         glm::quat& prev_parent_rot, const glm::vec3& rotation_axis,
                         float min_limit, float max_limit) { ... }
```

**Note:** Clamp AFTER velocity integration but BEFORE rotation application. Zero velocity when clamped and pushing outward prevents jitter at limits (million-repetition thinking).

**Files Changed:** `src/character/animation.cpp`

### 3. Debug UI

- [ ] **File:** `src/gui/character_panel.cpp`
  - [ ] In "Secondary Motion" section (after line 114), add joint limit controls with robust rad↔deg conversion:
    - [ ] `ImGui::Separator();` (visual grouping)
    - [ ] `ImGui::Text("Joint Limits (degrees):");`
    - [ ] For each joint limit pair (8 total), use display-only local variables:
      ```cpp
      // Left elbow example (repeat pattern for all 8 limits)
      float left_elbow_min_deg = glm::degrees(character.animation.secondary_motion.left_elbow_min_offset);
      if (gui::widget::slider_float("Left Elbow Min", &left_elbow_min_deg, 0.0f, 180.0f)) {
          character.animation.secondary_motion.left_elbow_min_offset = glm::radians(left_elbow_min_deg);
      }
      float left_elbow_max_deg = glm::degrees(character.animation.secondary_motion.left_elbow_max_offset);
      if (gui::widget::slider_float("Left Elbow Max", &left_elbow_max_deg, 0.0f, 180.0f)) {
          character.animation.secondary_motion.left_elbow_max_offset = glm::radians(left_elbow_max_deg);
      }
      ```
    - [ ] Repeat for: right elbow min/max, left knee min/max, right knee min/max

**UI Layout:**
```
Secondary Motion
  [x] Enable Secondary Motion
  Stiffness: [====|=====] 15.0
  Damping Ratio: [====|=====] 2.0
  Response Scale: [====|=====] 0.05
  
  --- (separator)
  Joint Limits (degrees):
  Left Elbow Min: [====|=====] 0°
  Left Elbow Max: [====|=====] 150°
  Right Elbow Min: [====|=====] 0°
  Right Elbow Max: [====|=====] 150°
  (same for knees)
  
  (read-only state displays below)
```

**Rationale:** 
- Degrees more intuitive for tuning than radians
- Local display-only variables prevent double-conversion drift
- Write back to radian storage only when slider changes (gui::widget::slider_float returns bool)

### 4. Quality Gates

- [ ] Build passes (`CMake: Configure`, `CMake: Build Debug`)
- [ ] Lint/format clean (clang-format/clang-tidy if available)
- [ ] Smoke run: Launch, enable secondary motion, observe limbs during:
  - [ ] Jump mid-stride (T-pose blend during air)
  - [ ] Run gait transitions (walk ↔ run rapid switches)
  - [ ] Visual: Elbows/knees stay within 0-150° range (no backward hyperextension)
  - [ ] Tuning: Adjust min/max sliders in real-time, verify clamps take effect

---

## Known Polish Gaps (defer to backlog)

- **Joint limit visualization:** Could add debug draw of limit arcs (defer—tuning via offset displays sufficient)
- **Per-joint-type limits:** Currently all elbows/knees use same ranges; could expose different defaults per joint type (defer—wait for evidence of need)
- **Soft vs hard clamps:** Hard clamps may cause spring to "stick" at limits; could add soft spring-toward-limit behavior (defer—test hard clamps first, iterate if needed)
- **Asymmetric limits:** Real elbows/knees have asymmetric ranges (e.g., elbow can't bend backward but can hyperextend slightly forward); current symmetric min/max is simplification (defer—graybox acceptable)

---

## Validation Loop

**Test Sequence:**
1. Run game, enable secondary motion via character panel
2. Sprint → jump mid-stride → observe elbow/knee behavior during T-pose blend
3. Walk → sprint → walk transitions rapidly → observe limb behavior
4. Check character panel offset displays—should never exceed min/max ranges
5. Tune limits via sliders in real-time:
   - Reduce max to 90° → limbs should stop sooner
   - Increase max to 180° → more spring extension allowed
6. Verify no hyperextension (elbows/knees bending backward unnaturally)

**Expected Iteration Time:** Minutes (tune → observe → adjust)

**Success Criteria:**
- Elbows/knees stay within anatomical ranges during rapid transitions
- Spring dynamics still visible (wobble/lag) but constrained
- Tuning parameters respond in real-time via debug UI
- No visual artifacts (snapping, discontinuities) at limit boundaries

---

## Implementation Notes

**Why clamp after integration?** Springs need to "feel" the limit to settle correctly. Clamping after velocity integration but before rotation application allows the spring to compress against the limit (velocity goes to zero naturally) rather than creating discontinuous jumps.

**Degree vs radian choice:** Internal storage uses radians (consistent with quaternion math, no conversion overhead in update loop). UI uses degrees (more intuitive for tuning). Convert only at UI boundary.

**Default limit values:** 0° min (straight joint, no backward bend) and 150° max (typical hinge joint anatomical limit) are conservative starting points. Elbows/knees in real humans reach ~140-160° depending on flexibility; 150° is middle-of-road for graybox iteration.

**Future extension:** If other joint types need limits (shoulders, hips, spine), same pattern applies—add min/max fields per joint, pass to update_spring, clamp after integration. Pattern is reusable.
