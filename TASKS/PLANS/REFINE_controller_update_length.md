# Refinement: Controller Update Length

Extract sub-phases from monolithic update loop

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-23
**Complexity:** Standard (6 pts)
**Path:** B
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/character/controller.cpp:129`
**Principle:** Radical Simplicity
**Severity:** Low
**Type:** Function length - 163 lines (threshold 100)

**Current state:**
```
controller::update() - single function handling:
- Physics integration (gravity, drag, velocity, position)
- Collision resolution and grounding detection
- Landing event detection
- Jump timing forgiveness updates
- Locomotion state classification and phase calculation
```

**Why violation:**
- Cognitive overload: Five distinct phases in one function
- Testing: Cannot isolate phase behavior independently
- Maintenance: Changes to one phase require navigating entire function
- Reuse: Sub-phases locked inside monolithic structure

**Impact:**
- Blocks: Independent testing of locomotion classification, landing detection
- Cascades to: Future animation system integration, state machine clarity
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify

Extract five sub-phases into private helper methods called sequentially from update().

**From:** Monolithic 163-line update loop
**To:** Orchestrator calling five focused sub-phases

**Derive/Generalize:**
- Physics integration becomes `update_physics(dt)`
- Collision handling becomes `update_collision(world, dt)`
- Landing detection becomes `update_landing_state()`
- Jump timers become `update_jump_timers(dt)`
- Locomotion becomes `update_locomotion_state(dt)`

**Check duplicates:**
- No other functions perform these sub-phases
- Sub-phases remain private (single caller: update())

**Validation:**
- Behavior must remain identical (mathematical correctness)
- Epsilon comparison on position/velocity/state after extraction
- Verify debug visualization unchanged
<!-- END: SELECT/FIX -->
