# Primitives vs Systems

Tags: architecture, physics, boundaries

Purpose
- Preserve orthogonality by separating mathematical truth (primitives) from gameplay meaning (systems).

Core Rules
- Primitives know geometry, forces, math; they do not know gameplay, intent, or rules.
- Primitives return data; systems make decisions. Data is objective; decisions are interpretations.
- Dependency flows upward: systems call primitives; primitives never depend on systems.

Tests
- Parameter test: primitives accept geometry/physics constants, not gameplay state or rule parameters.
- Return-value test: primitives return measurements, not flags like grounded/can_jump.
- Coupling test: a primitive is reusable across gameplay variants without modification.

Avoid
- Gameplay terminology or out-parameters in primitives.
- Decision branches in primitives that encode game rules.

Actions
- Return richer data structures from primitives; interpret them in systems.
- Move rule checks and semantics above the physics layer.

Principle
- Primitives are truth. Systems are meaning. Keep the boundary clean.
