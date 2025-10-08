# Principle Review: Primary Skeletal Animation

### 1. Summary
Plans stay largely in sync with the procedural animation principles: they extend the proven locomotion phase into the skeleton while keeping the work inside the debug graybox, remove the legacy sine probe, and respect the layering (locomotion → skeletal → reactive). The limb angle authority is now centralized inside the skeletal animation system per the dependency guidance.

### 2. Violations & Contradictions
- None identified.

### 3. Misalignments & Risks
- **Misalignment:** Ambiguous elbow handling adds avoidable complexity
  - **Source:** `PLANS/ARCHIVE/20251007_220651_implementation_primary_skeletal_animation.md`
  - **Principle:** "Simplicity over sophistication" / "Clarity over cleverness" (`AGENTS.md`)
  - **Details:** Step 1 asks the keyframe struct to track elbows alongside shoulders/hips/knees even though the feature spec locks elbows at 90°. Later steps then reassert the lock manually. This invites contradictory edits and muddies the graybox scope.
  - **Suggestion:** Treat elbows as a fixed constant sourced from the cached T-pose (or a single tunable) instead of per-keyframe data; keep the run-cycle keyframe struct focused on the six driven joints.

### 4. Actionable Items
- [x] Consolidate limb angle authority inside `skeletal_animation_system` and have the GUI operate on that single source of truth. *(2025-10-07 plan update)*
- [ ] Trim elbow entries from the run-cycle keyframe data and rely on the locked 90° value when composing poses.