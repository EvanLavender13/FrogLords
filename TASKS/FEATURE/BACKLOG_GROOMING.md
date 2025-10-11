# Backlog Grooming

### 1. Review Document Accumulation

1. Open `PLANS/DEPENDENCY_STACK.md` and `PLANS/DESIGN_BACKLOG.md`
2. Identify signs of document bloat:
   - Completed layers (✅) with extensive retrospectives still inline
   - Backlog items marked complete with learning notes that duplicate implementation docs
   - Low-certainty (<30%) items deferred for multiple iterations without clarity gains
   - Architecture validation notes that served their purpose but no longer guide future work

### 2. Collapse Dependency Stack

1. **Consolidate Completed Layers:**
   - Group multiple ✅ layers into cohesive "Completed Foundation" sections
   - Keep: Architecture patterns that inform future work, critical learnings affecting design decisions
   - Remove: Detailed retrospectives already archived in `PLANS/*.md` and `PLANS/*.md`
   - Example: Collapse "Reactive Systems Layer (100%) ✅" details into single summary line with links to archived docs

2. **Simplify Layer Descriptions:**
   - Keep brief "Status" and "Dependencies" for completed layers
   - Move detailed "Key Learning" and "Architecture Validated" sections to implementation archives if not already done
   - Retain only systemic insights that affect dependency planning (e.g., "Motion vs. Structure" note)

3. **Update Current Focus:**
   - Ensure `← YOU ARE HERE` marker points to accurate layer
   - Verify percentage completions reflect current state
   - Clean up stale "Next Candidates" lists from completed sections

### 3. Prune Design Backlog

1. **Remove Completed Items:**
   - Delete backlog entries marked `✅ COMPLETE` (learnings already preserved in implementation docs)
   - Exception: Keep completed items only if they serve as explicit reference template for similar future work
   - Remove entire subsections if all items are complete

2. **Prune Low-Certainty Items:**
   - Remove items with certainty <30% that haven't gained clarity after 2+ iterations
   - Remove items marked "DEFERRED" without clear reconsideration trigger
   - Exception: Keep wild speculation sections (e.g., "Frogs Rule! 🐸") if they spark joy—mark clearly as creative exploration

3. **Consolidate Learning Notes:**
   - For completed items remaining as references, trim "Learning" sections to 1-3 key bullet points
   - Remove redundant learning notes that duplicate information in multiple places

### 4. Update Unlocked Dependencies

1. **Identify Newly Unblocked Features:**
   - Search for backlog items with prerequisites now marked ✅
   - Update certainty scores if prerequisite completion reduced implementation risk
   - Add "Status: Now unblocked" note with date

2. **Refresh Prerequisite Chains:**
   - Remove prerequisite references to features now part of consolidated foundation
   - Example: Change "Prerequisite: Debug Draw System ✅" to "Prerequisite: Foundation ✅" if debug draw is now foundational

### 5. Archive Historical Context

If collapsing removes significant historical context that might be useful for retrospectives:

1. Create `PLANS/ARCHIVE/` directory if it doesn't exist
2. Move detailed retrospectives and architecture notes to `PLANS/ARCHIVE/dependency_stack_snapshot_<date>.md`
3. Add link in main document: "Historical snapshots: See `PLANS/ARCHIVE/`"

### 6. Verify Consistency

1. Cross-check that certainty scores match between DEPENDENCY_STACK and DESIGN_BACKLOG
2. Ensure "YOU ARE HERE" marker aligns with current git branch (`feature/<feature_name>`)
3. Verify removed items aren't referenced elsewhere in active planning docs

### Tone & Constraints

- Be aggressive about pruning; documents should guide *future* work, not serve as retrospective archives
- Preserve only learnings that change how you approach similar problems
- Keep completed work summaries to one line + link to detailed docs
- Low-certainty items that haven't gained traction after 2+ iterations are noise—remove them
- Documents should feel "breathable" after collapse (not dense walls of completed work)

---

### Example: Before Collapse

```markdown
### Reactive Systems Layer (Implemented, ~100% certain) ✅

**Status:** Architecture proven, tuning UI complete, ready for iteration

**Completed: Acceleration Tilt ✅**
- Character-local space transformation of world acceleration
- Exponential smoothing for natural response
- Pure reactive layer ("do no harm" principle verified)
- 45 lines of code, 2 tunable parameters
- UI controls: tilt_smoothing, tilt_magnitude with real-time pitch/roll display

**Completed: Landing Spring ✅**
- Spring-damper driven crouch/recover on landing
- Impulse proportional to fall velocity
- Critically damped for natural feel (no bounce)
- Visual debug spring (collision sphere to body center)
- UI controls: stiffness, damping, impulse_scale with spring state display

**Architecture Validated:**
- `character::animation_state` pattern works well (proven twice)
- Update timing: after physics, after orientation, before render
- Transform injection: position → orient → landing offset → tilt → render
- Flag-based event communication (just_landed) pattern works
```

### Example: After Collapse

```markdown
### Reactive Systems Layer (100%) ✅

**Status:** Architecture proven and stable

- Acceleration tilt, landing spring implemented and tuned
- `character::animation_state` pattern validated
- See `PLANS/reactive_animation.md` for details

**Dependencies:** Requires stable physics core
```

---

### When to Run This Task

Run this collapse task when:
- DEPENDENCY_STACK has 3+ completed layers with inline retrospectives
- DESIGN_BACKLOG has 5+ completed items still listed
- Planning docs feel cluttered and hard to navigate
- Starting a new major development phase and want clean slate

Recommended frequency: Every 2-3 completed iterations, or when "YOU ARE HERE" marker moves up significantly in the stack.
