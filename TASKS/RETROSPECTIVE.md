# Retrospective: Process Review

### Purpose

Periodic review of archived iterations to identify workflow patterns, improve process efficiency, and prevent recurring issues.

### When to Run

- After every 5-10 completed iterations
- When workflow feels inefficient or repeatedly encounters similar issues
- Before major project milestones

---

## 1. Select Review Window

1. **Determine Sample Size:** Decide how many recent iterations and refactors to review (typically 5-10 iterations or equivalent work)
2. **List Archived Work:** Use `ls -lt PLANS/ARCHIVE/` to identify the most recent timestamped documents
3. **Identify Complete Sets:** 
   - **For feature iterations**, gather all related documents:
     - `*_feature_*.md`
     - `*_implementation_*.md`
     - `*_plan_review_*.md`
     - `*_code_review_*.md`
   - **For refactors**, gather:
     - `refactor_*.md` documents (contain full refactor history in single document)

---

## 2. Extract Patterns from Archives

For each iteration or refactor in the sample, read through the documents and extract:

### Feature Iterations: Planning Phase Analysis
- **REVIEW_PLAN outcomes:** Did plans pass on first try? What types of issues were found?
  - Principle violations (which principles?)
  - Scope concerns (too large, too vague, missing graybox boundaries?)
  - Missing prerequisites or dependencies
  - Testing protocol weaknesses
- **Scope changes:** Were ADD_SCOPE or MODIFY_PLAN tasks needed? Why?
- **Clarification needs:** Did the feature require CLARIFY_FEATURE? What was unclear initially?

### Feature Iterations: Implementation Phase Analysis
- **Step completion velocity:** How many IMPLEMENTATION_STEP cycles occurred? Were steps appropriately sized?
- **Self-verification effectiveness:** Did self-verification catch issues before final review?
- **Review feedback patterns:** What types of issues appeared in REVIEW_IMPLEMENTATION?
  - Code quality issues
  - Principle violations in implementation
  - Misalignment with plan
- **Changed files tracking:** Were implementation plans accurate in predicting file changes?

### Feature Iterations: Completion/Deferral Analysis
- **Successful completions:**
  - What enabled smooth completion?
  - What learnings were documented in UPDATE_BACKLOG?
  - Were there any surprises or serendipitous discoveries?
- **Deferred features:**
  - What phase was deferral triggered? (planning vs. implementation)
  - What category of deferral? (premature, missing prereq, principle violation, blocker)
  - Were there early warning signs that were missed?
  - What learnings were captured?

### Refactors: Planning Phase Analysis
- **REVIEW_PLAN outcomes:** Did refactor plans pass review? What issues were found?
  - Rule of three violations (pattern didn't appear 3+ times)
  - Stability gate failures (systems <70% certainty)
  - Scope creep or premature abstraction
  - Risk assessment accuracy
- **Complexity estimates:** Were estimates accurate? Did refactors exceed 8-point limit?
- **Migration strategy:** Were staged approaches chosen appropriately for risk level?

### Refactors: Execution Analysis
- **Stage completion:** For staged refactors, did each stage verify correctly?
- **Call site accuracy:** Were all call sites identified in planning?
- **Hidden dependencies:** What hidden dependencies were discovered during execution?
- **Code review findings:** What types of issues appeared in code review?
  - Principle alignment (clarity, simplicity)
  - Behavioral preservation
  - Scope discipline

### Refactors: Outcome Analysis
- **Successful completions:**
  - What impact was achieved? (e.g., "Reduced duplication 5→1")
  - What learnings were captured?
  - Did abstraction quality meet expectations?
- **Deferred refactors:**
  - What triggered deferral? (too risky, complexity exceeded estimate, unstable systems)
  - Were stability gates enforced properly?
  - What would make it viable in the future?

### Cross-Cutting: Iteration Reflections
- Review any explicit reflection notes from FINALIZE_ITERATION or refactor completions
- Were there recurring frustrations or friction points?
- What practices helped work go smoothly?
- How did feature and refactor work interleave? Any conflicts or synergies?

---

## 3. Synthesize Findings

Organize patterns into three categories:

### ✅ Keep Doing
Practices that prevented issues or enabled smooth iterations/refactors:
- Example: "Clarifying visual references before planning prevented implementation surprises"
- Example: "Small, focused iteration scopes completed faster with fewer issues"
- Example: "Staged migration approach for 10+ call sites prevented refactor issues"
- Example: "Enforcing 70% stability gate prevented refactor conflicts with active feature work"

### 🛑 Stop Doing
Practices that caused problems or friction:
- Example: "Planning without checking dependency stack led to repeated deferrals"
- Example: "Implementing multiple loosely-related features in one iteration caused scope creep"
- Example: "Abstracting patterns with only 2 instances caused premature complexity"
- Example: "Skipping rule-of-three verification led to over-engineered refactors"

### 🆕 Start Doing
New practices to try based on observed patterns:
- Example: "Add explicit 'complexity estimate' to iteration plans to catch over-scoped plans early"
- Example: "Run REVIEW_CODEBASE before planning major refactors to surface hidden coupling"
- Example: "Track refactor opportunities discovered during feature work more systematically"
- Example: "Review DEPENDENCY_STACK certainty scores before selecting refactors"

---

## 4. Propose Workflow Changes

Based on findings, propose specific, actionable changes:

### Task Definition Updates
- Which task files need updated instructions?
- What new checklist items or validation steps should be added?
- Are there missing tasks that would prevent recurring issues?

### Documentation Updates
- Does WORKFLOW.md need revised paths or decision points?
- Should any principles be clarified or examples added?
- Do backlog/stack templates need additional fields?

### Process Experiments
- Are there new practices to trial for 2-3 iterations before codifying?
- Should any existing practices be relaxed or removed?

---

## 5. Document Retrospective

Create `PLANS/RETROSPECTIVE_<timestamp>.md` with the following structure:

```markdown
# Retrospective: <Date Range>

**Work Reviewed:** <count> iterations, <count> refactors
**Date Range:** <first_timestamp> to <last_timestamp>
**Feature Iterations:** X completed, Y deferred
**Refactors:** Z completed, W deferred

---

## Pattern Analysis

### Feature Iterations

**Planning Phase:**
- [Summarize planning phase patterns]

**Implementation Phase:**
- [Summarize implementation phase patterns]

**Completion/Deferral:**
- [Summarize outcomes and learnings]

### Refactors

**Planning Phase:**
- [Summarize refactor planning patterns]
- Stability gate enforcement: [observations]
- Rule of three verification: [observations]
- Complexity estimation accuracy: [observations]

**Execution Phase:**
- [Summarize execution patterns]
- Staged vs linear approaches: [observations]
- Call site identification accuracy: [observations]
- Hidden dependency discovery: [observations]

**Completion/Deferral:**
- [Summarize refactor outcomes]
- Impact achieved: [examples]
- Learnings captured: [observations]

### Cross-Cutting Observations
- Feature/refactor interleaving: [patterns]
- Workflow friction points: [observations]
- Process synergies: [what worked well]

---

## Findings

### ✅ Keep Doing
- [List effective practices from both features and refactors]

### 🛑 Stop Doing
- [List problematic practices]

### 🆕 Start Doing
- [List proposed new practices]

---

## Proposed Workflow Changes

### Task Updates
- [ ] **TASK_NAME.md:** [Specific change description]
- [ ] **TASK_NAME.md:** [Specific change description]

### Documentation Updates
- [ ] **WORKFLOW.md:** [Specific change description]
- [ ] **DEPENDENCY_STACK.md:** [Specific change description]
- [ ] **REFACTOR_BACKLOG.md:** [Specific change description]

### Process Experiments
- [ ] **Experiment:** [Description and success criteria]

---

## User Decision

[User reviews and marks which changes to implement]
```

---

## 6. User Review and Implementation

1. **Present findings** to the user with the retrospective document
2. **User decides** which workflow changes to implement
3. **Update task files** based on approved changes

---

## 7. Organize Archive

After completing the retrospective, group the reviewed work with the retrospective document:

1. **Create subdirectory:** `PLANS/ARCHIVE/RETRO_<timestamp>/`
   - Use same timestamp format as retrospective document (YYYYMMDD_HHMMSS)
2. **Move retrospective document:** `RETROSPECTIVE_<timestamp>.md` into the new subdirectory
3. **Move reviewed artifacts:** Move all artifacts from the work that was reviewed into the subdirectory
   - **Feature iterations:** Include all files for each reviewed iteration (feature, iteration, implementation, review, code_review)
   - **Refactors:** Include complete `refactor_*.md` documents
   - Use `mv PLANS/ARCHIVE/<timestamp>_* PLANS/ARCHIVE/RETRO_<timestamp>/` for each iteration timestamp
   - Use `mv PLANS/ARCHIVE/refactor_* PLANS/ARCHIVE/RETRO_<timestamp>/` for refactor documents
4. **Verify organization:** Confirm the subdirectory contains:
   - The retrospective document
   - All artifacts from reviewed iterations (grouped by iteration timestamp prefix)
   - All refactor documents reviewed
   - Nothing else remains in flat ARCHIVE that should have been included

**Purpose:** Keeps ARCHIVE clean and navigable. Each RETRO_ subdirectory is a self-contained snapshot of reviewed work plus the analysis that covered it.

**Example structure after organization:**
```
PLANS/ARCHIVE/
├── RETRO_20251009_143000/
│   ├── RETROSPECTIVE_20251009_143000.md
│   ├── 20251006_120000_feature_refactor_game_world.md
│   ├── 20251006_120000_implementation_refactor_game_world.md
│   ├── 20251006_120000_code_review_refactor_game_world.md
│   ├── 20251006_130000_feature_attach-skeleton-to-body.md
│   ├── refactor_yaw_direction_utilities.md
│   ├── refactor_horizontal_velocity_utility.md
│   └── ... (all other artifacts from reviewed work)
├── 20251008_150000_feature_new_thing.md  ← newer feature, not yet retrospected
```

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
- If deferral rate is high (>40%) for features or refactors, dig deep into root causes
- If planning issues are recurring, consider adding CLARIFY_FEATURE or REVIEW_PLAN improvements
- Celebrate what's working; don't just focus on problems
- Archive organization keeps PLANS/ARCHIVE/ from becoming unnavigable; RETRO_ subdirectories are chronological snapshots of process reviews
- **Refactor-specific considerations:**
  - Are stability gates (70% certainty) being enforced consistently?
  - Is rule-of-three discipline preventing premature abstraction?
  - Are complexity estimates (3-8 points) accurate for refactors?
  - How well do staged vs linear approaches map to risk levels?
  - Are refactor learnings feeding back to improve future planning?
  - Is feature/refactor interleaving causing conflicts or working well?
