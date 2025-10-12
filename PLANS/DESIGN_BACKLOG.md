# Design Backlog

**Purpose:** Liquid reservoir of ideas waiting for the foundation to support them.

**Status:** Living document (pruned aggressively during backlog grooming).

**Historical Snapshots:** Completed work archived in `PLANS/ARCHIVE/dependency_stack_snapshot_2025-10-10.md`. Implementation specifics live in `PLANS/implementation_*.md` and `PLANS/code_review_*.md`.

---

## Snapshot

```
[Freeze Trail] [Axis Gizmo] [Running Gait Poses] [Extended Joints]
        |
        v
[Feature Pull | animation-led expansion]

Long tail: IK Systems, Wall Detection, Ragdoll, Dash, Terrain, Combat, Audio, UI Polish
Speculative: dY?, Frog Ideas dY?,
```

---

## Animation & Feel

### Debug Visualization (Pull-ready)

- **Freeze velocity trail on stop**
  - *Prerequisite:* Debug visual overhaul complete
  - *Certainty:* High (~95%)
  - *Status:* Now unblocked (2025-10-12)
  - *Notes:* Gate sampling on velocity magnitude (threshold ~0.05-0.1 m/s). Optional GUI slider on `velocity_trail_state`. Zero architectural risk.

- **Character axis gizmo**
  - *Prerequisite:* Debug draw system complete
  - *Certainty:* High (~90%)
  - *Status:* Now unblocked (2025-10-12)
  - *Notes:* Draw RGB axes from skeleton root, optional text labels. Toggle via GUI checkbox. Eliminates parent-space guesswork during pose tuning.

### Keyframe Extensions (Foundation proven)

- **Running gait keyframes**
  - *Prerequisite:* Quaternion pipeline locked
  - *Certainty:* High (~85%)
  - *Status:* Now unblocked (2025-10-12)
  - *Notes:* Add RUN_STEP_LEFT / RUN_NEUTRAL / RUN_STEP_RIGHT poses with longer strides. Unlocks speed-based gait switching.

- **Extended keyframe joint set**
  - *Prerequisite:* Keyframe architecture validated
  - *Certainty:* Medium (~60%)
  - *Status:* Candidate - keep deferred until readability gap appears
  - *Notes:* Add spine_upper and ankle joints once eight-joint greybox hits a readability wall.

- **Skeleton rest-pose reset**
  - *Prerequisite:* Debug animation toggle behavior
  - *Certainty:* Medium (~60%)
  - *Notes:* Copy reference pose back into local transforms so probes do not leave residual offsets.

- **Skeleton validation hooks**
  - *Prerequisite:* Debug draw overlays
  - *Certainty:* Medium (~50%)
  - *Notes:* Optional asserts or visual warnings for invalid parent indices; keeps authoring honest.

- **Tunable tilt velocity scaling**
  - *Prerequisite:* More playtest data
  - *Certainty:* Low (~30%)
  - *Notes:* Expose current hardcoded curve only if feel demands it.

- **Parameter persistence**
  - *Prerequisite:* Decide if iteration loop needs saved tuning
  - *Certainty:* Medium (~50%)
  - *Notes:* Lightweight config file (JSON/TOML) to load debug parameters across sessions.

---

## Movement & Physics

### Character Abilities (Medium Priority)
- Dash or dodge burst
- Wall jump
- Ledge grab (IK hands to edge)
- Climbing on tagged surfaces
- Swimming with buoyancy and drag
- Crouching with reduced collider

### Physics Enhancements (Low Priority)
- Velocity inheritance when jumping off moving platforms
- Slope physics (slide down steep inclines)
- Wind or current forces
- Momentum preservation (bunny hop, air strafe tuning)
- Dual-sphere experiment (restore bumper + weightlifter setup)

---

## Combat & Interaction

### Combat Mechanics (Uncertain - Defer)
- Melee attacks
- Ranged attacks
- Enemy AI behaviors
- Damage and health model
- Impact reactions (knockback, ragdoll on defeat)

### Interaction Systems (Uncertain - Defer)
- Object pickup and carry
- Throwing with applied velocity
- Doors, buttons, simple triggers
- Inventory for multiple items

---

## World & Environment

### Terrain (Low-Medium Priority)
- Heightmap terrain
- Uneven surfaces (drives foot IK need)
- Moving platforms (elevators, rotators)
- Destructible objects
- Water volumes

### Level Elements (Medium Priority)
- Ramps and stairs
- Obstacles (walls, barriers, gaps)
- Jump puzzles
- Collectibles or pickups

### World Building (Low Priority - Content Driven)
- Art style upgrade (post-graybox)
- Lighting polish
- Skybox swap
- Environmental props

---

## Camera & Controls

### Camera Enhancements (Low Priority)
- Camera collision avoidance
- Smart framing of target + character
- Camera shake impulses
- Follow smoothing tied to velocity

### Input Refinements (Low Priority)
- Gamepad support
- Input buffering
- Sensitivity curves
- Rebindable controls

---

## Audio & Feedback

### Audio System (Uncertain - Defer)
- Footstep events (distance-phased)
- Impact sounds
- Ambient loops
- Dynamic music system
- 3D positional audio

### Visual Feedback (Medium Priority)
- Screen effects (vignette, chromatic aberration)
- Particle systems (landing dust, dash trail)
- Screen shake
- Time manipulation (slow motion, freeze frames)

---

## UI & Menus

### Debug UI (Partially Complete)
- Extend visualization (acceleration vectors, velocity arrows, ground normal)
- Performance stats (frame time graph, draw call count)
- Physics state panel (grounded flag, contact debug)

### Game UI (Uncertain - Defer)
- Main menu
- Pause menu
- HUD elements
- Settings screen

---

## Content & Polish

### Visual Polish (Low Priority - Premature)
- Replace sphere with character mesh
- Materials and textures
- Post-processing pass
- Additional animation polish (secondary motion, follow-through)

### Game Loop (Uncertain - Core Unclear)
- Win and lose conditions
- Objectives and progression
- Scoring or evaluation
- Replayability hooks

---

## Frogs Rule! dY?,

### Frog-Specific Ideas (Wild Speculation)
- Tongue grapple
- Hop charge
- Sticky surfaces / wall-walk
- Water advantage
- Fly catching attacks
- Inflation for defense or flotation
- Spawn eggs that hatch helpers
- Croak-based interactions
- Metamorphosis (tadpole to frog)
- Lily pad platforms

*Certainty: ~0% - kept for inspiration and serendipity.*

---

**Notes:** Keep items non-interlocking, drop anything that clutters planning, and pull only when the foundation tier reaches 90%+ certainty.
