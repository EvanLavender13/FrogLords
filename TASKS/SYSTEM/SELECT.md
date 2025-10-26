# SELECT - Choose System to Build

**Build from certainty toward uncertainty. Choose what the foundation can support.**

---

## Purpose

Select a system from TASKS/BACKLOG_SYSTEMS.md based on dependency readiness.

**This is not about priority or excitement. This is about what we can build with what exists.**

---

## Process

### 1. Review Capabilities

Read @TASKS/DEPENDENCY_STACK.md to understand what exists at each layer and what it enables

---

### 2. Select System

Read @TASKS/BACKLOG_SYSTEMS.md to find systems at ready layer.

Choose based on dependency readiness and foundational value. Apply the principles.

---

### 3. Gather Internal References

**Search the codebase first.**

Before asking the user for external context, understand what already exists:

**Search NOTES/:**
- Research documents related to this system
- Architectural decisions already made
- Known constraints or approaches

**Search src/:**
- Existing implementations this integrates with
- Established patterns to follow
- Lower-layer primitives available

**Document findings:**
- Relevant source files and line numbers
- Existing research notes
- Integration points
- Architectural conflicts (defer if fundamental)

---

### 4. Gather External References

**FULL STOP: Ask the user what they actually want.**

Collect external context that informs the implementation. Document everything provided.

---

### 5. Assess Complexity

Evaluate scope, integration needs, mathematical certainty, and validation requirements.

---

### 6. Create Plan

Create `TASKS/PLANS/<name>_SYSTEM.md`:

```markdown
<!-- BEGIN: SELECT/REFERENCES -->
[Internal and external context that informed this selection]
<!-- END: SELECT/REFERENCES -->

---

<!-- BEGIN: SELECT/SELECTED -->
[System decision, complexity assessment, dependencies, core mechanics, mathematical foundation, and graybox approach]
<!-- END: SELECT/SELECTED -->

---

### 7. Update Current Plan

Update `TASKS/CURRENT_PLAN.md`:

```markdown
# Current Plan

**Status:** 🏗️ System - <Name>
**Branch:** system/<name>
---

## Current Work

**System:** <description>
**Location:** @TASKS/PLANS/<name>_SYSTEM.md
**Next Task:** GRAYBOX

```

**CRITICAL:** `@` reference ensures plan loads automatically via `CLAUDE.md`.

---

### 8. Create Branch

```bash
git checkout -b system/<name>
git add TASKS/PLANS/<name>_SYSTEM.md TASKS/CURRENT_PLAN.md
git commit -m "select: <name> system for layer <N>

[concise description]

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com>"
```

---

## Outputs

- [ ] System selected
- [ ] References gathered
- [ ] Complexity assessed
- [ ] `PLANS/<name>_SYSTEM.md` created
- [ ] `CURRENT_PLAN.md` updated
- [ ] Branch created

---

## Next

**Always → GRAYBOX** (build simplest possible version)

**Defer if:**
- Dependencies don't actually exist
- Core unclear or too complex
- Better choice available

---

**Build from what exists. Choose by dependencies. Enable emergence.**
