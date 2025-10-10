# Task Workflows

This document provides an overview of all task workflows and how backlogs feed into each workflow.

## Workflow Overview

```mermaid
graph TB
    subgraph Backlogs["PLANS/ - Backlogs"]
        DB[DESIGN_BACKLOG]
        MB[MAINTENANCE_BACKLOG]
        RB[REFACTOR_BACKLOG]
    end

    subgraph FeatureTasks["FEATURE/ - Feature Workflow"]
        RF[REQUEST_FEATURE]
        NF[NEXT_FEATURE]
        FW[Feature Tasks<br/>PLAN → DECOMPOSE → REVIEW<br/>IMPLEMENT → FINALIZE]
        UB[UPDATE_BACKLOG]
        UD[UPDATE_DEPENDENCIES]
    end

    subgraph MaintenanceTasks["MAINTENANCE/ - Maintenance Workflow"]
        RC[REVIEW_CODEBASE]
        PI[PICK_ITEM]
        MW[Maintenance Tasks<br/>IMPLEMENT → REVIEW → FINALIZE]
    end

    subgraph RefactorTasks["REFACTOR/ - Refactor Workflow"]
        IR[IDENTIFY_REFACTORS]
        RW[Refactor Tasks<br/>TBD]
    end

    %% Feature workflow flows
    RF -->|add new ideas| DB
    DB -->|select next| NF
    NF --> FW
    FW --> UB
    FW --> UD
    UB -->|update status| DB

    %% Maintenance workflow flows
    RC -->|populate items| MB
    MB -->|select item| PI
    PI --> MW
    MW -->|mark complete| MB

    %% Refactor workflow flows
    IR -->|populate opportunities| RB
    RB -.->|ad-hoc selection| RW
    RW -.->|update status| RB

    style DB fill:#e1f5ff
    style MB fill:#fff3cd
    style RB fill:#ffe1e1
    style FW fill:#d4edda
    style MW fill:#d4edda
    style RW fill:#f0f0f0
    style NF fill:#b8e6b8
    style PI fill:#b8e6b8
    style IR fill:#b8e6b8
    style RF fill:#e1f5ff
    style RC fill:#fff3cd
    style UB fill:#e1f5ff
```

### Feature Development Workflow
- **Location:** `TASKS/FEATURE/`
- **Purpose:** Full iteration workflow for implementing new features
- **Entry Point:** NEXT_FEATURE
- **Input Backlog:** [PLANS/DESIGN_BACKLOG.md](../PLANS/DESIGN_BACKLOG.md)
- **Details:** [FEATURE/WORKFLOW.md](FEATURE/WORKFLOW.md)

### Maintenance Workflow
- **Location:** `TASKS/MAINTENANCE/`
- **Purpose:** Quick fixes and code quality improvements
- **Entry Point:** PICK_ITEM
- **Input Backlog:** [PLANS/MAINTENANCE_BACKLOG.md](../PLANS/MAINTENANCE_BACKLOG.md)
- **Details:** [MAINTENANCE/WORKFLOW.md](MAINTENANCE/WORKFLOW.md)

### Refactor Workflow
- **Location:** `TASKS/REFACTOR/`
- **Purpose:** Architectural improvements and pattern extraction
- **Entry Point:** IDENTIFY_REFACTORS
- **Input Backlog:** [PLANS/REFACTOR_BACKLOG.md](../PLANS/REFACTOR_BACKLOG.md)
- **Status:** Backlog and entry task exist, full workflow not yet formalized

---

## Backlog-to-Workflow Mapping

### Design Backlog → Feature Workflow
**Source:** [PLANS/DESIGN_BACKLOG.md](../PLANS/DESIGN_BACKLOG.md)
**Workflow:** [FEATURE/WORKFLOW.md](FEATURE/WORKFLOW.md)
**Process:**
1. Items mature in DESIGN_BACKLOG with certainty scores and prerequisites
2. NEXT_FEATURE task selects ready items based on dependency stack
3. Feature flows through planning → implementation → finalization
4. UPDATE_BACKLOG marks items complete/deferred with learnings

**Key Tasks:**
- REQUEST_FEATURE (add new items)
- NEXT_FEATURE (select from backlog)
- UPDATE_BACKLOG (mark complete/deferred)

### Maintenance Backlog → Maintenance Workflow
**Source:** [PLANS/MAINTENANCE_BACKLOG.md](../PLANS/MAINTENANCE_BACKLOG.md)
**Workflow:** [MAINTENANCE/WORKFLOW.md](MAINTENANCE/WORKFLOW.md)
**Process:**
1. REVIEW_CODEBASE populates MAINTENANCE_BACKLOG with code quality items
2. Items prioritized by severity (Critical → High → Medium → Low)
3. PICK_ITEM selects appropriate fixes based on available time
4. Quick fix-test-commit cycle

**Key Tasks:**
- REVIEW_CODEBASE (populate backlog)
- PICK_ITEM (select from backlog)
- Maintenance workflow tasks (see MAINTENANCE/WORKFLOW.md)

### Refactor Backlog → Refactor Workflow
**Source:** [PLANS/REFACTOR_BACKLOG.md](../PLANS/REFACTOR_BACKLOG.md)
**Workflow:** Not yet formalized (entry task exists)
**Process:**
1. IDENTIFY_REFACTORS populates REFACTOR_BACKLOG with pattern extraction opportunities
2. Items mature when systems reach 90%+ stability
3. Follows "rule of three" (3+ uses) before extraction
4. Currently handled ad-hoc or during maintenance windows

**Key Tasks:**
- IDENTIFY_REFACTORS (populate backlog)
- (Future: formalized refactor workflow tasks)

---

## Shared Utility Tasks

**Location:** `TASKS/` (root)

These tasks support all workflows:

- **COMMIT**: Format and create git commits following project conventions
- **RETROSPECTIVE**: Periodic process review to identify patterns and improve workflow (run every 5-10 iterations)

## Workflow-Specific Tasks

### Feature Workflow (`TASKS/FEATURE/`)
- **UPDATE_DEPENDENCIES**: Update DEPENDENCY_STACK.md with new certainty scores
- **UPDATE_BACKLOG**: Mark features complete/deferred in DESIGN_BACKLOG, document learnings

### Maintenance Workflow (`TASKS/MAINTENANCE/`)
- **REVIEW_CODEBASE**: Random walk through src/ to populate MAINTENANCE_BACKLOG.md

### Refactor Workflow (`TASKS/REFACTOR/`)
- **IDENTIFY_REFACTORS**: Deep system analysis to populate REFACTOR_BACKLOG.md

---

## Meta-Process Tasks

- **RETROSPECTIVE**: Review last N iterations to identify workflow patterns, extract learnings, and propose process improvements (run every 5-10 iterations)

## Potential Future Tasks

- **BUILD_TEST**: Formalized quality gate execution (build, lint, format, smoke test)
- **PLAYTEST_GRAYBOX**: Execute testing protocol with target sample size and metrics
- **REFACTOR**: Extract repeated patterns into systems; clean up proven code
- **TUNE_PARAMETERS**: Real-time parameter tuning session to achieve desired feel

