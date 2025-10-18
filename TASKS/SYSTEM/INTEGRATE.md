# INTEGRATE - Production Implementation

**Polish when certain. Verify continuously. Ship truth.**

---

## Purpose

Transform the validated graybox into production-ready code while maintaining all mathematical guarantees and principle alignment.

**This is not adding features. This is replacing primitives with polish without changing behavior.**

---

## Prerequisites

- [ ] System APPROVED in validation (avg ≥7.0)
- [ ] `PLANS/<name>_VALIDATE.md` shows passing scores
- [ ] Graybox fully functional
- [ ] All tests passing
- [ ] Emergent behaviors documented

---

## Process

### 1. Plan Integration

In `PLANS/<name>_SYSTEM.md`, add:

```markdown
## Integration Plan

**What needs production implementation:**
- [ ] Visual assets (replace cubes/primitives)
- [ ] Audio (if needed)
- [ ] UI polish (if needed)
- [ ] Performance optimization
- [ ] Debug visualization (keep functional)

**What stays from graybox:**
- [x] All mathematical logic
- [x] All behavior
- [x] All tests
- [x] Debug visualization (kept active)

**Non-negotiable:**
- Keep all tests passing
- Maintain debug visualization
- No behavior changes
- No new features
- Verify no regressions

**Estimated time:** [hours/days]
```

---

### 2. Replace Primitives (If Needed)

**Often, graybox IS production.**

Ask first: **Does this need polish?**
- Graybox cubes often work fine in production
- Debug visualization is often better than art
- Don't polish for polish's sake

**Only polish if:**
- Clarity requires it (primitives confusing)
- Performance requires it (debug draw too slow)
- User experience requires it (visual communication needed)

**If polishing:**
1. Replace one primitive at a time
2. Run all tests after each replacement
3. Verify behavior identical
4. Keep debug visualization functional
5. Commit each replacement

---

### 3. Maintain Debug Visualization

**Debug visualization is not temporary. It's permanent.**

**Update debug visualization for production:**
```markdown
## Debug Visualization (Production)

**Always visible in dev builds:**
- Mathematical state (vectors, values)
- Decision points (state machine, behaviors)
- Relationships (dependencies, interactions)
- Performance (frame time, bottlenecks)

**Toggle in production builds:**
- [Key binding to show/hide]
- Default: hidden
- Saves to user config

**Visualization added:**
-
-

**Purpose:**
[How this helps debug and understand system in production]
```

**Never remove debug visualization. It's how you debug production issues.**

---

### 4. Performance Optimization

**Only if needed. Measure first.**

**Baseline performance:**
- Graybox FPS: __ fps
- Target FPS: __ fps
- Current bottlenecks: [profiled]

**If optimization needed:**
1. Profile (measure, don't guess)
2. Optimize biggest bottleneck only
3. Verify tests still pass
4. Measure improvement
5. Repeat if needed

**Rules:**
- Never optimize without profiling
- Never sacrifice correctness for speed
- Keep mathematical guarantees
- Maintain all tests

---

### 5. Integration Testing

**Verify the system integrates with existing systems:**

**Test matrix:**
```markdown
## Integration Testing

**System interactions:**
- [ ] System A + This = [expected behavior]
- [ ] System B + This = [expected behavior]
- [ ] System C + This = [expected behavior]

**Edge cases in integration:**
- [ ] Multiple systems active = [behavior]
- [ ] System disabled = [graceful degradation]
- [ ] System conflicts = [resolution]

**Performance in context:**
- [ ] FPS with all systems: __ fps
- [ ] Memory usage: __ MB
- [ ] Load time: __ ms

**Regressions:**
- [ ] System A still works
- [ ] System B still works
- [ ] System C still works
```

**Any regression → Fix before proceeding.**

---

### 6. Final Validation

**Re-run principle validation on integrated system:**

**Quick re-score:**
1. Radical Simplicity: __/10 (did integration add complexity?)
2. Composable Functions: __/10 (does it still compose?)
3. Mathematical Foundations: __/10 (guarantees maintained?)
4. Emergent Behavior: __/10 (emergence still happening?)
5. Consistency: __/10 (behavior still predictable?)
6. Principled Development: __/10 (decisions still justified?)

**Average: __/10**

**If average dropped below 7.0:**
- Integration violated principles
- Revert problematic changes
- Maintain graybox or simplify integration

**If average maintained or improved:**
- Integration successful
- Principles preserved
- Ready to strengthen

---

### 7. Update Documentation

In `PLANS/<name>_SYSTEM.md`:

```markdown
## Integration Complete

**Changes from graybox:**
- Visual: [what changed]
- Audio: [what added]
- Performance: [what optimized]
- Debug: [visualization maintained]

**What stayed the same:**
- All mathematical logic
- All behavior
- All tests
- All emergent properties

**Final principle scores:**
- Before integration: [avg]
- After integration: [avg]
- Change: [+/- delta]

**Regressions found and fixed:**
-
-

**Production status:**
- [x] Tests passing
- [x] Debug visualization functional
- [x] Performance acceptable
- [x] No regressions
- [x] Principles maintained
- [ ] Ready to strengthen foundation
```

---

### 8. Commit Integration

```bash
git add .
git commit -m "integrate: <name> production implementation

Graybox → Production: <what changed>
Tests: All passing
Performance: <fps before> → <fps after>
Principles: <avg score maintained>
Regressions: None

🤖 Generated with [Claude Code](https://claude.com/claude-code)
Co-Authored-By: Claude <noreply@anthropic.com)"
```

---

## Outputs

- [ ] Production implementation complete
- [ ] All tests passing
- [ ] Debug visualization maintained
- [ ] Performance acceptable
- [ ] No regressions
- [ ] Principle scores maintained (≥7.0)
- [ ] Integration committed
- [ ] Documentation updated

---

## Exit Criteria

**Proceed to STRENGTHEN when:**
- Integration complete
- All tests passing
- Debug visualization functional
- No regressions found
- Principle scores maintained
- System ready for production

**Return to previous steps if:**
- Tests fail → Fix or revert
- Regressions found → Fix integration
- Principles violated → Simplify or revert
- Performance unacceptable → Optimize or defer

---

## Next Step

**→ STRENGTHEN** (update foundation, celebrate, commit to main)

---

## Integration Philosophy

### Graybox Often IS Production

**Don't polish for polish's sake.**

Many successful games ship with:
- Primitive graphics (Minecraft, Dwarf Fortress)
- Debug visualization (strategy games, sims)
- Minimal UI (roguelikes, puzzle games)

**If graybox works, ship it.**

### Never Break What Works

**Integration is high-risk, low-reward.**

You're not adding value (system already validated). You're risking regressions while making it pretty.

**Minimize risk:**
- Small changes
- Constant testing
- Keep debug viz
- Maintain math

### Polish is Debt Payment

**Every polished asset is:**
- Code that must be maintained
- Art that must update with behavior changes
- Memory that could be used elsewhere
- Load time added to startup

**Only add if value exceeds cost.**

### Debug Visualization Forever

**In five years, you'll need to debug this.**

Your future self will thank you for keeping debug visualization:
- Shows what's happening (vs. guessing)
- Reveals edge cases (vs. hunting)
- Validates fixes (vs. hoping)
- Teaches new devs (vs. archeology)

**Never remove it.**

---

**Polish when certain. Test continuously. Ship truth.**
