# Design Backlog

**Purpose:** Unordered, liquid reservoir of ideas not being worked on soon. Most ideas go here until the foundation solidifies enough to support them.

**Status:** Living document (updated continuously)

**Historical Snapshots:** Completed items archived in [ARCHIVE/dependency_stack_snapshot_2025-10-10.md](ARCHIVE/dependency_stack_snapshot_2025-10-10.md). Implementation details in `PLANS/implementation_*.md` and `PLANS/code_review_*.md`.

---

## Item Fields (recommended)

Include lightweight metadata to improve selection and planning:
- **Complexity (points):** 1–2 simple, 3–5 medium, 6–8 complex. If >8, consider splitting.
- **Intended validation loop:** How you’ll verify success quickly (e.g., debug panel toggle, smoke test path).

## Triggers to Update
- After playtests or reviews surface new ideas, constraints, or patterns worth capturing.
- When an item is pulled into active work (move/link it to the dependency stack; mark prerequisites and certainty).
- When an item is deferred or deprecated (tag status, rationale, and any blockers discovered).
- After changes to the dependency stack that affect prerequisites or certainty tags.
- When a small “knowledge move” creates a new avenue (capture while it’s fresh; keep items non-interlocking).
- After building tools that shorten test loops (reassess priorities/certainty for related items).
- When items start to interlock or grow in scope (split into independent, pullable slices).

## Animation & Feel

### Skeletal Animation (Keyframe Foundation)


- **Extended keyframe joint set:** Add spine_upper, left_ankle, right_ankle to keyframe poses (11 joints total)
  - *Prerequisite:* Foundation âœ… (keyframe architecture proven)
  - *Certainty:* Medium (~60%) - defer until 8-joint validation reveals specific need
  - *Rationale:* Torso lean (spine_upper) and grounded foot placement (ankles) may improve visual quality, but 8-joint minimum sufficient for current graybox iteration
  - *Scope:* Add 3 quaternions to keyframe struct; update hardcoded poses with spine/ankle rotations; verify visual improvement justifies added complexity
  - *Origin:* Scoped out of Static Keyframe Preview iteration 1 (2025-10-07) per principle review to minimize graybox scope

- **Skeleton rest-pose reset:** Rehydrate local transforms from reference pose when the debug animation toggle turns off.
  - *Rationale:* Prevents accumulated offsets from leaving the elbow in a rotated state after probes.
  - *Certainty:* Medium (~60%) - likely a small helper that copies defaults stored alongside the T-pose.

- **Skeleton validation hooks:** Optional per-joint debug asserts/visual warnings when parent indices are invalid or hierarchy order breaks.
  - *Rationale:* Completes the "no cycles" guard promised in plan; keeps future authoring honest.
  - *Certainty:* Medium (~50%) - cheap runtime check toggled via debug flag.


- **Parameter persistence:** Save/load tuned values between sessions
  - *Current:* Manual adjustment at runtime, no persistence
  - *Prerequisite:* Determine if iteration workflow needs it
  - *Certainty:* Medium (~50%) - useful but not urgent
  - *Note:* Would require config file system (JSON/TOML)

  - *Completed:* 2025-10-13 (feature branch: feature/air_locomotion_weights)
  - *Implementation:* See [PLANS/air_locomotion_weights_PLAN.md](air_locomotion_weights_PLAN.md)

- **Secondary motion joint limits (elbow/knee constraints):** **DEFERRED (2025-10-15)** - Enforce anatomical bend limits to prevent unnatural hyperextension
  - *Status:* **DEFERRED** - Missing prerequisite understanding of skeleton local coordinate frames
  - *Prerequisite:* Skeleton debugging/visualization tools; transform hierarchy mastery
  - *Certainty:* Low (~30%) - multiple implementation attempts failed due to quaternion/hierarchy complexity
  - *Complexity:* Was 1-2 points; revealed to be 5-8 points (needs skeleton tooling first)
  - *Problem:* Secondary motion spring offsets apply without anatomical limits, causing hyperextension. Attempted fixes revealed fundamental gap in understanding bone local axes - left/right joints not symmetric as expected.
  - *Deferral Reason:* Small polish effect not worth debugging complex transform hierarchy issues. Multiple attempts (hard clamps, soft limits, axis flipping, absolute angle measurement) all failed due to asymmetric behavior between mirrored joints. Root cause: incomplete understanding of skeleton local coordinate frames.
  - *Reconsideration Criteria:* After building skeleton visualization tools, implementing IK (forces solving same problems), or when ROI justifies investment
  - *Key Learning:* Quaternion keyframe blending works treating skeleton as black box; per-bone axis-dependent rotations expose our lack of transform hierarchy mastery
  - *Review:* See [PLANS/secondary_motion_joint_limits_FEATURE.md](secondary_motion_joint_limits_FEATURE.md) deferral section
  - *Origin:* Identified 2025-10-13; attempted 2025-10-15; deferred 2025-10-15 after multiple failed approaches

### Input & Control Feel

- **Mouse delta tracking:** Provide functions to get mouse movement delta between frames.
  - *Certainty:* High
  - *Rationale:* Useful for implementing camera controls and other features that rely on mouse movement.
- **Gamepad support:** Add support for gamepads.
  - *Certainty:* High
  - *Rationale:* Allows for more flexible input options for players.



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

- **Debug panel layout improvements:** Reduce visual clutter, hide "settled" parameters
  - *Problem:* As features accumulate, character panel becomes dense with spring frequencies, damping ratios, and other tuned-once parameters that rarely change
  - *Potential solutions:*
    - Collapsible "Advanced" subsections within each header (e.g., "Landing Spring" shows position/velocity, hides stiffness/damping behind "Advanced" toggle)
    - "Lock" icon next to settled parameters to visually indicate "don't touch unless re-tuning"
    - Separate "Read-Only State" vs "Tunable Parameters" tabs
    - Auto-collapse sections that haven't been modified recently
  - *Validation:* Can tune new features without scrolling past 10+ irrelevant sliders
  - *Complexity:* 2-3 points (ImGui layout exploration, state persistence for collapsed sections)
  - *Certainty:* Low (~40%) - UX problem exists, but solution unclear without experimentation
  - *Priority:* Medium - quality-of-life improvement that becomes more valuable as system count grows
  - *Origin:* Identified 2025-10-13 during air locomotion weights planning (concern about adding another spring frequency slider)

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


