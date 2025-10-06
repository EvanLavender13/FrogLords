# Iteration Plan: Speed-Based Animation Scaling

### 1. Feature Overview

*   **Description:** This feature directly enhances the core gameplay feel by making the existing reactive animations (like acceleration tilt) more dynamic and responsive to player speed.
*   **Core Principle Alignment:** This aligns perfectly with the project's principles of rapid iteration and focusing on the core loop. It is a small, low-risk extension of a proven system.

### 2. Design Rationale

*   **Problem/Goal:** The current reactive animations are static and don't reflect the character's speed, making them feel less dynamic. The goal is to make the animations scale with speed to provide better feedback and a more responsive feel.
*   **Hypothesis:** By scaling animation intensity with the character's velocity, the player will feel more connected to the character's movement, enhancing the sense of speed and control.

### 3. Dependencies & Uncertainty

*   **Placement in Stack:** This feature builds directly on the existing "Reactive Animation System" (specifically acceleration tilt). It has no new dependencies.
*   **Certainty Score:** 80%. The high certainty comes from it being a small extension of a well-understood and functioning system. The primary uncertainty lies in the tuning required to make it "feel right."

### 4. Implementation & Testing Loop

*   **Graybox Scope (The "Stupid Simple Core"):**
    *   Modify the existing acceleration tilt logic to incorporate a scaling factor based on the character's ground speed.
    *   Expose a single tuning parameter: `speed_scaling_factor` to control the intensity of the effect.
    *   The core implementation will be a simple multiplication: `tilt_angle *= (1.0 + current_speed * speed_scaling_factor)`.
*   **Testing Protocol:**
    *   Self-test by running the game and adjusting the `speed_scaling_factor` in real-time.
    *   Focus on how the tilt feels at different speeds. Does it exaggerate the motion appropriately? Does it feel distracting or natural?
*   **Success Metrics (Definition of "Proven"):**
    *   The feature is "proven" when the animation scaling feels natural and enhances the sense of speed without being distracting. This will be determined through self-testing and a quick over-the-shoulder check with one other person.
*   **Iteration Cadence & Horizon:**
    *   The implementation and initial tuning should take no more than a few hours.

### 5. System Integration

*   **Mechanic Interactions:** This feature will primarily interact with the character's locomotion system. As the character speeds up and slows down, the animation scaling will automatically adjust.
*   **Interface & Feedback:** The feedback is purely visual, through the character's animation. No new UI is required.

### 6. Risks & Future Work

*   **Potential Risks:** The main risk is that the effect could be too subtle to notice or too exaggerated, making the character feel unstable. This will be mitigated through real-time tuning.
*   **Content Restrictions:** This feature imposes no new content restrictions.
*   **Decision Impacts (Checklist):**
    *   Implementation cost: Very low.
    *   Immaturity burden: Low.
    *   Critical failure risk: Very low.
    *   Process burden: None.
    *   Political/cultural effects: None.
    *   Decision cost: Very low.
*   **Backlog Candidates:** If successful, this concept could be applied to other reactive animations (e.g., jump anticipation, landing recovery). This can be added to the design backlog.

## Deferral

**Date:** 2025-10-06

**Reason:** Current constant magnitude feels good; no evidence of problem requiring speed scaling.

**Evidence Gap:** No documented gameplay moments where constant tilt magnitude fails (e.g., "tilt too subtle at max speed").

**Reconsideration Criteria:** After playtesting identifies specific moments where tilt magnitude is inadequate at high speeds or distracting at low speeds.

**Review:** See `PLANS/ARCHIVE/20251006_120000_plan_review_speed_animation_scaling.md` for full analysis
