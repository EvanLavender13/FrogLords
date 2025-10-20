# Dead Code Detection

Tags: hygiene, architecture

Purpose
- Eliminate unused storage, functions, and logic that add complexity without value.

Core Rules
- Never add storage without a named reader. One source of truth.
- Delete functions with zero call sites outside their definition.
- Remove duplicate storage; consumers read via reference from the owner.

Detection
- Storage never accessed; getters/setters unused; functions referenced only where defined.
- Dual writes of identical data across subsystems; “sync” code between duplicates.
- “Might need later” justifications; persistent TODOs to wire usage.

Prevention
- Name the consumer before adding storage or API.
- Define ownership explicitly; expose views instead of copies.
- Prefer composition over duplication; avoid “sync” pathways entirely.

Actions
- Delete dead fields, functions, and duplicate containers.
- Collapse to a single owning location; update readers to reference it.

Default
- If a reader cannot be named immediately, do not add the storage.

