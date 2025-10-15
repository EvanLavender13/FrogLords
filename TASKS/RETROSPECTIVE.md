# Retrospective: Process Review

### Purpose

Periodic review of archived iterations to identify workflow patterns, improve process efficiency, and prevent recurring issues.

### When to Run

- After every 5-10 completed iterations
- When workflow feels inefficient or repeatedly encounters similar issues
- Before major project milestones

---

## 1. Select Review Window

1. **Determine Sample Size:** Decide how many recent iterations and improvements to review (typically 5-10 iterations or equivalent work)
2. **List Archived Work:** Use `ls -lt PLANS/ARCHIVE/` to identify the most recent timestamped documents
3. **Identify Complete Sets:** 
   - **For feature iterations**, gather all related documents:
     - `*_FEATURE.md`
     - `*_PLAN.md`
     - `*_PLAN_REVIEW.md`
     - `*_CODE_REVIEW.md`
   - **For improvements**, gather:
     - `IMPROVE_*.md` documents (contain full improvement history in single document)

---

## 2. Extract Patterns from Archives

For each iteration or improvement in the sample, read through the documents and extract:

### Feature Iterations
**Planning:** REVIEW_PLAN outcomes (principle violations, scope issues, dependencies, testing), scope changes, clarification needs  
**Implementation:** Step velocity, self-verification effectiveness, review feedback, file prediction accuracy  
**Completion/Deferral:** Success factors, learnings, surprises, deferral phase/category, early warning signs
  - Example deferral triggers: dependency uncertainty, missing prerequisites, discovered complexity exceeds estimate
  - Example skeleton coordinate frame understanding gaps: Features requiring per-bone axis-dependent rotations (IK, joint limits, targeted limb control). Missing prerequisites: skeleton debugging/visualization tools, understanding of local→world transform chain. Reconsideration trigger: After building skeleton debugging tools or during IK implementation (forces solving same problems)

### Improvements
**Planning:** REVIEW_PLAN outcomes (rule of three, stability gates, scope, risk), complexity estimates, Trivial vs Standard classification accuracy  
**Execution:** Stage completion, call site identification accuracy, hidden dependencies, code review findings, scope discipline, discovery patterns  
**Outcome:** Impact achieved, learnings, abstraction quality, deferral triggers, stability gate enforcement, batch opportunities

### Cross-Cutting
Explicit reflections, recurring friction, effective practices, feature/improvement interleaving patterns

**Note:** See full analysis criteria in archived retrospectives if needed for reference.

---

## 3. Synthesize Findings

Organize patterns into three categories:

### ✅ Keep Doing
Practices that prevented issues or enabled smooth iterations/improvements:
- Example: "Clarifying visual references before planning prevented implementation surprises"
- Example: "Small, focused iteration scopes completed faster with fewer issues"
- Example: "Staged migration approach for 10+ call sites prevented refactor issues"
- Example: "Enforcing 70% stability gate prevented refactor conflicts with active feature work"

### 🛑 Stop Doing
Practices that caused problems or friction:
- Example: "Planning without checking dependency tree structure led to repeated deferrals"
- Example: "Implementing multiple loosely-related features in one iteration caused scope creep"
- Example: "Abstracting patterns with only 2 instances caused premature complexity"
- Example: "Skipping rule-of-three verification led to over-engineered refactors"

### 🆕 Start Doing
New practices to try based on observed patterns:
- Example: "Add explicit 'complexity estimate' to iteration plans to catch over-scoped plans early"
- Example: "Run REVIEW_CODEBASE before planning major improvements to surface hidden coupling"
- Example: "Track improvement opportunities discovered during feature work more systematically"
- Example: "Review DEPENDENCY_STACK tree structure and certainty scores before selecting improvements"

---

## 4. Propose Workflow Changes

Based on findings, propose specific, actionable changes. Tag each with type:
- **[TASK]** Task definition updates (which files, what changes)
- **[DOC]** Documentation updates (WORKFLOW.md, principles, templates)
- **[EXPERIMENT]** Process experiments to trial for 2-3 iterations before codifying

---

## 5. Document Retrospective

Create `PLANS/RETROSPECTIVE_<timestamp>.md` with the following structure:

```markdown
# Retrospective: <Date Range>

**Work Reviewed:** <count> iterations, <count> improvements
**Date Range:** <first_timestamp> to <last_timestamp>
**Feature Iterations:** X completed, Y deferred
**Improvements:** I completed

---

## Pattern Analysis

### Feature Iterations

**Planning Phase:**
- [Summarize planning phase patterns]

**Implementation Phase:**
- [Summarize implementation phase patterns]

**Completion/Deferral:**
- [Summarize outcomes and learnings]

### Improvements

**Planning Phase:**
- [Summarize improvement planning patterns]
- Stability gate enforcement: [observations]
- Rule of three verification (for pattern extractions): [observations]
- Complexity estimation accuracy: [observations]
- Trivial vs Standard classification: [observations]

**Execution Phase:**
- [Summarize execution patterns]
- Staged vs linear approaches: [observations]
- Call site identification accuracy (for refactors): [observations]
- Hidden dependency discovery: [observations]

**Scope & Discovery:**
- Scope discipline: [did fixes stay focused?]
- Unexpected discoveries: [trivial fixes revealing deeper issues]
- New improvement triggers: [how many led to additional IMPROVE_BACKLOG items?]

**Process Effectiveness:**
- Two-tier review working well: [observations]
- Learning capture quality: [observations]
- Batch opportunity identification: [observations]

**Completion/Deferral:**
- [Summarize improvement outcomes]
- Impact achieved: [examples]
- Learnings captured: [observations]

### Cross-Cutting Observations
- Feature/improvement interleaving: [patterns]
- Workflow friction points: [observations]
- Process synergies: [what worked well]
- Improvements as investigation tool: [how well did quick fixes surface architectural concerns?]

---

## Findings

### ✅ Keep Doing
- [List effective practices from both features and improvements]

### 🛑 Stop Doing
- [List problematic practices]

### 🆕 Start Doing
- [List proposed new practices]

---

## Proposed Workflow Changes

- [ ] **[TASK] TASK_NAME.md:** [Specific change description]
- [ ] **[DOC] WORKFLOW.md:** [Specific change description]
- [ ] **[EXPERIMENT] Name:** [Description and success criteria]

**User Decision:** [User reviews and marks which changes to implement]
```

---

## 6. Organize Archive

After completing the retrospective and user approval, run the automation script:

```powershell
.\scripts\archive_retro.ps1
```

**Script behavior:**
- Auto-detects most recent `RETROSPECTIVE_*.md`
- Auto-detects all artifacts (files matching `*_FEATURE.md`, `*_PLAN.md`, `*_CODE_REVIEW.md`, `*_PLAN_REVIEW.md`, `*_IMPROVE_*.md`)
- Creates `RETRO_<timestamp>/` subdirectory
- Moves retrospective + all artifacts using `git mv` (preserves history)
- Provides commit message suggestion

**Manual override:** Use `-Timestamp "20251012_225921"` to specify a different retrospective

**Purpose:** Keeps ARCHIVE clean and navigable. Each RETRO_ subdirectory is a self-contained snapshot of reviewed work plus analysis.

---

## Tone & Constraints

- **Data-driven:** Base findings on actual archived documents, not assumptions
- **Specific:** Cite concrete examples from archives when identifying patterns
- **Actionable:** Propose changes that can be implemented in task definitions
- **Honest:** If no clear patterns emerge, say so; don't force findings
- **Balanced:** Look for both successes and failures
- **Experimental mindset:** Frame new practices as experiments to validate

---

## Notes

- Retrospectives are about process improvement, not blame
- Small, incremental changes are better than sweeping overhauls
- Not all patterns need immediate action; some are just observations
- Track which workflow changes came from retrospectives to evaluate their impact in future retros
- If deferral rate is high (>40%), dig deep into root causes
- Celebrate what's working; don't just focus on problems
- **Key questions for improvements:**
  - Stability gates (70% certainty) enforced consistently?
  - Rule-of-three discipline preventing premature abstraction?
  - Complexity estimates (1-8 points) accurate?
  - Trivial vs Standard classification working well?
  - Are fixes uncovering deeper architectural issues?
  - Two-tier review approach appropriate?
  - Feature/improvement interleaving causing conflicts or working well?
