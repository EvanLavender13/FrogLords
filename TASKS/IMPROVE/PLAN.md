# Plan Improvement (Standard Path Only)

**Prerequisites:** Completed SELECT; created `PLANS/IMPROVE_<name>.md`; on `improve/<name>` branch

---

### 1. Analyze Current State

Read all affected files completely:
- Understand existing patterns and dependencies
- Identify all code that will change
- Note any edge cases or special handling
- Document file/line references in plan

**For pattern extractions:** Verify code structure shows genuine duplication before planning:
- Cite specific line numbers for each occurrence
- Confirm logic is duplicated (not reuse of existing abstractions)
- Example: "Lines 238-244, 319-323 in keyframe.cpp and 197-203 in animation.cpp all contain identical `apply_joint` lambda logic"
- Prevents false premise plans where assumed duplication doesn't exist

### 2. Assess Impact & Risk

**Impact Analysis:**
- Which systems depend on the code being changed?
- Are there indirect callers or consumers?
- Will changes propagate to other files?
- Check `#include` statements for dependency chains

**Risk Assessment:**
| Risk Level | Indicators |
|------------|------------|
| Low | Single system, well-tested, clear pattern, behavior-preserving |
| Medium | Multi-system, complex logic, some edge cases |
| High | Core architecture, many dependents, subtle behavior changes |

Document risk factors and mitigation strategies.

### 3. Design the Solution

**For Pattern Extraction:**
- Function signature (parameters, return type)
- Placement (which file/namespace?)
- Visibility (public API or internal helper?)
- Name (follows conventions in `CONVENTIONS.md`)

**For Architectural Changes:**
- New structure/organization
- Data flow after change
- Which headers move where?
- Updated `#include` dependencies

**For Cleanup:**
- Exactly what gets removed/simplified
- Validation that removed code is truly unused

### 4. Break Into Stages

Decompose work into logical, verifiable stages:

**Example (Decouple Layers):**
1. Create new shared header with extracted type
2. Update original file to include new header
3. Update consuming file to include new header
4. Remove old dependency
5. Verify build and test

**Each stage should:**
- Be independently verifiable
- Build successfully
- Leave codebase in working state
- Take <30 min to implement

### 5. Write Validation Checklist

Create concrete success criteria:

```markdown
## Validation Checklist

### Build & Compile
- [ ] Clean build succeeds (no errors, no warnings)
- [ ] All affected files compile

### Correctness
- [ ] Behavior unchanged (or changed as intended)
- [ ] No new compiler warnings
- [ ] Edge cases handled

### Architecture
- [ ] Dependency flow correct (Foundation → Character → Rendering → App)
- [ ] No reverse dependencies introduced
- [ ] Follows naming conventions (snake_case)

### Code Quality
- [ ] No duplicated logic remains
- [ ] Comments updated if needed
- [ ] clang-format applied
- [ ] clang-tidy passes
```

### 6. Append Plan to Document

Add to `PLANS/IMPROVE_<name>.md`:

````markdown
---

## PLAN

**Created:** [Date/Time]

**Branch:** `improve/<name>`

### Impact Analysis

**Affected Systems:**
- System A (file1.cpp, file2.h)
- System B (file3.cpp)

**Dependencies:**
[What depends on this code? Who calls it?]

**Risk Level:** [Low | Medium | High]

**Risk Factors:**
- Factor 1
- Factor 2

**Mitigation:**
- How we'll reduce risk

### Proposed Changes

**Stage 1:** [Description]
- Change X in file.cpp
- Update Y in header.h
- Validation: Build succeeds, no new warnings

**Stage 2:** [Description]
- ...

[Continue for all stages]

### Validation Checklist

[Copy checklist from step 5]
````

### 7. Verify Plan Completeness

**Checklist:**
- [ ] All affected files identified
- [ ] Impact and risk documented
- [ ] Solution designed with clear stages
- [ ] Each stage has validation criteria
- [ ] Follows architecture principles (check `AGENTS.md`)
- [ ] Plan appended to `PLANS/IMPROVE_<name>.md`

### 8. Document Plan Completion

State clearly:
- Plan appended to `PLANS/IMPROVE_<name>.md`
- Number of stages
- Estimated time per stage
- Risk level and mitigation approach
- Ready for REVIEW_PLAN (will be appended to same document)

---

## Tone & Constraints

- Be concrete: line numbers, file paths, exact changes
- Stage size: <30 min per stage for tight validation loops
- Validate often: every stage should build and work
- Conservative: favor small, safe changes over large rewrites
- Follow conventions: check `CONVENTIONS.md` for naming, formatting, structure
- Document trade-offs: if multiple approaches considered, explain choice
