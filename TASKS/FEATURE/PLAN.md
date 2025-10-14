# Decompose Feature Description into Implementation Steps

### 1. Review Development Principles

Read `AGENTS.md` to synthesize coding standards and architectural principles. Essential for ensuring implementation steps follow:
-   snake_case naming conventions and file organization
-   Dependency flow (Foundation → Character → Rendering → App)
-   Gameplay-first principles (interruptible transitions, physics-first)
-   Procedural animation patterns (spring-damper, cubic interpolation)

### 2. Identify Target Documents

Extract the feature name from the current git branch name (format: `feature/<feature_name>`) and locate:
-   Feature description: `PLANS/<feature_name>_FEATURE.md` (high-level goals, constraints, design philosophy, implementation approach)

### 3. Foundation Architecture Review (CRITICAL)

**This is the first detailed system audit.** The feature description identified *what* to build; now identify *exactly where* each piece goes and what needs to move.

**Before decomposing, perform a thorough architectural audit to identify existing system states and prevent duplicate/conflicting implementations:**

1.  **Identify All Systems Involved:**
    -   Read the feature description's "Graybox Scope" to list all systems the feature touches
    -   Check `PLANS/DEPENDENCY_STACK.md` to verify system placement and dependencies

2.  **Audit Current System Ownership:**
    -   For each identified system, search the codebase to find ALL current instances:
        -   Where is the system instantiated? (game_world, controller, etc.)
        -   Where is the system updated?
        -   Where is the system referenced for rendering/debug/GUI?
    -   Search for all references to key system types (e.g., `locomotion_system`, `skeleton`)

3.  **Document Current State:**
    -   List all files that currently own or reference each system
    -   Note any duplicate instances (e.g., `game_world.locomotion` AND `controller.locomotion`)
    -   Flag architectural conflicts BEFORE creating implementation steps

4.  **Plan Migration/Consolidation:**
    -   If systems need to move ownership (e.g., locomotion from game_world to controller):
        -   Include explicit steps to remove old instances
        -   Include steps to update all references (rendering, GUI, debug draw)
        -   List all files that need changes
    -   If new systems are being added, verify they don't conflict with existing ones

5.  **Multi-Mode Feature Comparison (If Applicable):**
    -   Does this feature span multiple modes? (e.g., camera ORBIT/FOLLOW, walk/run, manual/automatic)
    -   If yes, identify existing implementations in each mode:
        -   Compare update patterns (immediate vs deferred, when transforms propagate)
        -   Compare state management (separate variables vs shared)
        -   Compare parameter handling (mode-specific ranges vs unified)
    -   **Document differences BEFORE implementation** to prevent mode-specific bugs
    -   Example: Camera zoom discovered one-frame delay because ORBIT calls `update_eye_position()` immediately but FOLLOW used deferred update

    **Purpose:** Prevents mode-specific bugs by identifying pattern differences during planning, not testing (Retro 2: camera zoom one-frame delay caught late)

**Example Foundation Audit:**
```
Feature: Primary Skeletal Animation
Systems Involved: skeleton, locomotion, animation_state

Current State Audit:
- skeleton: 
  - game_world.t_pose_skeleton (separate T-pose for display)
  - controller.skeleton (MISSING - needs to be added)
  - Rendered from: runtime.cpp draws game_world.t_pose_skeleton
  
- locomotion:
  - game_world.locomotion (initialized in game_world::init, updated in game_world::update)
  - controller.locomotion (MISSING - needs to be added)
  - Referenced by: debug_draw, character_panel (all pass game_world.locomotion)

Architecture Issues Found:
1. locomotion exists in game_world but needs to be in controller
2. Multiple skeletons will exist - need to consolidate
3. All rendering/debug references point to game_world instances

Required Migrations:
- Remove game_world.locomotion, move to controller.locomotion
- Remove game_world.t_pose_skeleton
- Update runtime.cpp to draw controller.skeleton
- Update all debug_draw calls to use controller.locomotion
- Update character_panel to receive controller.locomotion
```

### 4. Analyze and Decompose

1.  **Read Feature Description:** Read `PLANS/<feature_name>_FEATURE.md` to understand high-level goals, design constraints, graybox scope, and implementation approach
2.  **Analyze Source Code:** Read identified source files (`.h` and `.cpp`) to understand current structure, data flow, and conventions
3.  **Generate Actionable Steps:** Decompose graybox implementation into a checklist of small, atomic, ordered tasks. Each task must be a concrete action on the codebase

-   **Task Specificity:** Each step must be a clear, imperative instruction referencing specific files, functions, and data structures (e.g., "In `character/controller.h`, add a `dash_speed` parameter to the `tuning` struct.").
-   **Include Migration Steps:** If foundation audit found duplicates/conflicts, include explicit removal and reference-update steps
-   **Natural Language First:** Use natural language descriptions and pseudocode to describe logic and algorithms. Avoid writing actual source code in the plan to prevent duplicating implementation effort.
-   **Logical Order:** Sequence tasks logically based on dependencies (e.g., define data structures before creating functions that use them).
-   **Include UI:** Ensure the plan includes a step to expose any new tunable parameters in the debug UI (`src/gui/character_panel.cpp`).
-   **Adhere to Conventions:** Follow `AGENTS.md` (snake_case, file organization, dependency flow; brief comments only where non‑obvious).
-   **Gameplay‑First:** Preserve input→acceleration mapping; rotate model to velocity; transitions remain interruptible. Prefer spring‑damper and cubic interpolation; use targeted IK only where necessary.

4.  **Planning Checks (Feature-Specific):**
    -   **Gate Mechanism Review:** Prefer geometric deltas (position/angle/phase differences) over heuristic thresholds and hysteresis for reactive/debug gating. Document rationale if a threshold is chosen.
    -   **UI Ordering Verified (if applicable):** When UI lists reflect semantic cycles (e.g., gait sequences), explicitly verify item order and ensure enum ordering and UI dropdown order match.

### 5. Save and Propose

Format as a markdown checklist and save to `PLANS/` directory.

-   **Filename:** `<feature_name>_PLAN.md`
-   **If the implementation plan already exists:**
    -   Add a changelog entry at the bottom noting the update date and reason (e.g., "Plan updated after scope modification via REVISE")
    -   Update the checklist to reflect current implementation steps
    -   Preserve completed checkboxes and historical context where relevant
-   Propose the document to the user for review

### 6. Complexity Estimate (NEW)

Add a one-line complexity estimate (points) to calibrate scope, mirroring refactor scale:

-   **Complexity:** [1–2 simple | 3–5 medium | 6–8 complex]. If >8, split or defer.

Include this field near the top of the plan (see example template below).

### 7. Known Polish Gaps

Document expected rough edges that should be deferred rather than expanded into scope mid-validation:

-   List visual/behavioral imperfections that are acceptable for graybox
-   Tag as "defer to backlog" rather than "discover during validation"
-   Prevents scope creep when testing reveals predictable polish needs

**Example:**
```markdown
**Known Polish Gaps (defer to backlog):**
- Secondary motion may cause joint hyperextension during rapid transitions
- T-pose blending may look stiff at high speeds (can add AIR_NEUTRAL keyframe later if needed)
- Transition thresholds need tuning based on feel (acceptable with debug sliders)
```

### 8. Quality Gates

-   Build passes (CMake: Configure/Build Debug tasks)
-   Lint/format clean (clang‑format/clang‑tidy tasks)
-   Smoke run launches and graybox switch is reachable

### Tone & Constraints

-   Concise, imperative instructions
-   Prefer natural language and pseudocode over actual source code
-   Reference specific files, functions, and data structures
-   Adhere to `AGENTS.md` conventions (snake_case, dependency flow)
-   Gameplay‑first: input→acceleration; rotate to velocity; interruptible transitions
-   Prefer spring‑damper and cubic interpolation

---

### Example Implementation Plan Format

```markdown
# Implementation Plan: [Feature Name]

**Complexity:** [X points] (feature)

### Graybox Implementation Checklist

#### 0. Planning Checks

- [ ] Gate Mechanism Review documented (geometric delta preferred OR rationale provided)
- [ ] UI Ordering Verified where cycles/lists apply (enum order == UI order)
- [ ] Parameter Cleanup Review: Check for unused parameters introduced by conditional removals or logic changes (e.g., removing `if (is_grounded)` gates may leave `is_grounded` parameter unused)

#### 1. Data Structures & State

- [ ] **File:** `src/character/character_state.h`
    - [ ] Add `bool is_dashing` flag to the `character_state` struct.
    - [ ] Add `float dash_timer` to the `character_state` struct.

#### 2. Tuning Parameters

- [ ] **File:** `src/character/tuning.h`
    - [ ] Add `float dash_duration = 0.2f;` to the `locomotion_tuning` struct.
    - [ ] Add `float dash_speed = 50.0f;` to the `locomotion_tuning` struct.

#### 3. Core Logic

- [ ] **File:** `src/character/locomotion.cpp`
    - [ ] In `update_locomotion()`, check for dash input
    - [ ] If dash input detected and not already dashing: set `is_dashing = true`, reset `dash_timer`
    - [ ] While `is_dashing == true`: override velocity with `tuning.dash_speed` in input direction
    - [ ] Decrement `dash_timer` by delta time; set `is_dashing = false` when timer reaches zero
    - [ ] Ensure animation/pose transitions use spring‑damper interpolation and remain interruptible

**Pseudocode:**
```
if dash_input_pressed and not is_dashing:
    is_dashing = true
    dash_timer = tuning.dash_duration

if is_dashing:
    velocity = input_direction * tuning.dash_speed
    dash_timer -= delta_time
    if dash_timer <= 0:
        is_dashing = false
```

#### 4. Debug UI

- [ ] **File:** `src/gui/character_panel.cpp`
    - [ ] In the "Locomotion" section, add a new `ImGui::InputFloat` for "Dash Speed".
    - [ ] Add a new `ImGui::InputFloat` for "Dash Duration".
    - [ ] Add a read-only `ImGui::Text` to display the current value of `is_dashing`.

#### 5. Quality Gates

- [ ] Build passes (`CMake: Build (Debug)`).
- [ ] Lint/format clean (run clang-format/clang-tidy tasks).
- [ ] Smoke run compiles and launches (`Run (Debug)`), and feature toggle works.
```
