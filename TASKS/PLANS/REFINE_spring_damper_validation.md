# Refinement: Spring Damper Delta Time Validation

Add precondition assertion to spring_damper::update for delta_time validation.

---

<!-- BEGIN: SELECT/SELECTED -->
## Selected

**Date:** 2025-10-26
**Complexity:** Trivial (1 pt)
**Path:** A (trivial)
<!-- END: SELECT/SELECTED -->

---

<!-- BEGIN: SELECT/VIOLATION -->
## Violation

**Location:** `src/foundation/spring_damper.cpp:5`
**Principle:** Humble Validation
**Severity:** High
**Type:** Missing precondition validation

**Current state:**
```cpp
float spring_damper::update(float current, float target, float velocity, float delta_time, float& out_velocity)
{
    // No validation of delta_time
    float damping_factor = 2.0f * std::sqrt(spring_constant);
    // ... uses delta_time directly in integration
}
```

**Why violation:**
- Zero dt causes division by zero or infinite acceleration
- Negative dt reverses time, creating physically impossible states
- NaN dt propagates through entire simulation
- Silent failure compounds into cascading errors

**Impact:**
- Blocks: Safe simulation at any framerate
- Cascades to: Vehicle tilt system, dynamic FOV system, any future spring-damped behavior
<!-- END: SELECT/VIOLATION -->

---

<!-- BEGIN: SELECT/FIX -->
## Fix

**Approach:** Document

**Why keep:**
Spring-damper is battle-tested Layer 2 primitive used throughout codebase. Adding validation maintains mathematical correctness guarantee.

**Derivation:**
Physical time delta must be positive and finite. Zero time means no change. Negative time violates causality. Infinite/NaN time has no physical meaning.

**Trade-off:**
Debug assertion adds zero release overhead while catching integration errors immediately at violation point rather than downstream.
<!-- END: SELECT/FIX -->
