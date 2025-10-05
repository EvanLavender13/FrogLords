# Review of Collision System Implementation Plan - Phase B

**Date:** October 5, 2025
**Reviewer:** Gemini
**Subject:** Phase B: Step-Up Behavior

This review assesses the Phase B plan against the principles outlined in `AGENTS.md` and the GDC talk notes.

---

## Overall Assessment

The Phase B plan is well-aligned with the project's core principles of simplicity, iteration, and creating emergent behavior from simple components. The proposal to use the weightlifter sphere for step-up is a good example of the "multi-use tools" principle.

However, there are several concerns with the proposed implementation, one of which is critical and requires re-evaluation of the technical approach.

---

## Alignment with Principles

### Positive Alignment

*   **Simplicity over Sophistication:** The proposed logic is a simple, rule-based system that avoids introducing a complex physics or animation subsystem for stepping. This is consistent with the project's philosophy.
*   **Multi-use Tools:** Assigning the step-up function to the weightlifter sphere, which already handles slope grounding, is an excellent application of this principle. It gives the component a clear, expanded role.
*   **Iteration over Planning:** The phased approach is commendable. This plan builds logically on the completed Phase A.

### Points of Concern

*   **Clarity over Cleverness / Procedural Foundation:** The proposed implementation for the vertical lift (`position.y += step_height + 0.01f;`) is an instantaneous position change. This may create a visually jarring "snap" or "teleport" effect, which could violate the principle of smooth, procedural motion. While acceptable for a first pass, we should be prepared to iterate on this to use a time-based transition (e.g., a spring-damper or easing function) if playtesting reveals a poor feel.

---

## Technical Concerns

### 1. **CRITICAL: Incorrect `step_height` Calculation**

The plan proposes `float step_height = col.penetration;`. This is fundamentally incorrect.

*   **The Problem:** For a horizontal collision with the side of a box, the `penetration` value from `resolve_sphere_aabb` will represent the *horizontal* overlap between the sphere and the box. Using this horizontal distance as a vertical lift amount is incorrect and will not produce the desired step-up behavior. The amount of lift will be related to how far the character has pushed into the wall, not the height of the obstacle.
*   **Required Change:** The actual step height needs to be calculated. This would be the vertical distance from the character's current position to the top surface of the AABB they are colliding with. The `resolve_sphere_aabb` function has access to the `aabb` struct, so the height of the box's top surface (`box.center.y + box.half_extents.y`) is available. The lift should be based on this height.

The proposed code for B.1 needs to be completely re-thought to correctly calculate the required vertical lift.

### 2. **Vague Step-Up Condition**

The condition for identifying a step-up opportunity is `col.normal.y < 0.3f && col.normal.y > -0.3f`. This allows for normals that are not purely horizontal. While this may add some robustness, it could also trigger step-ups on very steep slopes that should be treated as walls. This threshold will need careful tuning.

### 3. **Empty `else` Block**

The `if (step_height < weightlifter.radius ...)` block has an `else` that is empty, with the comment "Step too high - treat as wall (bumper will handle)". While it is true that the bumper loop runs later, this logic feels implicit. It would be clearer to ensure that no state is modified here that would prevent the bumper from correctly identifying the wall. The current proposal seems safe, but this should be confirmed during implementation.

---

## Recommendations

1.  **Reject the current Phase B implementation plan.** The `step_height` calculation is incorrect and will not work as described.
2.  **Redesign the `B.1` implementation.** The logic must be updated to calculate the required vertical lift based on the AABB's top surface height, not the collision penetration.
3.  **Proceed with a corrected plan.** Once the `step_height` calculation is fixed, the rest of the plan (tuning, testing) is solid.
4.  **Add a playtesting goal to evaluate the "feel" of the instantaneous lift.** Be prepared to create a follow-up task to implement a smoother, time-based lift if the snapping is too noticeable.

---

## Developer Response

**Date:** October 5, 2025

### Analysis of Critical Issue

**AGREE: The `step_height` calculation is fundamentally wrong.**

Verified by examining `resolve_sphere_aabb()` implementation:
- `penetration` measures overlap distance along collision normal
- For horizontal collision (side of box), penetration is horizontal distance
- Using horizontal distance as vertical lift makes no geometric sense

**Correct approach:**
```cpp
// Given: weightlifter sphere colliding with box from side
// Need: vertical distance from sphere center to box top surface
float box_top = box.center.y + box.half_extents.y;
float sphere_bottom = weightlifter.center.y - weightlifter.radius;
float step_height = box_top - sphere_bottom;
```

This calculates the actual vertical lift needed to place the sphere's bottom on the box's top surface.

### Analysis of Secondary Issues

**Point 2 (Vague Step-Up Condition):**
- **AGREE** that `normal.y < 0.3f && normal.y > -0.3f` needs justification
- Allows ~73° to ~107° from vertical (not purely horizontal)
- Could trigger on 60°-70° slopes (should these step up or slide?)
- **Action:** Need playtest-driven tuning of this threshold

**Point 3 (Empty else Block):**
- **PARTIALLY AGREE** - implicit reliance on bumper loop is correct but unclear
- Current three-phase approach (Phase A) ensures bumper handles walls
- **Action:** Add comment clarifying intentional fall-through to bumper phase

### Analysis of Procedural Motion Concern

**Quote from review:** "instantaneous position change may create visually jarring snap"

**Counter-argument from AGENTS.md principles:**
- "Graybox until mechanics proven" - Test simple version first
- "Iteration over planning" - Don't solve feel problems before they exist
- "More iterations > perfect first attempts"

**Justification for instant lift:**
1. Overgrowth (source inspiration) likely uses instant lift - no spring-damper mentioned in GDC talk
2. If step-up happens once per obstacle, snapping may be imperceptible
3. Adding spring-damper before playtesting violates "avoid therapeutic planning"
4. Spring-damper introduces frame-rate dependent state (position transitioning over time)

**Decision:** Implement instant lift. Add TODO comment for spring-damper if playtesting reveals jarring feel.

### Alignment Check: Recommendations vs. Principles

**Recommendation 1 (Reject plan):** ✅ **CORRECT** - Bad math is bad design  
**Recommendation 2 (Redesign calculation):** ✅ **CORRECT** - Fix is straightforward  
**Recommendation 3 (Proceed with corrected plan):** ✅ **CORRECT** - Rest of design is sound  
**Recommendation 4 (Playtest instantaneous lift):** ✅ **CORRECT** - Aligns with iteration principle

---

## Final Decision

**ACCEPT ALL RECOMMENDATIONS**

### Implementation Actions

1. **Update Phase B code in `collision_system_implementation.md`:**
   - Fix `step_height` calculation to use box top surface vs. sphere bottom
   - Add comment justifying instant lift (with spring-damper TODO)
   - Add comment for empty else block (intentional bumper fall-through)
   - Document normal.y threshold tuning as playtest goal

2. **Preserve iteration philosophy:**
   - Instant lift is the graybox implementation
   - Spring-damper is polish (Phase C or later)
   - Let playtesting drive feel improvements

3. **Testing addition:**
   - Add explicit test: "Does instant lift feel responsive or jarring?"
   - If jarring: Create Phase C+ task for spring-damper transition

### Why This Approach Honors AGENTS.md

- **Simplicity over sophistication:** Fixed calculation is still simple
- **Clarity over cleverness:** Correct math > wrong math with clever workarounds
- **Iteration over planning:** Implement simplest correct version, improve based on feel
- **Graybox before polish:** Instant lift is functional, spring-damper is polish

**Status:** Phase B plan requires revision before implementation. Core design valid, calculation method incorrect.
