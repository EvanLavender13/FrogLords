# Physics Integration

Tags: physics, math, simulation

Purpose
- Choose integration and state handling that preserve stability, correctness, and frame independence.

Integration vs Cache
- Integrate only when the value cannot be derived from instantaneous sources (numerical solution to differential equations).
- Caches store derivable results; they drift and require sync. Prefer derivation over caching.
- If you can derive it, derive it. If you must integrate, integrate unconditionally.

Integrator Choice
- Semi-implicit Euler as default for gameplay: stable, simple, fast.
- Explicit methods can explode energy; implicit methods are stable but costly.
- Prefer stability over high-order accuracy for damped, reset-heavy gameplay systems.

Frame-Rate Independence
- Time is a parameter. Multiply all rates by delta-time.
- Validate units on every equation; mismatched dimensions indicate errors.
- Behavior must be consistent across frame rates.

Error and Drift
- All numerical methods drift; in damped, bounded, frequently-reset systems, error remains bounded.
- Higher-order methods are unnecessary for our domain unless precision requirements explicitly demand them.

Signals of Misuse
- Conditional accumulation masquerading as physics; self-referential smoothing; running totals that duplicate derivable values; reset-heavy caches.

Default
- Semi-implicit Euler; dt-scaled rates; derive whenever possible; accumulate only as part of integration.

## The Direction of Truth

**Physics integration flows forward through time.**

Given initial conditions and forces, we integrate to discover future state. The arrow of causation is clear: past determines present, present determines future.

**Cache pretends to flow forward but actually references backward.**

A smoothed value that uses its own previous value creates circular dependency. The accumulation isn't solving an equation—it's approximating one while introducing lag and staleness.

**Integration discovers. Cache duplicates.**

Know which you're doing. Integrate when mathematics demands it. Derive when source truth exists.

**The best way to prevent accumulated state bugs is still to eliminate accumulated state—except where state is the solution itself.**
