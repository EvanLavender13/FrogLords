# Clarify Feature Requirements

Use this task **after receiving a feature description** and **before creating an iteration plan**. The goal is to eliminate ambiguity by asking targeted questions that expose gaps, assumptions, or missing visual references. This prevents building the wrong thing and wasting iteration loops.

**Use this task when:**
- A feature description feels vague or open to interpretation.
- You suspect the user has a specific visual or behavioral reference in mind that hasn't been shared.
- The feature involves animation, poses, or visual behavior that could be implemented multiple ways.
- The description uses abstract terms ("natural," "smooth," "dynamic") without concrete examples.
- You're uncertain about the exact end state or success criteria.

---

### 1. Review Development Principles

Read `AGENTS.md` to synthesize core project principles. This clarification process aligns with:
- **Iteration over planning:** Short loops require clear targets; ambiguity kills speed.
- **Paradox of quality:** Clarity upfront maximizes iteration count; confusion compounds with each loop.
- **Knowledge creation > implementation:** Asking the right questions is a form of knowledge creation.
- **Testing protocol:** You can't test if you don't know what "correct" looks like.

---

### 2. Identify Ambiguities

Analyze the feature description and flag potential sources of confusion:

**Visual/Behavioral Ambiguities:**
- Are there multiple ways to interpret the described motion or appearance?
- Does the user have a reference image, animation, or video in mind?
- Are terms like "natural," "smooth," "believable," or "dynamic" used without concrete examples?
- Could the feature be implemented with different visual styles that all technically match the description?

**Mechanical Ambiguities:**
- What is the exact input→output mapping? (e.g., "swing arm" = rotate which joint, around which axis?)
- Are there multiple joints/systems that could satisfy the description?
- What is the relationship between user input and the animation/behavior?
- Is the feature time-based, distance-based, or event-based?

**Scope Ambiguities:**
- What is the minimum viable implementation vs. full vision?
- Which parts are graybox-acceptable roughness vs. must-be-correct-now?
- What dependencies exist that aren't explicitly stated?
- What does "done" look like for this iteration?

**Success Criteria Ambiguities:**
- How will you know if the implementation is correct?
- What visual/behavioral test can verify correctness?
- Are there specific keyframes, poses, or moments that must match a reference?
- What would make the user say "yes, that's it" vs. "no, not quite"?

---

### 3. Ask Targeted Questions

Formulate questions that **reduce uncertainty** and **expose hidden assumptions**. Prefer questions that:
- Request concrete examples over abstract descriptions.
- Ask for visual references (images, videos, animations) when describing motion or appearance.
- Clarify the relationship between systems (e.g., "should arms move with legs or independently?").
- Identify the most critical visual/behavioral element to get right first.
- Reveal the user's mental model of how the feature should work.

**Question Categories:**

**Visual References:**
- "Do you have a reference image or video showing the desired result?"
- "Can you describe the specific pose/keyframe that defines this feature?" (e.g., "both arms down" vs. "one arm forward, one back")
- "What existing game/animation does this resemble?"

**Mechanical Clarity:**
- "Should this be driven by time, distance traveled, or user input?"
- "Which joints/bones need to move to achieve this?" (list specific joint names)
- "Should this animation loop, blend, or switch between discrete states?"
- "What axes of rotation are involved?" (pitch/yaw/roll, X/Y/Z)

**Scope Boundaries:**
- "For the graybox, what's the minimum that proves the concept works?"
- "Which parts can look rough vs. which parts must be accurate now?"
- "Are there edge cases we can ignore in this iteration?"

**Success Criteria:**
- "How will we know if this is correct? What visual test confirms it?"
- "What would make you say 'yes, that's exactly right'?"
- "If I show you two implementations, what detail would help you pick the correct one?"

---

### 4. Synthesize Understanding

After receiving answers, summarize your understanding of:
- **Exact visual/behavioral target:** What does the final result look like/feel like?
- **Reference materials:** What images/videos define correctness?
- **Mechanical approach:** What systems/joints are involved and how do they interact?
- **Minimum viable implementation:** What's the simplest graybox that proves the concept?
- **Test criteria:** How will you verify correctness without user feedback?

State this synthesis back to the user for confirmation: **"My understanding is..."**

---

### 5. Document Clarifications

Update the feature description document (`PLANS/feature_<name>.md`) with:
- Concrete visual references (attach images, link videos).
- Specific joint/system names and rotation axes.
- Exact keyframe descriptions or pose breakdowns.
- Clear success criteria (e.g., "arms must alternate 180° out of phase, pointing down at rest").

This becomes the **authoritative reference** for the iteration plan and implementation.

---

### 6. Proceed to Planning

Once clarity is achieved and documented, proceed to `PLAN_ITERATION.md`. The iteration plan should reference the clarified requirements explicitly and include the visual references as validation checkpoints.

---

## Example Clarification Flow

**Initial Feature Description:**
> "Add skeletal animation for running. Arms and legs should move naturally using the surveyor-wheel pattern."

**Identified Ambiguities:**
- What does "naturally" mean? (arms swinging? pumping? T-pose?)
- How do arms "move" — which joints rotate, around which axes?
- What is the relationship between arm and leg motion? (alternating? synchronized?)
- Is there a reference animation showing the desired result?

**Targeted Questions:**
1. "Do you have a reference image or animation showing the desired arm/leg poses?"
2. "Should arms alternate with legs (left arm forward when right leg forward) or move independently?"
3. "In the rest pose, where should the arms point — down at sides, or extended horizontally?"
4. "Which joints need to rotate — just shoulders/hips, or also elbows/knees?"

**User Provides Reference:**
> [Bunny running animation image showing two keyframes: PASS pose and REACH pose, arms down at sides, alternating forward/back]

**Synthesized Understanding:**
- **Visual target:** Two distinct keyframes (PASS/REACH) that alternate based on distance traveled.
- **Arm position:** Arms hang down at sides (not T-pose), swing forward/back like pendulums.
- **Joint involvement:** Shoulders (3-axis rotation), elbows (bend), hips, knees.
- **Motion pattern:** Left leg forward = right arm forward (opposite limbs move together).
- **Success criteria:** When standing still, skeleton should show discrete PASS or REACH pose matching bunny reference.

**Confirmation:**
> "My understanding is you want two hardcoded keyframe poses (PASS and REACH) that alternate based on the surveyor-wheel pattern, matching the bunny reference image. Arms must hang down at sides and swing forward/back, with opposite limbs moving together (left leg forward = right arm forward). Correct?"

**User Confirms:** ✅

**Result:** Clear, unambiguous feature description ready for iteration planning.

---

## Tone & Constraints

- **Direct and specific:** Ask concrete questions, avoid philosophical debates.
- **Visual-first:** Prioritize images/videos over verbal descriptions of motion.
- **Challenge assumptions:** If something sounds vague, it probably is — push for clarity.
- **Respect iteration speed:** Ambiguity costs time; clarification upfront is an investment.
- **Document everything:** Clarity discovered in conversation must be written down.

---

## When to Skip This Task

- Feature description already includes concrete visual references and specific implementation details.
- Feature is a pure refactor or bug fix with objective correctness criteria.
- Feature is a simple parameter tweak with no new behavior.

---

**Next Steps After Clarification:**
1. Update feature description document with clarifications.
2. Proceed to `PLAN_ITERATION.md` with clear requirements.
3. Reference visual materials throughout implementation as validation checkpoints.
