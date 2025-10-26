# GRAYBOX - Build Simplest Possible Version

**Validate with cubes. Prove it works before polish.**

---

## Purpose

Build the core mechanic in the simplest possible form. Prove it works mathematically. Make state visible through debug visualization.

**Crude but correct. Architecture refinement happens in ITERATE.**

If it doesn't work in graybox, it won't work polished. Save months by discovering this in hours.

---

## Prerequisites

- Read @TASKS/CURRENT_PLAN.md
- `PLANS/<name>_SYSTEM.md` exists with core identified

---

## Anti-Patterns

Before implementing, review @TASKS/PATTERNS.md for known code and process anti-patterns.

Prevention is cheaper than detection.

---

## Process

### 1. Plan Implementation

**Before writing code, map the structure.**

Study existing patterns in similar systems first. Identify reusable patterns before inventing new ones.

Update `PLANS/<name>_SYSTEM.md` — add:

```markdown
<!-- BEGIN: GRAYBOX/PLAN -->
[Implementation approach: scope, structure, integration points, debug visualization strategy]
<!-- END: GRAYBOX/PLAN -->
```

---

### 2. Implement Core and Debug Visualization

**Prove correctness. Make it visible. Don't overthink architecture.**

- Implement primitive/system code (simplest approach that works)
- Add debug visualization (make state visible)
- Add debug assertions for preconditions/postconditions
- Build successfully

**Architecture will be refined in ITERATE. Focus on functional correctness.**

---

### 3. Provide Testing Instructions

**DO NOT run the application. Provide instructions and wait for user to test.**

Present testing instructions covering:
- How to build and run
- What behaviors to validate
- Expected outcomes
- Edge cases to check

Wait for user to report results.

---

### 4. Iterate Based on Feedback

User reports: works, broken, or surprises.

**If broken:** Fix, rebuild, provide new instructions, repeat step 3.

**If works:** Proceed to document results.

---

### 5. Document Results

In `PLANS/<name>_SYSTEM.md` — add:

```markdown
<!-- BEGIN: GRAYBOX/RESULTS -->
[What works, what doesn't, surprises, and readiness for next phase]
<!-- END: GRAYBOX/RESULTS -->
```

Update `CURRENT_PLAN.md` phase: `GRAYBOX → ITERATE`

---

### 6. Commit

```bash
git add .
git commit -m "graybox: <name> core mechanic

[concise description]

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## Outputs

- [ ] Implementation planned
- [ ] Core implemented
- [ ] Debug visualization functional
- [ ] Build successful
- [ ] Results documented
- [ ] Committed

---

## Next

**→ ITERATE** (if core works)

**Defer if:**
- Core fundamentally flawed
- Math doesn't work as expected
- Too complex to simplify
- Foundation insufficient
- Different approach needed

**Deferral is success. Most systems should fail in graybox. That's why we graybox.**

---

**Cheap to build. Cheap to change. Cheap to throw away. Validate with cubes.**
