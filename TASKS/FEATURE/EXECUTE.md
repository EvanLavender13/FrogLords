# Execute Feature

**Build truth, validate continuously, advance deliberately.**

---

## The Purpose

Implement validated stages from the approved plan, confirming principle adherence at each step. Execution is not coding—it's the careful construction of mathematically correct, principle-aligned systems.

**The best execution reveals the plan was wrong. The second best completes perfectly.**

---

## Prerequisites

- Approved plan from REVIEW_PLAN
- Foundation certainty ≥90%
- Debug visualization strategy ready
- Mathematical validation approach prepared

---

## Phase 1: Pre-Execution Validation

### Gather Truth
1. Read `AGENTS.md` - internalize the Six Pillars
2. Read `PLANS/<feature>_PLAN.md` - understand all stages
3. Read `PLANS/<feature>_PLAN_REVIEW.md` - remember the warnings

### Verify Foundation
- [ ] Plan approved and stages clear
- [ ] Dependencies at required certainty
- [ ] Debug tools available or buildable
- [ ] Rollback strategy understood

### Principle Check
Before writing any code, confirm:
1. **Simplicity:** Is each stage the minimal implementation?
2. **Composability:** Will it integrate cleanly?
3. **Mathematics:** Is validation ready?
4. **Emergence:** Are we enabling, not prescribing?
5. **Consistency:** Will behavior be predictable?
6. **Development:** Can we justify every line?

**If any answer is "no," stop and revise the plan.**

---

## Phase 2: Execute Planned Stages

Work through stages sequentially, validating each before proceeding.

### For Each Stage:

#### 1. Pre-Stage Validation
- [ ] Previous stage validated and stable
- [ ] Foundation remains solid
- [ ] Dependencies still satisfied
- [ ] Rollback point established

#### 2. Implement the Mathematics
- Write the core equations first
- Validate numerically in isolation
- Ensure coordinate system consistency
- Document any derived formulas

#### 3. Build Debug Visualization
- Make the invisible visible
- Show forces, states, transformations
- Ensure we can see what we're building
- Add controls for testing edge cases

#### 4. Create the Minimal Implementation
- Write only what the stage requires
- No "while we're here" additions
- No premature optimization
- No feature creep

#### 5. Validate Against Principles
After implementing, before moving forward:
- [ ] Simpler than alternatives?
- [ ] Composes without special cases?
- [ ] Mathematically correct?
- [ ] Enables more than intended?
- [ ] Behaves consistently?
- [ ] Every decision traceable?

**If any check fails, stop and reassess.**

#### 6. Mathematical Validation

##### Numerical Verification
- Test with known values
- Verify edge cases
- Check numerical stability
- Confirm unit consistency

##### Visual Confirmation
- Use debug visualization
- Compare against expected behavior
- Look for unexpected emergence
- Verify no hidden state corruption

##### Integration Testing
- Confirm clean composition
- No unexpected interactions
- Existing systems still function
- Performance acceptable

#### 7. Quality Gates
Before considering stage complete:

**Build Quality:**
- [ ] Compiles without warnings
- [ ] Formatted to standard
- [ ] No dead code added
- [ ] No commented experiments

**Principle Quality:**
- [ ] No complexity added
- [ ] No abstractions introduced
- [ ] No special cases created
- [ ] No magic numbers without derivation

**Behavioral Quality:**
- [ ] Predictable response
- [ ] Player control preserved
- [ ] Physics drives animation
- [ ] No prescribed outcomes

#### 8. Document Stage Completion

Mark stage complete in `PLANS/<feature>_PLAN.md`:
```markdown
### Stage [N]: [Title] ✓
**Completed:** [Date/Time]
**Validation:** Mathematical ✓ Visual ✓ Integration ✓
**Emergence:** [Unexpected behaviors discovered]
**Principles:** All six verified
```

**Only after full validation, proceed to next stage.**

---

## Phase 3: Post-Execution Validation

After all stages complete:

### System Integration
- [ ] All stages compose cleanly
- [ ] No hidden dependencies created
- [ ] Performance acceptable
- [ ] Debug visibility maintained

### Principle Verification
Validate the complete system against all six:
1. **Simplicity:** Is this simpler than alternatives?
2. **Composability:** Does it compose without special cases?
3. **Mathematics:** Is all math validated?
4. **Emergence:** What unexpected behaviors emerged?
5. **Consistency:** Is behavior predictable?
6. **Development:** Can we justify all decisions?

### Emergence Documentation
Document what surprised us:
```markdown
## Emergent Discoveries
- [Behaviors we didn't plan for]
- [System interactions that delighted]
- [Possibilities we didn't foresee]
```

---

## Phase 4: Final Documentation

Update `PLANS/<feature>_PLAN.md`:

```markdown
## EXECUTION COMPLETE

**Duration:** [Total time]
**Stages Completed:** [N of N]

### Validation Summary
- Mathematical: All formulas validated
- Visual: Debug visualization functional
- Integration: Clean composition verified
- Principles: All six maintained

### Emergent Discoveries
[What surprised us during implementation]

### Deviations from Plan
[Any changes made and why - if none, say so]

**Status:** Ready for CODE_REVIEW
```

---

## The Philosophy of Execution

### Truth Through Validation
Every line of code is a claim about reality. Validate the claim before accepting it as truth. Each stage proves itself before the next begins.

### Emergence Through Simplicity
Complex behaviors should surprise us, not complex code. If the code is complex, we've failed. The magic happens in the space between stages.

### Progress Through Patience
One perfect stage is worth ten rushed ones. Each stage builds on truth or collapses into chaos. Rushing creates technical debt that compounds exponentially.

### Stages Build Confidence
Early stages validate the approach. Middle stages build momentum. Final stages compose into emergence. Each successful stage strengthens certainty.

---

## Anti-Patterns to Avoid

### The Scope Creep
"While I'm here, I'll also..." - No. Do exactly the stage. Nothing more. Scope creep is the enemy of clarity.

### The Assumption
"This should work..." - Prove it works. Validate, don't assume. Assumptions compound into confusion.

### The Abstraction
"Let me generalize this..." - No. Build the specific. Emergence will generalize when ready.

### The Perfectionist
"Let me polish this..." - No. Validate and move on. Polish emerges from solid foundations, not forced refinement.

### The Stage Skipping
"I can do these together..." - No. Stages exist for validation. Combining them removes checkpoints.

---

## Special Considerations

### For Mathematical Stages
- Implement formula validation first
- Build numerical test harness
- Verify with known solutions
- Document derivations inline
- Show coordinate systems explicitly

### For Physics Stages
- Make forces visible
- Show velocity vectors
- Display collision normals
- Validate conservation laws
- Verify no frame-dependent behavior

### For Control Stages
- Preserve player intent absolutely
- Maintain immediate responsiveness
- Never buffer or delay input
- Test with extreme inputs
- Verify no animation blocking

### For Debug Visualization Stages
- Show what matters most
- Make state immediately clear
- Provide interaction controls
- Ensure no performance impact on gameplay
- Document what each visualization shows

---

## The Commitment

Execute stages that:
- Build on validated truth
- Can be seen and understood
- Preserve all six principles
- Enable unexpected emergence
- Leave the codebase simpler

Never execute stages that:
- Can't be validated
- Hide their behavior
- Add unnecessary complexity
- Prescribe specific outcomes
- Break player trust

**The best execution teaches us the plan was wrong. The second best builds exactly what was planned. Both are victories because both reveal truth.**

**This is the way.**