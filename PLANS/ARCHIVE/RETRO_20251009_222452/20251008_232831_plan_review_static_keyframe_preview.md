# Principle Review: Static Keyframe Preview

**Review Status:** All actionable items resolved (2025-10-07). Plans are now aligned with core principles.

### 1. Summary

The plans are generally well-aligned with core principles. The feature correctly scopes down from the failed primary skeletal animation attempt by validating the quaternion keyframe data structure in isolation before re-attempting locomotion integration. The graybox approach is sound—hardcoded poses with manual GUI selection removes complexity while proving the foundation. The implementation plan demonstrates good bottom-up thinking with a clear architectural audit. 

**Resolution:** Minor misalignments identified during review have been addressed. Plans now commit to clear scope (8 joints, 3 poses, manual selection only), establish concrete T-pose baseline verification, defer tuning UI appropriately, and focus success criteria on visual correctness rather than numeric precision.

### 2. Violations & Contradictions

No direct violations of core principles found.

### 3. Misalignments & Risks

- **Misalignment:** Premature Implementation Detail (Transform Composition)
  - **Source:** `PLANS/implementation_static_keyframe_preview.md` (Step 3, lines 115-140)
  - **Principle:** "Simplicity over sophistication" / "Wait for the third use before abstracting"
  - **Details:** The implementation plan includes detailed pseudocode for quaternion-to-matrix composition with T-pose baseline extraction (`glm::quat_cast`, translation extraction, matrix multiplication order). While technically thorough, this level of detail risks implementation paralysis before the core validation test ("do poses display correctly?"). The plan also contains a technical error in the pseudocode: `glm::translate(rotation_mat, t_pose_pos)` applies translation incorrectly (translation should be applied first, then rotation: `glm::translate(identity, pos) * rotation_mat`).
  - **Suggestion:** Simplify Step 3 to: "Apply keyframe quaternions to skeleton joints; compose with T-pose baseline using GLM functions; call `update_global_transforms()`. If visual artifacts appear, debug composition order." The implementation checklist already captures the correct approach ("translation first, then rotation"). Remove conflicting pseudocode or mark it as exploratory notes rather than implementation spec.

- **Misalignment:** Scope Uncertainty (Extended Joint Set)
  - **Source:** `PLANS/ARCHIVE/20251008_232831_iteration_static_keyframe_preview.md` (Section 4, Graybox Scope, Step 1)
  - **Principle:** "Stupid Simple Core" / "Iteration over planning"
  - **Details:** The plan defines a "minimum joint set" (8 joints: shoulders, elbows, hips, knees) but repeatedly mentions an "extended set" (11 joints: add spine_upper, ankles) with language like "stretch to 11 if time permits." This introduces scope ambiguity. The core validation hypothesis ("quaternion keyframes work correctly") can be tested with just arms (4 joints) or even a single joint. Adding 11 joints doesn't increase validation confidence proportionally to implementation time.
  - **Suggestion:** Commit to the 8-joint minimum for iteration 1. Defer spine/ankles to a follow-up iteration if the 8-joint validation succeeds and reveals a need for torso lean or grounded feet. The plan horizon is already tight (1 day max, 3-5 hours first loop). Avoid "stretch goals" that blur the acceptance criteria.

- **Misalignment:** Tuning UI Inclusion (Unnecessary Complexity)
  - **Source:** `PLANS/ARCHIVE/20251008_232831_iteration_static_keyframe_preview.md` (Section 4, Graybox Scope, Step 4)
  - **Principle:** "Graybox before polish" / "Premature optimization"
  - **Details:** Step 4 proposes adding real-time tuning exposure (Euler angle sliders) during the initial implementation. The plan itself hedges: "if time allows; fallback to hardcoded-only if quaternion UI is complex." This is premature—the validation hypothesis doesn't require runtime tweaking. Hardcoded poses can be adjusted by editing `keyframe.cpp` and rebuilding (already the workflow for all other tuning parameters like acceleration tilt, which only gained a UI *after* the core system was validated).
  - **Suggestion:** Defer Step 4 entirely to Section 8 "Iteration & Tuning" where it's already documented as optional. The first loop (Steps 1-3) should produce a binary pass/fail: "Do the 3 hardcoded poses display correctly when selected via GUI?" If angles are wrong, edit `keyframe.cpp`, rebuild, retest. Add tuning UI only if the edit-rebuild loop becomes intolerable (principle: "wait for the third use before abstracting").

- **Risk:** Over-Engineering Transform Extraction
  - **Source:** `PLANS/ARCHIVE/20251008_232831_implementation_static_keyframe_preview.md` (Step 3, pseudocode)
  - **Principle:** "Clarity over cleverness" / "Bottom-up dependencies"
  - **Details:** The plan suggests extracting T-pose rotations via `glm::quat_cast(local_transform)` and composing with pose quaternions. However, the current T-pose (`t_pose.cpp`) likely uses identity rotations (joints oriented along their local axes without additional rotation). Attempting to extract a quaternion from an arbitrary transform matrix is fragile (loses information if the matrix includes non-uniform scaling or skew, though unlikely here). The simpler approach: assume T-pose rotations are identity, compose pose quaternions directly, apply to fresh `translate(pos) * mat4_cast(quat)` transforms.
  - **Suggestion:** Start with the assumption that T-pose local rotations are identity (verify by inspecting `t_pose.cpp` initialization). Build joint transforms as: `local_transform = translate(t_pose_pos) * mat4_cast(pose_quat)`. Only add rotation extraction if visual testing reveals the T-pose has non-identity rotations. This matches the "build, test, iterate" principle rather than pre-solving a problem that may not exist.

### 4. Actionable Items

- [x] Simplify `20251008_232831_implementation_static_keyframe_preview.md` Step 3 pseudocode: remove conflicting transform composition examples; defer detailed GLM usage to implementation discovery (the checklist already has correct guidance). **RESOLVED 2025-10-07:** Removed detailed pseudocode; replaced with high-level checklist + implementation notes covering key concerns (T*R order, T-pose baseline verification).
- [x] Remove "extended joint set (11 joints)" from iteration 1 scope. Commit to 8-joint minimum. Add spine/ankles as a separate backlog item if validation reveals a need. **RESOLVED 2025-10-07:** Removed all references to "extended/stretch to 11 joints." Iteration 1 commits to 8 joints only (shoulders, elbows, hips, knees). Added "Extended keyframe joint set" to DESIGN_BACKLOG.md as future extension (deferred until 8-joint validation succeeds). Updated feature, iteration, and implementation plans to remove scope ambiguity.
- [x] Defer Step 4 (real-time tuning UI) from graybox scope entirely. Move to Section 8 "Iteration & Tuning" as optional follow-up. First loop validates with hardcoded poses + edit-rebuild workflow only. **RESOLVED 2025-10-07:** Step 4 already removed from graybox scope (Steps 1-3 only). Updated implementation plan Section 8 to emphasize edit-rebuild as primary workflow; tuning UI documented as optional fallback only if edit-rebuild becomes intolerable. Updated iteration plan risks section and backlog candidates to clarify tuning UI deferral criteria. Implementation cost estimate reduced from 3-5 hours to 2.5-3.5 hours.
- [x] Verify T-pose initialization in `t_pose.cpp`: are joint local rotations identity or non-identity? Update implementation plan with concrete finding (avoids guessing during implementation). **RESOLVED 2025-10-07:** Inspected `src/character/t_pose.cpp` - verified all joint local_transform fields are pure translations with identity rotations (e.g., `glm::translate(glm::mat4(1.0f), glm::vec3(...))`). Added "T-Pose Baseline Verification" section to implementation plan Foundation Architecture Audit with concrete code example and implementation implications. Updated Step 3 implementation notes to reference verified finding and provide exact code pattern for transform composition.
- [x] Clarify success criteria: primary test is "STEP_LEFT displays visibly different from T_POSE with correct limb directions"; full validation is "all 4 poses selectable, instant switching, no visual artifacts." Remove ambiguous angle values (~30°, ~45°) from acceptance—those are implementation targets, not validation metrics. The test is visual correctness, not numeric precision. **RESOLVED 2025-10-07:** Updated success criteria in both implementation and feature plans. Removed specific angle values from acceptance criteria; reframed as visual quality checks (e.g., "left arm swings back, right arm swings forward"). Added note distinguishing implementation targets (hardcoded angle values) from acceptance criteria (visual correctness). Expanded full validation checklist with concrete visual checks (symmetry, transitional poses, coherence).
- [x] Consider adding a "Step 0" to implementation plan: inspect `t_pose.cpp` and document actual joint positions and rotations. This establishes the baseline before modifications (principle: "gather context first"). **RESOLVED 2025-10-07:** Added "T-Pose Baseline Verification" section to Foundation Architecture Audit (before "Systems Involved" section). Documents verified finding, provides code example, and states implementation implications. This serves as Step 0 context gathering.
