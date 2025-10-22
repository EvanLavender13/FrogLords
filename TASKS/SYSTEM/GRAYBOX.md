# GRAYBOX - Build Simplest Possible Version

**Validate with cubes. Prove it works before polish.**

---

## Purpose

Build the core mechanic in the simplest possible form. No art, no polish, no extras. Just the mathematical truth of the system, made visible.

**If it doesn't work in graybox, it won't work polished. Save months by discovering this in hours.**

---

## Prerequisites

- `PLANS/<name>_SYSTEM.md` exists with core identified
- Branch `system/<name>` active
- Dependencies exist (check DEPENDENCY_STACK.md)
- Mathematical approach clear

---

## Anti-Patterns

Before implementing, review @TASKS/PATTERNS.md for known code and process anti-patterns.

Prevention is cheaper than detection.

---

## Process

### 1. Define Scope

Update `PLANS/<name>_SYSTEM.md` — add:

```markdown
<!-- BEGIN: GRAYBOX/SCOPE -->
## Graybox

Define the minimal scope that proves the core works.

**What will be built:**
[Describe the core mechanic and how it will be validated]

**Complexity:** Small | Medium | Large
<!-- END: GRAYBOX/SCOPE -->
```

---

### 2. Plan Implementation

**Before writing code, map the structure.**

Update `PLANS/<name>_SYSTEM.md` — add:

```markdown
<!-- BEGIN: GRAYBOX/IMPLEMENTATION_PLAN -->
## Implementation Plan

**Files to modify:**
List each file path and what changes.

**Call structure:**
Describe function call chain.

**Debug data flow:**
Describe how computed values reach visualization.

**Integration points:**
Identify which existing functions modified and where new code added.
<!-- END: GRAYBOX/IMPLEMENTATION_PLAN -->
```

---

### 2.5. Validate Plan

**Get a second opinion before implementing:**

Use Codex to validate the implementation plan against principles.

```bash
bash scripts/bash/codex.sh "@TASKS/PLANS/<name>_SYSTEM.md @PRINCIPLES.md @CONVENTIONS.md [your validation question]"
```

**IMPORTANT:** Use `run_in_background: true` in Bash tool call.

**WAIT for user confirmation that review is complete.** This takes several minutes. Do NOT monitor output. User will confirm when finished.

**Evaluate feedback:**
- Concerns raised? Revise plan
- Alternative suggested? Consider it
- Need clarification? Continue with `bash scripts/bash/codex.sh --resume "[follow-up question]"`
- Validation confirms? Proceed

**Document the review** in `PLANS/<name>_SYSTEM.md`:

```markdown
<!-- BEGIN: GRAYBOX/REVIEW -->
## Implementation Review

**Tool:** Codex CLI
**Date:** YYYY-MM-DD

**Question asked:**
[What you asked for validation]

**Feedback received:**
- [Key points from the review]
- [Concerns or recommendations]

**Impact on implementation:**
- [How feedback changed your approach]
- [What you kept vs modified]
<!-- END: GRAYBOX/REVIEW -->
```

---

### 3. Implement Core and Debug Visualization

**Prove correctness. Make it visible. Keep it minimal.**

- Implement primitive/system code
- Add debug visualization (make state visible)
- Add debug assertions for preconditions/postconditions
- Build successfully

---

### 4. Provide Testing Instructions

**DO NOT run the application. Provide instructions and wait for user to test.**

Present testing instructions covering:
- How to build and run
- What behaviors to validate
- Expected outcomes
- Edge cases to check

Wait for user to report results.

---

### 5. Iterate Based on Feedback

User reports: works, broken, or surprises.

**If broken:** Fix, rebuild, provide new instructions, repeat step 4.

**If works:** Proceed to document results.

---

### 6. Document Results

In `PLANS/<name>_SYSTEM.md` — add:

```markdown
<!-- BEGIN: GRAYBOX/RESULTS -->
## Results

**Status:**
- [x] Core functional
- [x] Build successful
- [x] Debug viz working
- [ ] Ready for iteration

**Works:**
-

**Doesn't:**
-

**Surprises:**
-

**Next:**
- ITERATE phase
<!-- END: GRAYBOX/RESULTS -->
```

Update `CURRENT_PLAN.md` phase: `GRAYBOX → ITERATE`

---

### 7. Commit

```bash
git add .
git commit -m "graybox: <name> core mechanic

Works: <what>
Validated: <properties>
Emergent: <surprises>
Status: <ready | needs work | defer>

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## Outputs

- [ ] Implementation plan documented
- [ ] Core implemented (primitives)
- [ ] Debug viz functional
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
