# Design Backlog

**Purpose:** Unordered, liquid reservoir of ideas not being worked on soon. Most ideas go here until the foundation solidifies enough to support them.

**Status:** Living document (updated continuously)

**Historical Snapshots:** Completed items archived in [ARCHIVE/dependency_stack_snapshot_2025-10-10.md](ARCHIVE/dependency_stack_snapshot_2025-10-10.md). Implementation details in `PLANS/implementation_*.md` and `PLANS/code_review_*.md`.

---

## Animation & Feel

### Debug Visualization

- **Character Axis Gizmo:** Clearly labeled 3D coordinate axes (RGB = XYZ) attached to character root ✅ COMPLETE
  - *Prerequisite:* Foundation ✅
  - *Certainty:* 100%
  - *Learning:* Debug visualization pattern proven - pure reactive layer with zero gameplay coupling. Key technique: ImGui foreground label projection at arrow endpoints using viewport transform (same approach as skeleton joint labels). Safe normalization with fallback prevents degenerate cases. Pattern now validated for future debug tools: toggle-driven, depth-tested wireframe primitives + viewport-projected labels = fast, zero-risk visualization. Font scaling (1.4x) critical for legibility at typical camera distances.
  - *Origin:* Identified 2025-10-08 during Static Keyframe Preview iteration when rotation axis confusion required deep investigation of parent-space transforms
  - *Completed:* 2025-10-12

### Skeletal Animation (Keyframe Foundation)

- **Extended keyframe joint set:** Add spine_upper, left_ankle, right_ankle to keyframe poses (11 joints total)
  - *Prerequisite:* Foundation âœ… (keyframe architecture proven)
  - *Certainty:* Medium (~60%) - defer until 8-joint validation reveals specific need
  - *Rationale:* Torso lean (spine_upper) and grounded foot placement (ankles) may improve visual quality, but 8-joint minimum sufficient for current graybox iteration
  - *Scope:* Add 3 quaternions to keyframe struct; update hardcoded poses with spine/ankle rotations; verify visual improvement justifies added complexity
  - *Origin:* Scoped out of Static Keyframe Preview iteration 1 (2025-10-07) per principle review to minimize graybox scope

- **Running gait keyframes:** Add RUN_STEP_LEFT, RUN_NEUTRAL, RUN_STEP_RIGHT poses to keyframe library
  - *Prerequisite:* Foundation âœ… (quaternion architecture proven)
  - *Certainty:* High (~85%) - direct extension of proven pattern
  - *Rationale:* Walking gait keyframes already validated. Running gait differs in limb extension/timing but uses same quaternion architecture. Needed before implementing speed-based gait switching.
  - *Reference:* See [NOTES/pose_blending_explained.md](../NOTES/pose_blending_explained.md) for full concept breakdown
  - *Scope:*
    - Add 3 new `pose_type` enum values: `RUN_STEP_LEFT`, `RUN_NEUTRAL`, `RUN_STEP_RIGHT`
    - Author 3 hardcoded run poses (greater limb extension, higher arm swing than walk)
    - Extend GUI dropdown to include run poses for manual selection
    - Reuse existing `apply_pose()` logic (no architecture changes)
  - *Success Criteria:* Run poses visually distinct from walk poses (more aggressive motion); instant switching between all 7 poses stable; no visual artifacts
  - *Next Step:* Enables speed-based gait switching (walk â†’ run blending)

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

### Procedural Animation Systems

- **Bounce gravity:** Vertical oscillation of character center of mass during locomotion
  - *Prerequisite:* Running gait keyframes, surveyor wheel locomotion system
  - *Status:* Concept fleshed out - 2025-10-11
  - *Certainty:* Medium (~60%) - concept clear, implementation details TBD
  - *Rationale:* Character behaves like "bouncing ball" (GDC principle). Small upward impulse at footfall + constant gravity creates natural parabolic arcs. Faster gaits = flatter arcs (less airtime), slower gaits = bouncier arcs (more airtime). Pure emergence from physics, zero hand-authored curves.
  - *Reference:* See [NOTES/bounce_gravity_explained.md](../NOTES/bounce_gravity_explained.md) for full concept breakdown
  - *Scope:*
    - Detect footfall events (gait phase crosses 0.0 or 0.5)
    - Apply small upward velocity impulse (~2.0 m/s, tune by feel)
    - Gravity continuously pulls down during airborne portions
    - Redefine "grounded" to include locomoting state (fix pause issue)
    - Layer additively with existing spring-damper (land/crouch)
  - *Open Questions:*
    - Same impulse for walk/run, or per-gait tuning?
    - How to handle locomotion system pausing during `!is_grounded`?
    - Does jump need input buffering to feel responsive mid-bounce?
  - *Success Criteria:* Natural bouncing visible during run; no control interference; smooth interaction with jump/land; feels alive and dynamic (like Overgrowth character)
  - *Origin:* GDC talk reference (David Rosen - Overgrowth), see [NOTES/Images/bounce.png](../NOTES/Images/bounce.png)

- **Air locomotion weights (phase continuity + contact/air):** Prevent frozen poses in air by reweighting locomotion rather than halting it
  - *Prerequisite:* Phase-based gait system; 1D pose slerp blending (see pose blending). Existing landing/secondary motion springs.
  - *Certainty:* Medium-High (~70%) — aligns with GDC “do no harm” and synchronized blending; low risk, small scope.
  - *Rationale:* Current system freezes pose when airborne, causing visual pops and loss of continuity. Keeping phase advancing in air and reweighting stride amplitude preserves coherence and feel without affecting control.
  - *References:*
    - [NOTES/air_locomotion_explained.md](../NOTES/air_locomotion_explained.md)
    - [NOTES/pose_blending_explained.md](../NOTES/pose_blending_explained.md)
    - [NOTES/GDC/GDC_DesignPhilosophy.md](../NOTES/GDC/GDC_DesignPhilosophy.md), [NOTES/GDC/GDC_TechnicalWhitepaper.md](../NOTES/GDC/GDC_TechnicalWhitepaper.md)
  - *Scope:*
    - Keep gait phase advancing while airborne (distance/velocity-based); do not pause.
    - Add spring-smoothed weights (dual-reference): `contact_weight` (1→0) and `air_weight` (0→1) with short time constants; include coyote window.
    - Suppress stride amplitude via `slerp(neutral, gait_blend, contact_weight)`.
    - Optional: Add `AIR_NEUTRAL` keyframe; blend toward it by `air_weight` for a clear in-air silhouette.
    - Scale acceleration tilt by `contact_weight` to avoid extreme lean while falling.
    - Keep footfall bounce impulses ground-only; gravity in air (no “air bounce”).
  - *Open Questions:*
    - Ship without `AIR_NEUTRAL` initially (amplitude suppression only) to minimize content?
    - Parameter defaults for springs and weights; expose in debug UI?
  - *Dependencies:* 1D slerp pose blending (phase) recommended; uses existing secondary motion + landing springs. No input system changes required.
  - *Success Criteria:* No frozen pose in air; smooth takeoff/landing without pops; phase continuity prevents foot sliding; input remains fully responsive; works across walk and future run.
  - *Origin:* Brainstorm 2025-10-12 integrating GDC principles; see added note above.

### Input & Control Feel

- **Mouse delta tracking:** Provide functions to get mouse movement delta between frames.
  - *Certainty:* High
  - *Rationale:* Useful for implementing camera controls and other features that rely on mouse movement.
- **Gamepad support:** Add support for gamepads.
  - *Certainty:* High
  - *Rationale:* Allows for more flexible input options for players.

- **Coyote time + jump buffer:** Elastic timing forgiveness for jumps and micro air gaps
  - *Prerequisite:* Basic jump impulse; grounded detection available from physics controller.
  - *Certainty:* High (~85%) — widely used, low risk, improves feel for broad audience per DG_Skill.
  - *Rationale:* Current “physics-only grounded” can eat jump inputs during bounce or ledge micro-gaps. Short coyote window and input buffering expand accessibility without reducing skill expression.
  - *References:*
    - [NOTES/DesigningGames/DG_Skill.md](../NOTES/DesigningGames/DG_Skill.md)
    - [NOTES/bounce_gravity_explained.md](../NOTES/bounce_gravity_explained.md)
    - [NOTES/GDC/GDC_DesignPhilosophy.md](../NOTES/GDC/GDC_DesignPhilosophy.md)
  - *Scope:*
    - Add `coyote_time_ms` (e.g., 120–150ms) after leaving ground during which jump still executes.
    - Add `jump_buffer_ms` (~150ms) to store recent jump input to trigger on next valid grounded frame.
    - Define `is_grounded_gameplay = is_grounded_physics || within_coyote` for eligibility checks (inputs remain interruptible).
    - Debug UI readouts for timers/flags; tune values by feel.
  - *Open Questions:*
    - Per-surface or per-speed adjustments to coyote time, or keep global?
  - *Dependencies:* Physics grounded flag; jump controller hook. No new assets.
  - *Success Criteria:* Jumps feel reliable during micro air gaps and bounce; no accidental multi-jumps; experts retain timing mastery.
  - *Origin:* Brainstorm 2025-10-12; codifies DG_Skill “elastic challenges” for movement.

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

## Frogs Rule! ðŸ¸

### Frog-Specific Ideas (Wild Speculation)
- **Tongue grapple:** Long-range grab mechanic
- **Hop charge:** Hold jump for bigger leap
- **Sticky surfaces:** Walk on walls/ceilings
- **Water advantage:** Move faster in water than on land
- **Fly catching:** Projectile tongue attacks
- **Inflation:** Puff up for defense or floating
- **Spawn eggs:** Lay eggs that hatch into allies?
- **Croak communication:** Audio signals affect environment
- **Metamorphosis:** Tadpole â†’ frog progression system
- **Lily pad platforms:** Environmental interaction

*Certainty: ~0% - Pure creative exploration. Most will be cut. Kept here for inspiration.*

---

**Notes:**
- Ideas here are deliberately non-interlocking and liquid
- No assumed certainty about implementation
- Pull from backlog only when foundation reaches 90%+ certainty
- Most items will never be implemented (feature, not bug)



