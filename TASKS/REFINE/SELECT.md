# SELECT - Choose Violation to Fix

**Delete first. Simplify second. Document last.**

---

## Purpose

Choose highest-impact violation from backlog. Classify complexity. Determine path.

---

## Process

### 1. Review Current Capabilities

Read @TASKS/DEPENDENCY_STACK.md to understand what exists and what the violations affect

---

### 2. Select Violation

Read @TASKS/BACKLOG_REFINEMENTS.md

Choose the highest-impact violation. Apply the principles.

---

### 3. Classify Complexity

Assess the violation's scope and risk:

**Considerations:**
- Isolation: Single location vs. system-wide
- Behavior changes: Static vs. runtime impact
- Cascade risk: Contained vs. propagating changes
- Validation needs: Obvious correctness vs. requires testing strategy

**Planning decision:**
- Create plan when: Learning value justifies documentation cost, or approach needs validation
- Skip plan when: Fix is immediate and documentation would exceed the work itself

**Brevity principle:** Documentation should not substantially exceed the work. For simple changes, concise plans or direct commits with descriptive messages may suffice.

---

### 4. Create Plan

Create `TASKS/PLANS/REFINE_<name>.md`:

```markdown
# Refinement: <Name>

**One-line description**

---

<!-- BEGIN: SELECT/PLAN -->
[Violation description, complexity assessment, and fix approach]
<!-- END: SELECT/PLAN -->
```

---

### 5. Determine Path

Based on complexity assessment, choose execution approach:

**Simple fixes:** Direct execution, minimal or no formal planning
**Standard refinements:** Planned execution with validation steps
**System-level changes:** Defer to SYSTEM/SELECT - build new system instead of refining existing

---

### 6. Update Current Plan

Update `TASKS/CURRENT_PLAN.md`:

```markdown
# Current Plan

**Status:** 🔧 Refinement - <Name>
**Branch:** refine/<name>
**Phase:** SELECT → REFINE

---

## Current Work

**Refinement:** <description>
**Location:** @TASKS/PLANS/REFINE_<name>.md
**Next:** REFINE
```

**CRITICAL:** `@` reference ensures plan loads automatically via `CLAUDE.md`.

---

### 7. Create Branch

```bash
git checkout -b refine/<name>
git add TASKS/PLANS/REFINE_<name>.md TASKS/CURRENT_PLAN.md
git commit -m "select: refinement of <violation>

[concise description]

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## Outputs

- [ ] Violation selected
- [ ] `TASKS/PLANS/REFINE_<name>.md` created
- [ ] Complexity assessed
- [ ] Execution approach determined
- [ ] `TASKS/CURRENT_PLAN.md` updated
- [ ] Branch created

---

## Next

**→ REFINE** (execute the fix)

**System-level changes:** → SYSTEM/SELECT (build new system instead)

---

**Choose by impact. Classify by risk. Delete when possible.**
