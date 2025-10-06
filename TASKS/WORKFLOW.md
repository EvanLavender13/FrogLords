# Task Workflow

This diagram shows the complete iteration workflow and task dependencies.

```mermaid
graph TD
    A[NEXT_FEATURE] --> B[PLAN_ITERATION]
    B --> C[DECOMPOSE_PLAN]
    C --> D[REVIEW_PLAN]
    D --> E{Issues?}
    E -->|Violations/Major| F[DEFER_FEATURE]
    E -->|Minor| G[Revise Plans]
    G --> B
    E -->|No| H[IMPLEMENTATION_STEP]
    H --> I{More Steps?}
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
    
    style A fill:#e1f5ff
    style B fill:#e1f5ff
    style C fill:#e1f5ff
    style D fill:#e1f5ff
    style F fill:#ffe1e1
    style H fill:#e1f5ff
    style J fill:#e1f5ff
    style M fill:#e1f5ff
    style N fill:#e1f5ff
    style O fill:#e1f5ff
    style P fill:#e1f5ff
```

## Existing Tasks

### Primary Workflow
- **NEXT_FEATURE**: Identify next feature from backlog based on dependencies and certainty
- **PLAN_ITERATION**: Create detailed iteration plan with graybox scope and testing protocol
- **DECOMPOSE_PLAN**: Break down iteration plan into atomic implementation steps
- **REVIEW_PLAN**: Check iteration plan against principles for violations and misalignments
- **IMPLEMENTATION_STEP**: Execute one major step from implementation checklist
- **REVIEW_IMPLEMENTATION**: Verify code against standards and principles; approve if passing
- **FINALIZE_ITERATION**: Update stack, backlog, archive documents; prepare for next feature

### Alternative Path
- **DEFER_FEATURE**: Cleanly back out of premature/unnecessary features identified during planning phase

### Supporting Tasks
- **UPDATE_DEPENDENCIES**: Update DEPENDENCY_STACK.md with new certainty scores
- **UPDATE_BACKLOG**: Mark features complete/deferred, document learnings, update dependent items
- **ARCHIVE_ITERATION**: Move planning documents to ARCHIVE with timestamp prefix
- **COMMIT**: Format and create git commits following project conventions
- **MERGE**: Merge feature branch to main using fast-forward only

## Potential Future Tasks

- **BUILD_TEST**: Formalized quality gate execution (build, lint, format, smoke test)
- **PLAYTEST_GRAYBOX**: Execute testing protocol with target sample size and metrics
- **REFACTOR**: Extract repeated patterns into systems; clean up proven code
- **TUNE_PARAMETERS**: Real-time parameter tuning session to achieve desired feel

## Workflow Paths

### Success Path
1. NEXT_FEATURE → PLAN_ITERATION → DECOMPOSE_PLAN → REVIEW_PLAN (pass)
2. IMPLEMENTATION_STEP (loop until complete)
3. REVIEW_IMPLEMENTATION (pass) → FINALIZE_ITERATION
4. UPDATE_DEPENDENCIES + UPDATE_BACKLOG + ARCHIVE_ITERATION → Manual Git → NEXT_FEATURE

### Deferral Path (Planning Phase)
1. NEXT_FEATURE → PLAN_ITERATION → DECOMPOSE_PLAN → REVIEW_PLAN (violations/major issues)
2. DEFER_FEATURE
3. UPDATE_BACKLOG + UPDATE_DEPENDENCIES + ARCHIVE_ITERATION → Manual Git → NEXT_FEATURE

### Revision Path
1. REVIEW_PLAN identifies minor issues → Revise Plans → Re-review
2. REVIEW_IMPLEMENTATION identifies issues → Fix Code → Re-implement

## Notes

- Short loops: implementation should be tight (hours to days)
- Interruptibility: revision loops at multiple decision points
- Deferral is cheap: celebrate avoiding expensive mistakes during planning
- Bottom-up: dependency stack guides feature selection
- Knowledge capture: document learnings at finalization/deferral while fresh
- Manual git: tasks prepare but don't execute commits
