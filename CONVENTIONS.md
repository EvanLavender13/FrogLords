# FrogLords: Coding Conventions

Standards that enforce principles.

---

## Naming

**snake_case for everything:**
- Types
- Functions
- Variables
- Files

**Exception: Enum constants are UPPER_CASE** (enforced by `.clang-tidy`)

---

## Testing

### Foundations/Primitives: Debug Assertions

Foundation and primitive code validates contracts with `foundation/debug_assert.h`.

Assert preconditions, postconditions, and invariants:

```cpp
void camera_follow::zoom(float delta) {
    FL_PRECONDITION(std::isfinite(delta), "zoom delta must be finite");
    distance = std::clamp(distance + delta, min_distance, max_distance);
    FL_POSTCONDITION(distance >= min_distance && distance <= max_distance,
                     "distance must be clamped to limits");
}
```

**Why:** Continuous validation every frame. Zero release overhead. Immediate failure at violation point.

Assertion failures are show-stoppers.

### Systems/Integration: Manual Testing

High-level systems validated through gameplay and observation.

---

## Code Style

**Formatting:** Use `.clang-format` via `scripts/bash/format.sh`.

**Comments explain why, not what.** Document contracts and non-obvious behavior.

---

Principles override conventions.
