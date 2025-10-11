# Design Backlog

**Purpose:** Unordered, liquid reservoir of ideas not being worked on soon. Most ideas go here until the foundation solidifies enough to support them.

**Status:** Living document (updated continuously)

---

## Animation & Feel

### Debug Visualization

- **Debug Visual Overhaul:** Add focused debug visualizations (velocity trail, temporal plots, speed gradient ring) ✅ **COMPLETE**
  - *Prerequisite:* Debug Draw System ✅, GUI System ✅
  - *Certainty:* 100%
  - *Learning:* Three debug visualizations validated with strong systemic reusability. Key findings:
    - Velocity trail: 0.1s sampling (changed from 1.0s) provides readable turning radius; 25 samples = 2.5s history sufficient
    - Speed gradient ring: Dynamic expansion (current_speed) more intuitive than fixed radius (max_speed); blue→green→yellow→red gradient clear
    - Plot functions: Generic `plot_value()`/`plot_histogram()` unlocked temporal debugging (FPS, speed, blend factors); axis labels critical for readability
    - Buffer management: Simple vector erase (<500 samples) sufficient for debug use; no need for circular buffer complexity
    - State ownership pattern: `velocity_trail_state` in `game_world` (app layer), visualization in `debug_draw` (rendering layer) prevents circular dependencies
    - Graybox discipline: White spheres, gradient colors, ImGui defaults (no premature polish)
  - *Next Step:* Plot functions ready for reuse (speed graphs, blend factors); velocity trail pattern applicable to trajectory prediction; gradient ring pattern for cooldown/range indicators
  - *Completion Date:* October 10, 2025
  - *Implementation:* See `PLANS/implementation_debug_visual_overhaul.md` and `PLANS/code_review_debug_visual_overhaul.md`

- **Character Axis Gizmo:** Clearly labeled 3D coordinate axes (RGB = XYZ) attached to character root
  - *Prerequisite:* Debug Draw System ✅
  - *Certainty:* High (~90%) - simple debug visualization, no gameplay impact
  - *Rationale:* Eliminates confusion about parent-space vs bone-local rotations when tuning joint angles. Visible reference frame makes it immediately obvious which axis does what (e.g., "Y-axis points up, so rotation around Y is horizontal swing"). Critical for manual pose authoring and validating rotation behavior.
  - *Scope:* Draw 3 colored lines from character root: Red=+X (right), Green=+Y (up), Blue=+Z (forward). Add optional text labels "X", "Y", "Z" at line endpoints. Toggle via GUI checkbox (default: off).
  - *Implementation:* Use existing `debug_draw::line()` API; attach to skeleton root joint global position; scale lines to ~0.3-0.5m length (visible but not obtrusive).
  - *Success Criteria:* Axes visible in all camera angles; colors/labels clearly readable; rotation behavior immediately understandable when watching joint angle sliders move character
  - *Origin:* Identified 2025-10-08 during Static Keyframe Preview iteration when rotation axis confusion required deep investigation of parent-space transforms

### Skeletal Animation (Keyframe Foundation)

- **Static Keyframe Preview:** Manual GUI selection between hardcoded skeletal poses (validates quaternion-based keyframes before adding locomotion) ✅ **COMPLETE**
  - *Prerequisite:* Skeleton Debug System ✅, Attach Skeleton to Body ✅
  - *Certainty:* 100%
  - *Learning:* Quaternion keyframe architecture fully validated. Key findings:
    - Quaternion composition formula works: `local_transform = translate(t_pose_pos) * mat4_cast(pose_quat)`
    - 8-joint minimum set (shoulders, elbows, hips, knees) sufficient for walk validation
    - Manual pose selection proves data structure before adding locomotion complexity
    - T-pose baseline uses identity rotations (translation-only), simplifying composition
    - Instant pose switching stable with no visual artifacts or crashes
    - Clean separation of keyframe data from application logic enables future extensions
    - UPPER_CASE enum convention established and enforced via clang-tidy
  - *Next Step:* Primary skeletal animation (locomotion-driven) now unblocked with proven quaternion foundation
  - *Completion Date:* October 8, 2025
  - *Implementation:* See `PLANS/implementation_static_keyframe_preview.md` and `PLANS/code_review_static_keyframe_preview.md`

- **Extended keyframe joint set:** Add spine_upper, left_ankle, right_ankle to keyframe poses (11 joints total)
  - *Prerequisite:* Static Keyframe Preview validated (8-joint minimum working)
  - *Certainty:* Medium (~60%) - defer until 8-joint validation reveals specific need
  - *Rationale:* Torso lean (spine_upper) and grounded foot placement (ankles) may improve visual quality, but 8-joint minimum sufficient for validating quaternion architecture
  - *Status:* Now unblocked - Static Keyframe Preview validated 8-joint minimum successfully
  - *Scope:* Add 3 quaternions to keyframe struct; update hardcoded poses with spine/ankle rotations; verify visual improvement justifies added complexity
  - *Origin:* Scoped out of Static Keyframe Preview iteration 1 (2025-10-07) per principle review to minimize graybox scope

- **Running gait keyframes:** Add RUN_STEP_LEFT, RUN_NEUTRAL, RUN_STEP_RIGHT poses to keyframe library
  - *Prerequisite:* Static Keyframe Preview ✅ (quaternion architecture proven)
  - *Certainty:* High (~85%) - direct extension of proven pattern
  - *Rationale:* Walking gait keyframes validated in Static Keyframe Preview. Running gait differs in limb extension/timing but uses same quaternion architecture. Extends pose library before locomotion-driven animation integration.
  - *Scope:*
    - Add 3 new `pose_type` enum values: `RUN_STEP_LEFT`, `RUN_NEUTRAL`, `RUN_STEP_RIGHT`
    - Author 3 hardcoded run poses (greater limb extension, higher arm swing than walk)
    - Extend GUI dropdown to include run poses for manual selection
    - Reuse existing `apply_pose()` logic (no architecture changes)
  - *Success Criteria:* Run poses visually distinct from walk poses (more aggressive motion); instant switching between all 7 poses stable; no visual artifacts
  - *Excludes:* Locomotion-driven triggering (still manual selection); speed-based walk/run blending (deferred to primary skeletal animation)
  - *Next Step:* Provides complete pose library (walk + run) for primary skeletal animation to blend between
  - *Origin:* Scoped out of Static Keyframe Preview iteration 1 (2025-10-07) to minimize initial validation scope

- **Primary skeletal animation (locomotion-driven):** Distance-phased limb animation synchronized to movement (walk/run arm swing cycles) ✅ **COMPLETE**
  - *Prerequisite:* Static Keyframe Preview ✅
  - *Certainty:* 100%
  - *Learning:* Distance-phased pose switching validated with threshold-based selection. Key findings:
    - Surveyor-wheel pattern extends naturally to skeletal animation (`phase = fmod(distance_traveled, cycle_length) / cycle_length`)
    - Threshold-based pose selection (0.25/0.5/0.75 splits) sufficient for graybox validation before adding interpolation
    - Cumulative distance tracking provides stable, speed-independent cycling
    - Stop behavior correct by design (pose freezes when distance stops, no special handling needed)
    - Manual override parameter preserves debug UI functionality alongside automatic animation
    - Walk speed lock (SHIFT key) critical for precise tuning observation
    - Distance accumulation fix critical: changed from `distance = phase * stride` (wrapped) to `distance += speed * dt` (cumulative)
  - *Next Step:* Pose blending (lerp/slerp) now unblocked to smooth transitions between discrete poses
  - *Completion Date:* October 9, 2025
  - *Implementation:* See [implementation_primary_skeletal_animation.md](PLANS/implementation_primary_skeletal_animation.md) and [code_review_primary_skeletal_animation.md](PLANS/code_review_primary_skeletal_animation.md)

- **Secondary motion (skeletal follow-through):** Per-bone spring-damper lag creating natural limb wobble during pose transitions ✅ **COMPLETE**
  - *Prerequisite:* Primary Skeletal Animation ✅
  - *Certainty:* 100%
  - *Learning:* Spring-damper lag successfully scaled from root transform to individual skeletal joints. Key findings:
    - Velocity-injection approach (discovered during iteration) superior to direct offset manipulation for preventing overshoot artifacts
    - 4 spring states (left_elbow, right_elbow, left_knee, right_knee) sufficient for natural follow-through
    - Final parameters: stiffness=15.0 Hz, damping_ratio=1.0, response_scale=0.02
    - Axis-specific lag (elbows Y-axis, knees X-axis) provides natural swing motion
    - Pure reactive layer maintains "do no harm" principle (zero gameplay impact)
    - Pattern proven for expansion to additional joints (head, spine) or cloth/appendage physics
  - *Next Step:* Secondary motion architecture validated; ready for advanced skeletal reactivity (head bobble, spine flex) when needed
  - *Completion Date:* October 9, 2025
  - *Implementation:* See [implementation_secondary_motion.md](PLANS/implementation_secondary_motion.md) and [code_review_secondary_motion.md](PLANS/code_review_secondary_motion.md)
  
- **Speed-based animation scaling:** Tilt magnitude/bounce height scale with velocity (like surveyor wheel physics)
  - *Prerequisite:* Acceleration tilt working ✅
  - *Status:* **DEFERRED** - Investigated 2025-10-06, current constant magnitude feels good
  - *Certainty:* Low (~30%) - no evidence of problem; premature optimization
  - *Note:* Needs test-first evidence before implementation. Document specific gameplay moments where constant magnitude fails (e.g., "tilt too subtle at max speed"). Current system works; don't fix what isn't broken.
  - *Review:* See `PLANS/plan_review_speed_animation_scaling.md` for detailed analysis

- **Skeleton rest-pose reset:** Rehydrate local transforms from reference pose when the debug animation toggle turns off.
  - *Rationale:* Prevents accumulated offsets from leaving the elbow in a rotated state after probes.
  - *Certainty:* Medium (~60%) - likely a small helper that copies defaults stored alongside the T-pose.

- **Skeleton validation hooks:** Optional per-joint debug asserts/visual warnings when parent indices are invalid or hierarchy order breaks.
  - *Rationale:* Completes the "no cycles" guard promised in plan; keeps future authoring honest.
  - *Certainty:* Medium (~50%) - cheap runtime check toggled via debug flag.

- **Tunable tilt velocity scaling:** Expose hardcoded velocity scaling constants (0.5x-1.5x)
  - *Current:* Velocity scaling automatically applies (faster = more tilt)
  - *Prerequisite:* Test current feel first (may be unnecessary)
  - *Certainty:* Low (~30%) - premature optimization until feel requires it
  - *Note:* Added to backlog from animation_tuning_plan learnings

- **Independent walk/run thresholds:** Decouple from max_speed auto-sync
  - *Current:* Thresholds procedurally derived (walk = 20% max_speed, run = 100%)
  - *Risk:* Breaks elegant abstraction, adds parameter complexity
  - *Certainty:* Very Low (~10%) - only if procedural approach fails
  - *Note:* Deliberately excluded from tuning UI to preserve simplicity

- **Parameter persistence:** Save/load tuned values between sessions
  - *Current:* Manual adjustment at runtime, no persistence
  - *Prerequisite:* Determine if iteration workflow needs it
  - *Certainty:* Medium (~50%) - useful but not urgent
  - *Note:* Would require config file system (JSON/TOML)

### Input & Control Feel

- **Walk Speed Lock Refactor:** Investigate acceleration-based walk speed limiting instead of hard velocity cap
  - *Prerequisite:* Primary Skeletal Animation validated ✅
  - *Certainty:* Low (~30%) - current implementation works for tuning/debug purposes
  - *Current Implementation:* SHIFT key hard-caps velocity at 2.0 m/s (controller.cpp:92-99), overriding momentum
  - *Rationale:* Current approach violates "input maps to acceleration" principle (AGENTS.md Procedural Animation). Hard velocity cap creates brief overshoot before clamping, breaking physics-first feel. Production version should adjust acceleration or max_speed target instead.
  - *Scope:*
    - Option A: Modulate `max_speed` target directly (changes cap without velocity clamp)
    - Option B: Scale `ground_accel` to limit acceleration into walk speeds (preserves momentum decay)
    - Option C: Keep hard cap but add hysteresis/smoothing to prevent visual jitter
  - *Success Criteria:* SHIFT key limits speed without breaking momentum/physics feel; no visible velocity overshoot; transitions smooth
  - *Risk:* May feel less responsive than current implementation; tuning required
  - *Note:* Current hard cap approach is acceptable for debug/tuning feature (quick addition during Primary Skeletal Animation iteration 2025-10-09). Only refactor if feature graduates to production or feel issues emerge during playtesting.
  - *Origin:* Identified in code review 2025-10-09 as misalignment with gameplay-first principles

### Advanced Animation (Low Priority)
- **Wall slide/run detection:** "Solving for stupid" - when face-first into wall, transition to wall run
- **Active ragdoll:** Spring-driven pose matching for dynamic reactions
- **Contextual poses:** Different arm positions for climbing, hanging, bracing
- **Flip/roll animations:** Manufactured momentum with anticipation curves
- **IK for interactions:** Hands reach for ledges, feet step on obstacles

## Movement & Physics

### Character Abilities (Medium Priority)
- **Dash/dodge:** Short burst of velocity in input direction
- **Wall jump:** Jump off walls when grounded against them
- **Ledge grab:** Detect ledge collision, IK hands to edge
- **Climbing:** Vertical movement on climbable surfaces
- **Swimming:** Buoyancy, drag, different movement rules
- **Crouching:** Lower collision height, slower movement

### Physics Enhancements (Low Priority)
- **Velocity inheritance:** Jumping off moving platform adds platform velocity
- **Slope physics:** Slide down steep slopes, gain speed
- **Wind/current forces:** Environmental forces affecting movement
- **Momentum preservation:** Bunny hopping, air strafing
- **Dual-sphere experiment:** Restore bumper + weightlifter system (currently single sphere)

## Combat & Interaction

### Combat Mechanics (Uncertain - Defer)
- **Melee attacks:** Punch, kick, grapple
- **Ranged attacks:** Throw objects, projectiles
- **Enemy AI:** Patrol, chase, attack patterns
- **Damage/health:** Hit points, death state
- **Impact reactions:** Knockback, stagger, ragdoll on death

### Interaction Systems (Uncertain - Defer)
- **Object pickup:** Grab and carry physics objects
- **Throwing:** Apply velocity to held objects
- **Doors/buttons:** Simple triggered interactions
- **Inventory:** Hold multiple items

## World & Environment

### Terrain (Low-Medium Priority)
- **Heightmap terrain:** Procedural ground with elevation
- **Uneven surfaces:** IK foot placement becomes necessary here
- **Moving platforms:** Elevators, rotating platforms
- **Destructible objects:** Break on impact
- **Water volumes:** Swimming areas

### Level Elements (Medium Priority)
- **Ramps and stairs:** Traversal structures
- **Obstacles:** Walls, barriers, holes
- **Jump puzzles:** Platforms requiring precise movement
- **Collectibles:** Items to gather (if game needs them)

### World Building (Low Priority - Content Driven)
- **Art style:** Currently graybox/wireframe (intentional for iteration)
- **Lighting:** Basic directional light exists, could enhance
- **Skybox:** Environment backdrop
- **Props:** Visual detail objects (no mechanical meaning = noise)

## Camera & Controls

### Camera Enhancements (Low Priority)
- **Mouse Wheel Camera Zoom:** ✅ **COMPLETE**
  - *Prerequisite:* `camera` ✅, `input` ✅
  - *Certainty:* 100%
  - *Learning:* Mouse wheel zoom successfully implemented for both ORBIT and FOLLOW modes with tunable distance limits. Key findings:
    - One-frame delay bug in FOLLOW mode required immediate eye position recalculation in `zoom()` method (pattern from ORBIT mode's `update_eye_position()` call)
    - Spherical camera orbit around `follow_height_offset` (1.5m above character) creates intentional "shift" effect during zoom - not a bug, but geometric behavior
    - Debug UI (camera panel) proved immediately valuable for tuning height offset (0-3m) and zoom limits (1.5-15m default)
    - GUI refactoring to unified Debug Panel with collapsing headers improves maintainability and sets pattern for future debug panels
    - Default range (1.5-15m) appropriate for skeletal animation inspection; very close zoom (<2m) reveals graybox mesh gaps (expected, not issue)
  - *Scope Growth:* Original estimate 15-30 lines, actual ~60 lines across 8 files. Growth justified: bug fix necessary, debug UI high ROI (saves 10+ min per future panel), GUI refactoring improves architecture
  - *Next Step:* Camera debug panel architecture proven; pattern reusable for physics panel, audio panel, profiling panel
  - *Completion Date:* October 10, 2025
  - *Implementation:* See `PLANS/feature_camera_zoom.md` for detailed retrospective (includes bug fix, GUI refactoring notes)
- **Camera collision:** Prevent clipping through walls
- **Smart framing:** Keep character and target both visible
- **Camera shake:** Impact feedback
- **Follow smoothing:** Lag/lead based on velocity

### Input Refinements (Low Priority)
- **Gamepad support:** Already have sokol support, just wire it
- **Input buffering:** Queue commands during animations
- **Sensitivity curves:** Configurable response curves
- **Rebindable keys:** Let player remap controls

## Audio & Feedback

### Audio System (Uncertain - Defer)
- **Footstep sounds:** Distance-phased triggering (matches surveyor wheel)
- **Impact sounds:** Landing, collision, attack
- **Ambient audio:** Environmental loops
- **Music system:** Dynamic music based on state
- **3D audio:** Spatial sound positioning

### Feedback (Medium Priority)
- **Screen effects:** Chromatic aberration on impact, vignette
- **Particle systems:** Dust on landing, trail on dash
- **Screen shake:** Camera impulse on impacts
- **Time effects:** Slow motion, freeze frames

## UI & Menus

### Debug UI (Partially Complete)
- **Debug visualization:** Draw acceleration vectors, tilt angles, velocity
  - *Status:* Tilt visualization complete via character body box
  - *Potential additions:* Acceleration vector arrows, velocity trails, ground normal visualization
  - *Certainty:* High (~90%) - straightforward debug draw additions
  - *Priority:* Low - current visualization sufficient for tuning
  
- **Performance stats:** Frame time, draw calls, memory usage
  - *Status:* FPS display exists in character panel
  - *Potential additions:* Frame time graph, draw call count, memory profiling
  - *Priority:* Low - premature optimization
  
- **State display:** Grounded, velocity magnitude, collision contacts
  - *Status:* Partial - some state visible in locomotion/orientation sections
  - *Potential additions:* Physics state panel, collision debug info
  - *Priority:* Medium - useful for debugging physics edge cases

### Game UI (Uncertain - Defer)
- **Main menu:** Start, options, quit
- **Pause menu:** Resume, restart, quit
- **HUD elements:** Health, stamina, whatever game needs
- **Settings menu:** Graphics, audio, controls

## Content & Polish

### Visual Polish (Low Priority - Premature)
- **Character mesh:** Replace sphere with visible body
  - *Note:* Currently sphere is intentional (graybox iteration)
  - *When:* After core feel proven through playtesting
  - *Risk:* Art hides mechanical problems
- **Texture/materials:** PBR materials, surface detail
- **Post-processing:** Bloom, color grading, anti-aliasing
- **Animation polish:** Secondary motion, anticipation, follow-through

### Game Loop (Uncertain - Core Unclear)
- **Win/lose conditions:** What is player trying to do?
- **Objectives:** What are the goals?
- **Progression:** How does game evolve?
- **Scoring:** How is player performance measured?
- **Replayability:** What brings player back?

*Note: These are therapeutic planning - written to reduce anxiety, not coordinate work. Actual game design will emerge through iteration and serendipity capture.*

## Frogs Rule! 🐸

### Frog-Specific Ideas (Wild Speculation)
- **Tongue grapple:** Long-range grab mechanic
- **Hop charge:** Hold jump for bigger leap
- **Sticky surfaces:** Walk on walls/ceilings
- **Water advantage:** Move faster in water than on land
- **Fly catching:** Projectile tongue attacks
- **Inflation:** Puff up for defense or floating
- **Spawn eggs:** Lay eggs that hatch into allies?
- **Croak communication:** Audio signals affect environment
- **Metamorphosis:** Tadpole → frog progression system
- **Lily pad platforms:** Environmental interaction

*Certainty: ~0% - Pure creative exploration. Most will be cut. Kept here for inspiration.*

---

**Notes:**
- Ideas here are deliberately non-interlocking and liquid
- No assumed certainty about implementation
- Pull from backlog only when foundation reaches 90%+ certainty
- Most items will never be implemented (feature, not bug)
