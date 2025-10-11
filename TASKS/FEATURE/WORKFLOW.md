# Feature Development Workflow

This document provides the detailed workflow diagram and task descriptions for the feature development process.

## Workflow Diagram

This diagram shows the complete feature workflow and task dependencies.

```mermaid
graph TD
    REQ[REQUEST_FEATURE] --> A[SELECT_FEATURE]
    A --> CLAR{Needs<br/>Clarity?}
    CLAR -->|Yes| CF[CLARIFY_FEATURE]
    CF --> C[PLAN_IMPLEMENTATION]
    CLAR -->|No| C
    C --> D[REVIEW_PLAN]
    D --> E{Issues?}
    E -->|Violations/Major| F[DEFER_FEATURE]
    E -->|Minor| G[Revise Plans]
    G --> C
    E -->|No| H[IMPLEMENTATION_STEP<br/>with Self-Verify]
    H --> I{More Steps?}
    I -->|Yes| H
    I -->|No| J[REVIEW_IMPLEMENTATION]
    J --> K{Issues?}
    K -->|Yes| L[Fix Code]
    L --> H
    K -->|No| M[FINALIZE_FEATURE]
    M --> N[UPDATE_DEPENDENCIES]
    M --> O[UPDATE_BACKLOG]
    N --> Q[Manual Git]
    O --> Q
    Q --> A
    F --> R[UPDATE_BACKLOG]
    F --> S[UPDATE_DEPENDENCIES]
    R --> U[Manual Git]
    S --> U
    U --> A

    %% Mid-feature scope changes
    H --> AS[ADD_SCOPE]
    AS --> MP[MODIFY_PLAN]
    MP --> C

    %% Implementation-phase deferral
    H --> F
    J --> F

    style REQ fill:#d4edda
    style A fill:#e1f5ff
    style CF fill:#fff3cd
    style C fill:#e1f5ff
    style D fill:#e1f5ff
    style F fill:#ffe1e1
    style H fill:#e1f5ff
    style J fill:#e1f5ff
    style M fill:#e1f5ff
    style N fill:#e1f5ff
    style O fill:#e1f5ff
    style AS fill:#fff3cd
    style MP fill:#fff3cd
```

## Feature Tasks

### Pre-Workflow
- **REQUEST_FEATURE**: Propose a new feature for evaluation and backlog entry

### Primary Workflow
- **SELECT_FEATURE**: Identify next feature from backlog based on dependencies and certainty; create feature branch; write comprehensive feature description with graybox scope, implementation approach, and testing protocol; save to `PLANS/feature_<feature_name>.md`
- **CLARIFY_FEATURE**: (Optional) Eliminate ambiguity by asking targeted questions before implementation planning; captures visual references and concrete requirements
- **PLAN_IMPLEMENTATION**: Break down feature description into atomic implementation steps with detailed system audit; update existing plan with changelog if it exists
- **REVIEW_PLAN**: Check implementation plan against principles; prepend changelog entry on re-review
- **IMPLEMENTATION_STEP**: Execute one major step from implementation checklist; run quality gates (build, format, smoke test); self-verify against principles; mark completed items and track changed files
- **REVIEW_IMPLEMENTATION**: Comprehensive final code review against standards and principles; approve if passing
- **FINALIZE_FEATURE**: Update stack, backlog, archive documents; prepare for next feature

### Mid-Feature Adjustments
- **ADD_SCOPE**: Add requirements to an active feature (triggers plan update)
- **MODIFY_PLAN**: Update implementation plan to match modified feature scope

### Alternative Path
- **DEFER_FEATURE**: Cleanly back out of premature/unnecessary features identified during planning or implementation phase

## Workflow Paths

### Feature Request Path
1. REQUEST_FEATURE → Evaluate against principles & dependencies
2. Add to backlog if viable → Eventually pulled via SELECT_FEATURE

### Success Path
1. SELECT_FEATURE → (Optional: CLARIFY_FEATURE) → PLAN_IMPLEMENTATION → REVIEW_PLAN (pass)
2. IMPLEMENTATION_STEP (loop until complete, with self-verification)
3. REVIEW_IMPLEMENTATION (pass) → FINALIZE_FEATURE
4. UPDATE_DEPENDENCIES + UPDATE_BACKLOG → Manual Git → SELECT_FEATURE

### Deferral Path (Planning Phase)
1. SELECT_FEATURE → (Optional: CLARIFY_FEATURE) → PLAN_IMPLEMENTATION → REVIEW_PLAN (violations/major issues)
2. DEFER_FEATURE
3. UPDATE_BACKLOG + UPDATE_DEPENDENCIES → Manual Git → SELECT_FEATURE

### Deferral Path (Implementation Phase)
1. During IMPLEMENTATION_STEP or REVIEW_IMPLEMENTATION, discover blockers/missing prerequisites
2. DEFER_FEATURE
3. UPDATE_BACKLOG + UPDATE_DEPENDENCIES → Manual Git → SELECT_FEATURE

### Revision Path
1. REVIEW_PLAN identifies minor issues → Revise Plans → Re-review (prepends changelog to review)
2. IMPLEMENTATION_STEP self-verification identifies issues → Fix immediately before marking complete
3. REVIEW_IMPLEMENTATION identifies issues → Fix Code → Re-implement step

### Mid-Feature Scope Change Path
1. During IMPLEMENTATION_STEP, discover need for scope adjustment
2. ADD_SCOPE → Update feature description with changelog entry
3. MODIFY_PLAN → Update implementation plan with changelog entry
4. Return to IMPLEMENTATION_STEP with synchronized plans

## Notes

- Short loops: implementation should be tight (1-3 complexity points)
- Interruptibility: revision loops at multiple decision points
- Clarification upfront: use CLARIFY_FEATURE when ambiguity exists; visual references prevent wasted iteration loops
- Deferral is cheap: celebrate avoiding expensive mistakes during planning or implementation
- Deferral phases: planning phase (principle violations, premature features) and implementation phase (missing prerequisites, blockers discovered)
- Bottom-up: dependency stack guides feature selection
- **Problem evidence required:** Features with backlog warnings ("may not be necessary," "low priority") require extra scrutiny during SELECT_FEATURE selection. Document specific gameplay moments or observations that motivate the feature before planning.
- Knowledge capture: document learnings at finalization/deferral while fresh
- Manual git: tasks prepare but don't execute commits
- Scope changes: ADD_SCOPE + MODIFY_PLAN keep plans synchronized when discovery requires mid-feature adjustment
- Feature requests: REQUEST_FEATURE formalizes backlog entry with principle alignment and dependency analysis
- Plan versioning: PLAN_IMPLEMENTATION and REVIEW_PLAN add changelog entries when updating existing plans; IMPLEMENTATION_STEP tracks changed files
- Re-reviews: REVIEW_PLAN prepends changelog entries on re-review to maintain historical context
- Retrospectives: Run RETROSPECTIVE every 5-10 iterations to review archived documents, identify patterns in what works/fails, and propose workflow improvements
- **Certainty calibration:** FINALIZE_FEATURE now tracks predicted vs actual certainty to improve planning accuracy over time
- **Backlog grooming:** Run BACKLOG_GROOMING periodically (every 2-3 iterations) to collapse completed work, prune low-certainty items, and refresh unlocked dependencies; keeps planning docs focused on future work rather than retrospectives
- **Organic scope evolution:** Debug tool scope growth acceptable if: (1) same session completion, (2) zero gameplay impact, (3) architectural improvements discovered. Scope can grow up to 2x estimate under these conditions without being considered harmful "scope creep." Example: Camera zoom (30→90 min) creating unified Debug Panel architecture. Distinguish from harmful feature bloat (multi-session coordination, gameplay changes, gold-plating).

## Proposed Numbering Scheme

Once workflow stabilizes, consider renaming files with 3-digit prefixes to provide visual roadmap while allowing growth:

### Primary Flow (000-099)
- **000_REQUEST_FEATURE** - Entry point: propose new feature
- **001_SELECT_FEATURE** - Select from backlog, create branch, write feature description
- **002_CLARIFY_FEATURE** - Optional: eliminate ambiguity
- **003_PLAN_IMPLEMENTATION** - Break into atomic steps with system audit
- **004_REVIEW_PLAN** - Validate plans against principles
- **005_IMPLEMENTATION_STEP** - Execute one step with self-verification and quality gates
- **006_REVIEW_IMPLEMENTATION** - Comprehensive final code review
- **007_FINALIZE_FEATURE** - Wrap up and prepare next

### Mid-Feature Adjustments (100-199)
- **100_ADD_SCOPE** - Add requirements to active feature
- **101_MODIFY_PLAN** - Update implementation plan

### Alternative Paths (200-299)
- **200_DEFER_FEATURE** - Clean exit from premature features

### Finalization Tasks (300-399)
- **300_UPDATE_DEPENDENCIES** - Update DEPENDENCY_STACK.md
- **301_UPDATE_BACKLOG** - Mark complete/deferred, document learnings
- **302_BACKLOG_GROOMING** - Periodic cleanup: collapse completed work, prune low-certainty items, update unlocked dependencies

### Version Control (400-499)
- **400_COMMIT** - Format and create git commits

Three-digit prefixes group by hundreds (0xx=main path, 1xx=adjustments, 2xx=alternatives, 3xx=cleanup, 4xx=git). Sequential numbering within each hundred allows up to 100 tasks per group. Defer renaming until workflow proves stable.
