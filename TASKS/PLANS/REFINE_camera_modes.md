# Refinement: Camera Modes Special Casing

**Separate camera modes into orthogonal, composable components**

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-18
**Complexity:** Standard (5 pts)
**Path:** B (standard)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/camera/camera.cpp:50-58`
**Principle:** Composable Functions | Score: 6/10
**Severity:** Medium
**Type:** Special case, Overlapping responsibilities

**Current state:**
```cpp
void Camera::zoom(float delta) {
    if (mode_ == CameraMode::FOLLOW) {
        follow_distance = std::clamp(follow_distance + delta, min_distance, max_distance);
    } else if (mode_ == CameraMode::ORBIT) {
        distance = std::clamp(distance + delta, min_distance, max_distance);
    }
}
```

**Why violation:**
- Special casing on mode enum instead of composable behavior
- Two separate state variables (`follow_distance` vs `distance`) for same concept
- Camera has overlapping responsibilities: view/projection AND mode-specific behavior
- Not orthogonal - can't cleanly combine or extend modes

**Impact:**
- Blocks: Clean camera mode extension, camera system testability
- Cascades to: Any new camera modes or behaviors
- Foundation: Layer 1 (Core) at 97%
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Simplify

**High-level strategy:**

Separate concerns into orthogonal components:
- **Base Camera**: Pure view/projection matrix generation
- **Camera Controller Component**: Updates target position/orientation

**FROM:**
```
Camera {
    mode enum
    follow_distance state
    distance state
    zoom() with if/else branching
    update() with mode checks
}
```

**TO:**
```
Camera {
    position
    target
    generate_view_matrix()
    generate_projection_matrix()
}

CameraFollower : ICameraController {
    distance
    update(target_entity) -> (position, target)
}

CameraOrbiter : ICameraController {
    distance
    angle
    update(pivot_point) -> (position, target)
}
```

**Conceptual approach:**

1. **Extract interface**: Define `ICameraController` with pure function: `update() -> (position, target)`

2. **Implement components**:
   - `CameraFollower`: Maintains distance behind target, derives position from target entity
   - `CameraOrbiter`: Maintains distance from pivot at angle, derives position from input

3. **Simplify Camera**: Remove mode enum, remove mode-specific state, accept position/target from controller

4. **Compose at runtime**: Runtime/GameWorld owns camera + controller, updates controller then camera

**Derive position, not store mode-specific state. Compose behaviors, not branch on types.**

**Key principle:**
- Camera generates matrices from position/target (pure math)
- Controller generates position/target from game state (pure derivation)
- No shared state, no mode checks, no special cases

**Tests to update:**
- Camera tests: Focus on matrix generation only
- Controller tests: Focus on position/target derivation
- Integration test: Verify composition works

<!-- END: SELECT/FIX -->

---

<!-- BEGIN: SELECT/SUCCESS -->
## Success

- [ ] Violation resolved (no mode enum, no if/else)
- [ ] Principle score improved (Composable Functions 6→9)
- [ ] Tests passing (camera + controller + integration)
- [ ] No regressions (existing camera behavior preserved)

**Metrics:**
- Before: LOC ~150, Composable Functions 6/10, Cyclomatic complexity 8
- After: LOC ~180 (+30), Composable Functions 9/10 (+3), Cyclomatic complexity 2 (-6)

**Impact:**
- Layer 1: 97% → 98% (+1%)
- Foundation: 97%+ → 98%+ (+1%)
<!-- END: SELECT/SUCCESS -->
