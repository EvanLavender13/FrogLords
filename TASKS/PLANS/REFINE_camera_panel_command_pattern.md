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

---

<!-- BEGIN: REFINE/PLAN -->
## Refinement Plan

### Step 1: Define camera_command struct
**Changes:** Create `src/gui/camera_command.h`
**Pattern:** Duplicate parameter_command.h structure but for camera parameters
**Lines:**
```cpp
enum class camera_parameter_type {
    distance,
    height_offset,
    min_distance,
    max_distance,
};

struct camera_command {
    camera_parameter_type type;
    float value;
};
```
**Tests:** None (header only)
**Validation:** Compiles successfully

### Step 2: Update draw_camera_panel signature
**Changes:**
- `src/gui/camera_panel.h:14` - Change signature from `void` to `std::vector<camera_command>`
- `src/gui/camera_panel.h` - Add `#include "gui/camera_command.h"`
- `src/gui/camera_panel.h:14` - Change parameter `camera& cam` to `const camera& cam`
- `src/gui/camera_panel.h:14` - Change parameter `camera_follow& cam_follow` to `const camera_follow& cam_follow`
- `src/gui/camera_panel.cpp:7` - Update function signature
- `src/gui/camera_panel.cpp` - Add `#include "gui/camera_command.h"` and `#include <vector>`
- `src/gui/camera_panel.cpp:7` - Add `std::vector<camera_command> commands;` at start
- `src/gui/camera_panel.cpp:9` - Change early return from `return;` to `return commands;`
- `src/gui/camera_panel.cpp:end` - Add `return commands;` before closing brace

**Tests:** None yet (function body not updated)
**Validation:** Compiles with warnings (function body still violates const)

### Step 3: Implement local copies and command emission
**Changes:** `src/gui/camera_panel.cpp:31-46`
**From:**
```cpp
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

**To:**
```cpp
// Local copies for slider interaction (GUI needs mutable values)
float distance = cam_follow.distance;
float height_offset = cam_follow.height_offset;
float min_distance = cam_follow.min_distance;
float max_distance = cam_follow.max_distance;

if (ImGui::SliderFloat("Distance", &distance, min_distance, max_distance, "%.1f m")) {
    commands.push_back({camera_parameter_type::distance, distance});
}

if (ImGui::SliderFloat("Height Offset", &height_offset, 0.0f, 3.0f, "%.1f m")) {
    commands.push_back({camera_parameter_type::height_offset, height_offset});
}

if (ImGui::SliderFloat("Min Distance", &min_distance, 0.5f, 10.0f, "%.1f m")) {
    commands.push_back({camera_parameter_type::min_distance, min_distance});
}
if (ImGui::SliderFloat("Max Distance", &max_distance, 5.0f, 30.0f, "%.1f m")) {
    commands.push_back({camera_parameter_type::max_distance, max_distance});
}
```

**Tests:** None yet (no consumer)
**Validation:** Compiles without warnings, GUI no longer modifies camera state

### Step 4: Update runtime.cpp call site to consume commands
**Changes:** `src/app/runtime.cpp:148-149`
**From:**
```cpp
// Camera section
gui::draw_camera_panel(camera_panel_state, world.cam, world.cam_follow);
```

**To:**
```cpp
// Camera section
auto camera_commands = gui::draw_camera_panel(camera_panel_state, world.cam, world.cam_follow);

// Apply camera commands (unidirectional flow: GUI → commands → game state)
// Enforce invariants: min_distance <= distance <= max_distance
for (const auto& cmd : camera_commands) {
    switch (cmd.type) {
    case gui::camera_parameter_type::distance:
        world.cam_follow.distance = std::clamp(cmd.value,
                                               world.cam_follow.min_distance,
                                               world.cam_follow.max_distance);
        break;
    case gui::camera_parameter_type::height_offset:
        world.cam_follow.height_offset = cmd.value;
        break;
    case gui::camera_parameter_type::min_distance:
        world.cam_follow.min_distance = cmd.value;
        // Clamp distance and max_distance to respect new minimum
        world.cam_follow.distance = std::max(world.cam_follow.distance, cmd.value);
        world.cam_follow.max_distance = std::max(world.cam_follow.max_distance, cmd.value);
        break;
    case gui::camera_parameter_type::max_distance:
        world.cam_follow.max_distance = cmd.value;
        // Clamp distance and min_distance to respect new maximum
        world.cam_follow.distance = std::min(world.cam_follow.distance, cmd.value);
        world.cam_follow.min_distance = std::min(world.cam_follow.min_distance, cmd.value);
        break;
    }
}
```

**Tests:** Manual - verify sliders control camera correctly, verify range changes don't violate invariants
**Validation:** All camera controls functional, behavior identical to before, no assertion failures

## Rollback
`git reset --hard HEAD` or revert individual commits in reverse order
<!-- END: REFINE/PLAN -->

---

<!-- BEGIN: REFINE/REVIEW -->
## Second Opinion Review

**Tool:** Codex CLI
**Date:** 2025-10-19

**Question asked:**
Review this refactoring plan for aligning camera_panel with the command pattern used by character_panel. Validate the 4-step approach, check for edge cases with slider synchronization, evaluate command dispatch location, assess const-correctness, and consider whether camera_command should share parameter_command infrastructure.

**Concerns evaluated:**
- Command pattern implementation correctness
- Slider value synchronization with local copies
- Command dispatch location (runtime.cpp vs game_world)
- Const-correctness and unidirectional data flow
- Command header organization

**Feedback received:**
- **[Medium]** Command application must enforce camera_follow invariants (min_distance <= distance <= max_distance). Without clamping, user can violate preconditions and trip debug assertions.
- **[Low]** Should make camera parameter const (not just camera_follow) for consistency with read-only data flow.
- **Open questions:** Command dispatch location acceptable in runtime.cpp (matches character pattern), separate camera_command header acceptable (avoid premature abstraction).

**Impact on implementation:**
- **Step 2 updated:** Added `const camera& cam` parameter
- **Step 4 updated:** Added clamping logic when applying commands to enforce invariants:
  - distance: clamp to [min_distance, max_distance]
  - min_distance change: clamp distance and max_distance upward
  - max_distance change: clamp distance and min_distance downward
- Kept command dispatch in runtime.cpp (consistency with character_panel)
- Kept separate camera_command.h (no premature generalization)
<!-- END: REFINE/REVIEW -->

---

<!-- BEGIN: REFINE/COMPLETED -->
## Completed

**Change:** Refactored camera_panel to use command pattern for architectural consistency

**Files Modified:**
- `src/gui/camera_command.h` - Created (25 LOC)
- `src/gui/camera_panel.h` - Updated signature (+3 LOC)
- `src/gui/camera_panel.cpp` - Implemented local copies and command emission (+16 LOC, -7 deleted)
- `src/app/runtime.cpp` - Added command consumption with invariant enforcement (+29 LOC)

**Architecture:**
- **Before:** Bidirectional coupling (GUI ↔ camera_follow via mutable references)
- **After:** Unidirectional flow (camera_follow → GUI → commands → camera_follow)

**Pattern Alignment:**
- camera_panel now mirrors character_panel command pattern exactly
- GUI reads state (const refs), emits commands, game loop applies commands
- Architectural consistency across all debug panels

**Invariant Enforcement:**
- distance clamped to [min_distance, max_distance]
- min_distance changes propagate upward (distance, max_distance)
- max_distance changes propagate downward (distance, min_distance)
- Prevents assertion failures in camera_follow::zoom

**Tests:** All passing
- Build successful, no compilation warnings
- Program runs without errors
- Camera controls functional via command pattern
- Invariants enforced at command application
- No regressions detected

**Result:** ✓ Violation removed, principle score improved
<!-- END: REFINE/COMPLETED -->
