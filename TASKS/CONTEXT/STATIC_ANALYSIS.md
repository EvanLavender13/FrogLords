# Static Analysis: False Positives

Tags: hygiene, tools, validation

Purpose
- Distinguish semantic correctness from tool pattern-matching. Suppress when tools cannot understand requirements.

Core Rules
- Verify semantic correctness first; suppress only when code is right and tool is wrong.
- Write-only fields require consumer verification (grep for reads); delete if no consumer exists.
- Temporal dependencies (pre/post mutation state) justify wider scope; suppress with explanation.
- Partial initialization is inconsistency; complete pattern consistently across all members.

Suppress Pattern
- `// cppcheck-suppress <warning> - <semantic requirement tool cannot see>`
- `// NOLINTNEXTLINE(<check>) - <why correct>`
- Enable: `cppcheck --inline-suppr`, clang-tidy NOLINT comments

Decision Tree
1. Semantically correct? No → fix code. Yes → continue.
2. Consumer exists? Write-only → verify reads. None → delete.
3. Scope minimal given mutation? Pre-capture → suppress. Can move → move.
4. Pattern consistent? Partial init → complete. Drift → align.

Common False Positives
- `variableScope`: Variable captures pre-mutation state before function modifies by reference.
- `unusedStructMember`: Field consumed by reactive system (debug viz, animation) not visible to static flow.
- `uninitMemberVar`: C-style handle init pattern (sokol `{0}`) applied inconsistently.

Avoid
- Blind compliance with tool suggestions without semantic verification.
- Suppressing real violations because pattern seems acceptable.
- Deleting reactive-consumed state without grepping for consumers.

Principle
- Tools enforce patterns. Humans understand semantics. Suppress with explanation when correct.
