# Accumulated State

Tags: state, math, architecture

Purpose
- Prevent divergence from truth caused by cross-frame accumulation.

Core Rules
- Derive when possible; do not accumulate derivable values.
- If accumulation is required, update unconditionally every frame.
- Validate before use: guard normalization, division, and inversion with epsilon checks.
- Prefer timestamps and derivation over running totals.

Why
- Conditional updates create staleness; floating-point error compounds; divisions by near-zero produce instability.

Apply
- Compute state from authoritative inputs each frame.
- Keep targets separate from filtered values (dual-reference pattern).
- Treat accumulated values as suspect in control flow and denominators.

Avoid
- Conditional accumulation or reset-driven correction.
- Self-referential smoothing that feeds on its own output.

Signals
- “Sometimes updates,” “needs reset,” “added epsilon to stop NaN,” or duplicated truth stored for convenience.

Default
- Derived truth over accumulated cache. When forced to accumulate, make it unconditional and validated.
