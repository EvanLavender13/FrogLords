# Design Backlog

**Purpose:** Unordered, liquid reservoir of ideas not being worked on soon. Most ideas go here until the foundation solidifies enough to support them.

**Status:** Living document (updated continuously)

**Historical Snapshots:** Completed items archived in [ARCHIVE/dependency_stack_snapshot_2025-10-10.md](ARCHIVE/dependency_stack_snapshot_2025-10-10.md). Implementation details in `PLANS/implementation_*.md` and `PLANS/code_review_*.md`.

---

## Animation & Feel

### Debug Visualization

- **Freeze Velocity Trail on Stop:** Stop trail sampling when character velocity drops below threshold; resume when movement resumes
  - *Prerequisite:* Debug Visual Overhaul ✅ (velocity trail system exists)
  - *Certainty:* High (~95%) - trivial implementation, threshold may need tuning iteration
  - *Problem:* Trail continuously updates regardless of movement, causing spheres to accumulate on stationary character. Visually confusing—trail should show "where character has been during motion," not "time passing while stationary."
  - *Hypothesis:* Frozen trail improves visual clarity; acts as "motion breadcrumb" showing last path taken. Better understanding of past trajectory during playtesting.
  - *Rationale:* Aligns with "reactive systems interpret state" principle. Trail should respond to velocity (motion source), not just time. Improves debug tool usefulness for analyzing movement patterns.
  - *Scope:*
    - Add velocity magnitude check before trail sampling ([game_world.cpp:29-45](src/app/game_world.cpp#L29-L45))
    - Only sample when `glm::length(character.velocity) > velocity_threshold`
    - Threshold likely ~0.05-0.1 m/s (tune to feel—standing still vs. very slow walk)
    - Optional: Add threshold parameter to `velocity_trail_state` for runtime tuning
  - *Implementation:* ~5 lines of code (single conditional check). Zero architectural changes.
  - *Success Criteria:*
    - Trail stops updating when character stands still
    - Trail resumes immediately when movement starts
    - No visual artifacts (positions remain stable while frozen)
    - Threshold feels natural (doesn't freeze during slow intentional movement)
  - *Risk:* Low. Debug-only feature; worst case revert takes 30 seconds.
  - *Origin:* User request 2025-10-11; improves existing trail system from Debug Visual Overhaul iteration

- **Character Axis Gizmo:** Clearly labeled 3D coordinate axes (RGB = XYZ) attached to character root
  - *Prerequisite:* Debug Draw System ✅
  - *Certainty:* High (~90%) - simple debug visualization, no gameplay impact
  - *Rationale:* Eliminates confusion about parent-space vs bone-local rotations when tuning joint angles. Visible reference frame makes it immediately obvious which axis does what (e.g., "Y-axis points up, so rotation around Y is horizontal swing"). Critical for manual pose authoring and validating rotation behavior.
  - *Scope:* Draw 3 colored lines from character root: Red=+X (right), Green=+Y (up), Blue=+Z (forward). Add optional text labels "X", "Y", "Z" at line endpoints. Toggle via GUI checkbox (default: off).
  - *Implementation:* Use existing `debug_draw::line()` API; attach to skeleton root joint global position; scale lines to ~0.3-0.5m length (visible but not obtrusive).
  - *Success Criteria:* Axes visible in all camera angles; colors/labels clearly readable; rotation behavior immediately understandable when watching joint angle sliders move character
  - *Origin:* Identified 2025-10-08 during Static Keyframe Preview iteration when rotation axis confusion required deep investigation of parent-space transforms

### Skeletal Animation (Keyframe Foundation)

- **Extended keyframe joint set:** Add spine_upper, left_ankle, right_ankle to keyframe poses (11 joints total)
  - *Prerequisite:* Foundation ✅ (keyframe architecture proven)
  - *Certainty:* Medium (~60%) - defer until 8-joint validation reveals specific need
  - *Rationale:* Torso lean (spine_upper) and grounded foot placement (ankles) may improve visual quality, but 8-joint minimum sufficient for current graybox iteration
  - *Scope:* Add 3 quaternions to keyframe struct; update hardcoded poses with spine/ankle rotations; verify visual improvement justifies added complexity
  - *Origin:* Scoped out of Static Keyframe Preview iteration 1 (2025-10-07) per principle review to minimize graybox scope

- **Running gait keyframes:** Add RUN_STEP_LEFT, RUN_NEUTRAL, RUN_STEP_RIGHT poses to keyframe library
  - *Prerequisite:* Foundation ✅ (quaternion architecture proven)
  - *Certainty:* High (~85%) - direct extension of proven pattern
  - *Rationale:* Walking gait keyframes already validated. Running gait differs in limb extension/timing but uses same quaternion architecture. Needed before implementing speed-based gait switching.
  - *Scope:*
    - Add 3 new `pose_type` enum values: `RUN_STEP_LEFT`, `RUN_NEUTRAL`, `RUN_STEP_RIGHT`
    - Author 3 hardcoded run poses (greater limb extension, higher arm swing than walk)
    - Extend GUI dropdown to include run poses for manual selection
    - Reuse existing `apply_pose()` logic (no architecture changes)
  - *Success Criteria:* Run poses visually distinct from walk poses (more aggressive motion); instant switching between all 7 poses stable; no visual artifacts
  - *Next Step:* Enables speed-based gait switching (walk → run blending)

- **Pose blending (lerp/slerp):** Smooth transitions between discrete walk poses ✅ COMPLETE
  - *Prerequisite:* Foundation ✅ (distance-phased triggering proven)
  - *Certainty:* 100%
  - *Learning:* Quaternion slerp successfully eliminates pops at phase boundaries (0.25, 0.5, 0.75). Hemisphere correction critical for shortest-path interpolation (dot product check before slerp). GLM slerp function industry-standard and reliable. Secondary motion springs shifted from "compensating for discontinuities" to "natural follow-through." Architecture validated for 2D blend spaces (walk↔run speed blending) and custom easing curves (non-linear phase mapping). Implementation stayed within scope (46 lines vs 40-60 estimate). Zero architectural changes required—quaternion foundation was architecturally sufficient. See [iteration_pose_blending.md](iteration_pose_blending.md) and [implementation_pose_blending.md](implementation_pose_blending.md) for details.

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
