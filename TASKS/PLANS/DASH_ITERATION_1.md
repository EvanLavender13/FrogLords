# Iteration 1: Dash

**Started:** 2025-10-19
**Status:** REVISE

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

Properties to validate through iteration:

- [x] Air acceleration removed (physics only when airborne)
  - Validated: No input acceleration in air (controller.cpp:76-82)
  - Commit: (pending)
- [x] Dash enters committed state (no steering during burst)
  - Validated: dash_active_timer blocks acceleration (controller.cpp:76-82)
  - Commit: (pending)
- [x] Dash cooldown prevents spam
  - Validated: dash_timer + dash_active_timer both checked (controller.cpp:122)
  - Commit: (pending)
- [x] Ground-only dash constraint enforced
  - Validated: is_grounded check (controller.cpp:122)
  - Commit: (pending)
- [ ] Dash momentum carries into jump (BLOCKER - see design problem below)
- [ ] Speed cap behavior feels natural (BLOCKER - see design problem below)
<!-- END: ITERATE/CONTRACT -->

---

<!-- BEGIN: ITERATE/DESIGN_PROBLEM -->
## Design Problem: Speed Cap vs Momentum Preservation

**Core conflict:**
- Want: Dash → Jump should carry momentum (skill-based movement)
- Problem: Speed clamp kills momentum mid-air
- Attempted fix: Skip clamp during dash state, skip friction on jump frames
- Result: Growing complexity, awkward edge cases

**Root cause:**
Hard speed clamp conflicts with temporary overspeed from dash impulse.

**Options for VALIDATE review:**

**Option 1: No air clamp**
```cpp
if (is_grounded) {
    clamp_horizontal_speed(velocity, max_speed);
}
// Air: no clamp, preserve momentum
```
- Pros: Dead simple, dash→jump naturally works
- Cons: Could enable high-speed exploits if multiple mechanics chain

**Option 2: Only accelerate below max_speed**
```cpp
if (current_speed < max_speed) {
    acceleration = input_direction * ground_accel;
} else {
    acceleration = glm::vec3(0.0f);
}
// Dash impulse always works (not acceleration-based)
```
- Pros: Prevents infinite acceleration at source
- Cons: Might feel unresponsive at speed threshold

**Option 3: Remove dash committed state**
```cpp
if (dash_input && can_dash) {
    velocity += input_direction * dash_impulse;
    dash_timer = dash_cooldown;
}
// No active timer, no state - just impulse + friction
```
- Pros: Extremely simple, fewer moving parts
- Cons: Less "skill-based" feel (no commitment)

**Current implementation:**
- Dash active state timer
- Skip friction on jump frames
- Skip clamp during dash state
- Multiple interacting flags/timers

**Recommendation for VALIDATE:**
Need external review (Codex) to choose architecture. Current approach adding complexity without clear benefit.

**Status:** Paused iteration pending design decision
<!-- END: ITERATE/DESIGN_PROBLEM -->

---

<!-- BEGIN: VALIDATE/REVIEW -->
## External Review

**Tool:** Codex CLI
**Date:** 2025-10-19

**Question:**
Evaluate the dash system design problem: speed cap vs momentum preservation. Review the three architectural options (no air clamp, accelerate only below max_speed, remove committed state). Score against the six principles (1-10 each): Radical Simplicity, Fundamental Composable Functions, Solid Mathematical Foundations, Emergent Behavior, Consistency, Principled Development. Which option best serves the principles? What's the mathematically correct solution?

**Scores:**

**Option 1 - No air clamp:**
- Radical Simplicity: 7
- Fundamental Composable Functions: 6
- Solid Mathematical Foundations: 4
- Emergent Behavior: 8
- Consistency: 6
- Principled Development: 5
- **Average: 6.0/10**

**Option 2 - Accelerate only below max_speed:**
- Radical Simplicity: 5
- Fundamental Composable Functions: 5
- Solid Mathematical Foundations: 5
- Emergent Behavior: 6
- Consistency: 4
- Principled Development: 4
- **Average: 4.83/10**

**Option 3 - Remove committed state:**
- Radical Simplicity: 9
- Fundamental Composable Functions: 8
- Solid Mathematical Foundations: 3
- Emergent Behavior: 4
- Consistency: 5
- Principled Development: 4
- **Average: 5.5/10**

**Critical Insight:**
All three options sit on top of the same mathematically incorrect hard clamp. Option 1 best upholds principles overall (6.0 average), but still fails Solid Mathematical Foundations (4/10) because it doesn't fix the root problem.

**Root Cause Identified:**
The hard speed clamp at controller.cpp:25-33 is a non-physical truncation. Propulsion (ground_accel) exceeds friction, so without the clamp, speed grows unbounded. This violates Solid Mathematical Foundations principle.

**Mathematically Correct Solution:**
Replace hard clamp with velocity-dependent drag or scaled friction such that:
- At speed < max_speed: Net acceleration positive
- At speed = max_speed: Forces balance (equilibrium)
- At speed > max_speed: Net acceleration negative (natural deceleration)

This creates a natural equilibrium at max_speed without truncating velocity, allowing dash momentum to decay naturally through physics rather than being clipped by software.

**Best Proposed Option:** Option 1 (no air clamp) - 6.0/10 average
<!-- END: VALIDATE/REVIEW -->

---

<!-- BEGIN: VALIDATE/DECISION -->
## Decision

**Status:** REVISE

**Reasoning:**
Best option (Option 1) scores 6.0 average with multiple principles <6 (Solid Mathematical Foundations: 4, Principled Development: 5), meeting REVISE criteria. More critically, external review revealed all three options are architectural patches around a foundational physics flaw.

The hard speed clamp violates Solid Mathematical Foundations. The mathematically correct solution is velocity-dependent friction that creates natural equilibrium at max_speed, not software truncation.

**Required changes:**

**1. Refactor speed cap system (NEW SYSTEM - prerequisite):**
- Replace hard clamp with velocity-dependent drag/friction
- Model forces such that equilibrium occurs at max_speed
- Ensure net acceleration = 0 at target speed, negative above it
- Location: controller.cpp friction/propulsion balance
- Complexity: Small-to-Medium (affects foundational movement)

**2. Re-implement dash (return to ITERATE after prerequisite):**
- Implement Option 1 (no air clamp) on top of natural speed cap
- Dash impulse exceeds max_speed, velocity-dependent forces naturally decelerate
- Remove all clamp exceptions and special-case logic
- Simple impulse + cooldown, physics handles the rest

**Learning:**
Attempting to build dash revealed a crack in the foundation. The speed cap should emerge from force balance, not be imposed by software. This is a textbook violation of "build from certainty toward uncertainty" - we built on an incorrect assumption (hard clamp) and it betrayed us when we needed temporary overspeed.

**Next Steps:**
1. Document speed cap refactor as new system in SELECT phase
2. Defer dash until prerequisite complete
3. OR: Accept limited dash (no momentum carry) and defer full implementation

**Status:** Iteration 1 incomplete - prerequisite system identified
<!-- END: VALIDATE/DECISION -->

---
