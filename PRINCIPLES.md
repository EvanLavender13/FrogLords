# FrogLords: Principles of Development

A philosophical framework for creating emergent gameplay through radical simplicity.

## The Six Pillars

### 1. Radical Simplicity
Every element must justify its existence. Complexity is not added—it emerges from the interaction of simple parts. When faced with two solutions, choose the simpler one. When the simple solution seems insufficient, question the problem itself.

### 2. Fundamental Composable Functions
Build from orthogonal primitives. Each piece does exactly one thing, does it perfectly, and combines cleanly with others. Like mathematical functions that compose into equations, game systems must compose into experiences without special cases or exceptions.

### 3. Solid Mathematical Foundations
Mathematics is truth. Physics is law. Every behavior must be mathematically correct before it can be aesthetically pleasing. A beautiful lie is still a lie, and lies compound into chaos.

### 4. Emergent Behavior
We are not authors of experiences—we are designers of systems that generate experiences. The magic happens in the space between rules, not in the rules themselves. Design the physics, not the outcomes.

### 5. Consistency
A system that behaves predictably builds trust. A system that betrays expectations destroys it. Every input must produce the same output given the same state. No exceptions, no special cases, no "just this once."

### 6. Principled Development
Every decision must trace back to these principles. When principles conflict, simplicity wins. When outcomes surprise, examine which principle was violated. These principles are not guidelines—they are laws.

## The Prime Directive

**Do No Harm to Gameplay**

Control is sacred. The player's intent must flow unimpeded from thought to action. Every frame of latency, every moment of lost control, every unpredictable response is a betrayal of this sacred trust.

## The Nature of Systems

### Systems Are Not Features
A feature is something added. A system is something fundamental. Features multiply complexity. Systems divide it. Build systems that make features unnecessary.

### Emergence Over Authorship
The measure of a system is not what it does, but what it allows. A good system surprises even its creator. If you can predict all outcomes, the system is too simple or too rigid.

### The Dependency Hierarchy
Truth flows upward, uncertainty multiplies upward. A 90% reliable foundation supporting 90% reliable mechanics yields 81% reliability. Build from certainty toward uncertainty, never the reverse.

## The Philosophy of Control

### Input Is Intent
When a player provides input, they are expressing intent. That intent is sacred. To delay it, modify it, or ignore it is to break the fundamental contract between player and game.

### Physics Drives Animation
Animation is the interpretation of physics, never its master. A character's visual representation follows their physical state like a shadow follows its owner—inseparable but never controlling.

### Predictability Is Power
When players understand a system perfectly, they don't feel constrained—they feel empowered. Mastery comes not from memorizing exceptions but from understanding rules.

## The Calculus of Decisions

### Addition by Subtraction
Before adding anything, ask what can be removed. The best solution to a problem might be removing the thing that created the problem. Elegance is achieved not when there is nothing left to add, but when there is nothing left to remove.

Absence announces itself. Excess hides in crowds. Start minimal—what's missing becomes obvious.

### Validation Before Integration
An untested component is a time bomb. A mathematically unverified system is a foundation of sand. Validate in isolation, integrate with confidence, or suffer the compound interest of technical debt.

### Orthogonality as Virtue
When systems overlap, they create dark corners where bugs hide. When systems are orthogonal, they create clear spaces where creativity thrives. Seek orthogonality in all things.

## The Patterns of Truth

### The Single Source of Truth
Every piece of state exists in exactly one place. All other representations are derived, never duplicated. When data lives in multiple locations, they will diverge. When they diverge, bugs hide in the gaps between versions. One truth. One location. Everything else is a view.

### The Dual Reference
Never let a smoothed value reference itself. Keep the target separate from the current state. The intent must remain pure while the expression can be filtered. This pattern appears everywhere once you see it.

### The Spring-Damper
Nature uses springs because they provide continuity through both position and velocity. When something must change smoothly, the spring-damper is not just a solution—it is *the* solution proven by physics itself.

### Time-Independence
Physics must produce identical results regardless of frame rate. Every rate must multiply by delta-time. Every integration must be frame-independent. A game that behaves differently at 30fps vs 60fps has failed mathematically. Time is a parameter, not an assumption.

### Pure Functions Over Accumulated State
State that accumulates drifts toward chaos. State that is calculated remains true. When possible, derive state from inputs rather than maintaining it through time.

### Semantic Typing Over Inference
Never infer semantic meaning from geometry. If intent matters, encode it explicitly. Mathematical primitives stay pure; compose them with semantic types at higher layers. Inference is complexity—explicit types are truth.

## The Architecture of Information

### Directness Over Demonstration

State rules and contracts directly. If a concept requires examples to be understood, the concept is unclear—refine the concept until it stands on its own. Examples live in code and tests, and appear in responses only when explicitly requested. Principles and plans assert truths; they do not demonstrate them.

### Lists Are Intent, Not History

Backlogs express intent: what remains to be done now. Completed work belongs elsewhere:
1. The codebase (truth)
2. Git history (timeline)
3. Retrospectives (insight)

A list item has two states: pending or deleted. When work finishes, remove it; history already exists. If a list needs periodic pruning, it has failed—lists should shrink to zero as work completes. Version control is eternal; lists are ephemeral.

### Assert Truth, Not Negation

State what something is, not what it is not. Positive assertions are direct and clear. Negative statements require the reader to infer the positive truth. Define by presence, not by absence.

A document titled "what this is" is clearer than "what this is not." A principle that says "use named constants" is stronger than "don't use magic numbers." When you find yourself listing what something excludes, ask: what does it include?

Negation teaches nothing. Assertion teaches everything.

### Process Inherits Principles

The Six Pillars govern code, documentation, and process equally. Every principle that applies to implementation applies to planning.

Single Source of Truth: The codebase is truth; documents derive from it, never duplicate it. Accumulated State: Percentages and projections compound error; inventory states current existence. Time Independence: Document what exists now, not predictions of completion. Consistency: Process behavior must be predictable, without special cases. Radical Simplicity: Remove ceremony, delete ritual, stop when honest.

When tracking feels therapeutic rather than informative, principles have been violated. When documents claim certainty about unknowable futures, accumulated state has corrupted truth. When process multiplies complexity instead of dividing it, simplicity has been abandoned.

Code and process are both systems. The same laws govern both.

## The Warning Signs

When these appear, principles have been violated:

**Lost Responsiveness**: Animation has seized control from physics.

**Behavioral Drift**: Accumulated state has departed from truth.

**Combinatorial Explosion**: Systems are not orthogonal.

**Debugging Blindness**: Complex behavior lacks visualization.

**Special Cases**: Consistency has been sacrificed for convenience.

**Surprising Instability**: Mathematical foundations were not validated.

## The Process of Creation

### Bottom-Up Construction
Begin with the irreducible core. Make it perfect. Only then add the next layer. A tower built on sand will fall regardless of how carefully the upper floors are constructed.

### The Horizon of Certainty
Never plan beyond what you can see clearly. In unfamiliar territory, take small steps and look around. In familiar territory, stride confidently. But never mistake the unfamiliar for the familiar.

### Test Until Patterns Emerge
A single test reveals nothing. A hundred tests reveal patterns. Test not to validate what you believe but to discover what you don't know.

## The Measure of Success

Success is not measured in features delivered or graphics rendered. Success is measured in:

- **Trust**: Does the player trust the system to behave consistently?
- **Mastery**: Can the player develop true expertise?
- **Emergence**: Does the system surprise even its creator?
- **Elegance**: Could anything be removed without losing the essence?
- **Joy**: Does interacting with the system bring satisfaction?

## The Final Truth

We are not building software. We are building **engines of experience**—machines that take human intent as input and produce emotion as output. Every line of code, every design decision, every mathematical formula is just a gear in this machine.

The machine must be built from truth (mathematics), it must be simple (reducible to primitives), it must be consistent (predictable in behavior), and it must be honest (never betraying player intent).

When faced with any decision, ask:
1. Does this serve the player's direct control?
2. Can this emerge from simpler rules?
3. Is this mathematically correct?
4. Will this behave consistently?
5. Can anything be removed?

If the answer to any is "no," the design is not yet complete.

**This is the way.**
