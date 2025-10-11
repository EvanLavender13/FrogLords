# Create Refactor Plan

### 1. Review Development Principles

Read `AGENTS.md` to ensure alignment with:
- "Clarity over cleverness"
- "Simplicity over sophistication"
- "Wait for third use" (rule of three)

### 2. Create Branch

Create refactor branch:
```powershell
./scripts/create-branch.ps1 refactor <refactor_name>
```

### 3. Gather Context

Extract refactor name from branch (`refactor/<refactor_name>`):

1. Read `PLANS/refactor_<refactor_name>.md` (from SELECT_REFACTOR)
2. Read all affected `.h` and `.cpp` files
3. Search for all usage locations to map dependencies

### 4. Analyze Impact

**Identify Call Sites:**
- Search for all usage locations
- Map which systems depend on the code
- Check for indirect dependencies
- Document every affected file with line numbers

**Assess Current State:**
- Understand original design rationale
- Identify repeated patterns or complexity
- Find edge cases to preserve
- Check for explanatory comments

**Evaluate Risk:**

Risk factors:
- Number of call sites (more = higher risk)
- System criticality (core systems = higher risk)
- Behavioral complexity (edge cases = higher risk)
- Certainty score (lower = higher risk)

Risk classification:
- **Low:** Single file, <5 call sites, high certainty (≥90%)
- **Medium:** Multi-file, 5-15 call sites, good certainty (70-90%)
- **High:** Cross-system, 15+ call sites, moderate certainty (70-80%)

**Check Hidden Dependencies:**
- Initialization order impacts?
- Memory layout changes (struct size/alignment)?
- Performance characteristic changes?
- Debug/GUI updates needed?
- Serialization impacts?

### 5. Define Before/After State

Show concrete examples:

**Before:**
```cpp
// Current code pattern (representative example with context)
```

**After:**
```cpp
// Refactored code (target state with context)
```

**Rationale:** Explain how "after" is clearer, simpler, or more maintainable.

### 6. Design Migration Strategy

Choose approach based on risk:

**Staged Approach (Medium/High Risk):**

*Pattern Extraction:*
1. **Prepare:** Create new abstraction without removing old code
2. **Migrate:** Convert call sites one-by-one
3. **Cleanup:** Remove old code

*Simplification:*
1. **Prepare:** Add new simplified implementation alongside old
2. **Switch:** Update call sites
3. **Cleanup:** Remove old implementation

*API Design:*
1. **Prepare:** Add new API with improved interface
2. **Deprecate:** Mark old API deprecated, forward to new
3. **Migrate:** Update call sites
4. **Cleanup:** Remove deprecated API

**Linear Approach (Low Risk):**
- Make all changes in one pass (<5 call sites)
- Test immediately after

### 7. Create Call Site Checklist

```markdown
### Call Site Migration
- [ ] `src/file1.cpp:123` - [what changes]
- [ ] `src/file2.cpp:456` - [what changes]
- [ ] `src/file3.h:78` - [what changes]
```

### 8. Create Validation Checklist

```markdown
### Validation Protocol

**Per-Stage:**
- [ ] Compiles without errors/warnings
- [ ] [Specific behavior 1] works
- [ ] [Specific behavior 2] works
- [ ] No regressions in [system X]

**Final:**
- [ ] All test scenarios pass
- [ ] Debug visualizations work
- [ ] Performance characteristics preserved
```

### 9. Estimate Complexity

**Per-Stage Estimate:**
- Stage 1: [X points] - [description]
- Stage 2: [Y points] - [description]
- Stage 3: [Z points] - [description]
- **Total:** [X+Y+Z] (max 8 points)

**Scale:**
- 1-2: Simple (single file, <5 call sites)
- 3-5: Medium (multi-file, 5-15 call sites)
- 6-8: Complex (cross-system, 15+ call sites)
- >8: Should be split or deferred

### 10. Append Plan to Refactor Document

Append to `PLANS/refactor_<refactor_name>.md`:

```markdown
---

## Refactor Plan

**Date:** [YYYY-MM-DD]
**Risk Level:** Low | Medium | High
**Estimated Complexity:** [X points]

### Impact Analysis

**Scope:**
- **Files Affected:** [count]
- **Call Sites:** [count]
- **Systems Involved:** [list]

**Call Site Inventory:**
- `src/file1.cpp:123` - [description]
- `src/file2.cpp:456` - [description]

**Risk Assessment:**
- **Level:** Low/Medium/High
- **Justification:** [brief explanation]
- **Certainty Scores:** [system: score]

### Before/After Examples

**Before:**
```cpp
// Current code
```

**After:**
```cpp
// Refactored code
```

**Key Improvements:**
- [Improvement 1]
- [Improvement 2]

### Migration Strategy

**Approach:** Staged | Linear

**Stage 1: Prepare**
- **Goal:** [What this accomplishes]
- **Changes:** [Specific changes]
- **Verification:**
  - [ ] Compiles without warnings
  - [ ] [Specific check]

**Stage 2: Migrate**
- **Goal:** [What this accomplishes]
- **Call Sites:**
  - [ ] `src/file1.cpp:123`
  - [ ] `src/file2.cpp:456`
- **Verification:**
  - [ ] All call sites compile
  - [ ] [Behavior check]

**Stage 3: Cleanup**
- **Goal:** Remove old code
- **Changes:** [What gets deleted]
- **Verification:**
  - [ ] No references remain
  - [ ] [Final check]

### Validation Protocol

**Per-Stage:**
- [ ] Compiles without errors/warnings
- [ ] [Stage-specific check 1]
- [ ] [Stage-specific check 2]

**Final:**
- [ ] **Scenario 1:** [Description] → [Expected outcome]
- [ ] **Scenario 2:** [Description] → [Expected outcome]
- [ ] No regressions in [systems]
- [ ] Debug visualizations work
- [ ] Performance preserved

### Hidden Dependencies

- **Debug/GUI:** [Files needing updates]
- **Documentation:** [Docs needing revision]
- **Performance:** [Profiling/checks needed]

### Complexity Breakdown

- Stage 1: [X points]
- Stage 2: [Y points]
- Stage 3: [Z points]
- Validation: [W points]
- **Total:** [sum] (max 8)
```

### Tone & Constraints

- Clear and concrete; avoid vagueness
- Show, don't tell (use code examples)
- Realistic about risk and effort
- Favor staged approaches for medium/high risk
- Respect "clarity over cleverness" and "simplicity over sophistication"
