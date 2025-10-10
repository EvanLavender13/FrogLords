# Task Workflows

This document provides an overview of all task workflows. Each workflow has its own directory with specific task definitions.

## Workflow Types

### Feature Development Workflow
**Location:** `TASKS/FEATURE/`
**Purpose:** Full iteration workflow for implementing new features from the design backlog
**Entry Point:** NEXT_FEATURE
**See:** Feature Development Workflow section below for detailed diagram and paths

### Maintenance Workflow
**Location:** `TASKS/MAINTENANCE/`
**Purpose:** Quick fixes and code quality improvements from maintenance backlog
**Entry Point:** PICK_ITEM
**See:** [MAINTENANCE/WORKFLOW.md](MAINTENANCE/WORKFLOW.md) for detailed diagram and paths

### Shared Utility Tasks
**Location:** `TASKS/` (root)
- **COMMIT**: Format and create git commits following project conventions
- **UPDATE_DEPENDENCIES**: Update DEPENDENCY_STACK.md with new certainty scores
- **UPDATE_BACKLOG**: Mark features complete/deferred, document learnings
- **ARCHIVE_ITERATION**: Move planning documents to ARCHIVE with timestamp prefix
- **REVIEW_CODEBASE**: Random walk through src/ to populate MAINTENANCE_BACKLOG.md
- **IDENTIFY_REFACTORS**: Deep system analysis to populate REFACTOR_BACKLOG.md
- **RETROSPECTIVE**: Periodic process review to identify patterns and improve workflow (run every 5-10 iterations)

---

# Feature Development Workflow (Detailed)

This diagram shows the complete iteration workflow and task dependencies.

```mermaid
graph TD
    REQ[REQUEST_FEATURE] --> A[NEXT_FEATURE]
    A --> CLAR{Needs<br/>Clarity?}
    CLAR -->|Yes| CF[CLARIFY_FEATURE]
    CF --> B[PLAN_ITERATION]
    CLAR -->|No| B
    B --> C[DECOMPOSE_PLAN]
    C --> D[REVIEW_PLAN]
    D --> E{Issues?}
    E -->|Violations/Major| F[DEFER_FEATURE]
    E -->|Minor| G[Revise Plans]
    G --> B
    E -->|No| H[IMPLEMENTATION_STEP]
    H --> RS[REVIEW_STEP]
    RS --> RSI{Issues?}
    RSI -->|Yes| L2[Fix Code]
    L2 --> H
    RSI -->|No| I{More Steps?}
    I -->|Yes| H
    I -->|No| J[REVIEW_IMPLEMENTATION]
    J --> K{Issues?}
    K -->|Yes| L[Fix Code]
    L --> H
    K -->|No| M[FINALIZE_ITERATION]
    M --> N[UPDATE_DEPENDENCIES]
    M --> O[UPDATE_BACKLOG]
    M --> P[ARCHIVE_ITERATION]
    N --> Q[Manual Git]
    O --> Q
    P --> Q
    Q --> A
    F --> R[UPDATE_BACKLOG]
    F --> S[UPDATE_DEPENDENCIES]
    F --> T[ARCHIVE_ITERATION]
    R --> U[Manual Git]
    S --> U
    T --> U
    U --> A
    
    %% Mid-iteration scope changes
    H --> AS[ADD_SCOPE]
    AS --> B
    B --> MP[MODIFY_PLAN]
    MP --> C
    
    %% Implementation-phase deferral
    H --> F
    RS --> F
    J --> F
    
    style REQ fill:#d4edda
    style A fill:#e1f5ff
    style CF fill:#fff3cd
    style B fill:#e1f5ff
    style C fill:#e1f5ff
    style D fill:#e1f5ff
    style F fill:#ffe1e1
    style H fill:#e1f5ff
    style RS fill:#e1f5ff
    style J fill:#e1f5ff
    style M fill:#e1f5ff
    style N fill:#e1f5ff
    style O fill:#e1f5ff
    style P fill:#e1f5ff
    style AS fill:#fff3cd
    style MP fill:#fff3cd
```

## Feature Tasks (TASKS/FEATURE/)

### Pre-Workflow
- **REQUEST_FEATURE**: Propose a new feature for evaluation and backlog entry

### Primary Workflow
- **NEXT_FEATURE**: Identify next feature from backlog based on dependencies and certainty; write high-level feature description and save to `PLANS/feature_<feature_name>.md`
- **CLARIFY_FEATURE**: (Optional) Eliminate ambiguity by asking targeted questions before planning; captures visual references and concrete requirements
- **PLAN_ITERATION**: Create detailed iteration plan from feature description with graybox scope and testing protocol
- **DECOMPOSE_PLAN**: Break down iteration plan into atomic implementation steps; update existing plan with changelog if it exists
- **REVIEW_PLAN**: Check iteration and implementation plans against principles; prepend changelog entry on re-review
- **IMPLEMENTATION_STEP**: Execute one major step from implementation checklist; mark completed items and track changed files
- **REVIEW_STEP**: Verify last completed step against plan alignment, code standards, and gameplay-first principles; provide inline feedback
- **REVIEW_IMPLEMENTATION**: Verify code against standards and principles; approve if passing
- **FINALIZE_ITERATION**: Update stack, backlog, archive documents; prepare for next feature

### Mid-Iteration Adjustments
- **ADD_SCOPE**: Add requirements to an active iteration plan (triggers re-decompose)
- **MODIFY_PLAN**: Update implementation plan to match modified iteration scope

### Alternative Path
- **DEFER_FEATURE**: Cleanly back out of premature/unnecessary features identified during planning or implementation phase

## Meta-Process Tasks

- **RETROSPECTIVE**: Review last N iterations to identify workflow patterns, extract learnings, and propose process improvements (run every 5-10 iterations)

## Potential Future Tasks

- **BUILD_TEST**: Formalized quality gate execution (build, lint, format, smoke test)
- **PLAYTEST_GRAYBOX**: Execute testing protocol with target sample size and metrics
- **REFACTOR**: Extract repeated patterns into systems; clean up proven code
- **TUNE_PARAMETERS**: Real-time parameter tuning session to achieve desired feel

## Workflow Paths

### Feature Request Path
1. REQUEST_FEATURE → Evaluate against principles & dependencies
2. Add to backlog if viable → Eventually pulled via NEXT_FEATURE

### Success Path
1. NEXT_FEATURE → (Optional: CLARIFY_FEATURE) → PLAN_ITERATION → DECOMPOSE_PLAN → REVIEW_PLAN (pass)
2. IMPLEMENTATION_STEP → REVIEW_STEP (loop until complete)
3. REVIEW_IMPLEMENTATION (pass) → FINALIZE_ITERATION
4. UPDATE_DEPENDENCIES + UPDATE_BACKLOG + ARCHIVE_ITERATION → Manual Git → NEXT_FEATURE

### Deferral Path (Planning Phase)
1. NEXT_FEATURE → (Optional: CLARIFY_FEATURE) → PLAN_ITERATION → DECOMPOSE_PLAN → REVIEW_PLAN (violations/major issues)
2. DEFER_FEATURE
3. UPDATE_BACKLOG + UPDATE_DEPENDENCIES + ARCHIVE_ITERATION → Manual Git → NEXT_FEATURE

### Deferral Path (Implementation Phase)
1. During IMPLEMENTATION_STEP, REVIEW_STEP, or REVIEW_IMPLEMENTATION, discover blockers/missing prerequisites
2. DEFER_FEATURE
3. UPDATE_BACKLOG + UPDATE_DEPENDENCIES + ARCHIVE_ITERATION → Manual Git → NEXT_FEATURE

### Revision Path
1. REVIEW_PLAN identifies minor issues → Revise Plans → Re-review (prepends changelog to review)
2. REVIEW_STEP identifies issues → Fix Code → Re-implement step
3. REVIEW_IMPLEMENTATION identifies issues → Fix Code → Re-implement

### Mid-Iteration Scope Change Path
1. During IMPLEMENTATION_STEP, discover need for scope adjustment
2. ADD_SCOPE → Update iteration plan with changelog entry
3. MODIFY_PLAN → Update implementation plan with changelog entry
4. DECOMPOSE_PLAN → Synchronize implementation steps (adds changelog if updating existing plan)
5. Return to IMPLEMENTATION_STEP with synchronized plans

## Notes

- Short loops: implementation should be tight (hours to days)
- Interruptibility: revision loops at multiple decision points
- Clarification upfront: use CLARIFY_FEATURE when ambiguity exists; visual references prevent wasted iteration loops
- Deferral is cheap: celebrate avoiding expensive mistakes during planning or implementation
- Deferral phases: planning phase (principle violations, premature features) and implementation phase (missing prerequisites, blockers discovered)
- Bottom-up: dependency stack guides feature selection
- Knowledge capture: document learnings at finalization/deferral while fresh
- Manual git: tasks prepare but don't execute commits
- Scope changes: ADD_SCOPE + MODIFY_PLAN + DECOMPOSE_PLAN keep plans synchronized when discovery requires mid-iteration adjustment
- Feature requests: REQUEST_FEATURE formalizes backlog entry with principle alignment and dependency analysis
- Plan versioning: DECOMPOSE_PLAN and REVIEW_PLAN add changelog entries when updating existing plans; IMPLEMENTATION_STEP tracks changed files
- Re-reviews: REVIEW_PLAN prepends changelog entries on re-review to maintain historical context
- Retrospectives: Run RETROSPECTIVE every 5-10 iterations to review archived documents, identify patterns in what works/fails, and propose workflow improvements

## Proposed Numbering Scheme

Once workflow stabilizes, consider renaming files with 3-digit prefixes to provide visual roadmap while allowing growth:

### Primary Flow (000-099)
- **000_REQUEST_FEATURE** - Entry point: propose new feature
- **001_NEXT_FEATURE** - Select from backlog
- **002_CLARIFY_FEATURE** - Optional: eliminate ambiguity
- **003_PLAN_ITERATION** - Create iteration plan
- **004_DECOMPOSE_PLAN** - Break into atomic steps
- **005_REVIEW_PLAN** - Validate plans against principles
- **006_IMPLEMENTATION_STEP** - Execute one step
- **007_REVIEW_STEP** - Validate completed step
- **008_REVIEW_IMPLEMENTATION** - Final code review
- **009_FINALIZE_ITERATION** - Wrap up and prepare next

### Mid-Iteration Adjustments (100-199)
- **100_ADD_SCOPE** - Add requirements to active iteration
- **101_MODIFY_PLAN** - Update implementation plan

### Alternative Paths (200-299)
- **200_DEFER_FEATURE** - Clean exit from premature features

### Finalization Tasks (300-399)
- **300_UPDATE_DEPENDENCIES** - Update DEPENDENCY_STACK.md
- **301_UPDATE_BACKLOG** - Mark complete/deferred, document learnings
- **302_ARCHIVE_ITERATION** - Move plans to ARCHIVE

### Version Control (400-499)
- **400_COMMIT** - Format and create git commits

Three-digit prefixes group by hundreds (0xx=main path, 1xx=adjustments, 2xx=alternatives, 3xx=cleanup, 4xx=git). Sequential numbering within each hundred allows up to 100 tasks per group. Defer renaming until workflow proves stable.

