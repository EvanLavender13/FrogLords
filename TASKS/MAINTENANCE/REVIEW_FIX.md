# Review Maintenance Fix

### 1. Review Development Principles

Read `AGENTS.md` for coding standards and quality guidelines.

### 2. Gather Context

1. Reference original item from SELECT_ITEM
2. Check modified files from IMPLEMENT_FIX
3. Read all changed source files (`.h` and `.cpp`)

### 3. Verify Fix Quality

**Issue Resolution:** Fix address maintenance item completely?

**Code Standards:**
- Naming: snake_case for all symbols
- Formatting: 4-space indent, braces on same line
- Organization: Correct structure (header guards, include order)

**Dependency Flow:**
- Includes respect bottom-up layering (Foundation → Character → Rendering → App)
- No circular dependencies

**Scope Discipline:**
- Only stated issue fixed
- No unrelated "while I'm here" changes

**Behavior Preservation:**
- Existing functionality unchanged (unless fixing bug)
- No new edge cases

### 4. Check for Side Effects

- **References:** If symbols renamed/moved, all call sites updated?
- **Headers:** If includes changed, no missing dependencies?
- **Constants:** If values consolidated, all uses point to new location?

### 5. Report Findings

Provide concise analysis:
1. **Fix Summary:** One sentence describing changes
2. **Verification:** Confirm fix resolves item (or note gaps)
3. **Issues Found:** List violations with file/line references
4. **Recommendation:** "Fix verified. Ready to finalize." or "Address issues before proceeding."

### 6. Await Guidance

Await user decision to:
- Fix identified issues
- Accept minor deviations
- Proceed to FINALIZE_FIX

### Tone & Constraints

- Concise; detail scales with complexity
- Focus on standards compliance and completeness
- Flag scope creep or unrelated changes
- Distinguish critical issues from polish
- Verify fix doesn't introduce new maintenance items
