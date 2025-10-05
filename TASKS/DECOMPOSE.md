# Decompose Iteration Plan into Implementation Steps

### 1. Identify Target Plan

Receive the file path for an iteration plan located in the `PLANS/` directory from the user.

### 2. Analyze and Decompose

1.  **Read the Iteration Plan:** Ingest the specified plan file.
2.  **Identify Core Systems:** Determine which existing systems and source files are affected by the "Graybox Scope" defined in the plan. Consult `PLANS/dependency_stack.md` to verify.
3.  **Analyze Source Code:** Read the contents of the identified source files (`.h` and `.cpp`) to understand their current structure, data flow, and conventions.
4.  **Generate Actionable Steps:** Decompose the graybox implementation into a checklist of small, atomic, and ordered tasks. Each task must be a concrete action that can be performed on the codebase.

-   **Task Specificity:** Each step must be a clear, imperative instruction referencing specific files, functions, and data structures (e.g., "In `character/controller.h`, add a `dash_speed` parameter to the `tuning` struct.").
-   **Logical Order:** Sequence tasks logically based on dependencies (e.g., define data structures before creating functions that use them).
-   **Include UI:** Ensure the plan includes a step to expose any new tunable parameters in the debug UI (`src/gui/character_panel.cpp`).
-   **Adhere to Conventions:** All proposed changes must strictly follow the project's existing code standards (snake_case, file structure, etc.) as defined in `AGENTS.md`.

### 3. Format and Save

Format the output as a markdown checklist and save it to a new file in the `TASKS/` directory.

-   **Filename:** `implementation_[feature_name].md`.
-   Propose the generated implementation plan to the user for review.

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

#### 4. Debug UI

- [ ] **File:** `src/gui/character_panel.cpp`
    - [ ] In the "Locomotion" section, add a new `ImGui::InputFloat` for "Dash Speed".
    - [ ] Add a new `ImGui::InputFloat` for "Dash Duration".
    - [ ] Add a read-only `ImGui::Text` to display the current value of `is_dashing`.
```
