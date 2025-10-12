# Maintenance Backlog

**Purpose:** Tracking for small improvements, refactors, and cleanup items that don't constitute full features but benefit code quality.

**Status:** Living document (updated after codebase reviews)

**Last Review:** October 12, 2025

---

## Critical

(Direct violations, bugs, correctness issues requiring immediate attention)

*No items currently tracked.*

---

## High

(Significant code smells impacting readability or maintainability)

*No items currently tracked.*

---

## Medium

(Minor inconsistencies or quality improvements)

*No items currently tracked.*

---

## Low

(Nitpicks and polish items)

### Inconsistent Constant Naming Scope
- **File:** Various (e.g., `src/character/controller.cpp:15-16`)
- **Severity:** Low
- **Description:** Some constants are in anonymous namespaces, others are `constexpr` at function scope. While both are valid, inconsistency exists.
- **Suggested Fix:** Document convention in `CONVENTIONS.md`: file-scope constants → anonymous namespace; function-local constants → `constexpr` at function scope. Align outliers opportunistically (on-touch).

