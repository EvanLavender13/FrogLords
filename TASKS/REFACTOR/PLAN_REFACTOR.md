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

1. Read `PLANS/REFACTOR_<refactor_name>.md` (from SELECT_REFACTOR)
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
 - Check naming families for consistency (e.g., math utilities: `_radians`/`_degrees` suffix symmetry, singular/plural, verb/object order)

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
- [ ] Naming consistency across new/affected utilities verified (e.g., `_radians`/`_degrees` families)
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

Append to `PLANS/REFACTOR_<refactor_name>.md`:

```markdown
---

## Refactor Plan

**Date:** [YYYY-MM-DD]
**Risk Level:** Low | Medium | High
**Complexity:** [X points] (max 8)

### Impact Analysis

**Files:** [count] | **Call Sites:** [count] | **Systems:** [list]

**Call Site Inventory:**
- `src/file1.cpp:123` - [description]
- `src/file2.cpp:456` - [description]

**Risk Justification:** [Why Low/Medium/High - cite call site count, system criticality, certainty]

### Before/After Examples

**Before:**
```cpp
// Current code
```

**After:**
```cpp
// Refactored code
```

**Improvements:** [How this is clearer/simpler]

### Migration Strategy

**Approach:** Staged | Linear

**Stage 1: Prepare**
- [What to implement]
- Verify: [ ] Compiles [ ] [Specific check]

**Stage 2: Migrate**
- [ ] `src/file1.cpp:123`
- [ ] `src/file2.cpp:456`
- Verify: [ ] Compiles [ ] [Behavior check]

**Stage 3: Cleanup**
- [What to delete]
- Verify: [ ] No references [ ] [Final check]

### Validation Protocol

**Per-Stage:** [ ] Compiles [ ] [Behavior check]

**Final:**
- [ ] [Scenario 1] → [Expected]
- [ ] [Scenario 2] → [Expected]
- [ ] No regressions in [systems]
```

### Tone & Constraints

- Clear and concrete; avoid vagueness
- Show, don't tell (use code examples)
- Realistic about risk and effort
- Favor staged approaches for medium/high risk
- Respect "clarity over cleverness" and "simplicity over sophistication"
