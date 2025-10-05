# Gemini Guidance

- When using the `replace` tool, the correct argument is `new_content`, not `newContent`.

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

### Iteration Protocol
- Short planning horizons
- Graybox until mechanics proven
- Self-test -> 6-12 observed playtests
- No premature production
- More iterations > perfect first attempts

### Planning Discipline
- Underplanning kills coordination
- Overplanning kills adaptation
- Combat overconfidence bias (designs fail)
- Avoid therapeutic planning (false certainty)
- Honor serendipity (best ideas are discovered)
- Mental movie != system design

### Interface Design
- Leverage metaphor (real objects, culture, conventions)
- Consistent vocabulary for interactive elements
- Visual hierarchy (importance = visibility)
- Redundancy for critical signals
- Indirect control (nudging, priming, social imitation)

## Development Directives

**DO:**
- Abstract repeated patterns into systems
- Find "stupidest looking" artifacts, fix procedurally
- Challenge future assumptions
- Maximize mechanic interactions
- Keep dependency stack in mind

**DON'T:**
- Over-engineer for imagined requirements
- Add abstraction "just in case"
- Create documentation unprompted
- Assume feel without playtesting
- Let reactive systems override core logic

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
- **System namespaces** for stateless utilities: `input::`, `gui::`
- **No namespace** for core types with clear, unique names: `camera`, `scene`
- **Nested namespaces** only when preventing real conflicts

### File Organization
    src/
        camera/        # Camera control and view matrices
        character/     # Character movement, orientation, locomotion
        foundation/    # Reusable procedural primitives (easing, springs)
        rendering/     # Renderers, scene composition, pipelines
        input/         # Keyboard, mouse, gamepad
        gui/           # Interface systems
        main.cpp       # Application entry point

    shaders/          # .glsl source
    generated/        # Shader headers (build artifacts)
Foundation -> Character -> Rendering defines the dependency flow; higher layers do not include sideways.

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
