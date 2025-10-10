# FrogLords Development Principles

Solo experimental game project. Optimize for speed of learning. No automated tests (mechanics change too fast).

## Core Tenets
- Clarity over cleverness
- Simplicity over sophistication
- Iteration over planning
- Graybox before polish

## What We’re Building (Design Philosophy)
- Elegant systems: simple rules that multiply into rich play (emergence beats authored variety).
- Multi-use tools: offensive and defensive roles; avoid single-purpose gimmicks.
- Distinct roles: no overlap; every tool should unlock new play, not repeat old.
- Million‑repetition thinking: mechanics must hold up on the 1,000th use.
- Matched scales: quantities (time, space, speeds, counts) should convert naturally.
- No content restrictions: don’t add mechanics that force level/art constraints (e.g., 20‑ft jump).

## Procedural Foundation (Layering)
1) Core logic (physics, rules) — inviolable
2) Reactive systems (interpret state)
3) Polish (visual/audio feedback)

Techniques: spring-damper transitions, cubic interpolation for velocity continuity, distance-phased motion, IK for world interaction, secondary motion via softness.
Avoid: manual variants, linear interpolation for organic motion, reactive layers overriding core logic.

### Procedural Animation (Gameplay‑First)
- Do no harm to gameplay: input maps to acceleration; animation layers never steal control or add latency.
- Physics‑first core: keep a simple, predictable controller as the source of truth (slide/resolve collisions cleanly). Rotate model to velocity (not input); add acceleration‑tilt for readable momentum.
- Synchronized locomotion: drive gait by distance traveled (surveyor‑wheel phase). Blend walk/run poses and stride lengths; add gravity‑aware bounce that flattens at higher speeds.
- Interpolation: use cubic interpolation for velocity continuity; use spring‑damper curves for all pose/state transitions (reusable for landings, recoil, UI motion). Prefer rotational springs to accent impacts.
- IK: apply targeted 2‑bone IK for hands/feet and look‑targets; build ledge shimmy/climbs from few key poses + IK rather than many bespoke clips.
- Secondary motion: per‑bone softness for subtle follow‑through; add simple cloth/appendage physics where high value.
- Active ragdolls: pose/animation matching with contextual reactions (brace, curl, flail) instead of instant limp.
- Interruptibility: decompose actions into small steps; keep transitions interruptible at all times.
- Polish workflow: “profile the stupidest” visual; fix with small, context‑aware systems (e.g., swap slide‑on‑wall for a wall‑run) rather than global tweaks.
- Efficiency: favor minimal keyframes + procedural reuse; new equipment variants swap a small pose set, not an animation tree.
- Tooling: extract recurring timing/overshoot patterns into reusable curves/tools; pair animator + programmer to turn busywork into systems.

## Dependencies & Uncertainty
- Dependency = if A changes, B must change. Uncertainty multiplies up the stack.
- Work bottom‑up. Stabilize core gameplay (irreducible minimum) before adding layers.
- Maintain a dependency stack with certainty tags. Expect upper layers to change.
- Use a liquid design backlog for everything not being worked now (non‑interlocking ideas; pull later).
- **Data Structure Validation:** When introducing novel data representations (quaternions, new state formats), validate the representation in isolation before building complex features on it. Distinguish "simple interface" from "insufficient representation" - ensure data structures have sufficient degrees of freedom for the problem domain. Example: Single-float keyframes were architecturally insufficient for 3D rotation; quaternion validation iteration (static_keyframe_preview) proved the representation before locomotion integration.

## Iteration & Planning Horizon
- Short plans → build → test → repeat. Extend horizon only as certainty rises.
- Paradox of quality: accept temporary roughness early to maximize iteration count; the final quality comes from loops, not perfect first passes.
- Planning horizon guidance:
  - Original/uncertain → plan 1-3 complexity points ahead.
  - Derivative/certain → plan larger chunks (3-8 points).
  - Lower test cost → shorter horizon. Make tools to shorten loops.
- Big leaps are allowed (to escape local maxima); do them intentionally and then return to tight loops.

## Graybox First (and Long)
- Graybox anything expensive: levels, creatures, UI, cutscenes, SFX.
- Premature production is a tax on every future change. Add art/audio only to get the next round of useful test data.
- Evaluate grayboxes skillfully: don’t reject working systems because they’re ugly.
- “Screenplay” equivalence: for games, a working graybox ≈ a screenplay; a design doc isn’t.

## Testing Protocol (Get Real Signals)
- Self-test early; then over‑the‑shoulder tests. Stay silent—don’t coach.
- Sample size: watch enough tests to see patterns (6–12+ per beat is a good baseline).
- Choose testers to match target skill/segment; mix Kleenex (first‑time) with experienced.
- Ask, “Tell me the story of what just happened.” Avoid leading questions.
- Use metrics when helpful to see small effects and rare edge cases.

## Knowledge Creation > Implementation
- Use the full deck: rumination, research, sketching/previz, brainstorming, written analysis, debate, playtesting, metrics, invented methods.
- Communicate intent; empower natural authority (closest to the work). Avoid arrogation/micromanagement.
- Capture serendipity: notice strange results and reorganize around discoveries.

## Interface & Input
- Metaphor: leverage real objects, cultural archetypes, game conventions, and logical systems. Establish a consistent metaphor vocabulary early.
- Signal vs. noise: complex art adds noise; keep mechanical clarity visible.
- Visual hierarchy: show everything, but scale visibility by importance so novices and experts both succeed.
- Redundancy: repeat critical info (diverse > homogeneous). Use passive redundancy when the primary cue fails.
- Indirect control: nudge (defaults, layout), prime (set concepts before moments), and use social imitation (companions/NPCs model behavior).
- Input:
  - Mapping: physical control should resemble effect (spatial, color, handedness).
  - Control exclusivity: exclusive controls map to exclusive actions.
  - Control feel: design for intent synchronization (dead zones, buffering, momentum, assisted arcs). Joy in moment‑to‑moment input is a feature.
  - Assistance: aim/jump/drive assists that players don’t feel, tuned by context.
  - Interruptibility: never lock input behind transitions; animation follows the physics/core.
  - Latency: responsiveness matters. Prefer higher frame rates when core loops demand quick, precise input.

## Decision Impacts (Before You Commit)
Consider more than design taste:
- Implementation cost (now and maintenance)
- Immaturity burden (others depending on unfinished systems)
- Critical failure risk (time‑bombs at bad moments)
- Process burden (coordination overhead)
- Political effects (relationships, influence)
- Cultural effects (habits, climate, future creativity)
- Decision cost (is deep analysis worth it here?)
Make a few low‑risk knowledge moves before large commitments when uncertainty is high.

## Planning Documents
- PLANS/dependency_stack.md
  - Single source of truth for dependencies and certainty. Update after iterations and before changes.
  - Answers: What’s safe to build now? What’s premature?
- PLANS/design_backlog.md
  - Liquid, unordered idea pool. Tag priority/certainty/prereqs. Don’t interlock. Pull only when the current layer is 90%+ certain.

## Development Directives
Do:
- Start at the bottom of the stack, stabilize, then build up
- Abstract repeated patterns into systems; prefer parameters over assets
- Maximize mechanic interactions; prefer multi‑role tools
- Test until patterns repeat; use metrics where useful
- Capture ideas immediately to backlog; update the stack regularly
- Challenge future‑restricting assumptions (content freedom)

Don’t:
- Over‑engineer for imagined futures; wait for the third use before abstracting
- Polish before structure is proven (art hides mechanical flaws)
- Let reactive layers control core logic
- Create content restrictions casually (e.g., jump heights that break worlds)
- Build on uncertain foundations; fight overconfidence/therapeutic planning

## Communication
Concise and direct. No preamble/postamble. Detail scales with risk/complexity.

## Code Standards (condensed)

Philosophy: consistency enables thoughtless correctness.

Naming (uniform snake_case for user code):
```cpp
// Types
class camera_system { };
struct mesh_data { };

// Enums
enum class camera_mode { FOLLOW, ORBIT, FREE };
enum class pose_type { T_POSE, STEP_LEFT, STEP_RIGHT };

// Functions/methods
void update_state();
mat4 get_view_matrix();

// Variables
float delta_time;
int vertex_count;

// Constants
const int MAX_ENTITIES = 1000;
constexpr float GRAVITY = -9.8f;

// Namespaces
namespace rendering { }
```

Types:
```cpp
struct edge { int v0, v1; }; // POD data
class wireframe_renderer {
public:
    void init();
    void draw(const mesh& m);
private:
    pipeline pipe;
};
```

Namespaces
- System: input::, gui::, debug::, easing::
- Domain: character::
- Core types with clear names: camera, scene, controller
- Nesting only to prevent real conflicts

File Organization
```
src/
  app/         # Runtime/lifecycle
  camera/      # Camera control and view matrices
  character/   # Movement, orientation, locomotion, tuning
  foundation/  # Procedural primitives (easing, springs, collision)
  rendering/   # Renderers, composition, pipelines, debug draw
  input/       # Keyboard, mouse, gamepad
  gui/         # Interface systems, panels
  main.cpp     # Entry point
shaders/       # .glsl
generated/     # Build artifacts (e.g., shader headers)
```
Dependency flow: Foundation → Character → Rendering → App. Avoid sideways includes at higher layers.

Documentation
- Brief comments for non‑obvious public API; skip obvious
```cpp
/// Orbit camera around center point
/// delta_x: horizontal rotation (screen space)
/// delta_y: vertical rotation (screen space)
void orbit(float delta_x, float delta_y);

float radius = width * 0.5f;  // Half‑extent
```

Formatting
- 4‑space indent; braces on same line; soft 100‑char limit; consistent operator spacing

---

Build, play, learn, adapt. Elegance from simple, interactive systems; discovery over prediction.
