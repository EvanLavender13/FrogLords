# Design Backlog

**Purpose:** Unordered, liquid reservoir of ideas not being worked on soon. Most ideas go here until the foundation solidifies enough to support them.

**Status:** Living document (updated continuously)

---

## Animation & Feel

### Reactive Animation (Architecture Proven ✅)
- **Acceleration tilt:** ✅ COMPLETE - Character leans into acceleration (Segway effect)
  - *Status:* Implemented, tested, tuned, UI exposed
  - *Files:* `src/character/animation.h/cpp`
  - *Learning:* Character-local space transformation critical, exponential smoothing sufficient
  
- **Spring-damper landings:** ✅ COMPLETE - Crouch/recover on landing driven by spring-damper
  - *Status:* Implemented, tested, tuned, UI exposed
  - *Files:* `src/character/animation.h/cpp`, detailed in `landing_spring_implementation.md`
  - *Learning:* Flag-based event communication works well, spring visualization critical for tuning, collision-sphere-to-body endpoint prevents stretching during jumps

- **Animation tuning UI:** ✅ COMPLETE - Real-time parameter adjustment for all animation systems
  - *Status:* Implemented across acceleration tilt, landing spring, orientation, locomotion
  - *Files:* `src/gui/character_panel.h/cpp`, detailed in `animation_tuning_plan.md`
  - *Learning:* Public member access sufficient for tuning (no getters needed), consistent UX pattern (collapsing headers + live feedback) scales well, scope expanded during implementation (orientation/locomotion smoothing added), walk/run thresholds deliberately excluded (procedurally derived from max_speed)
  
- **Attach skeleton to character body:** Attach the T-pose skeleton's root to the character controller's rigid body.
  - *Prerequisite:* Skeleton Debug System ✅
  - *Certainty:* High (~95%) - A simple transform update; moving the skeleton's root position to the character's position each frame.
  - *Rationale:* The skeleton is currently static. This is the necessary next step to connect the skeleton to the player's actions and unblock any further skeletal animation work (like secondary motion or IK).

- **Secondary motion:** Bone "softness" parameters for wobble, follow-through on limbs
  - *Prerequisite:* Skeletal system with joints ✅ (T-pose debug skeleton now available)
  - *Certainty:* Low (~30%) - rendering approach unknown
  
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

- **Game World Refactor:** ✅ COMPLETE - Extracted simulation state into `game_world` struct.
  - *Status:* Implemented, tested, verified.
  - *Files:* `src/app/game_world.h/cpp`, `src/app/runtime.h/cpp`
  - *Learning:* Separating the application lifecycle from the game simulation logic greatly improves clarity and testability. It makes it easier to swap out levels or test different scenarios in the future.

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
- **Zoom control:** Dynamic FOV or distance

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
- **Real-time parameter tuning:** ✅ COMPLETE - GUI sliders for animation/orientation/locomotion parameters
  - *Status:* Comprehensive tuning UI implemented
  - *Exposed:* Acceleration tilt (smoothing, magnitude), landing spring (stiffness, damping, impulse_scale), orientation (yaw_smoothing), locomotion (speed_smoothing)
  - *Pattern:* Collapsing headers with real-time state feedback displays
  - *Files:* `src/gui/character_panel.h/cpp`, see `animation_tuning_plan.md`
  
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
