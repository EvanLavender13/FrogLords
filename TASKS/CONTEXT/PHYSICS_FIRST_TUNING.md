# Physics-First Tuning

Tags: physics, tuning, parameters

Purpose
- Expose real physical parameters and derive feel from them.

Core Rules
- Physics drives feel; feel does not drive physics.
- Parameters must map to real quantities with valid ranges and standard formulas.
- Do not inflate or compensate values to fit desired outcomes.

Method
- Identify the physical system (friction, spring-damper, drag, gravity).
- Use standard equations; do not invent formulas.
- Expose tunables with physical meaning and bounds.
- Tune by adjusting physical parameters; let behavior emerge.

Avoid
- Ratios without reference, magic constants, and back-solved parameters.
- Self-compensating math that cancels itself elsewhere.

Trade-offs
- Non-physical mechanics are allowed only when serving the Prime Directive (player intent). State the trade-off explicitly.

Default
- μ in [0,1]; dt-scaled rates; critical damping framed via damping_ratio; semi-implicit integration; remove invented knobs.
## The Test

Ask: "Can I explain this parameter to a physicist?"

**Good:**
- "Friction coefficient between rubber and asphalt: 0.7"
- "Spring constant for camera lag: 200 N/m"
- "Damping ratio: 0.7 (slightly underdamped)"

**Bad:**
- "Acceleration ratio: 0.25" (ratio of what to what?)
- "Friction loss factor: 0.75" (how does this relate to μ?)
- "Magic damping: 0.83" (why this value?)

**If you can't explain it in physical terms, you're tuning the wrong parameter.**

## The Rule

**Start physical. Stay physical. Tune within physical bounds.**

When gameplay demands deviation from physics:
1. Document the principle trade-off
2. Use parameters with physical units/meaning
3. Explain why physics was insufficient

**Physics is the foundation. Game feel is built on top, not instead of.**

---

**See also:** @TASKS/CONTEXT/PHYSICS_INTEGRATION_PATTERNS.md - How to integrate physics over time
