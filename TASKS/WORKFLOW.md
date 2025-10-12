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
        NF[SELECT_FEATURE]
        FW[Feature Tasks<br/>PLAN → DECOMPOSE → REVIEW<br/>IMPLEMENT → FINALIZE]
        UB[UPDATE_BACKLOG]
        UD[UPDATE_DEPENDENCIES]
    end

    subgraph MaintenanceTasks["MAINTENANCE/ - Maintenance Workflow"]
        RC[REVIEW_CODEBASE]
        PI[SELECT_ITEM]
        MW[Maintenance Tasks<br/>IMPLEMENT → REVIEW → FINALIZE]
    end

    subgraph RefactorTasks["REFACTOR/ - Refactor Workflow"]
        IR[IDENTIFY_REFACTORS]
        SR[SELECT_REFACTOR]
        RW[Refactor Tasks<br/>PLAN → REVIEW → EXECUTE<br/>VERIFY → FINALIZE]
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
    RB -->|select refactor| SR
    SR --> RW
    RW -->|update status| RB

    style DB fill:#e1f5ff
    style MB fill:#fff3cd
    style RB fill:#ffe1e1
    style FW fill:#d4edda
    style MW fill:#d4edda
    style RW fill:#d4edda
    style NF fill:#b8e6b8
    style PI fill:#b8e6b8
    style IR fill:#b8e6b8
    style SR fill:#b8e6b8
    style RF fill:#e1f5ff
    style RC fill:#fff3cd
    style UB fill:#e1f5ff
```

## Document Naming Conventions

Workflow documents follow distinct naming patterns optimized for their usage:

### Feature Documents (Feature-First)
**Pattern:** `PLANS/<feature_name>_<WORKFLOW>.md`  
**Archive:** `YYYYMMDD_HHMMSS_<feature_name>_<WORKFLOW>.md`

- `<feature_name>_FEATURE.md` - Feature description and rationale
- `<feature_name>_PLAN.md` - Detailed implementation steps
- `<feature_name>_CODE_REVIEW.md` - Implementation review findings
- `<feature_name>_PLAN_REVIEW.md` - Planning review findings

**Rationale:** Features involve multi-phase iterative work. Grouping by feature name clusters all related documents together alphabetically, making it easy to see the complete arc of a feature at a glance (planning → implementation → reviews).

### Refactor Documents (Workflow-First)
**Pattern:** `PLANS/REFACTOR_<description>.md`  
**Archive:** `YYYYMMDD_HHMMSS_REFACTOR_<description>.md`

**Rationale:** Refactors are typically one-off architectural improvements. Grouping by workflow type (REFACTOR_) clusters all refactoring work together, aiding pattern recognition and retrospective analysis of architectural evolution.

### Maintenance Documents (Workflow-First)
**Pattern:** `PLANS/MAINTENANCE_<description>.md`  
**Archive:** `YYYYMMDD_HHMMSS_MAINTENANCE_<description>.md`

**Rationale:** Maintenance items are quick, isolated fixes. Grouping by workflow type (MAINTENANCE_) clusters all maintenance work together, making it easy to review code quality trends and identify systemic issues.

### Feature Development Workflow
- **Location:** `TASKS/FEATURE/`
- **Purpose:** Complete feature development workflow for implementing new features
- **Entry Point:** SELECT_FEATURE
- **Input Backlog:** [PLANS/DESIGN_BACKLOG.md](../PLANS/DESIGN_BACKLOG.md)
- **Details:** [FEATURE/WORKFLOW.md](FEATURE/WORKFLOW.md)
- **Document Approach:** Multi-file with feature-first naming (`PLANS/<name>_FEATURE.md`, `<name>_PLAN.md`, `<name>_CODE_REVIEW.md`, `<name>_PLAN_REVIEW.md`)

### Maintenance Workflow
- **Location:** `TASKS/MAINTENANCE/`
- **Purpose:** Quick fixes and code quality improvements
- **Entry Point:** SELECT_ITEM
- **Input Backlog:** [PLANS/MAINTENANCE_BACKLOG.md](../PLANS/MAINTENANCE_BACKLOG.md)
- **Details:** [MAINTENANCE/WORKFLOW.md](MAINTENANCE/WORKFLOW.md)
- **Document Approach:** Single document per fix (`PLANS/MAINTENANCE_<name>.md`) accumulates all phases

### Refactor Workflow
- **Location:** `TASKS/REFACTOR/`
- **Purpose:** Architectural improvements and pattern extraction
- **Entry Point:** SELECT_REFACTOR
- **Input Backlog:** [PLANS/REFACTOR_BACKLOG.md](../PLANS/REFACTOR_BACKLOG.md)
- **Details:** [REFACTOR/WORKFLOW.md](REFACTOR/WORKFLOW.md)
- **Document Approach:** Single document per refactor (`PLANS/REFACTOR_<name>.md`) accumulates all phases

---

## Backlog-to-Workflow Mapping

### Design Backlog → Feature Workflow
**Source:** [PLANS/DESIGN_BACKLOG.md](../PLANS/DESIGN_BACKLOG.md)
**Workflow:** [FEATURE/WORKFLOW.md](FEATURE/WORKFLOW.md)
**Process:**
1. Items mature in DESIGN_BACKLOG with certainty scores and prerequisites
2. SELECT_FEATURE task selects ready items based on dependency stack
3. Feature flows through planning → implementation → finalization
4. UPDATE_BACKLOG marks items complete/deferred with learnings

**Key Tasks:**
- REQUEST_FEATURE (add new items)
- SELECT_FEATURE (select from backlog)
- UPDATE_BACKLOG (mark complete/deferred)

### Maintenance Backlog → Maintenance Workflow
**Source:** [PLANS/MAINTENANCE_BACKLOG.md](../PLANS/MAINTENANCE_BACKLOG.md)
**Workflow:** [MAINTENANCE/WORKFLOW.md](MAINTENANCE/WORKFLOW.md)
**Process:**
1. REVIEW_CODEBASE populates MAINTENANCE_BACKLOG with code quality items
2. Items prioritized by severity (Critical → High → Medium → Low)
3. SELECT_ITEM selects appropriate fixes based on available time
4. Quick fix-test-commit cycle

**Key Tasks:**
- REVIEW_CODEBASE (populate backlog)
- SELECT_ITEM (select from backlog)
- IMPLEMENT_FIX (execute fix)
- REVIEW_FIX (Path B only - verify quality)
- FINALIZE_FIX (update backlog, document learnings, prepare commit)

### Refactor Backlog → Refactor Workflow
**Source:** [PLANS/REFACTOR_BACKLOG.md](../PLANS/REFACTOR_BACKLOG.md)
**Workflow:** [REFACTOR/WORKFLOW.md](REFACTOR/WORKFLOW.md)
**Process:**
1. IDENTIFY_REFACTORS populates REFACTOR_BACKLOG with pattern extraction opportunities
2. Items mature when systems reach ≥70% certainty (stability gate)
3. Follows "rule of three" (3+ uses) before extraction
4. SELECT_REFACTOR chooses ready items based on priority and stability
5. Refactor flows through planning → execution → review → finalization (impact analysis and validation live inside the refactor plan)

**Key Tasks:**
- IDENTIFY_REFACTORS (populate backlog)
- SELECT_REFACTOR (select from backlog)
- Refactor workflow tasks (see REFACTOR/WORKFLOW.md)

---

## Shared Utility Tasks

**Location:** `TASKS/` (root)

These tasks support all workflows:

- **COMMIT**: Format and create git commits following project conventions

## Workflow-Specific Tasks

### Feature Workflow (`TASKS/FEATURE/`)
- **UPDATE_DEPENDENCIES**: Update DEPENDENCY_STACK.md with new certainty scores
- **UPDATE_BACKLOG**: Mark features complete/deferred in DESIGN_BACKLOG, document learnings

### Maintenance Workflow (`TASKS/MAINTENANCE/`)
- **REVIEW_CODEBASE**: Random walk through src/ to populate MAINTENANCE_BACKLOG.md

### Refactor Workflow (`TASKS/REFACTOR/`)
- **IDENTIFY_REFACTORS**: Deep system analysis to populate REFACTOR_BACKLOG.md
- **SELECT_REFACTOR**: Choose refactor opportunity from backlog based on priority and stability
- **PLAN_REFACTOR**: Capture impact analysis, migration strategy, and validation checklist in a single plan
- **REVIEW_PLAN**: Verify plan against principles and stability requirements
- **IMPLEMENT_REFACTOR**: Implement refactor in stages with verification
- **REVIEW_REFACTOR**: Comprehensive code review for correctness and principle alignment
- **FINALIZE_REFACTOR**: Run plan's validation checklist, update backlog, document learnings, prepare commits

---

## Meta-Process Tasks

- **RETROSPECTIVE**: Review last N iterations to identify workflow patterns, extract learnings, and propose process improvements (run every 5-10 iterations)
- **UNDERSTAND_CONCEPT**: Build conceptual knowledge through discussion, research, and documentation. Create intellectual capital that informs future problem-solving (run anytime to eliminate uncertainty)
- **ALIGN_TASKS**: Ensure all workflow tasks maintain consistent structure, tone, format, and principle alignment. Prevents drift when individual tasks are updated (run when tasks are modified, principles change, or during periodic audits)

## Workflow Complexity Comparison

### MAINTENANCE (Low Complexity)
- **Complexity:** 1-2 points
- **Scope:** Single-file or small multi-file changes
- **Review:** Two-tier (trivial skip review, standard get review)
- **Purpose:** Quick fixes and code quality improvements

### REFACTOR (Medium Complexity)
- **Complexity:** 3-8 points
- **Scope:** Multi-file, cross-system changes
- **Review:** Always reviewed and validated
- **Purpose:** Architectural improvements and pattern extraction
- **Gates:** Stability requirement (≥70% certainty), rule of three verification

### FEATURE (High Complexity)
- **Complexity:** 8+ points (or split into smaller iterations)
- **Scope:** Full system implementation
- **Review:** Multi-stage (plan, step, implementation)
- **Purpose:** New mechanics and behaviors

---

## Potential Future Tasks

- **BUILD_TEST**: Formalized quality gate execution (build, lint, format, smoke test)
- **PLAYTEST_GRAYBOX**: Execute testing protocol with target sample size and metrics
- **TUNE_PARAMETERS**: Real-time parameter tuning session to achieve desired feel

