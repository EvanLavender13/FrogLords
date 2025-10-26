# FrogLords Development Memory

## Core Philosophy

@PRINCIPLES.md

The principles are not suggestions—they are law. Every decision, every line of code, every architectural choice must trace back to these truths.

## Communication Protocol

**Critical feedback over comfort.** Skip the affirmations and "You're absolutely right" platitudes. If an idea is flawed, say so. If context is missing, demand it. If I've made a mistake, mock it appropriately—the bug won't care about my feelings.

Questions beat assumptions. When precision matters, interrogate until clarity emerges. Vague requests get clarifying questions, not best guesses.

## Physics-First Architecture

**Critical:** This is a physics-first application with force → acceleration → velocity → position integration.

- **Never bypass physics integration** with direct position setting
- **All movement through forces** - no teleporting, no position calculation
- **Time-independent** - identical results at any framerate
- **Research must adapt to our architecture** - not the other way around

See @NOTES/CarMechanics/PhysicsFirst_Research.md for compatible arcade techniques.

## Apply Principles to EVERYTHING

**You are not exempt from the principles.**

Every task, every document, every plan, every response must follow:
- **Radical Simplicity** - Your documentation, your explanations, your plans
- **Consistency** - Your process, your structure, your output
- **Principled Development** - Your decisions must trace back to principles

Delete until only insight remains. State facts once. Stop when done.

The principles judge your work the same way they judge the code.

## Coding Conventions

@CONVENTIONS.md

Standards that enforce principles in code. snake_case everywhere. Debug assertions in foundations. Comments explain why.

## Active Work

The current refinement or system build in progress:

@TASKS/CURRENT_PLAN.md

## Task Context

@TASKS/CONTEXT/INDEX.md

Maps context cards to task types. Use tags to find relevant architectural patterns before starting work.

### High-Value Injected Context

@TASKS/CONTEXT/ROTATION_CONVENTIONS.md
@TASKS/CONTEXT/REFERENCE_FRAMES.md

Critical patterns that prevent recurring confusion. Review before any rotation or transform work.

## Analysis and Validation

Verification is cheap. Mistakes compound. Use these tools liberally.

@TOOLS/GEMINI.md
@TOOLS/CODEX.md

When context limits visibility, expand it with Gemini. When decisions need validation, consult Codex. Don't wait for problems—seek verification before they emerge.

## Development Scripts

@TOOLS/SCRIPTS.md
