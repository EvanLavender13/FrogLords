# Secondary Motion (Skeletal Follow-Through)

**Core Mechanic:** Per-bone spring-damper lag applied to elbow/knee joints, creating natural follow-through as shoulders/hips drive the primary animation. Limbs "wobble" slightly behind their parent joints during pose transitions and speed changes.

**Pattern Reuse:** Direct application of proven spring-damper pattern from acceleration tilt and landing spring. Each affected joint gets a spring state (position, velocity) that lags behind the target rotation set by primary animation. Uses same critically-damped spring math from `foundation/spring.h`.

**Graybox Scope:**
- Target joints: elbows (2), knees (2) - 4 springs total
- Spring state per joint: current angle offset (float), velocity (float)
- Two tunable parameters per joint: stiffness, damping
- Apply offset as additional rotation after primary pose applied
- Integrate into existing `update_skeletal_animation()` after pose selection
- Expose parameters in Animation Tuning UI (collapsing header "Secondary Motion")

**Minimal Implementation:** ~40-60 lines
- `character/animation.h`: Add `secondary_motion_state` struct (4 springs: left_elbow, right_elbow, left_knee, right_knee)
- `character/animation.cpp`:
  - `update_secondary_motion()` function (compute spring offsets from parent delta)
  - Call after `apply_pose()`, before transform propagation
  - Inject rotation offsets into joint local transforms
- `gui/character_panel.cpp`: Add secondary motion tuning section
- No new state machine - pure reactive layer on top of existing animation

**Unlocks:**
- Foundation for advanced secondary motion (head bobble, spine flex, cloth simulation)
- Natural motion during walk/run transitions (visible wobble when switching poses)
- Polish layer that makes locomotion feel alive without changing core mechanics
- Validates spring-damper pattern for skeletal animation (could extend to IK softness later)

**Test Loop:**
- Visual coherence: "Do elbows lag naturally behind shoulder swings?"
- Stop test: "Does wobble settle smoothly when movement stops?"
- Transition test: "Visible follow-through during STEP_LEFT → NEUTRAL switches?"
- Parameter sweep: Tune stiffness (0.5-20 Hz) and damping (0.5-2.0) in real-time
- Self-test in minutes, iterate tuning in hours
- Expected iteration: 1-2 days (implementation + feel tuning)

**Certainty:** ~75% (increased from backlog 60%)
- **Why higher:** Prerequisite just validated (primary animation provides parent motion), spring-damper pattern proven twice
- **Risk:** Tuning time to find natural feel (stiffness/damping balance)
- **Architecture risk:** Low - isolated reactive layer, no core logic changes
- **Dependency:** Requires primary skeletal animation working ✅
- **Novelty:** Moderate - applying proven pattern to new domain (skeletal rotation vs body translation)

**Previous Attempt:**
- Deferred 2025-10-06 because static T-pose had no parent motion to lag behind
- **What changed:** Primary skeletal animation now provides distance-phased limb swings (shoulders/hips move, creating lag target for elbows/knees)

**Success Criteria:**
- Visual: Elbows/knees visibly lag behind shoulders/hips during walking
- Feel: Motion looks natural, not floppy or stiff
- Performance: No frame rate impact (<1ms per frame)
- Tuning: Parameters feel intuitive to adjust
- Integration: Works alongside existing reactive systems (tilt, landing spring)

**Excludes (Defer to Future):**
- Spine/head secondary motion (focus on limbs first)
- Per-bone damping ratios (start with global parameters)
- Collision-aware secondary motion (limbs react to world impacts)
- Cloth/appendage physics (wait for evidence of need)

---

**Implementation Notes:**

**Spring Update Pattern (Reuse from landing spring):**
```cpp
// Pseudo-code for elbow lag behind shoulder
float shoulder_angle = get_parent_rotation_y(SHOULDER_LEFT);
float target_elbow = shoulder_angle + pose_elbow_offset; // Primary pose target
float current_elbow = secondary_state.left_elbow_angle;

// Spring toward target
float spring_force = stiffness * (target_elbow - current_elbow);
float damping_force = damping * secondary_state.left_elbow_velocity;
float acceleration = spring_force - damping_force;

// Integrate
secondary_state.left_elbow_velocity += acceleration * dt;
secondary_state.left_elbow_angle += secondary_state.left_elbow_velocity * dt;

// Inject offset into joint transform
joints[ELBOW_LEFT].local_rotation *= quat_from_axis_angle(axis, secondary_state.left_elbow_angle);
```

**Update Order (Insert into existing flow):**
1. Physics update ✅
2. Orientation update ✅
3. Locomotion update (distance_traveled) ✅
4. **Primary animation (apply_pose)** ✅
5. **→ NEW: Secondary motion (spring lag)**
6. Skeleton transform propagation ✅
7. Render ✅

**Tuning UI Structure:**
```
[Animation Tuning]
  [Orientation] ...
  [Locomotion] ...
  [Skeletal Animation] ...
  [Secondary Motion]  ← NEW
    Stiffness: [slider 0.5-20.0 Hz]
    Damping:   [slider 0.5-2.0]
    Current State:
      L Elbow: angle=X.XX, vel=Y.YY
      R Elbow: angle=X.XX, vel=Y.YY
      L Knee:  angle=X.XX, vel=Y.YY
      R Knee:  angle=X.XX, vel=Y.YY
```

**Clarifications Resolved (2025-10-09):**

**Skeleton Hierarchy (from t_pose.cpp analysis):**
- Arms: `spine_upper (2)` → `shoulder (5/8)` → `elbow (6/9)` → `wrist (7/10)`
- Legs: `root (0)` → `hip (11/14)` → `knee (12/15)` → `ankle (13/16)`
- Parent joints: elbows are direct children of shoulders, knees are direct children of hips

**Lag Source:**
Spring toward pose target with natural lag (not parent delta). Primary animation sets target pose via `apply_pose()`, secondary motion adds lag after pose is applied. Each spring tracks difference between current pose target rotation and lagged actual rotation.

**Spring Axes (from keyframe.cpp rotation analysis + character_panel.cpp manual testing):**
- **Elbows: Y-axis** (lags behind shoulder forward/back swing, same axis as parent primary motion)
- **Knees: X-axis** (lags behind hip forward/back swing, same axis as parent primary motion)
- Rationale: Apply lag to same axis as parent's primary swing motion to smooth transitions (GDC: "helps smooth visual transitions")
- Note: Rotations are in parent space; arms point along ±X, legs point along -Y

**Damping Ratio:**
Start critically damped (ratio=1.0) using `critical_damping(stiffness)` pattern from landing spring. Smooth settle without overshoot, tune later if visible wobble needed.

**Parameter Structure:**
Global parameters (one stiffness, one damping for all 4 joints). Matches landing spring precedent, simpler tuning UI (2 sliders vs 8), aligns with GDC description of uniform "slight wobble" effect. Split to per-joint if tuning reveals need.

**Visual Magnitude (from GDC_DesignPhilosophy.md):**
Subtle polish effect. Character's limbs "wobble slightly" during locomotion. Should smooth transitions, not be a primary visual feature. Noticeable during close inspection or extreme transitions (STEP_LEFT ↔ NEUTRAL), not distracting during normal play.

**Expected Timeline:**
- **Phase 1 (2-4 hours):** Add secondary_motion_state, implement spring update for one joint (left elbow)
- **Phase 2 (1-2 hours):** Extend to all 4 joints (elbows, knees), verify no crashes
- **Phase 3 (2-4 hours):** Add tuning UI, expose parameters
- **Phase 4 (4-8 hours):** Feel tuning iteration (find natural stiffness/damping values)
- **Total:** 1-2 days

**Definition of Done:**
- [ ] Spring state struct added to animation_state
- [ ] update_secondary_motion() function implemented
- [ ] Integration into game loop (after apply_pose, before propagation)
- [ ] Tuning UI section added with live state display
- [ ] Visual test: wobble visible during walking
- [ ] Stop test: wobble settles when movement stops
- [ ] Transition test: follow-through visible during pose switches
- [ ] Performance test: <1ms frame time impact
- [ ] Code review: no violations of "do no harm" principle
- [ ] Commit with message following repo conventions
