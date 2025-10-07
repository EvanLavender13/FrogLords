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

**CRITICAL:** Add a clearly labeled `## CLARIFICATION QUESTIONS (Pending)` section to the feature description document (`PLANS/feature_<name>.md`) containing:

- **All identified ambiguities** organized by category (Visual/Behavioral, Mechanical, Scope, Success Criteria)
- **Specific questions** that need answers before proceeding
- **Priority indicators** highlighting the most critical missing information
- **Examples/options** where helpful to illustrate different interpretations

Once answers are received, update the same document by:
- Moving resolved items from "Pending" to "Resolved" (or removing the questions section)
- Adding concrete visual references (attach images, link videos)
- Adding specific joint/system names and rotation axes
- Adding exact keyframe descriptions or pose breakdowns
- Adding clear success criteria (e.g., "arms must alternate 180° out of phase, pointing down at rest")

This becomes the **authoritative reference** for the iteration plan and implementation.

---

### 6. Proceed to Planning

Once clarity is achieved and documented, proceed to `PLAN_ITERATION.md`. The iteration plan should reference the clarified requirements explicitly and include the visual references as validation checkpoints.

---

## Example Clarification Flow

**Initial Feature Description:**
> "Add smooth camera transitions when changing view modes. Should feel natural and responsive."

**Identified Ambiguities:**
- What does "smooth" mean? (linear? eased? spring-damped?)
- What does "natural" mean? (fast? slow? overshooting allowed?)
- What triggers a "view mode change"? (user input? automatic? distance-based?)
- How many view modes are there, and what defines each one?
- Is there a reference game/video showing the desired behavior?

**Targeted Questions:**
1. "Do you have a reference video showing the desired camera transition behavior?"
2. "Should transitions use spring-damper curves, easing curves, or linear interpolation?"
3. "What is the target transition duration — fast (<0.2s), medium (0.2-0.5s), or slow (>0.5s)?"
4. "Should the camera overshoot the target position and settle, or move directly without overshoot?"
5. "Which camera properties transition — position only, or also rotation and FOV?"

**User Provides Reference:**
> "Like the quick-snap in [Game X], about 0.3s with slight overshoot. Position and rotation both transition, FOV stays constant."

**Synthesized Understanding:**
- **Visual target:** Quick transition (~0.3s) with subtle overshoot/settle behavior matching [Game X].
- **Interpolation method:** Spring-damper curve (allows overshoot).
- **Properties affected:** Camera position and rotation; FOV remains constant.
- **Trigger:** User input (specific key/button).
- **Success criteria:** Transition completes in 0.3s ± 0.1s; visible overshoot <10% of travel distance.

**Confirmation:**
> "My understanding is you want a spring-damped camera transition (~0.3s duration with slight overshoot) that affects position and rotation but not FOV, triggered by user input, matching the feel of [Game X]. Correct?"

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
