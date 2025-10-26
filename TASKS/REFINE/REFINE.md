# REFINE - Execute the Fix

**Delete. Simplify. Document. In that order.**

---

## Purpose

Execute planned refinement—remove violation, restore principle, reduce complexity.

---

## Prerequisites

- Read @TASKS/CURRENT_PLAN.md

---

## Anti-Patterns

Before implementing, review @TASKS/PATTERNS.md for known code and process anti-patterns.

Prevention is cheaper than detection.

---

## Simple Fixes

For immediate, obvious fixes where documentation would substantially exceed the work itself, execute directly with minimal ceremony. Apply "Radical Simplicity" to the process itself.

### 1. Execute

Delete code, simplify logic, or document justification.

**Constraints:**
- Fix violation only
- No features, no scope creep
- Minimal change

### 2. Validate

```bash
[test command]
```

All tests pass. If not: fix, revert, or update tests with justification.

**Manual verification:**

If changes affect runtime behavior:
1. List what user must verify
2. Ask user to run app and verify
3. Wait for confirmation

### 3. Document

In `TASKS/PLANS/REFINE_<name>.md`:

```markdown
<!-- BEGIN: REFINE/COMPLETED -->
[What was changed and result]
<!-- END: REFINE/COMPLETED -->
```

### 4. Commit

```bash
git add .
git commit -m "refine: <name> - <what was removed/simplified>

[concise description]

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

**→ Proceed to MEASURE**

---

## Standard Refinements

### 1. Plan Steps

In `TASKS/PLANS/REFINE_<name>.md`:

```markdown
<!-- BEGIN: REFINE/PLAN -->
[Execution plan with steps]
<!-- END: REFINE/PLAN -->
```

### 2. Validate Plan

**Get a second opinion before executing:**

Use Codex to validate the approach.

```bash
bash scripts/bash/codex.sh "Read [TASKS/PLANS/REFINE_<name>.md PRINCIPLES.md CONVENTIONS.md relevant files] [your validation question]"
```

**IMPORTANT:** Use `run_in_background: true` in Bash tool call.

**WAIT for user confirmation that review is complete.** This takes several minutes. Do NOT monitor output. User will confirm when finished.

**Evaluate feedback:**
- Concerns raised? Revise plan
- Alternative suggested? Consider it
- Need clarification? Continue the conversation with `codex e resume --last`
- **Changes made?** Request at least 1 review of revised plan
- Validation confirms? Proceed
- **Process cycling?** Consider radical simplification instead

**Rule:** Certainty through convergence. When independent analyses agree, confidence increases.

**Document the review:**

In `TASKS/PLANS/REFINE_<name>.md`, add:

```markdown
<!-- BEGIN: REFINE/REVIEW -->
[Second opinion review findings and impact on approach]
<!-- END: REFINE/REVIEW -->
```

### 3. Execute Each Step

**Per step:**

A. Make change (one step only)
B. Run tests
C. Verify: passing, correct behavior, no regressions
D. **Manual verification** (if changes affect runtime behavior):
   - List what user must verify
   - Ask user to run app and verify
   - Wait for confirmation
E. Commit:

```bash
git add .
git commit -m "refine: <name> step N - <what changed>

[concise description]

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

F. Mark step complete in plan

### 4. Handle Blockers

**Blocked:** Document why, suggest alternative, commit findings, proceed to MEASURE

**Scope grew:** If >1 day, consider deferring to system build

**Breaking changes:** Document impact, update affected systems, expand tests

---

## Outputs

- [ ] Violation fixed
- [ ] All tests passing
- [ ] No regressions
- [ ] Principle upheld
- [ ] Steps committed
- [ ] Documentation updated

---

## Exit Criteria

**→ MEASURE when:**
- Fix complete
- Tests passing
- Principle upheld

**Defer if:**
- Blocker discovered
- Scope exceeds 1 day
- Breaking changes unacceptable

---

## Next Step

**→ MEASURE** (calculate before/after metrics)

---

## Core Rules

**Delete first:** Code you don't have can't have bugs, needs no maintenance, violates no principles.

**Small commits:** One change, all tests pass, independently revertable.

**No scope creep:** Fix violation only. Not features, not optimization, not nearby refactoring.

**Tests guard truth:** Green before → Green after = behavior preserved. Red after = fix or revert.

**Document learning:** Why violation appeared, how to prevent recurrence, patterns to watch.

---

**Delete first. Simplify second. Document last. Test always.**
