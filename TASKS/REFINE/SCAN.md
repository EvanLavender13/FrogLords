# SCAN - Automated Code Quality Checks

**Run format, lint, and analyze. Fix or document violations.**

---

## Purpose

Use automated tools to find code quality issues. Fix trivial violations immediately. Document larger violations in `TASKS/BACKLOG_REFINEMENTS.md`.

**Not manual review. Automated enforcement.**

---

## Prerequisites

- Foundation ≥70% stable
- Between system builds
- `TASKS/BACKLOG_REFINEMENTS.md` exists
- Code quality scripts available

---

## Process

### 1. Run Format Check

```bash
bash scripts/bash/format.sh
```

**Action:**
- If changes made: Review diffs, commit formatting fixes
- If no changes: Proceed to lint

---

### 2. Run Lint Analysis

```bash
bash scripts/bash/lint.sh
```

**Examine output for violations:**
- **Trivial** (naming, const-correctness, simple fixes): Fix immediately
- **Non-trivial** (design issues, complex refactoring): Document in backlog

**Action:**
- Fix trivial issues directly
- For complex issues: Document in backlog (see step 4)

---

### 3. Run Static Analysis

```bash
bash scripts/bash/analyze.sh
```

**Examine both cppcheck and lizard output:**

**cppcheck violations:**
- Logic errors, memory issues, undefined behavior: Fix immediately if simple
- Complex issues: Document in backlog

**lizard complexity violations:**
- CCN >15: Function needs simplification
- Length >100: Function needs decomposition
- Arguments >5: Interface needs refinement

**Action:**
- Simple fixes: Apply immediately
- Complex refactoring: Document in backlog

---

### 4. Document Non-Trivial Violations

For each violation requiring significant work in `TASKS/BACKLOG_REFINEMENTS.md`:

```markdown
## [Tool Name] - [Issue Type]

- **file/path:line**: Description of violation
  - Tool: [clang-tidy|cppcheck|lizard]
  - Severity: [Critical|High|Medium|Low]
  - Issue: [Specific problem]
  - Fix: [Required action]
```

**Severity:**
- **Critical**: Crashes, undefined behavior, memory issues
- **High**: Logic errors, maintainability threats
- **Medium**: Complexity debt, style violations
- **Low**: Cosmetic issues, minor improvements

---

### 5. Commit Trivial Fixes

If trivial fixes were applied:

```bash
git add -A
git commit -m "fix: apply code quality fixes from scan

- clang-format: [changes]
- clang-tidy: [fixes]
- cppcheck: [fixes]"
```

---

## Exit Criteria

- All three scripts executed
- Trivial violations fixed and committed
- Non-trivial violations documented in backlog
- Codebase passes format check

**Not "scripts ran" but "violations addressed or documented."**

---

## Next Step

→ **REFINE/SELECT** (if violations documented)
→ **Continue development** (if all clean)

---

## The Philosophy

**Automation**: Tools find violations mechanically
**Judgment**: Human decides fix vs. document
**Action**: Trivial fixes happen now, complex fixes enter backlog

Tools enforce standards. Judgment determines priority. Backlog preserves context.

**Automate detection. Apply judgment. Fix or document.**
