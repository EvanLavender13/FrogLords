# Refinement: Camera Panel Command Pattern

**Refactor camera_panel to use command pattern for consistency with character_panel**

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-19
**Complexity:** Standard (3-5 points)
**Path:** B (standard)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/gui/camera_panel.cpp:31-46` (draw_camera_panel function)
**Principle:** Fundamental Composable Functions, Consistency | Score: 7/10
**Severity:** High
**Type:** Tight coupling, Unidirectional data flow violation, Architectural inconsistency

**Current state:**
```cpp
// Lines 31-46 in camera_panel.cpp
if (ImGui::SliderFloat("Distance", &cam_follow.distance, cam_follow.min_distance,
                       cam_follow.max_distance, "%.1f m")) {
    // Value updated directly via slider
}

if (ImGui::SliderFloat("Height Offset", &cam_follow.height_offset, 0.0f, 3.0f, "%.1f m")) {
    // Value updated directly via slider
}

ImGui::SliderFloat("Min Distance", &cam_follow.min_distance, 0.5f, 10.0f, "%.1f m");
ImGui::SliderFloat("Max Distance", &cam_follow.max_distance, 5.0f, 30.0f, "%.1f m");
```

**Why violation:**
- GUI directly modifies camera_follow state via mutable references (tight coupling)
- Violates unidirectional data flow principle (GUI should emit commands, not modify state)
- Inconsistent architecture: character_panel uses command pattern (returns vector of commands), camera_panel does not
- Prevents composability - cannot intercept, validate, or log camera parameter changes
- Bidirectional coupling makes camera system dependent on GUI implementation

**Impact:**
- Blocks: Clean separation of GUI and game logic
- Cascades to: Future camera systems that need validation or change tracking
- Foundation: Layer 3 (Systems) - architectural inconsistency
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify (align with existing pattern)

**SIMPLIFY:**

**From:** Direct state modification via mutable references
```
GUI ←→ Camera (bidirectional, tight coupling)
```

**To:** Command pattern matching character_panel
```
GUI → Commands → Game World → Camera (unidirectional, loose coupling)
```

**Pattern alignment:**
1. Define camera command types (similar to parameter_command in character_panel)
2. Create local slider values instead of direct references
3. Return vector of camera commands when sliders change
4. Apply commands in game_world (similar to character parameter updates)

**Key changes:**
- Add camera_command struct to camera_panel.h
- Modify draw_camera_panel() signature to return std::vector<camera_command>
- Create local copies for slider interaction (break mutable reference coupling)
- Emit commands on slider changes instead of direct modification
- Update game_world to consume and apply camera commands

**Derive/Generalize:**
- Could extract common "command emitter" pattern if more panels emerge
- For now, duplicate character_panel pattern (consistency over premature abstraction)

**Check duplicates:**
- Search for other GUI components that directly modify state
- Verify all panels follow command pattern after fix
<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] camera_panel refactored to command pattern
- [ ] Returns std::vector<camera_command> instead of void
- [ ] game_world applies camera commands
- [ ] No direct state modification in GUI
- [ ] Architectural consistency with character_panel
- [ ] Principle score improved
- [ ] Tests passing
- [ ] No regressions in camera behavior

**Metrics:**
- Before: LOC ~47, Principle 7/10 (coupling), Complexity Medium
- After: LOC ~70 (+23, command struct + local copies), Principle 9/10 (+2, consistency), Complexity Low (follows existing pattern)

**Validation:**
- Camera controls function identically
- Commands properly flow GUI → World → Camera
- No compilation warnings
- Manual testing confirms behavior unchanged
<!-- END: SELECT/SUCCESS -->
