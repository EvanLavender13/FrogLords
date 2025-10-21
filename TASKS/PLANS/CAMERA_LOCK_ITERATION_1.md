# Iteration 1: Orientation-Locked Camera

**Started:** 2025-10-21
**Status:** In Progress

---

<!-- BEGIN: ITERATE/CONTRACT -->
## Foundation Contract

Define what must be proven about this system.

**Mathematical Correctness:**
- [ ] Locked position calculation produces finite results for all valid inputs
- [ ] Zero-length direction vector is rejected (assertion fires)
- [ ] Negative distance is rejected (assertion fires)
- [ ] Camera positioning matches expected geometry (behind direction at correct distance)

**Integration Correctness:**
- [ ] Mode toggle switches between free orbit and locked behavior
- [ ] Locked mode uses orientation system's smoothed yaw (not raw controller input)
- [ ] Zoom controls work identically in both modes
- [ ] Height offset applies correctly in locked mode
- [ ] Look target remains consistent across mode switches

**Behavioral Properties:**
- [ ] Locked camera snaps instantly (no additional smoothing beyond upstream orientation)
- [ ] Free orbit mode unaffected by lock implementation
- [ ] Direction vector derived once per frame (no redundant computation)
- [ ] No accumulated state in camera lock (pure function verification)

**Edge Cases:**
- [ ] Switching modes during motion maintains stability
- [ ] Extreme zoom values (min/max) work in locked mode
- [ ] Rapid orientation changes produce expected camera response
<!-- END: ITERATE/CONTRACT -->

---
