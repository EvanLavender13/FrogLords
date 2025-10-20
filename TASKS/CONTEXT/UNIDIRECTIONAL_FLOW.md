# Unidirectional Flow

Tags: architecture, dataflow

Purpose
- Ensure a single source of truth with one-way transformation and no sync cycles.

Core Rules
- One owner of authoritative state; others derive or reference.
- One transformation direction; no reverse reads or “sync” steps.
- If two places can write, one is wrong. Delete the duplicate.

Pattern
- Source of Truth → Transformation → Derived State → Usage.
- No backflow from derived to source.

Detection
- Presence of both apply_to() and read_from(); duplicate containers; dirty/sync flags; create/destroy cycles for persistent resources.

Decide Ownership
- Separate intent (design-time tuning) from implementation (runtime state).
- Transform intent → implementation only; never the reverse.

Actions
- Collapse dual storage; expose references/views from the owner.
- Delete reverse transforms and sync pathways.

Default
- One source. One direction. No cycles.
- If you need sync, you have two sources (bad)

**Unidirectional flow is not just cleaner—it's simpler.**

One function. One direction. One truth.
