# Mouse Wheel Camera Zoom

**Core Mechanic:** Mouse wheel scroll adjusts camera distance from character in both ORBIT and FOLLOW modes. Closer zoom enables detailed inspection of skeletal animation and character kinematics during iteration.

**Problem Evidence:** During skeletal animation iteration (Oct 9, 2025), need to closely observe limb motion, joint rotations, and procedural animation details. Current fixed camera distance (ORBIT: 2-30m, FOLLOW: 5m fixed) insufficient for detailed inspection of subtle motion (elbow lag, knee swing, acceleration tilt). User specifically requested ability to "zoom in closer to get a better view of the kinematics" as movement complexity increases.

**Pattern Reuse:** Extends existing proven systems:
- Input: `input::mouse_scroll_y()` already captured and wired to camera in `runtime.cpp:72-75`
- Camera: `camera::zoom()` method exists and works correctly for ORBIT mode with `min_distance`/`max_distance` clamping (2-30m)
- FOLLOW mode uses separate `follow_distance` state (currently hardcoded 5m, no zoom support)

**Graybox Scope:**
- **ORBIT mode:** Already functional ✅ (scroll controls `distance`, clamped 2-30m)
- **FOLLOW mode (main task):** Wire scroll to `follow_distance` with appropriate limits
  - Minimum distance: ~1.5m (close inspection without camera-inside-character)
  - Maximum distance: ~15m (wide view for spatial awareness)
  - Scroll sensitivity: Match ORBIT mode scaling (`-scroll_delta * 0.5f`)
- **Tuning parameters (expose via debug UI):**
  - `min_follow_distance` (default: 1.5m)
  - `max_follow_distance` (default: 15m)
  - `scroll_zoom_speed` (default: 0.5)
- **Visual feedback:** None required (camera movement is self-evident)

**Minimal Implementation:** ~15-30 lines
1. **camera.h:** Add public getter/setter for follow distance limits
   ```cpp
   void set_follow_distance_limits(float min_dist, float max_dist);
   float get_follow_distance() const { return follow_distance; }
   ```
2. **camera.cpp:** Update `zoom()` to handle FOLLOW mode
   ```cpp
   void camera::zoom(float delta) {
       if (mode == camera_mode::FOLLOW) {
           follow_distance = std::clamp(follow_distance + delta, min_follow_distance, max_follow_distance);
       } else {
           distance = std::clamp(distance + delta, min_distance, max_distance);
           update_eye_position();
       }
   }
   ```
   Add private members: `min_follow_distance`, `max_follow_distance`
3. **runtime.cpp:** No changes needed (scroll already wired to `cam.zoom()`)
4. **character_panel.cpp (optional tuning UI):** Add sliders for min/max distance limits

**Unlocks:** Nothing (pure quality-of-life feature). Improves iteration velocity for all future animation/movement work by enabling detailed observation.

**Test Loop:**
1. Launch game, switch to FOLLOW mode
2. Scroll mouse wheel → camera should zoom in/out smoothly
3. Verify minimum distance stops ~1.5m (character visible, not inside camera)
4. Verify maximum distance stops ~15m (still useful for spatial context)
5. Test ORBIT mode still works (regression check)
6. Iterate limits if 1.5-15m range feels wrong

Expected iteration time: 30-60 minutes (implementation + feel tuning)

**Certainty:** 95%
- **High certainty factors:**
  - Input capture proven ✅ (`mouse_scroll_y` works)
  - ORBIT zoom proven ✅ (existing `zoom()` method works)
  - FOLLOW mode camera math stable ✅ (just needs distance parameter exposed)
  - No new architecture/systems required (pure parameter wiring)
- **Low uncertainty factors:**
  - Ideal min/max distance limits (requires 5-10 min feel tuning)
  - Scroll sensitivity may need tweaking (current 0.5x may feel too slow/fast for closer distances)

**Dependencies:**
- `camera` ✅ (stable foundation)
- `input` ✅ (scroll capture working)
- `runtime` ✅ (scroll→zoom wiring exists)

**Non-Goals (Explicitly Excluded):**
- Camera collision (preventing wall clipping) — deferred to backlog
- Smooth zoom interpolation (spring-damper) — current instant zoom acceptable for debug/iteration
- FOV-based zoom (changing field-of-view instead of distance) — distance-based zoom is correct for inspection use case
- Per-mode zoom memory (remembering ORBIT vs FOLLOW distances separately) — unnecessary complexity

**Risk Assessment:** Very Low
- Isolated change (only affects camera distance parameter)
- No gameplay impact (pure camera UX)
- Existing ORBIT zoom proves pattern safe
- Easy to revert if feels wrong (delete ~15 lines, restore hardcoded `follow_distance = 5.0f`)

---

**Implementation Notes:**
- Consider making min/max limits mode-specific if ORBIT/FOLLOW need different ranges (current: ORBIT 2-30m, proposed FOLLOW 1.5-15m)
- If scroll feels too sensitive at close distances, consider non-linear scaling (e.g., zoom speed scales with current distance)
- Keep tuning sliders in debug UI, not production settings (this is a dev tool feature)

---

## Simple Feature Workflow

**Rationale:** This feature uses a streamlined workflow compared to complex multi-phase iterations. Standard workflow includes two-tier planning (high-level feature plan → detailed implementation plan → code review). Simple features skip the implementation plan tier when:
- Single atomic change (one coherent edit session)
- No architectural uncertainty (extends proven patterns)
- ~15-50 line scope (smaller than typical "minimal" plans)
- Infrastructure already validated (proven systems being wired together)
- Trivial to revert (low risk, isolated change)

**Workflow Steps:**

### 1. Create Iteration Branch
- **Task:** `TASKS/FEATURE/CREATE_ITERATION_BRANCH.md` (existing)
- **Branch naming:** `feature/camera-zoom` (descriptive, kebab-case)
- **Purpose:** Isolate work from main; enable clean revert if needed
- **Output:** New git branch, ready for commits

### 2. Single Implementation Step
- **Rule:** If feature requires more than one implementation step, it's not "simple" → create full implementation plan instead
- **This feature:** Single step = "Wire scroll wheel to follow_distance with clamping"
- **Files touched:** `camera.h`, `camera.cpp` (maybe `character_panel.cpp` for optional tuning UI)
- **Expected time:** 30-60 minutes (implementation + basic testing)
- **Success criteria:** 
  - Scroll wheel zooms in FOLLOW mode
  - Min/max limits enforced (1.5-15m default range)
  - ORBIT mode still works (regression check)
  - No crashes, no visual artifacts

### 3. Quick Review
- **Purpose:** Sanity check, not deep architectural analysis
- **Focus areas:**
  - Does it work as expected? (functional validation)
  - Are the default limits reasonable? (feel check)
  - Any unexpected side effects? (regression scan)
  - Code quality: naming, consistency, snake_case, no magic numbers
- **Scope:** Brief (~5-10 min), not exhaustive like complex feature reviews
- **Document:** Create `PLANS/review_camera_zoom.md` if non-trivial issues found
- **Skip if:** Everything works perfectly, feels good, no surprises

### 4. Finalize Iteration
- **New task needed:** `TASKS/FEATURE/FINALIZE_SIMPLE_ITERATION.md`
- **Purpose:** Wrap up simple feature work before archival
- **Steps:**
  1. Verify implementation matches feature description
  2. Test all success criteria (from feature description)
  3. Run linter/formatter (`clang-format`, `clang-tidy`)
  4. Build and run final validation
  5. Update `DESIGN_BACKLOG.md`:
     - Mark feature complete (add ✅ **COMPLETE** marker)
     - Add *Completion Date*
     - Add *Learning* section with key findings (even if brief)
     - Reference implementation/review docs if they exist
  6. Update `DEPENDENCY_STACK.md` if feature unlocks new capabilities
  7. Commit all changes with descriptive message
- **Differs from complex workflow:** No multi-plan reconciliation, simpler checklist

### 5. Archive Iteration
- **Task:** `TASKS/FEATURE/ARCHIVE_ITERATION.md` (existing, should work as-is)
- **Purpose:** Document completion, merge to main, create historical record
- **Verification:** Existing task calls out "confirm plans reconciled" → adapt for simple features:
  - Complex features: Verify feature plan + implementation plan + review all aligned
  - Simple features: Verify feature description + optional review aligned (no implementation plan to reconcile)
- **Output:** Merged to main, iteration snapshot archived, backlog updated

---

**Key Differences from Complex Workflow:**

| Aspect | Complex Feature | Simple Feature |
|--------|----------------|----------------|
| **Planning tiers** | 2-tier (feature plan → implementation plan) | 1-tier (feature description only) |
| **Implementation steps** | Multiple coordinated steps | Single atomic change |
| **Review depth** | Comprehensive architectural analysis | Quick sanity check |
| **Finalization** | Multi-plan reconciliation, detailed learning docs | Single-document update, brief learnings |
| **Time horizon** | Days to weeks | 30-60 minutes |
| **Certainty threshold** | 70-90% (requires investigation) | 95%+ (infrastructure proven) |

**When to Escalate to Complex Workflow:**
- Implementation reveals unexpected complexity (multi-step coordination needed)
- Architectural questions emerge during coding (pattern uncertain)
- Feature scope creeps beyond single atomic change
- Dependencies on uncertain systems discovered
- Testing reveals need for deeper iteration loops

**Simple Feature Criteria (All must be true):**
1. ✅ Single atomic implementation step
2. ✅ Infrastructure proven (extends existing validated patterns)
3. ✅ ~15-50 line scope
4. ✅ 95%+ certainty (no architectural unknowns)
5. ✅ Zero gameplay impact OR pure UX/debug feature
6. ✅ Trivial to revert if wrong

**Documentation Outputs:**
- **Always:** `feature_<name>.md` (this document)
- **Optional:** `review_<name>.md` (if non-trivial issues found during review)
- **Never:** `implementation_<name>.md` (reserved for complex multi-step features)

---

## Actual Implementation (October 9-10, 2025)

### Scope Evolution

**Original Plan:** ~15-30 lines, wire scroll wheel to `follow_distance` in FOLLOW mode, optional debug UI sliders.

**Actual Scope:** ~50-60 lines across 6 files, plus GUI refactoring. Scope increased mid-implementation due to:
1. **Discovered bug:** Camera "shift" artifact during zoom (one-frame delay in FOLLOW mode eye position update)
2. **User request:** Camera debug panel for tuning height offset and zoom limits
3. **Architectural improvement:** Refactored GUI to unified Debug Panel instead of separate windows

**Scope Justification:** All additions debug/iteration-focused, no gameplay impact. Increased value (tunable camera parameters) for minimal added complexity.

---

### Implementation Steps

#### 1. Core Camera Zoom (Initial Implementation)

**Files Modified:**
- `camera.h`: Added getters/setters and private members
- `camera.cpp`: Updated `zoom()` method for FOLLOW mode
- `CMakeLists.txt`: Added `camera_panel.cpp` to build

**Changes:**
```cpp
// camera.h additions
void set_follow_distance_limits(float min_dist, float max_dist);
float get_follow_distance() const { return follow_distance; }
float get_follow_height() const { return follow_height_offset; }

float min_follow_distance = 1.5f;
float max_follow_distance = 15.0f;
```

```cpp
// camera.cpp zoom() update (first version - had bug)
void camera::zoom(float delta) {
    if (mode == camera_mode::FOLLOW) {
        follow_distance = std::clamp(follow_distance + delta, 
                                     min_follow_distance, max_follow_distance);
    } else {
        distance = std::clamp(distance + delta, min_distance, max_distance);
        update_eye_position();
    }
}
```

**Test Result:** Build succeeded, but user observed camera "shift" artifact during zoom in FOLLOW mode.

---

#### 2. Bug Fix: One-Frame Delay

**Problem:** In FOLLOW mode, `zoom()` updated `follow_distance` but didn't recalculate `eye_pos` until next frame's `follow_update()` call. This caused visible "shift" as camera lagged one frame behind scroll input.

**Root Cause:** ORBIT mode calls `update_eye_position()` immediately in `zoom()`, but FOLLOW mode relied on deferred update.

**Fix:**
```cpp
// camera.cpp zoom() update (fixed version)
void camera::zoom(float delta) {
    if (mode == camera_mode::FOLLOW) {
        follow_distance = std::clamp(follow_distance + delta, 
                                     min_follow_distance, max_follow_distance);
        // Recalculate eye position immediately to avoid one-frame delay
        float lat_rad = glm::radians(latitude);
        float lon_rad = glm::radians(longitude);
        eye_pos.x = center.x + follow_distance * cosf(lat_rad) * sinf(lon_rad);
        eye_pos.y = center.y + follow_distance * sinf(lat_rad);
        eye_pos.z = center.z + follow_distance * cosf(lat_rad) * cosf(lon_rad);
    } else {
        distance = std::clamp(distance + delta, min_distance, max_distance);
        update_eye_position();
    }
}
```

**Learning:** Spherical coordinate math duplicated from `follow_update()`. Could refactor to shared helper, but code duplication acceptable for ~5 lines (avoids premature abstraction).

**Test Result:** Bug fixed, zoom smooth in both modes.

---

#### 3. Camera Debug Panel (Scope Addition)

**User Request:** "Can you add a Camera panel to the debug GUI?" for tuning height offset and zoom limits during iteration.

**New Files Created:**
- `gui/camera_panel.h` - Panel state and draw function declaration
- `gui/camera_panel.cpp` - Panel implementation

**Initial Implementation:**
```cpp
// camera_panel.cpp (initial - separate window)
void draw_camera_panel(camera_panel_state& state, camera& cam) {
    if (!state.show) return;
    
    ImGui::Begin("Camera", &state.show);
    
    const char* mode_name = (cam.get_mode() == camera_mode::ORBIT) ? "ORBIT" : "FOLLOW";
    ImGui::Text("Mode: %s", mode_name);
    ImGui::Text("Distance: %.2f m", cam.get_follow_distance());
    ImGui::Text("FOV: %.1f degrees", cam.get_fov());
    // ... sliders for distance, height offset, zoom limits
    
    ImGui::End();
}
```

**Files Modified:**
- `app/runtime.h` - Added `camera_panel_state` member
- `app/runtime.cpp` - Called `draw_camera_panel()`

**Test Result:** Worked, but created separate "Camera" window alongside "Character Tuning" window. User requested unified left-side panel.

---

#### 4. GUI Refactoring: Unified Debug Panel

**User Request:** "Instead of a new window; could the camera panel and character panel be part of the same left-side static panel window?"

**Challenge:** Avoid coupling `character_panel` and `camera_panel` while sharing same window.

**Solution:** Runtime manages window lifecycle, panels draw content as collapsing headers.

**Refactoring Steps:**

1. **Remove window management from character_panel:**
   ```cpp
   // character_panel.cpp (before - managed own window)
   void draw_character_panel(...) {
       ImGui::Begin("Character Tuning", &state.show, flags);
       // ... draw sections
       ImGui::End();
   }
   
   // After - just draws collapsing headers
   void draw_character_panel(...) {
       if (!state.show) return;
       
       if (ImGui::CollapsingHeader("Character Tuning", ImGuiTreeNodeFlags_DefaultOpen)) {
           // ... draw content
       }
       // ... other collapsing headers
   }
   ```

2. **Convert camera_panel to collapsing header:**
   ```cpp
   // camera_panel.cpp (refactored)
   void draw_camera_panel(camera_panel_state& state, camera& cam) {
       if (!state.show) return;
       
       if (!ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
           return;
       
       // ... draw content (no Begin/End)
   }
   ```

3. **Move window management to runtime:**
   ```cpp
   // runtime.cpp (final version)
   gui::begin_frame();
   
   // Create unified debug panel on left side
   ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
   ImGui::SetNextWindowSize(ImVec2(0.0f, static_cast<float>(sapp_height())), ImGuiCond_Always);
   ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove;
   
   if (ImGui::Begin("Debug Panel", nullptr, flags)) {
       // Character sections (independent collapsing headers)
       gui::draw_character_panel(panel_state, world.character, world.locomotion,
                                 world.character.orientation, world.character_params);
       
       // Camera section (independent collapsing header)
       gui::draw_camera_panel(camera_panel_state, world.cam);
       
       // FPS display (moved from character_panel, not character-specific)
       ImGui::Spacing();
       ImGui::Separator();
       ImGui::Text("FPS: %.1f", 1.0f / sapp_frame_duration());
   }
   ImGui::End();
   ```

4. **Move FPS display to runtime:**
   - **Rationale:** FPS is global metric, not character-specific. Moved from `character_panel.cpp` to `runtime.cpp` for proper separation of concerns.

**Files Modified:**
- `gui/character_panel.cpp` - Removed `ImGui::Begin()`/`End()`, removed FPS display
- `gui/camera_panel.cpp` - Removed `ImGui::Begin()`/`End()`
- `app/runtime.cpp` - Added window creation, added `#include <imgui.h>`, added FPS display

**Test Result:** Single "Debug Panel" window on left side with all sections as collapsing headers. Clean separation: runtime owns window, panels own content.

---

### Final File Manifest

**Files Modified (6 total):**
1. `src/camera/camera.h` (~8 lines added: getters, private members)
2. `src/camera/camera.cpp` (~10 lines modified: `zoom()` method with immediate eye position update)
3. `src/gui/camera_panel.h` (new file, ~15 lines: panel state struct, draw function declaration)
4. `src/gui/camera_panel.cpp` (new file, ~60 lines: camera controls as collapsing header)
5. `src/app/runtime.h` (~2 lines: added `camera_panel_state` member)
6. `src/app/runtime.cpp` (~15 lines: unified Debug Panel window management, FPS display)
7. `src/gui/character_panel.cpp` (~5 lines removed: `ImGui::Begin()`/`End()`, FPS display)
8. `CMakeLists.txt` (~1 line: added `camera_panel.cpp` to build)

**Total Lines Changed:** ~60 lines (vs. original estimate of 15-30)

**Scope Categories:**
- **Core feature (camera zoom):** ~18 lines (camera.h/cpp)
- **Bug fix (one-frame delay):** ~5 lines (camera.cpp)
- **Debug UI (camera panel):** ~75 lines (new files)
- **Refactoring (unified panel):** ~20 lines (existing files modified)

---

### Key Learnings

1. **Immediate update requirement:** When modifying state that affects rendering, always update dependent state in same frame to avoid visual artifacts. One-frame delays are noticeable during rapid input (mouse scroll).

2. **Spherical camera pivot:** `follow_height_offset` creates spherical orbit around point above character (not character center). This is intentional for better framing (camera looks at torso, not feet), but creates "shift" feel during zoom. Debug UI allows tuning this offset (0-3m slider).

3. **GUI refactoring value:** Small upfront investment (unified Debug Panel) improves future feature velocity. Adding new debug sections now trivial (just collapsing headers, no window management). Pattern scales to arbitrary panels (physics, audio, profiling).

4. **Scope creep acceptable for debug tools:** Original plan 15-30 lines, actual 60 lines. Increase justified because:
   - All additions are debug/iteration tools (zero gameplay impact)
   - Bug fix was necessary (not optional polish)
   - GUI refactoring improves long-term maintainability
   - Still completed in single session (~90 min vs. estimated 30-60 min)

5. **Simple feature criteria:** This feature stretched "simple" definition but remained appropriate for streamlined workflow because:
   - ✅ Single coherent session (no multi-day coordination)
   - ✅ No architectural uncertainty (all extensions proven patterns)
   - ✅ Trivial to revert (isolated to camera/GUI, no core gameplay)
   - ⚠️ Scope increased mid-implementation (but remained debug-only)
   
   **Retrospective question:** Should "scope must not increase beyond 2x estimate" be added to simple feature criteria?

6. **Coupling avoidance:** Initial impulse was to have `character_panel` call `camera_panel`, but this creates unnecessary coupling. Runtime-managed window with independent collapsing headers is cleaner architecture (follows single responsibility principle).

---

### Test Results

**Functional Tests (All Passed):**
- ✅ Mouse wheel zooms in FOLLOW mode (1.5-15m range)
- ✅ Mouse wheel zooms in ORBIT mode (2-30m range, regression check)
- ✅ Camera height offset slider updates camera look-at point (0-3m range)
- ✅ Zoom limit sliders constrain scroll wheel range dynamically
- ✅ No visual artifacts (shift bug fixed)
- ✅ FPS display visible at bottom of Debug Panel
- ✅ All collapsing headers work independently

**Code Quality:**
- ✅ Builds without errors
- ✅ No linter warnings (clang-tidy clean)
- ✅ Consistent snake_case naming
- ✅ No magic numbers (all constants named/tunable)

**Feel Testing:**
- ✅ Default range (1.5-15m) appropriate for skeletal animation inspection
- ✅ Scroll sensitivity (0.5x) feels responsive without being twitchy
- ✅ Height offset 1.5m default provides good torso framing
- ⚠️ Very close zoom (<2m) reveals character mesh gaps (expected for graybox sphere - not a bug)

---

### Retrospective Notes

**What Went Well:**
- Bug discovery during testing (one-frame delay) led to better implementation
- User-driven scope additions (camera panel, unified GUI) improved iteration tools
- Refactoring to unified Debug Panel sets good precedent for future panels

**What Surprised Us:**
- Spherical camera orbit creating "shift" effect (initially thought bug, actually geometry)
- GUI refactoring taking ~30 min (longer than core feature implementation)
- Height offset parameter proving immediately valuable for tuning camera feel

**Process Observations:**
- Simple feature workflow handled scope increase gracefully (stayed single session)
- Retrospective documentation time (~15 min) worth investment for future learning
- User involvement during implementation caught issues early (shift artifact)

**Future Improvements:**
- Consider adding "max 2x scope increase" rule to simple feature criteria
- Pre-emptively ask "will user want debug UI?" for UX features (answer usually yes)
- Document "spherical orbit geometry" concept in camera documentation (reusable learning)

---

### Success Metrics

**Original Goal:** Enable closer camera zoom for inspecting skeletal animation details.

**Achieved:**
- ✅ Zoom range 1.5-15m (vs. fixed 5m) enables close inspection
- ✅ Debug UI allows real-time tuning of all camera parameters
- ✅ Unified Debug Panel improves overall iteration workflow
- ✅ Zero gameplay impact (pure dev tool enhancement)
- ✅ Completed in single session (simple workflow validated)

**Bonus Outcomes:**
- Camera height offset now tunable (discovered during implementation)
- Zoom limits dynamically adjustable (future-proofs for different use cases)
- GUI architecture improved for future debug panel additions
- Bug fix (one-frame delay) improved overall camera responsiveness

**Time Investment:**
- Estimated: 30-60 minutes
- Actual: ~90 minutes (60% over estimate)
- Breakdown: 20 min core feature, 10 min bug fix, 30 min GUI, 15 min testing, 15 min documentation

**ROI:** High - ~30 min overhead for camera panel + unified GUI will save 10+ min on every future debug panel addition.

---

## Completion

**Completion Date:** October 10, 2025

**Final Certainty Score:** 100% (feature fully functional and stable)

**Outcome Summary:** Successfully implemented mouse wheel camera zoom for FOLLOW mode with debug UI controls. Scope expanded organically to include camera debug panel and unified Debug Panel GUI architecture, improving overall iteration workflow. All functionality tested and working without issues.

---

## Reflection

**What went well?**
- **User involvement during implementation:** Immediate feedback caught one-frame delay bug early, preventing merge of broken behavior
- **Organic scope evolution:** Camera panel and GUI refactoring emerged naturally from user needs during implementation rather than premature planning
- **Simple feature workflow validation:** Despite 2x scope increase (30→90 min), single-session completion proved streamlined workflow appropriate

**What caused friction?**
- **One-frame delay bug not anticipated:** Initial implementation overlooked need for immediate eye position update in FOLLOW mode (ORBIT mode pattern not fully analyzed)
- **GUI coupling false start:** Brief attempt to couple character_panel and camera_panel before recognizing cleaner architecture (runtime-managed window)
- **Spherical orbit geometry confusion:** "Shift" artifact initially perceived as bug rather than expected behavior from height offset, required explanation

**What would you do differently?**
- **Pre-implementation camera mode comparison:** Side-by-side analysis of ORBIT vs FOLLOW update patterns would have caught one-frame delay issue before first implementation
- **Debug UI assumption:** For any UX/camera feature, assume debug UI will be valuable and plan for it upfront (saves refactoring time)
- **Document geometric behaviors:** Spherical orbit "shift" effect should be documented in camera system notes to prevent future confusion (reusable learning)

---

## Certainty Calibration

**Predicted Certainty:** 95% (from feature description)

**Actual Outcome:**
- [x] Completed on first attempt (no deferrals, no major rework)
- [ ] Required iteration/rework (specify what changed)
- [ ] Deferred (specify reason and missing prerequisites)

**Variance Analysis:**
Prediction was accurate overall - feature completed successfully in single session with no deferrals. However:
- **Underestimated scope flexibility:** 95% certainty assumed ~30-60 min implementation, but didn't account for organic scope additions (debug UI, GUI refactoring)
- **Bug discovery not predicted:** One-frame delay bug wasn't anticipated in certainty estimate, but fix was trivial (~5 lines, 10 min)
- **Infrastructure already proven:** High certainty justified - scroll input, zoom clamping, and spherical camera math all validated by existing ORBIT mode

**Calibration Adjustment:**
For similar features in the future:
- **Camera features:** Maintain 90-95% certainty for parameter exposure (proven infrastructure)
- **Debug UI additions:** Expect ~30-50% time overhead for GUI components (not a certainty reduction, just time estimation)
- **Simple features with scope flexibility:** High certainty (90%+) appropriate even if scope may grow organically, as long as all growth is debug/iteration tooling (zero gameplay risk)
- **Cross-mode feature parity:** When adding feature to mode B that exists in mode A, explicitly compare implementation patterns during planning to catch subtle differences (e.g., ORBIT's immediate `update_eye_position()` vs FOLLOW's deferred update)

**Pattern Recognition:**
- Third use of parameter exposure pattern (after max_speed, jump_height, etc.) - could justify 98%+ certainty for future parameter UI additions
- Debug UI ROI validated: 30-min investment will save 10+ min on each future panel (5+ panels planned → 50+ min total savings)
- Unified Debug Panel architecture: New collapsing header additions now ~5 min instead of ~20 min (separate window creation)


