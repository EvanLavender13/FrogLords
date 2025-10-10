# Create Refactor Plan

### 1. Review Development Principles

Read `AGENTS.md` to ensure refactor aligns with core principles:
- "Clarity over cleverness"
- "Simplicity over sophistication"
- "Abstract repeated patterns into systems"
- "Wait for third use" (rule of three)

### 2. Create Branch

Create a new refactor branch using the script:

```powershell
./scripts/create_refactor_branch.ps1 <refactor_name>
```

### 3. Gather Context

1. **Read Refactor Description:** Open `PLANS/refactor_<refactor_name>.md`
2. **Read Affected Files:** Systematically read all `.h` and `.cpp` files mentioned in the description
3. **Search for Usage:** Find every place the code is used to map dependencies

### 4. Analyze Impact

#### Identify All Call Sites

1. **Search for Usage:** Use grep/search to find every place the code is used
2. **Map Dependencies:** Identify which systems depend on the code being changed
3. **Check Indirect Usage:** Look for indirect dependencies
4. **List Exhaustively:** Document every file and function that will need updating

**Output:** Comprehensive list of affected files with specific line numbers

#### Assess Current State

1. **Understand Current Design:** Why was it written this way originally?
2. **Identify Patterns:** What pattern is being repeated or what complexity exists?
3. **Find Edge Cases:** Are there subtle behaviors or edge cases to preserve?
4. **Check Documentation:** Are there comments explaining non-obvious choices?

**Output:** Clear description of current behavior and any subtle aspects to preserve

#### Evaluate Risk

**Risk Factors:**
- **Number of Call Sites:** More call sites = higher risk
- **System Criticality:** Core systems (physics, rendering) = higher risk
- **Test Coverage:** Manual testing only = higher risk
- **Behavioral Complexity:** Complex logic with edge cases = higher risk
- **Certainty Score:** Lower certainty = higher risk

**Risk Classification:**
- **Low Risk:** Single file, <5 call sites, well-understood behavior, high certainty (≥90%)
- **Medium Risk:** Multi-file, 5-15 call sites, some complexity, good certainty (70-90%)
- **High Risk:** Cross-system, 15+ call sites, complex behavior, moderate certainty (70-80%)

#### Check for Hidden Dependencies

Look for non-obvious impacts:
- **Initialization Order:** Does the refactor affect startup sequence?
- **Memory Layout:** Could struct changes affect size or alignment?
- **Performance:** Could the refactor change performance characteristics?
- **Debug/GUI:** Will debug visualizations or GUI panels need updates?
- **Serialization:** Does anything save/load this data?

### 5. Define Before/After State

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

### 6. Design Migration Strategy

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

**Choose approach based on risk level:**
- Low Risk → Linear approach acceptable
- Medium/High Risk → Stage-based approach required

### 7. Map Call Site Updates

For each call site identified:

```markdown
### Call Site Migration Checklist
- [ ] `src/file1.cpp:123` - [what changes here]
- [ ] `src/file2.cpp:456` - [what changes here]
- [ ] `src/file3.h:78` - [what changes here]
[complete exhaustive list]
```

### 8. Create Validation Checklist

Define how to verify behavior preservation:

```markdown
### Behavior Validation Checklist
**Must verify after each stage:**
- [ ] Code compiles without errors/warnings
- [ ] [Specific behavior 1] works as expected
- [ ] [Specific behavior 2] works as expected
- [ ] No regressions in [system X]

**Final validation:**
- [ ] All test scenarios pass
- [ ] Debug visualizations still work
- [ ] Performance characteristics unchanged
- [ ] [Any other critical checks]
```

### 9. Estimate Per-Stage Complexity

For each migration stage:
- **Stage 1:** [X points] - [brief description]
- **Stage 2:** [Y points] - [brief description]
- **Stage 3:** [Z points] - [brief description]
- **Total:** [X+Y+Z points] (must be ≤8 points total)

**Complexity Scale:**
- **1-2 points:** Simple (single file, <5 call sites)
- **3-5 points:** Medium (multi-file, 5-15 call sites)
- **6-8 points:** Complex (cross-system, 15+ call sites)
- **>8 points:** Should be split

**If total exceeds 8 points:** Recommend breaking into multiple refactors or deferring.

### 10. Generate Detailed Refactor Plan

Update the refactor plan file at `PLANS/refactor_<refactor_name>.md` with the full plan:

```markdown
# Refactor Plan: [Refactor Name]

**Date:** [YYYY-MM-DD]
**Category:** Pattern Extraction | Simplification | System Design | Utilities | API Design
**Risk Level:** Low | Medium | High
**Estimated Complexity:** [X points]

---

## 1. Overview

**Current State:** [Brief description of the problem/pattern]

**Desired State:** [What we want to achieve]

**Rationale:** [Why this refactor improves the codebase; cite principles]

---

## 2. Impact Analysis

### Scope
- **Files Affected:** [count] files
- **Call Sites:** [count] locations
- **Systems Involved:** [list]

### Call Site Inventory
- `src/file1.cpp:123` - [brief description]
- `src/file2.cpp:456` - [brief description]
- [complete list]

### Hidden Dependencies
- [any non-obvious impacts]

### Risk Assessment
- **Risk Level:** Low/Medium/High
- **Justification:** [brief explanation]
- **Certainty Scores:** [system: score, system: score]

---

## 3. Before/After Examples

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

## 4. Migration Strategy

**Approach:** Staged | Linear

### Stage 1: Prepare
**Goal:** [What this stage accomplishes]
**Changes:**
- [Specific change 1]
- [Specific change 2]

**Verification:**
- [ ] Compiles without warnings
- [ ] [Specific check]

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

---

### Stage 3: Cleanup
**Goal:** Remove old code/APIs
**Changes:**
- [What gets deleted]

**Verification:**
- [ ] No references to old code remain
- [ ] [Final behavior check]

---

## 5. Validation Protocol

### Per-Stage Validation
- [ ] Code compiles without errors/warnings
- [ ] [Stage-specific check 1]
- [ ] [Stage-specific check 2]

### Final Validation
- [ ] **Test Scenario 1:** [Description] → Expected: [outcome]
- [ ] **Test Scenario 2:** [Description] → Expected: [outcome]
- [ ] **Test Scenario 3:** [Description] → Expected: [outcome]
- [ ] No regressions in [affected systems]
- [ ] Debug visualizations work correctly
- [ ] Performance characteristics preserved

---

## 6. Hidden Dependencies

**Debug/GUI Updates:**
- [List of debug/GUI files that need updates]

**Documentation:**
- [Which docs need revision]

**Performance:**
- [Any profiling or performance checks needed]

**Other:**
- [Any other non-obvious impacts]

---

## 7. Complexity Estimate

- **Stage 1 (Prepare):** [X points]
- **Stage 2 (Migrate):** [Y points]
- **Stage 3 (Cleanup):** [Z points]
- **Validation:** [W points]
- **Total:** [X+Y+Z+W points] (max 8 points)

**Complexity Scale:**
- **1-2 points:** Simple (single file, <5 call sites)
- **3-5 points:** Medium (multi-file, 5-15 call sites)
- **6-8 points:** Complex (cross-system, 15+ call sites)
- **>8 points:** Should be split

**Confidence:** High | Medium | Low
```

### Tone & Constraints

- Clear and concrete; avoid vague descriptions
- Show, don't tell (use code examples)
- Realistic about risk and effort
- Favor incremental staged approaches for medium/high risk
- Ensure plan respects "clarity over cleverness" and "simplicity over sophistication"
