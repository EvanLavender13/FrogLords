# AUDIT - Find Principle Violations

**Leverage AI analysis. Synthesize truth.**

---

## Purpose

Use Gemini and Codex to examine the codebase against the six principles. Synthesize their findings into actionable violations in `TASKS/BACKLOG_REFINEMENTS.md`.

**Not manual scanning. Distributed analysis.**

---

## Prerequisites

- Foundation ≥70% stable
- Between system builds
- `TASKS/BACKLOG_REFINEMENTS.md` exists
- Gemini CLI and Codex CLI available

---

## Process

### 1. Launch Sequential Audits

Use both tools with principles and conventions as context. Craft your audit prompt based on:
- Current foundation status
- Known problem areas
- Backlog contents
- Recent changes

**Gemini (massive context):**
```bash
gemini -p "@PRINCIPLES.md @CONVENTIONS.md @src/ [your audit prompt]"
```

**Codex (independent opinion):**
```bash
echo "@PRINCIPLES.md @CONVENTIONS.md @src/ [your audit prompt]" | codex e 2>/dev/null
```

**Waiting for completion:**
Both tools can take time to analyze, especially Codex. If needed, check status every 20 seconds until output completes. Codex in particular may think for extended periods.

**Note**: Codex can resume with `codex e resume --last 2>/dev/null` for follow-up questions.

**Your audit prompt should:**
- Reference the six principles explicitly
- Specify what you're looking for (violations, patterns, specific principles)
- Define the output format you want
- Be tailored to current codebase state

---

### 2. Synthesize Findings

**Analyze both outputs:**
- Gemini's comprehensive view (from terminal output)
- Codex's independent analysis (from terminal output)

**Identify:**
- Convergence (both found it = high confidence)
- Divergence (only one found it = requires validation)
- Blind spots (what was missed)

**Validate:**
- Read actual code at violation locations
- Confirm violations are real
- Assess actual severity based on foundation impact
- Filter false positives

---

### 3. Document Violations

For each validated violation in `TASKS/BACKLOG_REFINEMENTS.md`:

```markdown
## [Principle Name]

- **file/path:line**: Description of violation
  - Principle: [Name]
  - Severity: [Critical|High|Medium|Low]
  - Type: [Classification]
  - Fix: [Delete|Simplify|Document] (rationale)
  - Source: [Gemini|Codex|Both]
```

**Severity:**
- **Critical**: Foundation threatening, blocks work
- **High**: Causes bugs or limits emergence
- **Medium**: Technical debt, compounds over time
- **Low**: Cosmetic, address opportunistically

**Type:**
- Complexity (unnecessary complication)
- Special case (breaks composability)
- Magic number (unjustified constant)
- Duplication (same logic in multiple places)
- Prescribed outcome (removes emergence)
- Lost responsiveness (violates Prime Directive)
- Unjustified decision (no principle backing)
- Undocumented (missing rationale)

**Fix:**
- **Delete**: Remove entirely (preferred)
- **Simplify**: Derive from simpler rules
- **Document**: If must exist, explain why

---

### 4. Find Patterns

Look across violations for patterns:
- Same violation type recurring
- Same system causing multiple violations
- Same root cause in different places

Review the terminal output from both audits to identify recurring themes.

Document patterns:
```markdown
## Patterns

**Pattern**: [Description]
- Instances: [Count/locations]
- Root cause: [Why it happens]
- Prevention: [How to avoid in future]
```

---

### 5. Prioritize

**Priority = f(severity, foundation_impact, cascade_risk, fix_simplicity)**

Where:
- `severity`: Critical > High > Medium > Low
- `foundation_impact`: Core systems > Leaf systems
- `cascade_risk`: Many dependents > Few dependents
- `fix_simplicity`: Delete > Simplify > Document

List top priorities in backlog.

---

## Exit Criteria

- Both tools have audited the codebase
- Findings synthesized and validated
- Violations documented in backlog
- Patterns identified
- Priorities assigned

**Not "both tools ran" but "findings validated and actionable."**

---

## Next Step

→ **REFINE/SELECT** (choose violation to fix)

---

## The Philosophy

**Analysis**: Tools examine code at scale
**Synthesis**: Human validates and filters
**Action**: Backlog items trace violations to fixes

Tools provide perspective. You provide judgment.

**Distribute work. Synthesize truth. Act on certainty.**
