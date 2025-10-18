# AUDIT - Find Principle Violations

**Make the invisible visible.**

---

## Purpose

Examine the codebase against the six principles. Document violations in `TASKS/BACKLOG_REFINEMENTS.md`.

**Not pattern matching. Understanding.**

---

## Prerequisites

- Foundation ≥70% stable
- Between system builds
- `TASKS/BACKLOG_REFINEMENTS.md` exists

---

## Process

### 1. Identify Systems to Audit

List all major systems/components:
```bash
# Find all headers in src/
find src/ -name "*.h" -o -name "*.hpp"

# Find all implementation files
find src/ -name "*.cpp" -o -name "*.c"

# Check directory structure
tree src/ -L 2
```

**Focus on:**
- Core systems (physics, input, rendering)
- Large files (>300 lines)
- Recently modified files
- Files with many dependencies

---

### 2. Question Claimed Certainty

Before examining code, read current metrics:
- `DEPENDENCY_STACK.md` - What percentages are claimed?
- `BACKLOG_REFINEMENTS.md` - What's considered "fixed"?

**Then ask:**
- Are these percentages actually true?
- Is the claimed certainty reflected in the code?
- Did previous refinements actually fix what they claimed?
- Are "100%" systems really perfect?

**Radical skepticism. Trust verification, not claims.**

---

### 3. Examine Against Principles

For each system/file, read and ask:

**Principle 1: Radical Simplicity**
- Can anything be deleted?
- Can this be split into smaller pieces?
- Does every line justify its existence?
- Is complexity necessary or accidental?

**Principle 2: Composable Functions**
- Do functions compose cleanly?
- Are there special cases for specific entities/states?
- Do systems overlap in responsibility?
- Can systems be combined without modification?

**Principle 3: Mathematical Foundations**
- Are constants derived or arbitrary?
- Are formulas documented and proven?
- Can behavior be predicted from equations?
- Are there "magic numbers"?

**Principle 4: Emergent Behavior**
- Are outcomes prescribed or enabled?
- Do systems interact to create emergence?
- Are there hard-coded sequences?
- Can unexpected behaviors arise?

**Principle 5: Consistency**
- Does same input produce same output?
- Is there non-deterministic behavior?
- Are there exceptions to rules?
- Does the system betray player expectations?

**Principle 6: Principled Development**
- Can every decision trace to a principle?
- Are there workarounds or hacks?
- Is technical debt documented?
- Are there unjustified arbitrary choices?

---

### 4. Document Violations

For each violation found:

```markdown
## [Principle Name]

- **file/path:line**: Description of violation
  - Principle: [Name]
  - Severity: [Critical|High|Medium|Low]
  - Type: [Classification]
  - Fix: [Delete|Simplify|Document] (rationale)
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

### 5. Find Patterns

After documenting individual violations, look for:
- Same violation type recurring
- Same system causing multiple violations
- Same root cause appearing in different places

Document patterns with prevention strategy:

```markdown
## Patterns

**Pattern**: [Description]
- Instances: [Count/locations]
- Root cause: [Why it happens]
- Prevention: [How to avoid in future]
```

---

### 6. Prioritize

Order violations by impact:

**Priority = f(severity, foundation_impact, cascade_risk, fix_simplicity)**

Where:
- `severity`: Critical > High > Medium > Low
- `foundation_impact`: Core systems > Leaf systems
- `cascade_risk`: Many dependents > Few dependents
- `fix_simplicity`: Delete > Simplify > Document

List top priorities in backlog.

---

## Common Searches (Optional)

Use as starting points for investigation, not as complete audit:

```bash
# Large files (complexity indicator)
find src/ -name "*.cpp" -o -name "*.c" | xargs wc -l | sort -rn | head -20

# TODO/FIXME comments (deferred decisions)
grep -r "TODO\|FIXME\|XXX\|HACK" src/ --include="*.cpp" --include="*.h"

# Magic numbers (unexplained constants)
grep -rE "\b[0-9]+\.[0-9]+\b|\b[2-9][0-9]+\b" src/ --include="*.cpp" --include="*.h" | grep -v "//"
```

**These find potential violations. Still requires reading and understanding.**

---

## Exit Criteria

- Major systems examined against all six principles
- Violations documented in backlog
- Patterns identified
- Priorities assigned

**Not "all searches run" but "codebase understood."**

---

## Next Step

→ **REFINE/SELECT** (choose violation to fix)

---

## The True Nature of Audits

**Bad audit**: Run predetermined searches, find nothing, declare success.

**Good audit**: Understand the system, compare to principles, find what's actually wrong.

Violations accumulate in the gaps between what you search for. The only defense is comprehension.

**Read. Understand. Compare. Document.**
