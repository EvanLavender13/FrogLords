# Create Refactor Plan

### 1. Review Development Principles

Read `AGENTS.md` to ensure refactor aligns with core principles:
- "Clarity over cleverness"
- "Simplicity over sophistication"
- "Abstract repeated patterns into systems"
- "Wait for third use" (rule of three)

### 2. Gather Context

1. **Recall Selected Item:** Reference from `SELECT_REFACTOR.md`
2. **Review Impact Analysis:** Read findings from `ANALYZE_IMPACT.md`
3. **Read Affected Code:** Ensure deep understanding of current implementation

### 3. Define Before/After State

Create clear examples showing the transformation:

#### Before Example
```cpp
// Current code pattern (show representative example)
// Include enough context to understand the problem
```

#### After Example
```cpp
// Refactored code (show target state)
// Include enough context to understand the improvement
```

**Rationale:** Explain how "after" is clearer, simpler, or more maintainable than "before"

### 4. Design Migration Strategy

Break the refactor into logical stages:

#### Stage-Based Approach (Recommended)

**Pattern Extraction Example:**
1. **Prepare:** Create new abstraction (function/class) without removing old code
2. **Migrate:** Convert call sites one-by-one to use new abstraction
3. **Cleanup:** Remove old code once all call sites migrated

**Simplification Example:**
1. **Prepare:** Add new simplified implementation alongside old
2. **Switch:** Update call sites to use new implementation
3. **Cleanup:** Remove old complex implementation

**API Design Example:**
1. **Prepare:** Add new API with improved interface
2. **Deprecate:** Mark old API as deprecated, forward to new API
3. **Migrate:** Update call sites to new API
4. **Cleanup:** Remove deprecated old API

#### Linear Approach (Simple Refactors)
For low-risk refactors with <5 call sites:
1. **Execute:** Make all changes in one pass
2. **Verify:** Test immediately after

**Choose approach based on risk level from ANALYZE_IMPACT:**
- Low Risk → Linear approach acceptable
- Medium/High Risk → Stage-based approach required

### 5. Map Call Site Updates

For each call site identified in ANALYZE_IMPACT:

```markdown
### Call Site Migration Checklist
- [ ] `src/file1.cpp:123` - [what changes here]
- [ ] `src/file2.cpp:456` - [what changes here]
- [ ] `src/file3.h:78` - [what changes here]
[complete exhaustive list]
```

### 6. Define Rollback Plan

For each stage, define how to undo:

#### Staged Refactor Rollback
- **After Stage 1:** Revert commit for Stage 1
- **After Stage 2:** Revert commit for Stage 2 (old code still exists)
- **After Stage 3:** Revert all commits (or use backup branch)

#### Linear Refactor Rollback
- **Single Revert:** `git revert <commit-hash>`
- **Backup Branch:** Create `backup/<refactor-name>` before starting

**Rollback Trigger:** Define conditions that require rollback (e.g., "build breaks", "crashes on startup", "gameplay regression detected")

### 7. Create Validation Checklist

Based on testing strategy from ANALYZE_IMPACT:

```markdown
### Behavior Validation Checklist
**Must verify after each stage:**
- [ ] Code compiles without errors/warnings
- [ ] [Specific behavior 1] works as expected
- [ ] [Specific behavior 2] works as expected
- [ ] No regressions in [system X]

**Final validation:**
- [ ] All test scenarios from ANALYZE_IMPACT pass
- [ ] Debug visualizations still work
- [ ] Performance characteristics unchanged
- [ ] [Any other critical checks]
```

### 8. Check for Hidden Impacts

Review hidden dependencies from ANALYZE_IMPACT and plan mitigations:

- **Initialization Order Changes:** [mitigation plan]
- **Debug/GUI Updates Needed:** [list of files to update]
- **Documentation Updates:** [which docs need revision]
- **Performance Considerations:** [profiling plan if needed]

### 9. Estimate Per-Stage Effort

For each migration stage:
- **Stage 1:** [X hours] - [brief description]
- **Stage 2:** [Y hours] - [brief description]
- **Stage 3:** [Z hours] - [brief description]
- **Total:** [X+Y+Z hours] (must be ≤16 hours total)

**If total exceeds 16 hours (2 work days):** Recommend breaking into multiple refactors or deferring.

### 10. Generate Refactor Plan Document

Save plan to `PLANS/refactor_<refactor_name>.md` using template below:

```markdown
# Refactor Plan: [Refactor Name]

**Date:** [YYYY-MM-DD]
**Category:** Pattern Extraction | Simplification | System Design | Utilities | API Design
**Risk Level:** Low | Medium | High
**Estimated Duration:** [X hours/days]

---

## 1. Overview

**Current State:** [Brief description of the problem/pattern]

**Desired State:** [What we want to achieve]

**Rationale:** [Why this refactor improves the codebase; cite principles]

---

## 2. Before/After Examples

### Before
```cpp
// Current code showing the pattern/problem
```

### After
```cpp
// Refactored code showing the improvement
```

**Key Improvements:**
- [Improvement 1: e.g., "Reduces duplication from 5 instances to 1"]
- [Improvement 2: e.g., "Clarifies intent with descriptive function name"]
- [Improvement 3: e.g., "Eliminates error-prone manual calculations"]

---

## 3. Migration Strategy

**Approach:** Staged | Linear

### Stage 1: Prepare
**Goal:** [What this stage accomplishes]
**Changes:**
- [Specific change 1]
- [Specific change 2]

**Verification:**
- [ ] Compiles without warnings
- [ ] [Specific check]

**Rollback:** Revert Stage 1 commit

---

### Stage 2: Migrate
**Goal:** [What this stage accomplishes]
**Call Sites to Update:**
- [ ] `src/file1.cpp:123` - [description]
- [ ] `src/file2.cpp:456` - [description]
[exhaustive list]

**Verification:**
- [ ] All call sites compile
- [ ] [Specific behavior check]

**Rollback:** Revert Stage 2 commit (Stage 1 remains)

---

### Stage 3: Cleanup
**Goal:** Remove old code/APIs
**Changes:**
- [What gets deleted]

**Verification:**
- [ ] No references to old code remain
- [ ] [Final behavior check]

**Rollback:** Revert all commits or restore from backup branch

---

## 4. Hidden Dependencies

**Debug/GUI Updates:**
- [List of debug/GUI files that need updates]

**Documentation:**
- [Which docs need revision]

**Performance:**
- [Any profiling or performance checks needed]

**Other:**
- [Any other non-obvious impacts]

---

## 5. Validation Protocol

### Per-Stage Validation
- [ ] Code compiles without errors/warnings
- [ ] [Stage-specific check 1]
- [ ] [Stage-specific check 2]

### Final Validation (VALIDATE_BEHAVIOR task)
- [ ] **Test Scenario 1:** [Description] → Expected: [outcome]
- [ ] **Test Scenario 2:** [Description] → Expected: [outcome]
- [ ] **Test Scenario 3:** [Description] → Expected: [outcome]
- [ ] No regressions in [affected systems]
- [ ] Debug visualizations work correctly
- [ ] Performance characteristics preserved

---

## 6. Rollback Plan

**Trigger Conditions:**
- Build breaks and fix isn't obvious within 15 minutes
- Crashes occur in previously stable code
- Gameplay regression detected
- Complexity exceeds estimate significantly

**Rollback Procedure:**
- **After Stage 1:** `git revert <stage1-commit>`
- **After Stage 2:** `git revert <stage2-commit>`
- **After Stage 3:** `git revert <stage3-commit> <stage2-commit> <stage1-commit>`
- **Backup Branch:** `backup/refactor_<name>` created before starting

---

## 7. Risk Assessment

**Risk Level:** Low | Medium | High

**Risk Factors:**
- [Factor 1: e.g., "15 call sites across 8 files"]
- [Factor 2: e.g., "Affects core physics system"]

**Mitigation:**
- [How we're reducing risk]

**Abort Conditions:**
- [Conditions that should trigger deferral]

---

## 8. Effort Estimate

- **Stage 1 (Prepare):** [X hours]
- **Stage 2 (Migrate):** [Y hours]
- **Stage 3 (Cleanup):** [Z hours]
- **Validation:** [W hours]
- **Total:** [X+Y+Z+W hours]

**Confidence:** High | Medium | Low
```

### 11. Propose Plan

Present the saved plan to the user for review. Highlight:
- Key improvements (clarity, simplicity, maintainability)
- Risk level and mitigation strategy
- Estimated duration
- Any concerns or uncertainties

### Tone & Constraints

- Clear and concrete; avoid vague descriptions
- Show, don't tell (use code examples)
- Realistic about risk and effort
- Favor incremental staged approaches for medium/high risk
- Explicitly plan for failure (rollback strategy)
- Ensure plan respects "clarity over cleverness" and "simplicity over sophistication"
