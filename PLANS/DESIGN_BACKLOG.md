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

### Skeletal Animation System (High Priority - Ground-Up Rebuild)

**Context:** Previous skeletal system removed (2025-10-15) due to fundamentally flawed behavioral assumptions discovered during quaternion math validation. See `NOTES/Math/QuaternionDecomp.md` for detailed analysis.

**Critical Learnings from Failed Implementation:**
- Quaternion decomposition requires **stable local coordinate frames** (must validate transform hierarchy first)
- Per-bone axis-dependent operations expose lack of skeleton tooling/visualization
- Cannot treat skeleton as black box when implementing joint limits or secondary motion
- Swing-twist decomposition is mathematically sound but requires prerequisite understanding
- Must validate novel data representations (quaternions, swing-twist) in **isolation** before integration

**Fresh Start Approach (Bottom-Up):**

1. **Skeleton Visualization & Debugging Tools** (Complexity: 5-6 points)
   - *Prerequisite:* None - foundation layer
   - *Certainty:* High (~70%) - established debug draw patterns exist
   - *Scope:* Debug draw for bones (lines), joints (spheres), local axes (RGB gizmos), bind pose vs. animated pose comparison
   - *Rationale:* Cannot debug what you cannot see; essential for validating all future skeletal work
   - *Validation:* Visual confirmation of T-pose, hierarchy traversal, local vs. world transforms

2. **Minimal Skeletal Hierarchy & Forward Kinematics** (Complexity: 4-5 points)
   - *Prerequisite:* Skeleton debugging tools ✅
   - *Certainty:* Medium-High (~65%)
   - *Scope:* Bone struct (parent index, local transform), skeleton struct (bone array), FK update loop (parent-to-child concatenation)
   - *Rationale:* Irreducible core; master transform hierarchy before adding complexity
   - *Validation:* Manual joint rotations produce expected world-space results (verified via debug draw)

3. **Quaternion Math Validation Suite** (Complexity: 3-4 points)
   - *Prerequisite:* None - parallel to skeleton work
   - *Certainty:* High (~75%) - isolated math testing
   - *Scope:* Standalone test file validating: quaternion construction from axis-angle, slerp, conjugate, swing-twist decomposition (with edge cases)
   - *Rationale:* Validate math in isolation before using in production (principle from QuaternionDecomp.md)
   - *Validation:* Unit tests with known inputs/outputs; measure angles, extract axes, convert to Euler

4. **Static Keyframe Poses** (Complexity: 3-4 points)
   - *Prerequisite:* FK working ✅, quaternion validation ✅
   - *Certainty:* Medium (~60%)
   - *Scope:* Hardcoded T-pose, walk cycle keyframes (quaternion rotations per joint), debug UI toggle
   - *Rationale:* Simplest use of quaternions; no interpolation complexity yet
   - *Validation:* Switch between poses via UI, verify visual correctness

5. **Keyframe Interpolation (Slerp)** (Complexity: 4-5 points)
   - *Prerequisite:* Static keyframes working ✅
   - *Certainty:* Medium (~55%)
   - *Scope:* Time-based interpolation between two keyframes using slerp; hemisphere check (dot product sign)
   - *Rationale:* Standard animation technique; validates quaternion pipeline
   - *Validation:* Smooth transitions, no flipping artifacts

6. **Joint Limits (Swing-Twist Decomposition)** (Complexity: 6-8 points)
   - *Prerequisite:* Slerp working ✅, local axes fully understood ✅
   - *Certainty:* Low-Medium (~40%)
   - *Scope:* Swing-twist decomposition per joint, clamp swing cone angle, clamp twist range, recombine
   - *Rationale:* Enables anatomical constraints; deferred from previous attempt due to missing prerequisites
   - *Validation:* Extreme rotations clamp to limits; left/right joints behave symmetrically (critical test)

**Deferred Until Prerequisites Met:**
- Secondary motion (spring offsets on top of keyframes)
- IK (requires robust FK and joint limits)
- Pose blending (multiple animation layers)
- Procedural gait generation (replace hardcoded poses)

---

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


