# Coyote Time + Jump Buffer

**Core Mechanic:** Elastic timing forgiveness that allows jumps to execute for a short window after leaving ground (coyote time) and stores recent jump inputs to execute on next valid grounded frame (jump buffer). Expands the "when can I jump?" window without reducing skill expression or affecting physics determinism.

**Pattern Reuse:** Extends proven timing patterns from existing frame-delta calculations. Simple time accumulator pattern (similar to delta_time tracking). Integrates with existing grounded detection from character controller and jump impulse system. Zero new state machines—pure timing windows layered on existing input handling.

**Graybox Scope:**
- Two float timers: `coyote_timer` (tracks time since leaving ground) and `jump_buffer_timer` (tracks time since jump input pressed)
- Two constants: `COYOTE_WINDOW_MS` (~120-150ms) and `JUMP_BUFFER_WINDOW_MS` (~150ms)
- Modified jump eligibility: `can_jump = is_grounded_physics || (coyote_timer < COYOTE_WINDOW_MS)`
- Buffered input execution: if `jump_buffer_timer < JUMP_BUFFER_WINDOW_MS` when grounded, trigger jump immediately
- Expose tuning parameters: both window durations, toggle on/off for A/B testing
- Debug visualization: HUD indicators showing active timers and eligibility state
- Estimated line count: ~40-60 lines total (timer updates, eligibility checks, debug UI)

**Minimal Implementation:** Modify `src/character/controller.cpp` in two places:
1. **Update timers** (in `update()` before input handling):
   - Increment `coyote_timer` when airborne, reset to 0 when grounded
   - Decrement `jump_buffer_timer` each frame, set to `JUMP_BUFFER_WINDOW_MS` on jump input
2. **Jump eligibility check** (replace `if (is_grounded)` with expanded logic):
   - Check `is_grounded_physics || coyote_timer < COYOTE_WINDOW_MS`
   - On successful jump, clear both timers to prevent double-jumps
   - On grounded frame with active jump buffer, execute buffered jump

**Problem Evidence:** Documented in backlog 2025-10-12: "Current 'physics-only grounded' can eat jump inputs during bounce or ledge micro-gaps." This became observable after bounce gravity mechanics were explored. Players naturally attempt jumps at apex of bounce cycles or near ledge edges, expecting input to register. Physics-strict grounded detection creates ~16ms vulnerability windows (single frame at 60fps) that feel like "dropped inputs" despite working as designed.

**Unlocks:**
- **Immediate:** Completes the "do no harm" input philosophy from GDC principles—intent synchronization without latency. Validates timing forgiveness as a systemic pattern for future inputs (dash buffer, attack queuing).
- **Future:** Enables confident jump-heavy level design (platforming sequences, bounce chains). Unblocks player testing sessions focused on movement feel rather than input precision. Creates foundation for coyote/buffer variations on other abilities.

**Test Loop:**
- **Primary test:** Run at full speed toward ledge, jump slightly late—should execute. Jump during bounce apex—should feel responsive.
- **Edge cases:** Spam jump while airborne (should buffer, execute on landing). Jump exactly at ledge edge (coyote should catch). Double-jump prevention (timers clear on successful jump).
- **A/B comparison:** Toggle feature off, compare feel. Measure success rate on test jump sequence (target: 90%+ success with feature vs ~60% without).
- **Expected iteration:** Initial tuning 10-15 minutes (adjust window sizes), edge case fixes 30-60 minutes, debug visualization 20 minutes. Total self-test loop: ~2 hours to confident tuning.

**Certainty:** ~85%—widely used pattern with well-understood implementation (Celeste, Hollow Knight, etc.). Primary risk is tuning window durations to feel natural without being exploitable. No architectural novelty, purely parameter-driven. References from DG_Skill validate approach for broad accessibility without skill ceiling reduction.

**Dependencies:** 
- Character controller grounded detection (95% certain ✅)
- Jump impulse system (95% certain ✅)
- Frame delta timing (100% certain ✅)

**Anti-dependencies (what this doesn't affect):**
- Physics determinism unchanged (timers are input-layer only)
- Animation systems unaffected (jump trigger timing shifts, but animation response identical)
- No new collision detection or state machines

**Success Criteria:**
- Jump feels reliable at ledge edges and during bounce cycles
- No accidental double-jumps or mid-air jump spam exploits
- Expert players retain timing mastery (windows tight enough to preserve skill expression)
- Debug UI clearly shows when coyote/buffer are active for parameter tuning
- Feature can be toggled off without code changes (parameter = 0 disables)

**References:**
- [NOTES/DesigningGames/DG_Skill.md](../NOTES/DesigningGames/DG_Skill.md) - Elastic challenges principle
- [NOTES/GDC/GDC_DesignPhilosophy.md](../NOTES/GDC/GDC_DesignPhilosophy.md) - "Do no harm" input philosophy
- [NOTES/bounce_gravity_explained.md](../NOTES/bounce_gravity_explained.md) - Context for micro-gap problem

**Open Questions (defer to implementation):**
- Should coyote window scale with speed? (Hypothesis: global constant simpler, test first)
- Should buffer trigger on *any* grounded frame or only "stable" grounded? (Hypothesis: any frame, prevents edge case frustration)
- Should debug visualization be world-space (above character) or HUD? (Hypothesis: HUD cleaner for tuning)

---

## Clarification Status

**CLARIFICATION SKIPPED** - Feature description is concrete and unambiguous.

**Rationale:**
- **Mechanical clarity:** Timer logic specified with pseudocode and explicit variable names (`coyote_timer`, `jump_buffer_timer`, `COYOTE_WINDOW_MS`)
- **Integration points identified:** Specific file (`src/character/controller.cpp`) and modification locations documented (timer updates before input handling, eligibility check modification)
- **Objective success criteria:** Testable outcomes (no double-jumps, ledge jump reliability, toggle functionality)
- **Concrete scope:** Line count estimate (40-60 lines), specific data structures (two floats, two constants)
- **No visual ambiguity:** Pure mechanical timing behavior—no animation poses, visual styles, or subjective "feel" requiring references
- **Open questions are tuning decisions:** Deferred items are implementation choices (speed scaling, grounded frame strictness), not architectural ambiguities

**Assumptions made:**
- `is_grounded` boolean already exists in controller (verified in controller.h line 37)
- Jump input currently uses `input::is_key_pressed(SAPP_KEYCODE_SPACE)` (verified in controller.cpp line 75)
- Frame delta time `dt` available in `update()` function (standard pattern)
- Timer units in milliseconds but implementation will use seconds internally (convert constants)
- Debug UI follows existing ImGui panel pattern from animation tuning system

**Ready for:** Direct transition to PLAN phase—no user clarification required.

---

## Completion

**Date Completed:** 2025-10-14

**Final Certainty Score:** 100% ✅

**Code Review:** Approved by Gemini (see [coyote_jump_buffer_PLAN.md](coyote_jump_buffer_PLAN.md) line 291)

**Outcome Summary:** Feature delivered exactly as specified with all validation tests passing. Timing forgiveness successfully extends jump eligibility window without introducing exploits or affecting physics determinism.

---

## Reflection

**What went well?**
- Concrete mechanical specification with pseudocode prevented planning ambiguity
- Explicit line count estimates (40-60 lines) matched actual implementation scope
- Pattern reuse from proven frame delta timing eliminated architectural risk
- Debug UI integration from start enabled immediate tuning feedback

**What caused friction?**
- None—linear implementation matched prediction

**What would you do differently?**
- Nothing—planning precision aligned perfectly with execution

---

## Certainty Calibration

**Predicted Certainty:** 85% (from Section 1 above)

**Actual Outcome:**
- [x] Completed on first attempt (no deferrals, no major rework)

**Variance Analysis:**
Predicted 85% certainty accurately reflected low risk. Completed smoothly because widely-used pattern with stable dependencies (grounded detection 95%+, frame timing 100%). The 15% uncertainty buffer appropriately covered tuning unknowns (window durations).

**Calibration Adjustment:**
For similar input-layer timing features: 85% remains appropriate. Pattern reuse of timing accumulators justifies +10% boost. Dependencies on stable physics state are low-risk.
