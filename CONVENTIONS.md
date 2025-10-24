# FrogLords: Coding Conventions

Standards that enforce principles.

---

## Naming

**snake_case for everything:**
- Types
- Functions
- Variables
- Files

**Exceptions (UPPER_CASE):**
- Enum constants
- Constexpr constants

(enforced by `.clang-tidy`)

---

## Coordinate System

**World Space:**
- Up: +Y
- Forward: +Z
- Right: **+X**

This is right-handed and matches standard OpenGL convention (+X right, +Y up, +Z forward).

**Authority:** `math::yaw_to_forward()` and `math::yaw_to_right()` in `foundation/math_utils.h` define the coordinate system through their implementation. Code is truth.

**Cross Products:**
- `cross(UP, forward)` produces the right vector (+X direction)
- `cross(forward, UP)` produces the left vector (-X direction)

---

## Testing

### Unit Tests: Stable Layer 1 Primitives

Write unit tests for stable, battle-tested foundation primitives that:
- Are pure mathematical functions with complex behavioral properties
- Won't change once validated
- Require trajectory or multi-step analysis to verify correctness

Reference: `tests/foundation/test_spring_damper.cpp`

### Debug Assertions: Active Development

All code under active development validates contracts with `foundation/debug_assert.h`.

Assert preconditions, postconditions, and invariants. Continuous validation every frame. Zero release overhead. Immediate failure at violation point.

Assertion failures are show-stoppers.

### Manual Testing: Systems and Integration

High-level systems validated through gameplay and observation.

---

## Code Style

**Formatting:** Use `.clang-format` via `scripts/bash/format.sh`.

**Comments explain why, not what.** Document contracts and non-obvious behavior.

**Consistency:** Before adding code, read the file to identify existing patterns. Match existing patterns unless insufficient.

---

## Documentation Hygiene

- Directness Over Demonstration (see `PRINCIPLES.md`): Do not include examples in principles or plans. Examples live in code and tests; include in responses only when explicitly requested.
- Lists Are Intent, Not History (see `PRINCIPLES.md`): Backlogs list only what remains to be done now. Do not add “Recently Completed” or “Completed Items” sections; remove items when finished.

These are enforced in reviews and may be checked by simple doc lints (flag phrases like “For example”, “Recently Completed”).

---

Principles override conventions.
