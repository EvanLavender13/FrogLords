# Metadata-Driven Design

Tags: architecture, tuning, interface, simplicity

Purpose
- Metadata as contract between domain logic and presentation eliminates duplication and enables generic tooling.

Core Rules
- Metadata lives with data, not presentation code.
- Display logic derives from metadata; never duplicates it.
- One metadata definition enables: UI widgets, serialization, validation, testing.
- Metadata describes structure; widgets interpret meaning.

Pattern
- Define param_meta alongside runtime fields (tuning_params, camera_follow).
- Metadata contains: name, units, min, max (semantic boundaries).
- Runtime contains: current value (mutable state).
- GUI reads metadata, generates widgets, emits commands.
- Validation uses metadata ranges as single source of truth.

Enables
- Generic parameter inspector: iterate metadata, auto-generate UI.
- Parameter presets: serialize/deserialize using metadata names as keys.
- Automated testing: metadata defines valid ranges, test harness explores them.
- Cross-system consistency: same metadata, same visual treatment.

Avoid
- Hardcoded labels, units, or ranges when metadata exists.
- Metadata in GUI layer creating domain→presentation dependency inversion.
- Unused metadata fields (ceremony without purpose).
- Duplicate validation logic (metadata ranges vs hardcoded bounds).

Tests
- Can new parameter get professional UI with 4 lines of metadata?
- Is display label derived from metadata.name, not passed explicitly?
- Does validation reference metadata.min/max, not magic constants?
- Can metadata support future tools (inspector, testing) without change?

Signals
- "I'll just hardcode this label" - metadata should provide it.
- "These ranges need to stay in sync" - single source missing.
- "Adding a parameter requires touching 5 files" - metadata not reused.

Composition
- Related metadata can inform each other (distance derives from min/max).
- Metadata-about-metadata: derived ranges, constrained relationships.
- Keep composition explicit; avoid hidden coupling.

Principle
- Metadata is interface. Define once, consume everywhere. When presentation duplicates domain knowledge, Single Source of Truth dies.

---

**See also:** @TASKS/CONTEXT/UNIDIRECTIONAL_FLOW.md - How metadata flows through command pattern
