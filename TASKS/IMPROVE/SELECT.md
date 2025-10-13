# Select Improvement Item

### 1. Review Development Principles

Read `AGENTS.md` to synthesize core project principles:
- "Clarity over cleverness; simplicity over sophistication"
- "Wait for third use" (rule of three for pattern extraction)
- Layered architecture (Foundation → Character → Rendering → App)
- Code quality standards and naming conventions

### 2. Review Improve Backlog

1. Open `PLANS/IMPROVE_BACKLOG.md`
2. Scan sections in priority order: Critical → High → Medium → Low
3. Look for items that:
   - Have clear, actionable descriptions
   - Affect stable, well-understood code (≥70% certainty)
   - Can be completed independently
   - Meet stability and rule-of-three requirements

### 2.5. Scan for Batching Opportunities

**Before selecting, check if multiple similar items can be batched:**

1. **Identify patterns:** Do 3+ items share the same file, category, or fix type?
   - Example: Multiple unused includes across different files
   - Example: Several constant extractions in same system
   - Example: Related enum/naming violations

2. **Evaluate batching criteria:**
   - Same file: Can all be fixed in one pass
   - Same category: Similar rationale and fix approach (check tags: #cleanup, #architecture, etc.)
   - Low interdependence: Fixes don't depend on each other
   - Same complexity tier: All trivial (1-2 pts) or all standard (3-5 pts)
   - Can complete same session: Realistic to finish in one sitting

3. **If 3+ similar items found:**
   - Create unified backlog item: "Cleanup: Remove Unused Includes" or "Extract Locomotion Constants"
   - Document all batched items with unified rationale
   - Implement as single improvement session (reduces commit overhead)
   - Tag batch item: #batch

4. **If no batch opportunity:**
   - Proceed with single-item selection

**Purpose:** Reduces commit overhead for related fixes; prevents multiple separate commits for items that share context and can be validated together.

### 3. Selection Criteria

#### Must-Have
- **Stability Gate:** System at ≥70% certainty in `DEPENDENCY_STACK.md` (unless Critical severity)
- **Clear Definition:** Current state and proposed fix concrete
- **No Blockers:** No missing prerequisites or active feature work on affected systems
- **Rule of Three:** (For pattern extractions) Pattern appears 3+ times, OR is canonical data consolidation with documented rationale

#### Tie-Breakers
- Higher severity/priority (Critical > High > Medium > Low)
- Higher impact (affects multiple systems, prevents bugs, enables future work)
- Lower complexity (prefer 1-3 points when learning or testing workflow)
- Better isolation (single file > multi-file; independent > coupled)

### 4. Verify Stability & Prerequisites

**Stability Check:**
1. Read `PLANS/DEPENDENCY_STACK.md` and locate affected systems in dependency tree
2. Verify all affected systems show ≥70% certainty
3. Confirm no active feature development on these systems (check tree connections and Layer Details)
4. **Critical items bypass this check** (architectural violations must be fixed regardless)

**Rule of Three Check (for pattern extractions tagged #pattern-extraction):**
1. Read files mentioned in backlog item
2. Confirm pattern appears 3+ times
3. Verify pattern is identical (not just similar)
4. **OR verify canonical data justification** (e.g., joint indices, global constants)

**If checks fail:** Skip to next candidate or defer until conditions met.

### 5. Classify Complexity

**Path A: Trivial (1-2 points)**
- Single file, <10 lines changed
- Mechanical, no behavior change
- Examples: Remove unused include, fix typo, extract file-scope constant

**Path B: Standard (3-8 points)**
- Multi-file or architectural changes
- Behavior-preserving refactors
- Pattern extraction with 3+ uses
- Examples: Decouple layers, extract helper function, consolidate duplicates

### 6. Create Improvement Document

Create description and save to `PLANS/IMPROVE_<item_name>.md`:

```markdown
# [Item Name]

**Severity/Priority:** [Critical | High | Medium | Low]

**Current Issue:** [What problem exists today? 1-2 sentences]

**Files Affected:**
- `path/to/file.cpp`
- `path/to/other.h`

**Proposed Fix:** [What improvement will be made? 1-2 sentences]

**Rationale:** [Why this improves the codebase - cite principles from AGENTS.md]

**Workflow Path:** Path A (Trivial) | Path B (Standard)

**Estimated Complexity:** [1-8 points]

**Risk Level:** Low | Medium | High [with brief justification]

**Tags:** [From backlog: #architecture, #cleanup, #pattern-extraction, etc.]
```

### 7. Optional: Create Branch (Path B recommended)

For Standard improvements (3-8 pts):
```powershell
./scripts/create-branch.ps1 improve <item_name>
```

Trivial fixes can work directly on current branch.

### 8. Document Selection

State clearly:
- Item name/description
- Severity/priority level
- Files affected
- Workflow path (A or B)
- Document created: `PLANS/IMPROVE_<item_name>.md`
- Branch created (if Path B): `improve/<item_name>`

---

## When Nothing Qualifies

If no items meet selection criteria:
1. Document why (stability gates, missing prerequisites, rule violations)
2. Suggest conditions needed for viability
3. Consider running REVIEW_CODEBASE to populate backlog
4. Return to other workflows (FEATURE or handle lower-priority improvements)

---

## Tone & Constraints

- Concise and direct
- Prefer Critical/High severity when stable
- Avoid uncertain or actively-changing systems (unless Critical architectural violation)
- Respect "rule of three" discipline for pattern extractions
- Select one item at a time (or one batch)
- Skip items requiring major architectural decisions (escalate to design discussion)
