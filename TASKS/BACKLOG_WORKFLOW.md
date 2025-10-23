# Workflow Backlog

**Process improvements. Template evolution. Workflow friction removal.**

**Last Updated:** 2025-10-22

---

## Active Items

### High Priority

**Complexity-Aware Templates**
- **Issue:** Trivial fixes (<10 lines) documented with 70-100 line plan templates
- **Evidence:** const_parameters (4 const qualifiers = 74-line plan), test_level_literals (19 lines = 108-line plan)
- **Pattern:** Documentation bloat violates Radical Simplicity in process itself
- **Solution:** Create template variants by complexity:
  - Trivial: Minimal template (~20 lines) - What/Why/Change/Commit
  - Standard: Current template structure
  - Complex: Current template + expanded validation sections
- **Triggers:**
  - Trivial: <10 line change, single file, no architectural impact
  - Standard: Multi-file, requires validation, moderate complexity
  - Complex: System builds, architectural changes, requires iterations
- **Impact:** Reduces process overhead, maintains principle compliance in documentation
- **Source:** RETRO_2025-10-22

### Medium Priority

**Documentation Guidance in Workflows**
- **Issue:** No explicit guidance on when to use which level of documentation detail
- **Solution:** Add complexity assessment step to REFINE.md and SYSTEM.md
- **Details:** Before plan creation, assess complexity and select appropriate template
- **Impact:** Prevents over-documentation of simple changes
- **Source:** RETRO_2025-10-22

### Low Priority

**Template Repository Organization**
- **Issue:** Templates currently embedded in workflow docs (REFINE.md, SYSTEM.md)
- **Consideration:** Extract to TASKS/TEMPLATES/ directory if template count grows >3 variants
- **Status:** Defer until need emerges
- **Principle:** Don't create structure before pain point exists

---

## Deferred

**Plan Section Consolidation**
- **Observation:** Some iteration plans have redundant METRICS/REFLECTION/SUMMARY sections
- **Action:** Monitor for actual redundancy across next 5 plans
- **Status:** Needs more data - workflow success rate is 100%, don't fix what works

**Automated Plan Linting**
- **Idea:** Simple scripts to flag "For example" in principles, "Completed Items" sections in backlogs
- **Status:** Defer until pattern emerges again (principles/conventions already document rules)

---

## Completed

None yet - first workflow backlog created.

---

## Selection Criteria

**Before implementing workflow changes:**
1. Pattern must appear ≥3 times (RETRO validation)
2. Current friction must outweigh change cost
3. New process must be simpler, not more complex
4. Test with 1-2 plans before mandating

**Process inherits principles. Radical Simplicity applies to workflow too.**

---
