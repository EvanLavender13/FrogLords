# Maintenance Backlog

**Purpose:** Tracking for small improvements, refactors, and cleanup items that don't constitute full features but benefit code quality.

**Status:** Living document (updated after codebase reviews)

**Last Review:** October 13, 2025

---

## Critical

(Direct violations, bugs, correctness issues requiring immediate attention)

### Character
- **Character Controller Depends on Rendering Scene**
  - *File:* `src/character/controller.cpp:4`
  - *Severity:* Critical
  - *Description:* `controller.cpp` includes `rendering/scene.h`, pulling rendering dependencies into the character layer and breaking the one-way architecture.
  - *Principle Violated:* Layered architecture (Foundation -> Character -> Rendering -> App)
  - *Suggested Fix:* Move collision queries to a foundation-level interface or pass a narrow collision data struct from the app so the controller no longer includes rendering headers.

### Rendering
- **Debug Draw Pulls in App Game World**
  - *File:* `src/rendering/debug_draw.cpp:5`
  - *Severity:* Critical
  - *Description:* Debug rendering directly includes `app/game_world.h` to access `velocity_trail_state`, creating a dependency from rendering back into the app layer.
  - *Principle Violated:* Layered architecture (Rendering must not depend on App)
  - *Suggested Fix:* Extract `velocity_trail_state` into a shared data header or pass the needed trail data through parameters so rendering stays decoupled from app types.

---

## High

(Significant code smells impacting readability or maintainability)

*No items currently tracked.*

---

## Medium

(Minor inconsistencies or quality improvements)

### Character
- **Unused stdio Include in Controller**
  - *File:* `src/character/controller.cpp:11`
  - *Severity:* Medium
  - *Description:* `<cstdio>` remains from earlier debugging and now adds an unnecessary dependency in the character layer.
  - *Suggested Fix:* Drop the unused include.

### Input
- **Unused stdio Include in Input Module**
  - *File:* `src/input/input.cpp:4`
  - *Severity:* Medium
  - *Description:* `<cstdio>` is included but not used anywhere in `input.cpp`.
  - *Suggested Fix:* Remove the redundant include to avoid needless dependencies.

---

## Low

(Nitpicks and polish items)

### Character
- **Inconsistent Constant Naming Scope**
  - *File:* Various (e.g., `src/character/controller.cpp:15-16`)
  - *Severity:* Low
  - *Description:* Some constants live in anonymous namespaces while others are `constexpr` at function scope, creating inconsistent guidance for future touches.
  - *Suggested Fix:* Document the preferred constant placement in `CONVENTIONS.md` (file-scope anonymous namespaces vs. function-level `constexpr`) and align outliers opportunistically.

---

## Completed

(Prune periodically)
