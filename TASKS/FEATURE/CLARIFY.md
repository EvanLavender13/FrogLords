# Clarify Feature Requirements

Use **after receiving a feature description** and **before implementation planning**. Goal: eliminate ambiguity via targeted questions that expose gaps, assumptions, or missing visual references.

**Use when:**
- Description feels vague or open to interpretation
- User has specific visual/behavioral reference not shared
- Feature involves animation/poses implementable multiple ways
- Abstract terms ("natural," "smooth," "dynamic") without concrete examples
- Uncertain about exact end state or success criteria

### 1. Review Development Principles

Read `AGENTS.md` to synthesize core principles:
- **Iteration over planning:** Short loops need clear targets; ambiguity kills speed
- **Paradox of quality:** Clarity upfront maximizes iteration count
- **Knowledge creation:** Asking right questions is knowledge creation
- **Testing:** Can't test without knowing what "correct" looks like

### 2. Identify Ambiguities

**Visual/Behavioral:**
- Multiple ways to interpret motion/appearance?
- User has reference image/animation/video?
- Abstract terms ("natural," "smooth") without examples?
- Different visual styles all match description?

**Mechanical:**
- Exact input→output mapping? (e.g., "swing arm" = which joint, which axis?)
- Multiple joints/systems could satisfy description?
- Relationship between user input and animation/behavior?
- Time-based, distance-based, or event-based?

**Scope:**
- Minimum viable vs. full vision?
- Graybox-acceptable roughness vs. must-be-correct-now?
- Unstated dependencies?
- What does "done" look like?

**Success Criteria:**
- How to verify correctness?
- Visual/behavioral test for verification?
- Specific keyframes/poses/moments matching reference?
- What makes user say "yes, that's it" vs. "no, not quite"?

### 3. Ask Targeted Questions

Formulate questions that **reduce uncertainty** and **expose hidden assumptions**:
- Request concrete examples over abstract descriptions
- Ask for visual references (images, videos, animations)
- Clarify relationships between systems
- Identify most critical element to get right first
- Reveal user's mental model

**Visual References:**
- "Reference image/video showing desired result?"
- "Specific pose/keyframe defining this?" (e.g., "both arms down" vs. "one forward, one back")
- "What existing game/animation does this resemble?"

**Mechanical Clarity:**
- "Driven by time, distance, or user input?"
- "Which joints/bones move?" (list specific names)
- "Loop, blend, or discrete states?"
- "Rotation axes involved?" (pitch/yaw/roll, X/Y/Z)

**Scope Boundaries:**
- "Minimum graybox proving concept?"
- "What can look rough vs. must be accurate now?"
- "Edge cases to ignore this iteration?"

**Success Criteria:**
- "How to verify correctness? Visual test?"
- "What makes you say 'yes, exactly right'?"
- "Detail distinguishing correct from incorrect implementation?"

### 4. Synthesize Understanding

After receiving answers, summarize:
- **Visual/behavioral target:** Final result look/feel?
- **Reference materials:** Images/videos defining correctness?
- **Mechanical approach:** Systems/joints involved and interactions?
- **Minimum viable:** Simplest graybox proving concept?
- **Test criteria:** How to verify without user feedback?

State synthesis for confirmation: **"My understanding is..."**

### 5. Document Clarifications

Add `## CLARIFICATION QUESTIONS (Pending)` section to `PLANS/<name>_FEATURE.md`:
- All identified ambiguities by category
- Specific questions needing answers
- Priority indicators for critical info
- Examples/options illustrating interpretations

Once answered, update document:
- Move items from "Pending" to "Resolved" (or remove section)
- Add concrete visual references (images, video links)
- Add specific joint/system names and rotation axes
- Add exact keyframe descriptions/pose breakdowns
- Add clear success criteria (e.g., "arms alternate 180° out of phase, pointing down at rest")

This becomes **authoritative reference** for implementation planning.

### 6. Consolidate Feature Description

Once clarifications resolved, perform consistency and redundancy pass:

**Consistency:**
- **Ordering:** Lists/sequences maintain identical structure throughout
- **Terminology:** Same terms used consistently (e.g., "forward" always same direction)
- **Value precision:** Remove contradictions ("~90°" vs "locked at 90°" — pick one)
- **Cross-references:** Sections reference correctly, content exists

**Redundancy Elimination:**
- **Single source of truth:** Each fact appears exactly once
- **Remove padding:** Strip phrases adding no information
- **Consolidate:** Merge repeated concepts into one statement
- **Collapse sections:** Merge overlapping sections

**Clarity:**
- **Remove ambiguous qualifiers:** Replace "approximately", "should", "may" with concrete values
- **Separate scope:** Active implementation vs. deferred features clearly separated
- **Streamline:** Follow AGENTS.md: "Concise and direct. No preamble/postamble."

**Output:** Terse, zero-redundancy, internally consistent description ready for planning.

### 7. Proceed to Implementation Planning

Once clarity achieved, documented, and consolidated, proceed to PLAN. Reference clarified requirements and visual references as validation checkpoints.

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

**Result:** Clear, unambiguous feature description ready for implementation planning.

---

## Tone & Constraints

- Direct and specific: concrete questions, no philosophical debates
- Visual-first: prioritize images/videos over verbal descriptions
- Challenge assumptions: if vague, push for clarity
- Respect iteration speed: ambiguity costs time; clarification is investment
- Document everything: clarity from conversation must be written

## When to Skip

- Description includes concrete visual references and specific details
- Pure refactor or bug fix with objective criteria
- Simple parameter tweak with no new behavior

**Next Steps:**
1. Update feature description with clarifications
2. Proceed to PLAN with clear requirements
3. Reference visual materials as validation checkpoints
