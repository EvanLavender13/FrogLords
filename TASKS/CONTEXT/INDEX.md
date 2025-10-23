# Context Index

Purpose: Fast, token‑efficient mapping of context cards to tasks.

Tag vocabulary
- Layers: foundation, primitives, systems, variation, polish
- Domains: physics, math, architecture, dataflow, hygiene, simulation, tuning, state

Cards
- `TASKS/CONTEXT/ACCUMULATED_STATE.md` — Derive instead of accumulate; unconditional updates; epsilon validation. Tags: foundation, architecture, math, state
- `TASKS/CONTEXT/DEAD_CODE_DETECTION.md` — Single owner and reader; delete unused storage/functions; remove duplicates. Tags: foundation, hygiene, architecture
- `TASKS/CONTEXT/METADATA_DRIVEN_DESIGN.md` — Metadata as interface; single source for UI, validation, testing; metadata lives with data. Tags: systems, architecture, tuning
- `TASKS/CONTEXT/PRIMITIVES_GUIDE.md` — Primitives return data; systems decide; dependency flows up. Tags: primitives, systems, architecture, physics
- `TASKS/CONTEXT/PHYSICS_INTEGRATION_PATTERNS.md` — Integrate only when necessary; semi‑implicit default; dt scaling; bounded drift. Tags: foundation, physics, math, simulation
- `TASKS/CONTEXT/PHYSICS_FIRST_TUNING.md` — Expose real physical parameters; use standard formulas; tune feel from physics. Tags: systems, variation, physics, tuning
- `TASKS/CONTEXT/STATIC_ANALYSIS.md` — Verify semantics first; suppress false positives with explanation; verify consumers exist. Tags: hygiene, tools, validation
- `TASKS/CONTEXT/UNIDIRECTIONAL_FLOW.md` — One source; one direction; delete sync cycles; expose views. Tags: systems, architecture, dataflow

Selection hints (from backlogs/stack)
- Movement/Character tasks → primitives, physics, tuning
- Camera tasks → physics, dataflow, architecture
- Animation/IK/ragdoll → primitives boundary, systems, tuning
- Parameter/GUI systems → architecture, tuning, dataflow
- Tools/Validation → hygiene, architecture, simulation
- Refinements from static analyzers → hygiene, validation

