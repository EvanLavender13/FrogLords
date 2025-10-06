# Decompose Iteration Plan into Implementation Steps

### 1. Review Development Principles

Read `AGENTS.md` to synthesize coding standards and architectural principles. Essential for ensuring implementation steps follow:
-   snake_case naming conventions and file organization
-   Dependency flow (Foundation → Character → Rendering → App)
-   Gameplay-first principles (interruptible transitions, physics-first)
-   Procedural animation patterns (spring-damper, cubic interpolation)

### 2. Identify Target Plan

Extract the feature name from the current git branch name (format: `iteration/<feature_name>`) and locate the corresponding iteration plan at `PLANS/iteration_<feature_name>.md`.

### 3. Analyze and Decompose

1.  **Read Iteration Plan:** Read the specified plan file
2.  **Identify Core Systems:** Determine which systems and source files are affected by the "Graybox Scope". Consult `PLANS/DEPENDENCY_STACK.md` to verify
3.  **Analyze Source Code:** Read identified source files (`.h` and `.cpp`) to understand current structure, data flow, and conventions
4.  **Generate Actionable Steps:** Decompose graybox implementation into a checklist of small, atomic, ordered tasks. Each task must be a concrete action on the codebase

-   **Task Specificity:** Each step must be a clear, imperative instruction referencing specific files, functions, and data structures (e.g., "In `character/controller.h`, add a `dash_speed` parameter to the `tuning` struct.").
-   **Logical Order:** Sequence tasks logically based on dependencies (e.g., define data structures before creating functions that use them).
-   **Include UI:** Ensure the plan includes a step to expose any new tunable parameters in the debug UI (`src/gui/character_panel.cpp`).
-   **Adhere to Conventions:** Follow `AGENTS.md` (snake_case, file organization, dependency flow; brief comments only where non‑obvious).
-   **Gameplay‑First:** Preserve input→acceleration mapping; rotate model to velocity; transitions remain interruptible. Prefer spring‑damper and cubic interpolation; use targeted IK only where necessary.

### 4. Save and Propose

Format as a markdown checklist and save to a new file in `PLANS/` directory.

-   **Filename:** `implementation_[feature_name].md`
-   Propose the document to the user for review

### 5. Quality Gates

-   Build passes (CMake: Configure/Build Debug tasks)
-   Lint/format clean (clang‑format/clang‑tidy tasks)
-   Smoke run launches and graybox switch is reachable

### Tone & Constraints

-   Concise, imperative instructions
-   Reference specific files, functions, and data structures
-   Adhere to `AGENTS.md` conventions (snake_case, dependency flow)
-   Gameplay‑first: input→acceleration; rotate to velocity; interruptible transitions
-   Prefer spring‑damper and cubic interpolation

---

### Example Implementation Plan Format

```markdown
# Implementation Plan: [Feature Name]

### Graybox Implementation Checklist

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
    - [ ] In `update_locomotion()`, add a check for the dash input.
    - [ ] If dash input is detected and character is not already dashing, set `is_dashing = true` and reset `dash_timer`.
    - [ ] While `is_dashing` is true, override velocity with `tuning.dash_speed` in the input direction.
    - [ ] Decrement `dash_timer` and set `is_dashing = false` when it reaches zero.
    - [ ] Ensure animation/pose transitions use spring‑damper interpolation and remain interruptible.

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
