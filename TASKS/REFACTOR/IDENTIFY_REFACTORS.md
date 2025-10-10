# Identify Refactoring Opportunities

### 1. Read Guiding Principles

Read `AGENTS.md` to ensure all evaluations align with development principles, especially:
- "Abstract repeated patterns into systems; prefer parameters over assets"
- "Clarity over cleverness"
- "Simplicity over sophistication"
- "Maximize mechanic interactions; prefer multi-role tools"

### 2. Perform Deep System Analysis

1.  **Read All Source Files:** Systematically read implementation files (`.cpp`, `.h`) across all subsystems
2.  **Map System Boundaries:** Identify logical groupings and responsibilities
3.  **Analyze Patterns:** Look for recurring code patterns, similar algorithms, and duplicated logic
4.  **Evaluate Abstractions:** Assess whether current abstractions are at the right level

### 3. Identify Refactoring Opportunities

Look for architectural improvements across these categories:

#### Pattern Extraction
- **Repeated logic:** Same code pattern appearing 3+ times that could become a reusable function/system
- **Similar algorithms:** Different implementations solving similar problems (unify with parameters)
- **Duplicated constants:** Same magic numbers/strings used across files (extract to shared constants)
- **Parallel structures:** Multiple systems with identical shape (template or base class opportunity)

#### Simplification
- **Over-engineered solutions:** Complex implementations where simpler approaches would work
- **Deep nesting:** Logic buried in 4+ levels of conditionals/loops (extract functions, early returns)
- **Long functions:** Functions exceeding ~50 lines that could be decomposed
- **State machine complexity:** Complex state tracking that could use simpler boolean flags
- **Premature abstraction:** Overly generic code used in only one place (inline and simplify)

#### System Design
- **Missing abstractions:** Common concepts not captured as types (e.g., velocity + position → motion_state)
- **Leaky abstractions:** Implementation details exposed in interfaces
- **Tight coupling:** Systems that know too much about each other's internals
- **Dependency inversions:** High-level code depending on low-level details (should be reversed)
- **Single Responsibility violations:** Classes/functions doing multiple unrelated things

#### Reusable Utilities
- **Math operations:** Repeated calculations that could become foundation utilities
- **Data transformations:** Common conversions/mappings used across systems
- **Validation logic:** Similar checks duplicated across entry points
- **Interpolation/easing:** Custom curves that could use spring_damper or easing utilities
- **Collection operations:** Manual loops that could use standard algorithms

#### API Design
- **Inconsistent interfaces:** Similar functions with different parameter orders or naming
- **Boolean parameters:** Functions with multiple bools (consider enum or separate functions)
- **Output parameters:** Functions using pointers/references for output (consider return values)
- **Asymmetric operations:** Init without cleanup, open without close (ensure pairs exist)
- **Verbose call sites:** Usage requiring lots of boilerplate (simplify with defaults or builders)

### 4. Evaluate Each Opportunity

For each potential refactor, assess:

1.  **Wait for Third Use:** Has this pattern appeared 3+ times, or is it premature?
2.  **Complexity vs. Value:** Does the refactor genuinely simplify, or just move complexity?
3.  **Stability:** Is the system stable enough to refactor, or still evolving rapidly?
4.  **Risk:** What's the blast radius if the refactor introduces bugs?
5.  **Testability:** Can we verify correctness after the change (manual testing protocol)?

### 5. Document in Refactor Backlog

1.  **Target File:** `PLANS/REFACTOR_BACKLOG.md`
2.  **Format:** Use structured entries with rationale, impact assessment, and approach
3.  **Group by Category:** Pattern Extraction, Simplification, System Design, Utilities, API Design
4.  **Priority Tiers:** Critical (blocking new work), High (significant improvement), Medium (nice-to-have), Low (polish)
5.  **Update Existing:** Append new findings if backlog exists; create if not

### 6. Propose High-Impact Refactors

If any **Critical** or **High** priority refactors are identified:

1.  Summarize the opportunity and expected benefits
2.  Estimate complexity and risk
3.  Suggest whether to tackle immediately or add to backlog

Otherwise, inform the user of total opportunities found and await guidance.

### Tone & Constraints

-   Concise and direct; no preamble or postamble
-   Focus on measurable improvements: fewer lines, clearer intent, easier to extend
-   Cite specific code examples and file paths
-   Be honest about trade-offs (every abstraction has a cost)
-   Favor "rule of three" - wait for third use before abstracting
-   Detail scales with priority (critical items get deep analysis)
-   Consider certainty from dependency stack (don't refactor uncertain systems)
-   Do not propose changes during analysis phase; document opportunities first

### Anti-Patterns to Avoid

-   **Premature abstraction:** Don't extract patterns seen only once or twice
-   **Over-generalization:** Don't build for imagined futures; solve current problems
-   **Clever code:** Simplicity beats cleverness; prefer obvious over compact
-   **Refactoring unstable code:** Wait until systems reach 70%+ certainty before major refactors
-   **Churn for churn's sake:** Only refactor when concrete benefits are clear

---

### Refactor Backlog Template

```markdown
# Refactor Backlog

**Purpose:** Tracking for architectural improvements, pattern extraction, and system simplification opportunities that improve code quality and maintainability.

**Status:** Living document (updated after system analysis)

**Last Review:** [Date]

---

## Critical

(Blocking new work, major technical debt, or architectural flaws)

- **[Refactor Title]**
  - *Category:* Pattern Extraction | Simplification | System Design | Utilities | API Design
  - *Files:* `src/path/to/file.cpp`, `src/path/to/other.cpp`
  - *Current State:* Brief description of the problem
  - *Proposed Change:* What to refactor and how
  - *Rationale:* Why this improves the codebase (cite principles)
  - *Impact:* What systems are affected
  - *Risk:* Low/Medium/High - blast radius and testing difficulty
  - *Certainty:* Is this system stable enough to refactor? (check dependency stack)
  - *Examples:* Specific code snippets showing the pattern (optional)

## High

(Significant improvement to clarity, maintainability, or extensibility)

## Medium

(Nice-to-have refactors that would improve quality)

## Low

(Polish items with minimal impact)

---

## Deferred

(Opportunities that failed "wait for third use" or stability checks)

- **[Refactor Title]**
  - *Reason:* Why this was deferred (e.g., "Only 2 uses", "System at 40% certainty")
  - *Reconsider When:* Conditions that would make this viable

---

## Completed

(Finished refactors moved here for reference; prune periodically)
```
