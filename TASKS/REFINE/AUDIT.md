# AUDIT - Find Principle Violations

**Leverage AI analysis. Synthesize truth.**

---

## Purpose

Use Gemini and Codex to examine the codebase against the six principles. Synthesize their findings into actionable violations in `TASKS/BACKLOG_REFINEMENTS.md`.

**Not manual scanning. Distributed analysis.**

---

## Prerequisites

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
bash scripts/bash/gemini.sh "@PRINCIPLES.md @CONVENTIONS.md @src/ [your audit prompt]"
```

**Codex (independent opinion):**
```bash
bash scripts/bash/codex.sh "@PRINCIPLES.md @CONVENTIONS.md @src/ [your audit prompt]"
```

**IMPORTANT:** Use `run_in_background: true` in Bash tool call.

**WAIT for user confirmation that review is complete.** This takes several minutes. Do NOT monitor output. User will confirm when finished.

**Note**: Codex can resume with `bash scripts/bash/codex.sh --resume "[follow-up question]"`.

**Your audit prompt should:**
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

Document patterns in `TASKS/PATTERNS.md`:
```markdown
## Pattern: [Name]
- **Detection:** [How to find it]
- **Root cause:** [Why it happens]
- **Fix:** [How to resolve]
- **Prevention:** [How to avoid in future]
```

---

## Exit Criteria

- Both tools have audited the codebase
- Findings synthesized and validated
- Violations documented in backlog
- Patterns identified

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
