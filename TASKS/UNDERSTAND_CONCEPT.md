# Understand Concept or Technique

**Purpose:** Build conceptual knowledge through discussion, research, and documentation. Create intellectual capital that informs future problem-solving.

**Context:** From *Designing Games* philosophy - "The hard part of game design is not physically implementing the game. It is inventing and refining knowledge about the design." This task is a **knowledge creation method** that deploys discussion, research, and written analysis to eliminate uncertainty.

**Use when:**
- Encountering unfamiliar technique or algorithm
- Abstract concept needs concrete grounding
- System interactions or layering unclear
- Trade-offs between approaches need exploration
- Building foundational understanding for future work

**Do NOT use when:**
- Concept already well understood
- Simple implementation with clear path forward
- Time-sensitive bug fix or trivial change

---

## Knowledge Creation Methods Deployed

This task combines multiple knowledge creation approaches:

### Discussion & Debate
Ask targeted questions to expose hidden assumptions and build intuitive understanding. Unlike formal debate, this is collaborative exploration aimed at eliminating ambiguity.

### Research
Study existing code, reference materials (GDC docs), and external sources to expand knowledge base. Both targeted (answering specific questions) and semi-random (feeding rumination).

### Written Analysis
Structure thoughts through documentation. Writing forces detail consideration that mental models alone miss. Creates reference material for future problems.

### Rumination
Allow unconscious processing through discussion flow. Often the best insights emerge mid-conversation when relaxed and exploring analogies.

---

## Process

### 1. Identify Knowledge Gap

Be specific about what you don't understand:
- **Algorithmic:** "How does slerp differ from lerp for rotations?"
- **Architectural:** "Where does secondary motion fit in the animation pipeline?"
- **Comparative:** "When should I use springs vs. easing curves?"
- **Interaction:** "How do pose blending and secondary motion interact?"
- **Trade-offs:** "What's the complexity of different pose counts per gait?"

### 2. Ask Targeted Questions

Frame questions to build intuitive understanding:
- Request **concrete examples** over abstract theory
- Ask for **order of operations** to understand system layering
- Seek **visual comparisons** (ASCII diagrams, timelines, before/after)
- Request **real-world analogies** (e.g., "forearms lag like weights being dragged")
- Identify **when to use** each approach (decision trees)

**Effective question patterns:**
- "Can you explain [concept] intuitively?"
- "What's the order of events when [system X] interacts with [system Y]?"
- "What's the difference between [approach A] and [approach B]?"
- "How does [technique] fit into the existing [system]?"
- "Would [variation] add complexity? How much?"

### 3. Explore Through Discussion

Have a conversational exchange to:
- Build mental models through examples
- Test understanding with follow-up questions
- Explore edge cases and future implications
- Compare with project philosophy (GDC docs, AGENTS.md)
- Discuss trade-offs between implementation strategies

**Characteristics of productive discussion:**
- Uses concrete examples from current codebase
- References specific line numbers and existing implementations
- Draws comparisons to familiar concepts
- Builds from simple → complex (e.g., 1D → 2D blending)
- Tests understanding with "what if" questions
- Allows for tangents and rumination

### 4. Document Understanding

Once concept is clear, create reference in `NOTES/`:

**Filename:** `<concept_name>_explained.md` (e.g., `pose_blending_explained.md`)

**Structure:** Progressive complexity (basics → advanced topics)

**Include:**
- Intuitive explanations with analogies
- Visual diagrams (ASCII art for timelines, pipelines, blend spaces)
- Order of operations for system interactions
- Code examples from current implementation
- Comparison tables (when to use what)
- References to relevant docs (GDC, AGENTS.md)
- Links to relevant source files
- Future extensions and open questions

**Tone:** Tutorial-style, not academic. Explain like teaching a colleague.

### 5. Link to Relevant Documents

Add reference links where this knowledge will be useful:
- `PLANS/DESIGN_BACKLOG.md`: Add `*Reference:*` line to related items
- Feature descriptions: Link in "Technical Approach" or "Background"
- Other NOTES documents: Cross-reference related concepts

**Example:**
```markdown
- **Pose blending (lerp/slerp):** Smooth transitions between discrete walk poses
  - *Reference:* See [NOTES/pose_blending_explained.md](../NOTES/pose_blending_explained.md)
```

### 6. Intellectual Capital Created

Document becomes:
- **Reference material** for future implementation
- **Teaching tool** for understanding similar problems
- **Decision guide** for choosing between approaches
- **Context** for code reviews and discussions
- **Foundation** for related features

---

## Example Session: Pose Blending

**Initial Question:**
> "I need to understand lerp/slerp for pose blending in the walk cycle task. Can you explain it intuitively?"

**Knowledge Creation Methods Used:**
- **Discussion:** Back-and-forth Q&A to build understanding
- **Research:** Referenced GDC docs, examined current animation code
- **Written Analysis:** Created comprehensive tutorial document
- **Rumination:** Explored tangents (secondary motion, 2D blending, future extensions)

**Topics Explored:**
1. Current state (discrete pose switching causes "pops")
2. Lerp vs. slerp (straight line vs. shortest arc for rotations)
3. Order of events (determine poses → slerp joints → apply)
4. Blend factor calculation (linear interpolation of phase)
5. Future: 2D blending for walk/run gait switching
6. Three layers of interpolation (slerp, springs, curves)
7. Secondary motion interaction (applied after, not blended)
8. Trade-offs (different pose counts per gait)

**Output Document:** `NOTES/pose_blending_explained.md`
- Covers 1D and 2D blending with visual diagrams
- Explains three interpolation layers and when to use each
- Order of operations for secondary motion
- Real-world observations (forearm lag like weights)
- References to GDC philosophy
- Decision guidance for implementation

**Backlog Updated:** Added reference link to pose blending backlog item

**Intellectual Capital Created:**
- Clear implementation path for pose blending feature
- Reusable knowledge for future animation features
- Understanding of interpolation layers applicable to other systems
- Documented rationale for future code reviews

---

## Common Concept Categories

### Animation & Interpolation
- Pose blending (lerp/slerp, bilinear)
- Easing curves (cubic, ease-in/out)
- Spring-damper systems
- IK (inverse kinematics)
- Blend trees and blend spaces

### Mathematics
- Quaternions vs. Euler angles
- Coordinate space transformations
- Vector projections
- Interpolation methods

### Architecture & Patterns
- System ownership and lifecycle
- Reactive vs. authoritative systems
- Layer ordering and dependencies
- State management patterns

### Physics & Feel
- Spring-damper tuning
- Critically damped vs. underdamped
- Velocity scaling and momentum
- Procedural animation principles

### Game Design Theory
- Knowledge creation methods
- Iteration philosophy
- Emergence vs. scripting
- Feel vs. simulation

---

## Integration with Workflow

This is a **meta-task** like RETROSPECTIVE - not part of feature workflow, but **feeds into** all workflows.

**Can be invoked:**
- Before planning (to understand approach options)
- During implementation (when encountering unfamiliar technique)
- After completion (to document learnings for future)
- Anytime (to build intellectual capital proactively)

**Feeds into:**
- Feature planning (informed by documented knowledge)
- Code reviews (shared understanding of techniques)
- Future iterations (reusable knowledge base)
- Team discussions (common vocabulary and mental models)

**Relationship to workflows:**
```
UNDERSTAND_CONCEPT (meta-task: knowledge creation)
        ↓
    [Knowledge stored in NOTES/]
        ↓
    Informs all workflows:
    - FEATURE (implementation approach)
    - IMPROVE (code quality decisions, pattern recognition)
    - RETROSPECTIVE (process improvements)
```

---

## Tone & Constraints

- **Conversational:** Explain like teaching a colleague, not writing a textbook
- **Concrete:** Use examples from current codebase, not abstract theory
- **Visual:** ASCII diagrams, timelines, comparison tables
- **Progressive:** Simple → complex, building on previous understanding
- **Practical:** Focus on "when to use" and "how it fits"
- **Exploratory:** Allow tangents, follow interesting threads
- **Honest:** Document uncertainties and open questions

From *Designing Games*:
> "Knowledge of economics, history, Nepalese culture, or fly-fishing techniques could all form part of a creative solution to a design problem. Humans think by analogy, so our thoughts are enriched when we have more knowledge from which to analogize."

Don't prematurely narrow scope - unexpected connections often yield best insights.

---

## When to Stop

Concept understanding is sufficient when:
- Can explain the technique in your own words
- Order of operations and system interactions clear
- Decision criteria for implementation choices understood
- Edge cases and future extensions explored
- Documentation provides reference for later work

**Do NOT:**
- Dive into implementation (save for actual feature work)
- Get stuck in theoretical details unrelated to practical use
- Create documentation duplicating existing references (link instead)
- Force understanding when more rumination needed (allow unconscious processing)

---

## Success Criteria

- ✅ Clear, intuitive explanation documented in NOTES/
- ✅ Visual diagrams and concrete examples included
- ✅ System interaction ordering understood
- ✅ Decision criteria for approaches established
- ✅ References linked from relevant documents
- ✅ Intellectual capital created for future problem-solving

**Result:** Expanded knowledge base that feeds rumination and informs future design decisions.

---

## Philosophy

From *Designing Games*:
> "The hard part of game design is not physically implementing the game. It is inventing and refining knowledge about the design."

This task recognizes that **knowledge creation is work**. Time spent understanding concepts deeply is not wasted - it's the actual work of design. Like the Wright brothers testing airfoils and building wind tunnels, we're inventing the tools we need to invent the game.

**This is therapeutic planning at its best:** Converting anxiety about unfamiliar techniques into concrete understanding and actionable knowledge.
