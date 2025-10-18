# AUDIT - Find Principle Violations

**Make the invisible visible. Complexity accumulates silently.**

---

## Purpose

Search the codebase systematically for principle violations, documenting them in TASKS/BACKLOG_REFINEMENTS.md for future refinement.

**Violations don't announce themselves. We must hunt for them.**

---

## Prerequisites

- [ ] Foundation ≥70% stable (can absorb changes)
- [ ] Between system builds (good time for reflection)
- [ ] `TASKS/BACKLOG_REFINEMENTS.md` exists

---

## Process

### 1. Principle 1: Radical Simplicity

**Search for unjustified complexity**

**File/code level:**
```bash
# Find long files (>500 lines often violates simplicity)
find src -name "*.ts" -o -name "*.js" | xargs wc -l | sort -rn | head -20

# Find high cyclomatic complexity
# (use your language's complexity analyzer)
```

**System level:**
```bash
# Find systems with many dependencies
# (check import counts, coupling metrics)
```

**Questions to ask:**
- Can this file be split?
- Can this function be simplified?
- Can this system be decomposed?
- What can be deleted entirely?

**Document violations:**
```markdown
## 1. Radical Simplicity

- **src/systems/combat.ts (850 lines)**: Too complex
  - Principle: Radical Simplicity
  - Severity: High
  - Type: Complexity
  - Fix: Delete | Simplify
```

---

### 2. Principle 2: Composable Functions

**Search for overlap and special cases**

**Grep for special cases:**
```bash
# Find entity-type checks (special casing)
rg "if.*type.*==|switch.*type" --type ts

# Find instanceof checks
rg "instanceof" --type ts

# Find hard-coded entity references
rg "entity.*===.*['\"]" --type ts
```

**Questions to ask:**
- Do systems overlap in functionality?
- Are there special cases for specific entities?
- Do systems depend on each other's internals?
- Can systems compose without modification?

**Document violations:**
```markdown
## 2. Fundamental Composable Functions

- **src/systems/movement.ts:145**: Special case for player entity
  - Principle: Composable Functions
  - Severity: Medium
  - Type: Special case
  - Fix: Simplify (make rule general)
```

---

### 3. Principle 3: Mathematical Foundations

**Search for magic numbers and unproven math**

**Grep for magic numbers:**
```bash
# Find unexplained constants
rg "\b(0\.[0-9]+|[2-9][0-9]+)\b" --type ts | grep -v "// "

# Find TODO comments about math
rg "TODO.*math|FIXME.*constant|XXX.*magic" --type ts
```

**Questions to ask:**
- Are all constants derived and documented?
- Is every formula mathematically proven?
- Are edge cases handled mathematically?
- Can behavior be predicted from equations?

**Document violations:**
```markdown
## 3. Solid Mathematical Foundations

- **src/physics/spring.ts:23**: Magic number 0.85 (damping)
  - Principle: Mathematical Foundations
  - Severity: High
  - Type: Magic number
  - Fix: Document (derive from desired behavior)
```

---

### 4. Principle 4: Emergent Behavior

**Search for prescribed outcomes**

**Grep for prescriptive patterns:**
```bash
# Find hard-coded behaviors
rg "if.*player.*{.*win|lose|die" --type ts

# Find scripted sequences
rg "sequence|script|trigger" --type ts

# Find linear progressions
rg "level.*===|stage.*===" --type ts
```

**Questions to ask:**
- Are outcomes prescribed or enabled?
- Can players discover unexpected strategies?
- Do systems interact to create emergence?
- Are there undocumented emergent behaviors?

**Document violations:**
```markdown
## 4. Emergent Behavior

- **src/progression/levels.ts**: Hard-coded level sequence
  - Principle: Emergent Behavior
  - Severity: Medium
  - Type: Prescribed outcome
  - Fix: Enable (system-driven progression)

- **src/systems/combat.ts**: Undocumented emergent behavior
  - Principle: Emergent Behavior
  - Severity: Low
  - Type: Undocumented
  - Fix: Document (capture in EMERGENCE.md)
```

---

### 5. Principle 5: Consistency

**Search for unpredictable behavior**

**Grep for inconsistency:**
```bash
# Find random without seed
rg "Math\.random\(\)" --type ts

# Find time-dependent logic (frame-rate dependent)
rg "Date\.now\(\)|performance\.now\(\)" --type ts

# Find mutable state
rg "let.*state|var.*cache" --type ts
```

**Questions to ask:**
- Is behavior deterministic?
- Are there special cases?
- Does input always produce same output?
- Is control ever taken from player?

**Document violations:**
```markdown
## 5. Consistency

- **src/systems/animation.ts:67**: Animation blocking player input
  - Principle: Consistency (Prime Directive)
  - Severity: Critical
  - Type: Lost responsiveness
  - Fix: Simplify (physics drives animation)
```

---

### 6. Principle 6: Principled Development

**Search for unjustified decisions**

**Grep for unexplained choices:**
```bash
# Find comments with "workaround" or "hack"
rg "workaround|hack|HACK|kludge" --type ts

# Find disabled code
rg "^\\s*//.*(?:TODO|FIXME|XXX)" --type ts

# Find arbitrary thresholds
rg "threshold|limit" --type ts
```

**Questions to ask:**
- Can every decision be traced to a principle?
- Are there workarounds that violate principles?
- Are there unexplained arbitrary choices?
- What technical debt exists?

**Document violations:**
```markdown
## 6. Principled Development

- **src/config.ts:12**: Arbitrary inventory limit (100)
  - Principle: Principled Development
  - Severity: Low
  - Type: Unjustified decision
  - Fix: Justify | Delete (remove limit entirely)
```

---

### 2. Classify Each Violation

**For every violation found, determine:**

**Severity:**
- **Critical**: Foundation threatening, actively harmful
- **High**: Causing bugs or limiting emergence
- **Medium**: Technical debt, makes future work harder
- **Low**: Cosmetic, address when convenient

**Type:**
- Magic number
- Special case
- Duplication
- Complexity
- Undocumented
- Prescribed outcome
- Lost responsiveness
- Behavioral drift

**Fix approach:**
- **Delete**: Remove entirely (preferred)
- **Simplify**: Derive from simpler rules
- **Document**: If must keep, explain why

---

### 3. Update Backlog

Open `TASKS/BACKLOG_REFINEMENTS.md` and add all violations to appropriate sections.

**By principle:**
```markdown
## 1. Radical Simplicity

- **src/systems/combat.ts (850 lines)**: Too complex
  - Principle: Radical Simplicity
  - Severity: High
  - Type: Complexity
  - Fix: Simplify (split into composable functions)
```

**By severity:**
```markdown
## Critical (Foundation Threatening)

- **src/systems/animation.ts:67**: Animation blocking input
  - Principle: Consistency (Prime Directive)
  - Severity: Critical
  - Type: Lost responsiveness
  - Fix: Simplify (physics drives animation)
```

---

### 4. Look for Patterns

**After documenting violations, analyze:**

**Recurring patterns:**
- Same violation type appearing multiple times?
- Same system causing multiple violations?
- Same developer patterns?
- Same phase of development?

**Document in** `TASKS/BACKLOG_REFINEMENTS.md`:
```markdown
## Patterns

**Recurring violations (found during audit [DATE]):**
- **Magic numbers in physics**: Found 12 instances
  - Pattern: Constants not derived from desired behavior
  - Root cause: Tweaking by feel instead of calculating
  - Prevention: Require derivation in VALIDATE gate

- **Special cases for player entity**: Found 8 instances
  - Pattern: if (entity === player) checks
  - Root cause: Easier than making rules general
  - Prevention: Ban entity-specific checks in code review
```

---

### 5. Prioritize

**Suggest priority order in backlog:**

**Priority factors:**
1. Severity (Critical > High > Medium > Low)
2. Foundation impact (Core systems > Leaf systems)
3. Cascade risk (Many dependents > Few dependents)
4. Fix simplicity (Delete > Simplify > Document)

**Add priority notes:**
```markdown
## Next to Fix

**Suggested order:**
1. [Critical] src/systems/animation.ts:67 - Blocks input
2. [High] src/physics/spring.ts:23 - Magic number in foundation
3. [High] src/systems/combat.ts - Complexity blocks future work
```

---

## Outputs

- [ ] All six principles audited
- [ ] Violations documented in `TASKS/BACKLOG_REFINEMENTS.md`
- [ ] Severity assigned to each
- [ ] Fix approach identified (Delete | Simplify | Document)
- [ ] Patterns identified
- [ ] Priority suggested

---

## Audit Checklist

**Run through this systematically:**

### Code Smells
- [ ] Files >500 lines (complexity)
- [ ] Functions >50 lines (complexity)
- [ ] High cyclomatic complexity (>10)
- [ ] Deep nesting (>3 levels)
- [ ] Long parameter lists (>5 params)

### Magic Numbers
- [ ] Unexplained constants
- [ ] Undocumented formulas
- [ ] Arbitrary thresholds
- [ ] Tweaked-by-feel values

### Special Cases
- [ ] Entity-type checks
- [ ] Player-specific logic
- [ ] Level-specific behavior
- [ ] Hard-coded sequences

### Duplication
- [ ] Copy-pasted code
- [ ] Similar logic in multiple files
- [ ] Overlapping system responsibilities

### Undocumented
- [ ] Emergent behaviors not in EMERGENCE.md
- [ ] Magic numbers without derivation
- [ ] Design decisions without rationale
- [ ] TODOs and FIXMEs

### Anti-Patterns
- [ ] Animation blocking physics
- [ ] Accumulated state vs. derived
- [ ] Non-deterministic behavior
- [ ] Frame-rate dependent logic
- [ ] Mutable global state

---

## Exit Criteria

**Audit complete when:**
- All six principles checked
- Violations documented
- Patterns identified
- Priority suggested
- Backlog updated

---

## Next Step

**→ REFINE/SELECT** (choose highest-priority violation to fix)

---

## Audit Philosophy

### Violations Accumulate Silently

**You can't fix what you can't see.**

Code reviews catch some violations. But many accumulate invisibly:
- Magic numbers added in "just this one place"
- Special cases for "just this entity"
- Complexity from "just one more feature"

**Regular audits make them visible.**

### Quantity Reveals Patterns

**One magic number is a violation.**
**Ten magic numbers is a pattern.**

Patterns reveal:
- Systemic issues (not individual mistakes)
- Training opportunities
- Process improvements
- Missing tools/infrastructure

### Not All Violations Are Equal

**Critical violations** threaten foundation
**Low violations** are cosmetic

**Fix by severity and impact, not by ease.**

### The Best Audit Finds Nothing

**If regular audits find few violations:**
- Principles are internalized
- Process is working
- Culture is healthy

**That's the goal. But verify regularly.**

---

**Make violations visible. Understand patterns. Prioritize by impact.**
