# Principle Review: Speed-Based Animation Scaling

### 1. Summary

The plans are generally well-aligned with the project principles. The feature is appropriately scoped as a small, low-risk extension of a proven system. The implementation plan is minimal and atomic. However, there is one significant misalignment: **this feature may be premature**. The design backlog explicitly notes "May not be necessary - current constant magnitude feels good," yet an entire iteration is being planned. This contradicts the "paradox of quality" principle and represents planning work that may be unnecessary.

### 2. Violations & Contradictions

- **Violation:** Premature Feature Development
  - **Source:** `PLANS/ARCHIVE/20251006_120000_iteration_speed_animation_scaling.md`, `PLANS/dependency_stack.md`
  - **Principle:** "Paradox of quality: accept temporary roughness early to maximize iteration count" / "Short plans → build → test → repeat"
  - **Details:** The design backlog states: "*May not be necessary - current constant magnitude feels good*" (line 34 in design_backlog.md). The dependency stack also lists this feature as "*May not be necessary*" (line 17). Despite these explicit warnings, a full iteration is being planned and executed. The principles emphasize testing before building: "*Evaluate grayboxes skillfully: don't reject working systems because they're ugly.*" If the current system feels good, there should be evidence of a problem first.
  - **Suggestion:** Before implementing, conduct focused self-testing with the current system. Document specific moments where the constant magnitude feels wrong (e.g., "at max speed, tilt feels too subtle" or "at slow speeds, tilt is distracting"). Only proceed if concrete problems are identified. Otherwise, mark this iteration as "investigated and deferred" and pull the next item from the backlog.

### 3. Misalignments & Risks

- **Misalignment:** Certainty Score May Be Inflated
  - **Source:** `PLANS/ARCHIVE/20251006_120000_iteration_speed_animation_scaling.md` (line 15)
  - **Principle:** "Dependency = if A changes, B must change. Uncertainty multiplies up the stack."
  - **Details:** The iteration plan claims 80% certainty based on it being "a small extension of a well-understood and functioning system." However, this assumes the feature is needed. The actual certainty should include the uncertainty about *whether the feature improves the experience*. If the current system already feels good (as noted in the backlog), the certainty of this feature adding value is much lower.
  - **Suggestion:** Revise the certainty score to ~50% to reflect uncertainty about the feature's value, not just implementation risk.

- **Misalignment:** Missing Test-First Evidence
  - **Source:** `PLANS/ARCHIVE/20251006_120000_iteration_speed_animation_scaling.md` (Section 2)
  - **Principle:** "Testing Protocol (Get Real Signals)" / "Short plans → build → test → repeat"
  - **Details:** The design rationale states a hypothesis ("*By scaling animation intensity with the character's velocity, the player will feel more connected...*") but doesn't include evidence that the current system fails this test. The principles emphasize: "*Sample size: watch enough tests to see patterns*" and "*Use metrics when helpful to see small effects*". This iteration should start with testing the *existing* system to validate the problem exists.
  - **Suggestion:** Add a pre-implementation testing step: "Play for 10 minutes at various speeds. Note specific moments where constant magnitude feels wrong. If no problems surface, defer this feature."

- **Misalignment:** Implementation Change Doesn't Match Current Code
  - **Source:** `PLANS/ARCHIVE/20251006_120000_implementation_speed_animation_scaling.md` (Section 2)
  - **Principle:** "Include at least 3 lines of context BEFORE and AFTER the target text"
  - **Details:** The implementation plan specifies changing `effective_tilt_magnitude = tilt_magnitude * (0.5f + velocity_scale * 1.0f);` to a new formula. However, this assumes the current implementation uses `velocity_scale`. If the actual code is different (e.g., already has some speed scaling), this change might be incorrect or redundant.
  - **Suggestion:** Before implementation, verify the exact current code in `animation.cpp`. Update the implementation plan with the actual context (3+ lines before/after) to ensure the replacement is unambiguous.

### 4. Actionable Items

- [ ] **Pre-Implementation Test:** Before coding, conduct 10-minute self-test with current system. Document specific moments where constant tilt magnitude feels problematic. If none found, defer feature.
- [ ] **Update Certainty Score:** Revise certainty in `iteration_speed_animation_scaling.md` from 80% to ~50% to reflect uncertainty about feature value, not just implementation ease.
- [ ] **Verify Current Implementation:** Read `src/character/animation.cpp` lines around `update_acceleration_tilt()` to confirm the exact code being changed. Update `implementation_speed_animation_scaling.md` with precise context.
- [ ] **Add Evidence Section:** If proceeding, add "Problem Evidence" section to iteration plan documenting specific gameplay moments that motivated this feature.
- [ ] **Consider Deferral:** Given the explicit "may not be necessary" notes in both the backlog and dependency stack, strongly consider deferring this feature and pulling the next item from the backlog instead. The principles emphasize maximizing iteration count, not implementing every idea.
