# Iteration Plan: Minimum Viable Skeleton System

### 1. Feature Overview

*   **Description:** A foundational system to define, manipulate, and render a hierarchical skeleton using debug primitives. The initial goal is to render a static, humanoid T-pose model on screen.
*   **Core Principle Alignment:** Bottom-of-stack, graybox-first, and iteration-over-planning. Establishes the minimal surface to unblock procedural animation (gameplay-first: visualization only; no control is stolen).

### 2. Design Rationale

*   **Problem/Goal:** Create the fundamental data structures and render path needed for all procedural character animation. This underpins secondary motion, IK, and active ragdolls.
*   **Hypothesis:** A visible, debug-rendered skeleton enables rapid prototyping, tuning, and validation of procedural mechanics without importing art.

### 3. Dependencies & Uncertainty

*   **Placement in Stack:** New foundational system above Rendering, below future Secondary Motion.
  * **Dependencies:** `foundation` (glm/math), `rendering` (debug draw).
  * **Dependents:** Secondary motion, IK, active ragdoll, locomotion visualizers.
*   **Certainty Score:** **95%** (well-understood patterns; low risk). Note: minor risk in matrix conventions.

### 4. Implementation & Testing Loop

*   **Graybox Scope (The "Stupid Simple Core"):**
    *   Define `skeleton`/`joint` in `src/character/skeleton.h` with clear transform conventions.
    *   Add `create_t_pose(skeleton&)` in `src/character/t_pose.h/.cpp` to populate a hardcoded humanoid.
    *   Implement `debug::draw_skeleton(draw_context&, const skeleton&)` in `src/rendering/debug_draw.*` (joints + bones via simple primitives).
    *   Integrate into `app_runtime` to create at startup and draw each frame.
    *   Add `update_global_transforms(skeleton&)` to compute hierarchical `model_transform` per joint.
    *   **Key params exposed (next loop):** show/hide toggle, show labels toggle.
*   **Conventions (Document + Assert):**
    *   Units: meters; Up-axis: Y (or project default).
    *   glm column-major; model = parent.model * local.
    *   Root `parent_index = -1` (NO_PARENT) at index 0.
*   **Testing Protocol:**
    *   Self-test: build, run, visually confirm T-pose.
    *   Over-the-shoulder: 3–6 quick observers; stay silent; prompt: “Tell me the story of what just happened.”
    *   Metrics: log joint count, parent validity (% non-root with valid parent), frame time delta (< 0.1 ms target), screenshot for regression.
    *   Avoid coaching; note any confusion about orientation/scale.
*   **Success Metrics (Definition of "Proven")**
    *   Renders a T-pose with correct joint/parent counts; root at index 0 with NO_PARENT.
    *   `update_global_transforms` verified by animating one child joint locally (manual code) and observing expected rotation in world.
    *   No content restrictions introduced; frame time delta negligible (< 0.1 ms on dev box).
*   **Iteration Cadence & Horizon**
    *   Horizon: 0.5–1 day for this loop. Next loop: add one animated joint tick + GUI toggles.

### 5. System Integration

*   **Mechanic Interactions:** None in first loop; later, secondary motion/IK drive joint locals to produce visible results.
*   **Interface & Feedback:**
    *   Add debug toggles: “Show Skeleton”, “Show Joint Labels”.
    *   Visual hierarchy: distinct color for root/leaves; thin lines for bones; small spheres or points for joints.

### 6. Risks & Future Work

*   **Potential Risks:** Matrix order/axis mismatch; missing primitives in debug draw API (fallback to lines/points).
*   **Content Restrictions:** None—enables work without art.
*   **Decision Impacts (Checklist):**
    *   Implementation cost: low; Immaturity burden: low; Critical failure risk: low; Process burden: low; Cultural: positive (clarity tools); Decision cost: low.
*   **Backlog Candidates:** Enable “Secondary Motion (per-bone softness)”, “IK anchors”, “Active ragdoll pose matching”.
