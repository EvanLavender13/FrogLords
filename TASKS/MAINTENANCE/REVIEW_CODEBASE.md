# Review Codebase for Maintenance Items

### 1. Read Guiding Principles

Read `AGENTS.md` to ensure all evaluations align with development principles, code standards, and project philosophy.

### 2. Perform Random Walk Review

Scan `src/` directory focusing on `.cpp` and `.h` files across all subsystems.

**Look for:**
- Code smells (dead code, commented blocks, debugging code)
- Naming violations (camelCase/PascalCase instead of snake_case)
- Unused includes
- Duplicate logic needing helper function
- Magic numbers needing named constants
- Inconsistent formatting (spacing, indentation, braces)
- Principle violations (dependency flow breaks, reactive controlling core)
- Leftover features (incomplete experiments, TODOs, orphaned systems)
- Missing const-correctness
- Documentation gaps (non-obvious public API)

### 3. Categorize Findings

**Severity tiers:**
- **Critical:** Principle violations, bugs, correctness issues
- **High:** Code smells significantly impacting readability/maintainability
- **Medium:** Minor inconsistencies or quality improvements
- **Low:** Nitpicks or polish with minimal impact

### 4. Document in Maintenance Backlog

1. Open `PLANS/MAINTENANCE_BACKLOG.md`
2. Use structured entries (file, description, severity, suggested fix)
3. Group by subsystem (character, rendering, foundation)
4. Append to existing or create new

### 5. Propose High-Value Fixes

**If Critical/High items found:**
1. Summarize findings
2. Propose immediate fixes for critical issues
3. Suggest prioritization for high-severity items

**Otherwise:** Inform user of total findings and await guidance.

### Tone & Constraints

- Concise and direct
- Concrete, actionable findings over subjective opinions
- Cite specific file paths and line ranges
- Detail scales with severity
- Avoid bikeshedding; flag real issues only
- Document first, don't propose changes during review

---

### Maintenance Backlog Template

```markdown
# Maintenance Backlog

**Purpose:** Small improvements, refactors, and cleanup benefiting code quality.

**Last Review:** [Date]

---

## Critical

- **[Issue Title]**
  - *File:* `src/path/to/file.cpp:123`
  - *Severity:* Critical
  - *Description:* Brief description
  - *Principle Violated:* (if applicable)
  - *Suggested Fix:* Concrete action

## High

## Medium

## Low

---

## Completed

(Prune periodically)
```
