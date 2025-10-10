# Iteration Plan: Attach Skeleton to Character Body

### 1. Feature Overview

*   **Description:** Attach the T-pose skeleton's root to the character controller's rigid body. This will make the skeleton move with the character.
*   **Core Principle Alignment:** This aligns with "Iteration over planning" by taking a small, incremental step. It builds on the existing "Skeleton Debug System" and enables future animation work, following the principle of building bottom-up from the dependency stack.

### 2. Design Rationale

*   **Problem/Goal:** The skeleton is currently static and does not move with the character. The goal is to connect the skeleton to the character's physics body so it moves and jumps with the character.
*   **Hypothesis:** This change will visually connect the skeleton to the player's input and movement, making the character feel more embodied. It's a foundational step towards more complex animations.

### 3. Dependencies & Uncertainty

*   **Placement in Stack:** This feature sits directly on top of the "Skeleton Debug System (100%)" and below "Secondary Motion / Bone Wobble". It is a direct dependency for any future skeletal animation.
*   **Certainty Score:** 95%. This is a very low-risk feature. It's a simple transform update, taking the character's position from the physics system and applying it to the skeleton's root. The high certainty comes from the fact that both the character controller and the skeleton system are stable and well-understood.

### 4. Implementation & Testing Loop

*   **Graybox Scope (The "Stupid Simple Core"):**
    *   In the character update loop, after the physics simulation, get the character's world position.
    *   Set the skeleton's root bone position to the character's world position.
    *   Apply the same orientation (yaw) and animation transforms (landing spring offset, acceleration tilt) that the debug body box uses.
    *   The skeleton should now follow the character's movement, including jumping and falling, AND rotate and tilt with the character's orientation and reactive animation.
    *   No new tunable parameters are needed for this feature.
*   **Testing Protocol:**
    *   Self-test by running the game and moving the character around.
    *   Verify that the skeleton moves with the character sphere.
    *   Verify that the skeleton is correctly positioned relative to the character sphere.
    *   Verify that jumping and falling also correctly affect the skeleton's position.
*   **Success Metrics (Definition of "Proven"):**
    *   The skeleton's root bone is correctly and consistently attached to the character's physics body.
    *   The skeleton moves smoothly with the character without any visible jitter or lag.
    *   The skeleton rotates with the character's facing direction (yaw).
    *   The skeleton tilts with acceleration and crouches with landing spring, matching the debug body box behavior.
*   **Iteration Cadence & Horizon:**
    *   This is a very short iteration, likely a few hours.

### 5. System Integration

*   **Mechanic Interactions:** This feature directly integrates the skeletal animation system with the character physics and movement systems. It will allow future animation work to be layered on top of the character's movement.
*   **Interface & Feedback:** The visual feedback is the skeleton moving with the character. No new UI is required.

### 6. Risks & Future Work

*   **Potential Risks:** Very few risks. The main risk is a potential offset issue between the character's position and the skeleton's root, which should be easy to debug and fix. Transform order (translate → rotate → offset → tilt) must match debug body to ensure visual consistency.
*   **Content Restrictions:** None.
*   **Decision Impacts (Checklist):**
    *   Implementation cost: Very low.
    *   Immaturity burden: None.
    *   Critical failure risk: Very low.
    *   Process burden: None.
    *   Political/cultural effects: None.
    *   Decision cost: Very low.
*   **Backlog Candidates:** This feature unblocks "Secondary motion" and other skeletal animation features in the backlog.

---

### Changelog

**2025-10-06:** Scope addition approved.
- Added orientation (yaw rotation) and reactive animation transforms (landing spring vertical offset, acceleration tilt) to skeleton root.
- Rationale: Skeleton should mirror the debug body box's transform hierarchy to maintain visual consistency and properly demonstrate the reactive animation layer working with the skeletal system.
- Impact: Minimal - uses existing transform code, no new dependencies, certainty remains ~95%.

---

## Completion

- Date completed: 2025-10-06
- Final certainty score: 100%
- Link to code review document: PLANS/ARCHIVE/20251006_130000_code_review_attach-skeleton-to-body.md
- Brief outcome summary: Successfully attached skeleton to character body, enabling movement and reactive animations. Unblocks secondary motion.

---

### Approval

- **Status:** Approved
- **Reviewer:** Gemini
- **Date:** 2025-10-06
