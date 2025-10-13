# Air Locomotion Weights (Phase Continuity + Contact/Air)

**Core Mechanic:** Preserve locomotion phase continuity during air time by reweighting stride amplitude rather than freezing poses. Smoothly transition between contact-driven limb movement (grounded) and suppressed/neutral poses (airborne) to eliminate visual pops and maintain animation flow.

**Pattern Reuse:** 
- Extends proven spring-damper smoothing (landing spring, acceleration tilt)
- Uses dual-reference pattern (validated in walk/run transitions)
- Leverages existing gait phase system (surveyor wheel locomotion)
- Builds on quaternion slerp blending (keyframe pose system)
- Follows GDC "do no harm" principle (animation never steals control)

**Graybox Scope:**
- Minimum testable implementation:
  - Phase advancement: horizontal distance (XZ velocity magnitude * dt) while airborne
  - Single spring-damped `contact_weight` (1.0 grounded → 0.0 airborne)
  - Derived `air_weight = 1.0 - contact_weight`
  - Amplitude suppression: `slerp(T_POSE, gait_blend, contact_weight)`
  - Scale acceleration tilt by `contact_weight`
  
- Key parameters to expose:
  - `contact_weight_spring_frequency` (Hz, start ~3-5 Hz for visibility, tune to ~8-12 Hz)
  - Debug toggle: visualize weight values, pose interpolation state
  
- Estimated line count: ~60-90 lines
  - Weight state management: ~20 lines (single spring-damper, derived air_weight)
  - Phase advancement (airborne): ~15 lines (horizontal XZ velocity)
  - Pose blend modification: ~20 lines (amplitude suppression)
  - Acceleration tilt scaling: ~10 lines
  - Debug UI integration: ~15 lines

**Minimal Implementation:**

**Where it hooks:**
- File: `src/character/animation.cpp`
- Update order: After `locomotion::update()`, before `animation::update_skeleton()`
- Integration points:
  1. Weight state management (new spring-damper pairs in `animation` namespace or struct)
  2. Modify existing pose blending in `update()` to apply amplitude suppression
  3. Scale `acceleration_tilt` by `contact_weight` in tilt application
  4. Add debug UI in `src/gui/character_panel.cpp` (animation section)

**Implementation approach options:**
1. **Minimal (ship first):** Contact/air weights, amplitude suppression to T_POSE, no AIR_NEUTRAL keyframe
   - Pros: Smallest scope, validates core concept, zero content cost
   - Cons: T-pose may look stiff in air (can iterate with AIR_NEUTRAL if needed)
   
2. **Extended (defer):** Add AIR_NEUTRAL keyframe, blend toward it via air_weight
   - Deferred until minimal version tested

**Minimal scope confirmed:** Option 1. No coyote time (separate feature). No footfall bounce (future bounce gravity feature).

---

## CLARIFICATION RESOLUTIONS ✅

**1. Phase Advancement While Airborne** ✅ RESOLVED
- **Decision:** Use horizontal distance (XZ velocity magnitude * dt) to match grounded surveyor wheel pattern
- **Rationale:** Vertical velocity shouldn't drive limb cycles; preserves locomotion semantics

**2. Air Weight Relationship** ✅ RESOLVED
- **Decision:** Derived `air_weight = 1.0 - contact_weight` (single spring-damped variable)
- **Rationale:** Simpler implementation; add separate spring only if asymmetric feel needed

**3. Visual Target for "Suppressed" Pose** ✅ RESOLVED
- **Decision:** Use existing `T_POSE` keyframe (arms out 90°, legs straight) as neutral target
- **Rationale:** No new content; simple initial implementation; can author AIR_NEUTRAL later if needed

**4. Coyote Time Scope** ✅ RESOLVED
- **Decision:** **Defer** to separate feature (overlaps with "Coyote time + jump buffer" backlog item)
- **Impact:** Minimal scope = contact/air weight blending only; cleaner feature boundary

**5. Footfall Bounce Reference** ✅ RESOLVED
- **Decision:** Future bounce gravity feature (not yet implemented); no action needed
- **Note:** Ensure grounded check available for future integration

**6. Weight Transition Visibility** ✅ RESOLVED
- **Decision:** Start with visible transitions (~3-5 Hz), tune faster (~8-12 Hz) after validation
- **Rationale:** Exaggerated behavior easier to debug; standard graybox iteration approach

---

**Problem Evidence:**

**Documented observation:** Current system freezes pose when airborne, causing:
- Visual pops on takeoff (limbs lock mid-stride)
- Loss of continuity on landing (pose jumps back to phase position)
- Disconnect between body motion and limb motion during air time

**Origin:** Identified during bounce gravity brainstorm (2025-10-12) and air locomotion concept breakdown. Problem visible in current graybox iteration whenever character jumps mid-stride.

**Test scenario:** Run forward, jump at various gait phases (0.0, 0.25, 0.5, 0.75). Observe limb freezing and landing discontinuity.

**Unlocks:**

**Immediate:**
- Validates phase continuity principle (critical for future air control)
- Proves weight-based blending pattern (reusable for contact surfaces, water, etc.)
- Completes animation responsiveness (no input latency from frozen states)
- Enables bounce gravity feature (requires continuous phase)

**Future:**
- AIR_NEUTRAL keyframe integration (clear silhouette)
- Multi-surface locomotion (walk → swim → climb transitions)
- Contact-driven secondary motion (cloth, hair, appendages)
- Advanced air control (air strafing, bunny hopping with visual coherence)

**Test Loop:**

**How to verify success:**
1. **Phase continuity test:** Jump mid-stride → limbs should continue moving smoothly in air
2. **Landing coherence test:** Land at different phases → no pops, smooth transition back to contact
3. **Control test:** Input during air → acceleration/orientation unchanged (zero gameplay impact)
4. **Weight visualization:** Debug UI shows smooth 0→1 transitions for contact/air weights
5. **Edge case test:** Rapid jumps (coyote time), long falls, walk vs run jumps

**Expected iteration time:**
- First pass: 1-2 hours (weight state + amplitude suppression + phase advancement)
- Tuning: 30-60 min (spring frequency, visual validation)
- Debug UI: 20-30 min (weight readouts, toggle)
- Total: ~2-3.5 hours

**Self-test characteristics:**
- Immediate visual feedback (limb motion vs freeze)
- Real-time parameter adjustment (spring frequencies via ImGui)
- No external tools needed (runtime tuning sufficient)

**Certainty:** 75% — Medium-High (increased from 70% after clarifications)

**Justification:**
- **Dependencies:** All proven (phase system ✅, spring-damper ✅, slerp blending ✅, grounded detection ✅)
- **Pattern reuse:** Direct application of validated techniques (dual-reference smoothing, slerp blending)
- **Scope reduction:** Coyote time deferred, single spring vs dual, T-pose reuse (no new content)
- **Alignment:** Matches GDC principles (do no harm, interruptibility, synchronized blending)
- **Risk factors:**
  - Spring frequency tuning (2-3 iterations expected)
  - XZ phase advancement feel validation (novel but low-risk)
  - T-pose visual acceptability in air (can iterate to AIR_NEUTRAL if needed)

**Uncertainty sources:**
- Optimal spring frequency for contact_weight (~5% uncertainty)
- XZ-based phase advancement feel vs alternatives (~10% uncertainty)
- Edge cases: rapid jump sequences, long falls (~10% uncertainty)

**Success criteria:**
- Zero visual pops during jump/land cycles
- Phase continuity across all gait transitions
- Input responsiveness unchanged (no added latency)
- Weight transitions feel "natural" (unnoticeable when tuned well)

---

## Completion

**Date Completed:** 2025-10-13

**Final Certainty Score:** 100% ✅

**Code Review:** See [air_locomotion_weights_CODE_REVIEW.md](air_locomotion_weights_CODE_REVIEW.md) - APPROVED

**Outcome:** All success criteria met. Phase continuity works seamlessly—limbs continue moving smoothly during air time without visual pops or landing discontinuities. Contact/air weight blending using dual-reference spring-damper pattern (8 Hz default) provides smooth transitions. T_POSE blend target works well for amplitude suppression (no AIR_NEUTRAL keyframe needed). Pattern proven reusable for future multi-surface locomotion.

---

## Reflection

**What went well:**
- **Pre-implementation clarifications prevented scope creep:** Explicitly resolving 6 open questions (phase advancement, weight relationship, T_POSE target, coyote time deferral) before planning locked scope and avoided mid-implementation uncertainty.
- **Dual-reference pattern reuse accelerated implementation:** Third use of spring-damper dual-reference (landing spring, acceleration tilt, now contact weights) meant no architecture discovery needed—copy validated pattern.
- **Minimal scope validation (T_POSE only) reduced content cost to zero:** Deferring AIR_NEUTRAL keyframe proved correct—T_POSE suppression looks acceptable in graybox, validates core concept without art investment.

**What caused friction:**
- **Grounded parameter cleanup missed in initial plan:** `is_grounded` parameter became unused after removing locomotion gate but wasn't caught until code review. Added minor post-implementation cleanup step.
- **Secondary motion joint limits discovered during visual validation:** Elbows hyperextending during rapid transitions (e.g., jump with T-pose) identified as polish gap. Not a blocker but required backlog item creation.

**What would you do differently:**
- **Include parameter cleanup in planning checklist:** When removing conditional gates, explicitly list parameter removal as a planning step to avoid code review suggestions.
- **Add "known polish gaps" section to feature description:** Document expected rough edges (like joint limits) upfront so they're tagged as "defer" rather than "discover during validation."

---

## Certainty Calibration

**Predicted Certainty:** 75% (Medium-High, from feature plan Section 3)

**Actual Outcome:**
- [x] Completed on first attempt (no deferrals, no major rework)

**Variance Analysis:**
- Predicted 75%, actual was 100% (smooth completion). What made it easier than expected?
  - All dependencies were truly 100% certain (phase system, spring-damper, slerp, grounded detection)
  - Pattern reuse (3rd use of dual-reference spring) eliminated architecture risk
  - Minimal scope (T_POSE only, deferred coyote time) cut uncertainty sources
  - Pre-implementation clarifications resolved all ambiguity before coding
- Certainty score was conservative but reasonable. The 25% uncertainty hedge covered:
  - Spring frequency tuning (resolved to 8 Hz default in ~2 test iterations)
  - XZ phase advancement feel (worked first try—horizontal velocity semantics matched expectations)
  - T_POSE visual acceptability (better than expected—no AIR_NEUTRAL needed for graybox)

**Calibration Adjustment:**
- **For similar features:** Pattern reuse on 3rd use justifies +10% certainty boost (75% → 85% would've been accurate here)
- **Clarification phase value:** Pre-implementation Q&A that resolves 6+ open questions reduces scope uncertainty significantly; consider +5-10% for thorough upfront resolution
- **Conservative estimation okay:** Underpromising (75%) then delivering clean (100%) is better than overpromising and discovering mid-implementation blockers

---

**Implementation Plan:** See [air_locomotion_weights_PLAN.md](air_locomotion_weights_PLAN.md) for detailed checklist.
