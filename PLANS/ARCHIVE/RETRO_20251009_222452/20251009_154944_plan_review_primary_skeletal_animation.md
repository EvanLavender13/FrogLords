# Principle Review: Primary Skeletal Animation

### 1. Summary

The plans are **well-aligned** with core principles. The feature correctly scopes down to a "stupid simple core" (threshold-based pose switching, no blending), builds on a proven foundation (Static Keyframe Preview validated quaternions), and follows the graybox-first approach. The implementation plan correctly identifies system ownership and integration points with no architectural conflicts. Certainty score (75%) is realistic given the simplicity of the scope. Minor risks exist around tuning `cycle_length`, but real-time feedback mitigates this. No significant violations or contradictions found.

### 2. Violations & Contradictions

**None identified.** The plans adhere to all relevant principles from `AGENTS.md`.

### 3. Misalignments & Risks

- **Potential Risk: Threshold Values May Need Iteration**
  - **Source:** [PLANS/ARCHIVE/20251009_154944_implementation_primary_skeletal_animation.md](PLANS/ARCHIVE/20251009_154944_implementation_primary_skeletal_animation.md:66-74)
  - **Principle:** "Iteration over planning" / "Short planning horizons"
  - **Details:** The implementation plan uses fixed threshold values (0.25/0.5/0.75) for pose selection. While these are reasonable starting points, the plan acknowledges they may feel arbitrary and suggests "tune visually; consider parametrizing if fixed values fail." This is appropriate mitigation, but there's a slight tension between "hard-code thresholds" and "defer parametrization until needed."
  - **Assessment:** This is **not a violation**—the plan correctly defers premature abstraction. However, be prepared to parametrize thresholds quickly if initial values don't feel right during first playtest.
  - **Suggestion:** If thresholds need tuning during implementation, add them to the character panel UI immediately rather than iterating on hardcoded values. This follows the proven pattern from reactive animation systems (landing spring, acceleration tilt) where exposing parameters early shortened iteration loops.

- **Minor Misalignment: Manual Pose UI Visibility Logic**
  - **Source:** [PLANS/ARCHIVE/20251009_154944_implementation_primary_skeletal_animation.md](PLANS/ARCHIVE/20251009_154944_implementation_primary_skeletal_animation.md:138-144)
  - **Principle:** "Clarity over cleverness"
  - **Details:** The plan proposes hiding pose combo box and joint overrides behind a `use_manual_pose_selection` checkbox. While this reduces UI clutter, it adds a layer of indirection (user must enable manual mode to see debug controls).
  - **Assessment:** Minor tradeoff between UI cleanliness and discoverability. Current debug UI has all controls visible by default (easier to discover but noisier).
  - **Suggestion:** Consider keeping controls visible but **disabled** (grayed out) when manual mode is off, with a tooltip explaining "Enable Manual Pose Selection to use." This preserves discoverability while preventing accidental interference with automatic cycling. However, if current plan feels cleaner during implementation, stick with it—this is a minor point.

- **Observation: Stop Behavior Is Implicit**
  - **Source:** [PLANS/ARCHIVE/20251009_154944_feature_primary_skeletal_animation.md](PLANS/ARCHIVE/20251009_154944_feature_primary_skeletal_animation.md:43-45), [PLANS/ARCHIVE/20251009_154944_iteration_primary_skeletal_animation.md](PLANS/ARCHIVE/20251009_154944_iteration_primary_skeletal_animation.md:96-98)
  - **Principle:** "Testing Protocol (Get Real Signals)"
  - **Details:** The plans correctly note that stopping mid-stride freezes the pose (distance stops → phase stops → same pose selected, no special handling). This is intended behavior and matches body bounce system.
  - **Assessment:** **Not a problem**, but worth explicit testing attention. The success criteria include "Stopping mid-stride freezes pose (no pop/snap to neutral)," which is good. Ensure this is tested in the first playtest—if frozen mid-step poses look broken, the threshold approach may need revision.
  - **Suggestion:** During self-test, deliberately stop at different phases (mid-STEP_LEFT, mid-NEUTRAL) and verify the visual feel. If frozen non-neutral poses feel awkward, consider adding a "velocity-threshold snap to neutral" behavior (e.g., when speed drops below 0.1 m/s, transition to NEUTRAL over 0.2s). However, **only add this if testing reveals a problem**—don't speculate.

### 4. Actionable Items

- [x] Plans are aligned—**proceed with implementation as written**
- [ ] During first playtest, **test stop behavior explicitly** at various phases (STEP_LEFT, NEUTRAL, STEP_RIGHT) to verify frozen poses feel acceptable
- [ ] If threshold values (0.25/0.5/0.75) feel wrong during implementation, **expose them as UI sliders immediately** rather than iterating on hardcoded values (follows proven tuning pattern)
- [ ] If manual pose UI visibility logic feels confusing during implementation, consider **grayed-out disabled state** instead of hiding controls entirely (minor UX improvement, not critical)
- [ ] After graybox validation, **update certainty score** in [DEPENDENCY_STACK.md](PLANS/DEPENDENCY_STACK.md) based on actual tuning difficulty (expect increase to 85-90% if thresholds work well)
