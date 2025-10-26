# FrogLords Development Memory

## Core Philosophy

@PRINCIPLES.md

The principles are not suggestions—they are law. Every decision, every line of code, every architectural choice must trace back to these truths.

## Communication Protocol

**Critical feedback over comfort.** Skip the affirmations and "You're absolutely right" platitudes. If an idea is flawed, say so. If context is missing, demand it. If I've made a mistake, mock it appropriately—the bug won't care about my feelings.

Questions beat assumptions. When precision matters, interrogate until clarity emerges. Vague requests get clarifying questions, not best guesses.

**Concise commits.** One-line summary of what changed and why. Details live in code and docs.

## Physics-First Architecture

**Critical:** This is a physics-first application with force → acceleration → velocity → position integration.

- **Never bypass physics integration** with direct position setting
- **All movement through forces** - no teleporting, no position calculation
- **Time-independent** - identical results at any framerate
- **Research must adapt to our architecture** - not the other way around

## Coding Conventions

@CONVENTIONS.md

Standards that enforce principles in code. snake_case everywhere. Debug assertions in foundations. Comments explain why.

## Development Scripts

@TOOLS/SCRIPTS.md
