# Principle Review: Air Locomotion Weights

**Date:** 2025-10-13
**Status:** APPROVED

### Summary

Plans align with AGENTS.md: graybox-first, minimal scope, and reactive layer that never controls core physics. The feature reuses proven patterns (dual-reference springs, distance-driven gait, slerp blending) and inserts cleanly between locomotion and animation. Primary risks are visual (T-pose suppression aesthetics) and tuning (spring frequency), not architectural. Recommendation: approve and implement with the listed validations.

### Violations (Must Fix)

**If none:** No violations found.

### Risks & Concerns

- **T-POSE visual stiffness:** Amplitude suppression toward T-POSE may look rigid mid-air.
  - **Mitigation:** Keep Option 2 ready (AIR_NEUTRAL keyframe). Gate behind debug toggle and add only if tests show benefit.

- **Phase advancement in air (edge cases):** Horizontal speed near zero (vertical jumps) will minimize phase advance; could appear frozen even with suppression.
  - **Mitigation:** This is acceptable for minimal scope; suppression hides pops. If needed later, bias a tiny epsilon phase advance when horizontal_speed ~ 0.

- **Spring tuning sensitivity:** Contact-weight frequency might oscillate or feel sluggish at extremes.
  - **Mitigation:** Start 3–5 Hz for visibility, tune toward 8–12 Hz. Use critical damping via `critical_damping(stiffness)`.

- **API consistency (frequency → stiffness):** Ensure frequency-to-stiffness uses `omega = TWO_PI * f` and `stiffness = omega^2` consistently with other springs.
  - **Mitigation:** Mirror landing spring setup; centralize constants in `foundation/math_utils.h` if needed.

- **Ownership clarity (locomotion gating removal):** Removing `if (is_grounded)` in `locomotion.update()` changes debug wheel spin and distance accumulation during air time.
  - **Mitigation:** This is intended (odometer semantics). Confirm no gameplay systems depend on grounded-only distance.

### Actionable Items

- [ ] Implement `contact_weight` dual-reference: `contact_weight_target` + `spring_damper` in `animation_state`.
- [ ] Add `update_contact_weight(bool is_grounded, float dt)` and call it between locomotion and skeletal animation in `game_world::update()`.
- [ ] Modify `locomotion_system::update()` to always advance phase/distance using XZ speed (remove grounded gate).
- [ ] Apply amplitude suppression: `final = slerp(T_POSE, gait_blended, contact_weight)` in skeletal blend pass.
- [ ] Scale acceleration tilt by `contact_weight` to zero out in air.
- [ ] GUI: slider for `contact_weight_frequency (Hz)` + readouts for target, smoothed, air_weight, spring velocity.
- [ ] Validate that T-POSE keyframe is identity quaternions for all animated joints.
- [ ] Visual tests: jump at multiple phases; confirm no pops and smooth land reentry.

### Recommendation

**Reasoning:**
- Cites AGENTS.md: “Clarity over cleverness; simplicity over sophistication.” The plan is the minimal, graybox-first slice.
- Layered Architecture respected: core logic remains inviolable; reactive weights only interpret state.
- “Do no harm”: animation never steals control; input continues to map to acceleration.
- Dual-Reference pattern applied to avoid circular dependencies.
- “Drive locomotion by distance traveled” upheld by continuing phase in air (XZ only).

**Next Steps:**
- If APPROVED: Proceed to IMPLEMENTATION_STEP (begin with Data Structures, then Weight Management, then Phase & Blending).
- If REVISE: N/A
- If DEFER: N/A

