# Iteration 1: Dash

**Started:** 2025-10-19
**Status:** Design Blocker - Ready for VALIDATE

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
