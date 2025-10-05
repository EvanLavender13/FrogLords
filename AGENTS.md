# FrogLords Development Principles

## Context
Solo experimental game project. Prioritize agility over process. No automated tests--mechanics change too frequently.

## Core Tenets
1. **Clarity over cleverness**
2. **Simplicity over sophistication**
3. **Iteration over planning**
4. **Graybox before polish**

## Design Principles

### Elegant Systems
**Mechanics:**
- Simple components, complex interactions
- Multi-use tools (not single-purpose gimmicks)
- Distinct roles (no overlap)
- High reuse frequency (million-repetition thinking)
- Matched scales between systems (enables natural conversions)
- No content restrictions on future work

**Implementation:**
- Stupidly simple cores, emergent complexity
- Parameters > assets (stiffness/damping > keyframes)
- Abstraction eliminates combinatorial explosion
- One system handles multiple use cases (spring-damper for transitions, landings, recoil)

**Elegance heuristics (smells):**
- Interacts with many other mechanics
- Stupidly simple (napkin-describable)
- Multiple roles (offensive + defensive)
- Distinct roles (no overlap with existing)
- Reused constantly (million-repetition thinking)
- No content restrictions (doesn't force level/art constraints)
- Matched scales (enables natural conversions between systems)
- Leverages conventions (real objects, culture, genre)

Elegant designs often sound boring on paper. Flash comes from emergence, not gimmicks.

### Procedural Foundation
**Layer by priority:**
1. Core logic (physics, rules) - never compromised
2. Reactive systems (interpret core state)
3. Polish (visual/audio feedback)

**Techniques:**
- Distance-phased motion (surveyor wheel)
- Spring-damper transitions
- Cubic interpolation for velocity continuity
- IK for world interaction
- Secondary motion via softness parameters

**Avoid:**
- Manual authoring of variations
- Linear interpolation for organic motion
- Reactive layers controlling core logic

### Dependency Management
- Work bottom-up through stack
- Core gameplay first (irreducible minimum)
- Design backlog for everything else (liquid, uncommitted)
- Detail only current implementation
- Expect upper stack changes (cascading uncertainty)

**Cascading uncertainty:** Per-element uncertainty multiplies through dependencies. With 80% certainty per layer, five layers = 33% certainty. Upper stack elements almost always need redesign. Start at bottom, solidify before building upward.

**Design backlog:** Unordered pool of ideas not being worked on soon. Most ideas go here. Keep liquid, non-interlocking until foundation solidifies. Pull from backlog only after current layer reaches 90%+ certainty.

### Iteration Protocol
- Short planning horizons (adjust by uncertainty level)
- Graybox until mechanics proven
- Self-test -> 6-12 observed playtests minimum
- No premature production (art hides mechanical flaws)
- More iterations > perfect first attempts

**Planning horizon:** Original/uncertain = plan days ahead. Derivative/certain = plan weeks/months ahead. Lengthen horizon as project solidifies. Low test cost = shorter horizon.

**Paradox of quality:** Accepting poor quality early enables more iterations, yielding better final quality. Perfecting each step slows iteration, reducing final quality.

**Graybox evaluation:** Requires practiced skill. Good graybox ≠ good visual feel. Avoid halo effect. Don't let inexperienced evaluators reject working grayboxes due to missing art.

### Planning Discipline
- Underplanning kills coordination
- Overplanning kills adaptation
- Combat overconfidence bias (90% confidence ≈ 30% reality)
- Avoid therapeutic planning (planning to feel better, not coordinate)
- Capture serendipity (observe strange behaviors, dig into hints)
- Mental movie != system design (vision shows best case, hides flaws)

**Cognitive threats:**
- **Overconfidence:** Humans systematically overestimate certainty
- **Therapeutic planning:** Planning to reduce anxiety, not improve coordination
- **Group planning bias:** Groups reward confident over correct
- **Hindsight bias:** Past always looks more predictable than it was

**Serendipity capture:** Most breakthrough ideas are discovered, not planned. Stay observant. Explore strange results. Reorganize thoughts around observations, not observations around worldview. Design = authorship + discovery.

### Interface Design
- Leverage metaphor (real objects, culture, conventions)
- Establish metaphor vocabulary, stay consistent
- Visual hierarchy (importance = visibility, skill-independent)
- Redundancy for critical signals (diverse > homogeneous)
- Indirect control (nudging, priming, social imitation)

**Signal vs noise:** Complex art creates noise. Every line/color without mechanical meaning requires mental filtering. Balance detail against comprehension. Players miss signals due to overload, not insufficient visibility.

**Visual hierarchy:** All visible at once, but importance determines visibility. Novices perceive most visible; experts perceive more. Eliminates skill-gap problem. Natural example: 3D space (distance = relevance = visibility).

**Metaphor vocabulary:** Establish which fictional elements are mechanics. Be consistent. Teach early in simple contexts. Unique-looking elements = interactive. Never change vocabulary mid-game.

## Planning Documents

### Dependency Stack (`PLANS/dependency_stack.md`)
**Consult before ANY change or decision.**

Visual map of system dependencies and uncertainty levels. Shows what to work on now vs. defer to backlog.

**When to use:**
- Before starting any new feature (am I building on solid foundation?)
- Before making design decisions (will this force changes elsewhere?)
- When prioritizing work (what's at bottom of stack?)
- When uncertain about work order (dependencies answer this)
- After completing iteration (update certainty estimates)

**Key questions it answers:**
- What should I work on next?
- Is this foundation solid enough to build on?
- What's the cascading uncertainty risk?
- Which ideas are premature?

### Design Backlog (`PLANS/design_backlog.md`)
**Groom regularly. Reference for inspiration.**

Liquid, unordered pool of ideas not being worked on soon. Most ideas live here until foundation solidifies.

**When to use:**
- Capture new ideas immediately (don't let inspiration degrade)
- Pull next feature when current layer reaches 90%+ certainty
- Groom during serendipity capture (reorganize around discoveries)
- Reference when stuck (explore alternative approaches)
- Prune when ideas become obsolete

**Key rules:**
- Keep ideas liquid and non-interlocking
- Tag with priority/certainty/prerequisites
- Accept most will be cut (feature, not bug)
- Never plan details for backlog items
- Pull, don't push (wait for readiness)

## Development Directives

**DO:**
- **Consult dependency stack before every change**
- Abstract repeated patterns into systems
- Find "stupidest looking" artifacts, fix procedurally (profiling for visual quality)
- Challenge future assumptions (avoid content restrictions)
- Maximize mechanic interactions (emergence)
- Test until patterns repeat (6-12 minimum)
- Question elegant-sounding complexity (probably inelegant)
- Reorganize around discoveries (serendipity over vision)
- **Capture ideas to backlog immediately**
- **Update stack after completing iterations**

**DON'T:**
- Over-engineer for imagined requirements (YAGNI)
- Add abstraction "just in case" (wait for third use)
- **Create detailed plans for backlog items** (liquid ideas only)
- Assume feel without playtesting (mental movies hide flaws)
- Let reactive systems override core logic (do no harm to gameplay)
- Make premature production (art hides mechanical problems)
- Trust confident predictions (overconfidence bias)
- Add mechanics that restrict content (jump height constrains all levels)
- Polish before structure proven (iteration count > per-step quality)
- **Build on uncertain foundations** (check stack first)

## Communication
Concise and direct. No preamble/postamble. Match detail to complexity.

## Code Standards

### Philosophy
**Consistency enables thoughtless correctness.** Standards eliminate decision fatigue and visual noise. Code becomes infrastructure--invisible until it needs changing.

### Naming Conventions
**Uniform snake_case for all user-defined names:**
```cpp
// Types
class camera_system { };
struct mesh_data { };

// Functions/methods
void update_state();
mat4 get_view_matrix();

// Variables (local, member, parameter)
float delta_time;
int vertex_count;

// Constants
const int MAX_ENTITIES = 1000;
constexpr float GRAVITY = -9.8f;

// Namespaces
namespace rendering { }
```

**Rationale:** Matches C++ standard library. One style = zero cognitive load.

### Type Definitions
```cpp
// Struct: Pure data (all public)
struct edge {
    int v0, v1;
};

// Class: Encapsulation (private data, public interface)
class wireframe_renderer {
public:
    void init();
    void draw(const mesh& m);

private:
    pipeline pipe;
};
```

### Namespaces
- **System namespaces** for stateless utilities: `input::`, `gui::`, `debug::`, `easing::`
- **Domain namespaces** for related utilities: `character::`
- **No namespace** for core types with clear, unique names: `camera`, `scene`, `controller`
- **Nested namespaces** only when preventing real conflicts

### File Organization
    src/
        app/           # Application runtime and lifecycle
        camera/        # Camera control and view matrices
        character/     # Character movement, orientation, locomotion, tuning
        foundation/    # Reusable procedural primitives (easing, springs, collision)
        rendering/     # Renderers, scene composition, pipelines, debug draw
        input/         # Keyboard, mouse, gamepad
        gui/           # Interface systems, panels
        main.cpp       # Application entry point

    shaders/          # .glsl source
    generated/        # Shader headers (build artifacts)
Foundation -> Character -> Rendering -> App defines the dependency flow; higher layers do not include sideways.

### Documentation
```cpp
// Brief comments for non-obvious public API
// Skip docs for self-evident functions

/// Orbit camera around center point
/// delta_x: horizontal rotation (screen space)
/// delta_y: vertical rotation (screen space)
void orbit(float delta_x, float delta_y);

// Implementation notes inline where needed
float radius = width * 0.5f;  // Half-extent
```

**Avoid:**
- Redundant comments (`// Set x` above `x = value;`)
- Documentation for private implementation details
- Over-documentation of obvious intent

### Formatting
- **Indentation:** 4 spaces
- **Braces:** Same line for functions/classes
- **Line length:** Soft limit 100 chars (break for readability)
- **Whitespace:** Consistent operator spacing

---

> "Every game exists already, hidden in the logic of the universe. We don't create them. We find them--not by adding, but by removing excess material that obscures the form within."

**Goal**: Elegant emergence from simple, interactive systems. Build, play, learn, adapt.
