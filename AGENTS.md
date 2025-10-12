# FrogLords Development Guide

Solo experimental project. Optimize for learning speed. No automated tests (mechanics change too fast).

## Quick Start
- Clarity over cleverness; simplicity over sophistication.
- Iterate, don’t speculate. Short plan -> build -> test -> repeat.
- Graybox first and long. Polish only to get better test data.
- Keep core physics/rules inviolable; layer reactivity and polish on top.
- Prefer multi-role tools with distinct, non-overlapping purposes.
- Avoid content restrictions that limit future level/art freedom.

## What We’re Building
- Elegant systems: simple rules that compound into rich play (emergence beats authored variety).
- Multi-use tools: offensive and defensive roles; avoid single-purpose gimmicks.
- Distinct roles: no overlap; every tool should unlock new play, not repeat old.
- Million-repetition thinking: mechanics must hold up on the 1,000th use.
- Matched scales: time, space, speeds, and counts should convert naturally.
- No content restrictions: don’t add mechanics that force level/art constraints (e.g., 20 ft jump).

## Layered Architecture
1) Core logic (physics, rules) — inviolable
2) Reactive systems (interpret state)
3) Polish (visual/audio feedback)

Defaults
- Use spring-damper transitions and cubic interpolation for velocity continuity.
- Drive locomotion by distance traveled (surveyor-wheel phase).
- Add secondary motion via softness; use IK for targeted world interaction.
- Avoid manual animation variants, linear lerps for organic motion, and any reactive layer overriding core logic.

Dual-Reference Pattern (for smoothed variables)
- Keep an immutable target: e.g., `run_speed`, `max_jump_height`.
- Maintain a smoothed state derived from the target: e.g., `max_speed`, `current_jump_height`.
- Rationale: prevent circular dependencies where the smoothed value references itself.

## Procedural Animation (Gameplay-First)
- Do no harm: input maps to acceleration; animation never steals control or adds latency.
- Physics-first core: simple, predictable controller; rotate model to velocity (not input); add acceleration-tilt.
- Synchronized locomotion: gait driven by distance; blend walk/run poses; gravity-aware bounce that flattens at speed.
- Interpolation: cubic for velocity continuity; spring-damper for pose/state transitions; prefer rotational springs to accent impacts.
- IK: targeted 2-bone IK for hands/feet and look-targets; build ledge shimmy/climbs from few poses + IK (avoid bespoke trees).
- Secondary motion: per-bone softness and simple cloth/appendage physics when valuable.
- Active ragdolls: pose/animation matching with contextual reactions (brace, curl, flail) instead of instant limp.
- Interruptibility: decompose actions into small steps; transitions stay interruptible.
- Polish workflow: “profile the stupidest” visual; fix locally with small systems (e.g., swap slide-on-wall for wall-run).
- Efficiency: minimal keyframes + procedural reuse; equipment variants swap a small pose set, not an animation tree.

## Dependencies and Planning
- Dependencies: if A changes, B must change. Uncertainty multiplies up the stack.
- Work bottom-up. Stabilize the irreducible core before adding layers.
- Maintain a dependency stack with certainty tags. Expect upper layers to churn.
- Use a liquid design backlog for everything not being worked now (non-interlocking ideas only).
- Data structure validation: validate novel representations (e.g., quaternions, state formats) in isolation before integrating.

Iteration Horizon
- Original/uncertain work: plan 1–3 complexity points ahead.
- Derivative/certain work: plan larger chunks (3–8 points).
- Lower test cost -> shorter horizon. Build tools to shorten loops.
- Big leaps are allowed to escape local maxima; take them intentionally, then return to tight loops.

Graybox and Testing
- Graybox expensive things: levels, creatures, UI, cutscenes, SFX.
- Don’t reject good systems because they’re ugly; graybox ≈ screenplay.
- Self-test early; then over-the-shoulder testing. Stay silent—don’t coach.
- Sample size: watch enough to see patterns (6–12+ per beat).
- Mix first-time “Kleenex” testers with experienced; use metrics when helpful for small effects/edge cases.

Knowledge Creation
- Use the full deck: rumination, research, previz, brainstorming, analysis, debate, playtests, metrics, invented methods.
- Communicate intent; empower the closest person to the work. Avoid micromanagement.
- Capture serendipity: notice strange results and reorganize around discoveries.

## Interface and Input
- Mapping: physical control should resemble effect (spatial, color, handedness).
- Exclusivity: exclusive controls map to exclusive actions.
- Control feel: design for intent synchronization (dead zones, buffering, momentum, assisted arcs).
- Assistance: aim/jump/drive assists tuned by context and ideally unnoticed.
- Interruptibility: never lock input behind transitions; animation follows physics/core.
- Latency: responsiveness matters; prefer higher frame rates when loops demand precision.

## Decision Impacts (before you commit)
- Consider: implementation cost, immaturity burden, critical failure risk, process burden, political/cultural effects, and decision cost.
- Make a few low-risk knowledge moves before large commitments when uncertainty is high.

## Planning Docs
- `PLANS/DEPENDENCY_STACK.md`: single source of truth for dependencies and certainty. Update after iterations and before changes.
- `PLANS/DESIGN_BACKLOG.md`: liquid, unordered idea pool. Tag priority/certainty/prereqs. Pull only when current layer is 90%+ certain.

## Development Directives
- Do: start at the bottom, stabilize, then build up; abstract repeating patterns into systems; prefer parameters over assets; maximize mechanic interactions; test until patterns repeat; capture ideas to backlog; challenge future-restricting assumptions.
- Don’t: over-engineer for imagined futures (wait for the third use to abstract); polish before structure is proven; let reactive layers control core logic; create content restrictions casually (e.g., jump heights that break worlds); build on uncertain foundations.

## Communication
Concise and direct. No preamble/postamble. Detail scales with risk/complexity.

## Code and Repo
- See `CONVENTIONS.md` for coordinate, rotation, camera, animation, and debug visualization conventions.
- File organization
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
- Dependency flow: Foundation -> Character -> Rendering -> App. Avoid sideways includes at higher layers.

Naming (uniform snake_case for user code)
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

Types
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

Documentation
- Brief comments for non-obvious public API; skip the obvious.
```cpp
/// Orbit camera around center point
/// delta_x: horizontal rotation (screen space)
/// delta_y: vertical rotation (screen space)
void orbit(float delta_x, float delta_y);

float radius = width * 0.5f;  // Half-extent
```

Formatting
- 4-space indent; braces on same line; soft 100-char limit; consistent operator spacing.

## References (deep dives)
- Designing Games notes: `NOTES/DesigningGames/DG_Dependencies.md`, `NOTES/DesigningGames/DG_Planning.md`, `NOTES/DesigningGames/DG_Interface.md`, `NOTES/DesigningGames/DG_Skill.md`, `NOTES/DesigningGames/DG_Elegance.md`.
- Procedural animation philosophy: `NOTES/GDC/GDC_DesignPhilosophy.md`, `NOTES/GDC/GDC_TechnicalWhitepaper.md`.

---

Build, play, learn, adapt. Elegance from simple, interactive systems; discovery over prediction.

