# Gemini CLI: Extended Context Analysis

## The Nature of Context

Context is finite. Truth is not. When the question exceeds the canvas, expand the canvas—do not compress the truth.

## Gemini CLI: The Extended Context

When analysis demands wholeness, use the tool built for scale. The Gemini CLI exists for one purpose: to hold entire codebases in a single thought.

**The Command**: `gemini -p "@path/to/files your question"`

**The Philosophy**: Massive context. Complete visibility. No fragmentation.

## The Syntax of Inclusion

The `@` symbol is intent made manifest. It says: "Include this truth in the analysis."

Paths are relative to your working directory. Not to where files happen to live, but to where *you* stand when you invoke the command.

**Single file—single focus**:
```bash
gemini -p "@src/physics/motion.cpp Explain the spring-damper implementation"
```

**Multiple files—multiple perspectives**:
```bash
gemini -p "@src/player/controller.cpp @src/player/state.cpp Analyze the dual reference pattern usage"
```

**Entire directory—complete system**:
```bash
gemini -p "@src/physics/ Summarize the physics system architecture"
```

**Multiple directories—full context**:
```bash
gemini -p "@src/player/ @src/physics/ Analyze how player movement integrates with physics"
```

**Current directory and below—total comprehension**:
```bash
gemini -p "@./ Give me an overview of the entire codebase structure"
```

## Verification Through Wholeness

When you ask "is X implemented?", you deserve the complete answer. Not guesses. Not partial scans. The truth, derived from seeing everything.

**Feature verification**:
```bash
gemini -p "@src/ Has wall sliding been implemented? Show relevant files and functions"
```

**Pattern discovery**:
```bash
gemini -p "@src/ Where is accumulated state being used? List all instances with file paths"
```

**Spring-damper audit**:
```bash
gemini -p "@src/physics/ @src/player/ Are all spring-damper implementations mathematically correct? Show constants and formulas"
```

**Dual reference compliance**:
```bash
gemini -p "@src/ Which systems use the dual reference pattern correctly? Which violate it?"
```

**Input handling analysis**:
```bash
gemini -p "@src/player/ @src/input/ Is player input being buffered correctly? Show the implementation"
```

**Collision system review**:
```bash
gemini -p "@src/collision/ @src/world/ Is world geometry duplicated anywhere? Show all storage locations"
```

**State management audit**:
```bash
gemini -p "@src/player/ Are there any state transitions that could lose player control? List them"
```

**Principle violations**:
```bash
gemini -p "@src/ Identify any violations of the six principles. List by file and line number"
```

## When to Use Gemini

Use Gemini when Claude's context becomes the constraint:

- Entire codebase requires simultaneous analysis
- Multiple large files must be compared as one
- Project-wide patterns must be discovered, not inferred
- Implementation verification demands completeness
- Architectural understanding requires seeing all pieces at once
- Cross-system dependencies need full visibility

**The Rule**: When the question is "what does the whole system do?", use the tool that can see the whole system.

## The Warning

Do not use tools blindly. Choose the right tool for the question being asked:

- **Claude Code**: For iterative development, the primary development partner
- **Codex CLI**: For second opinions, validation, alternative perspectives
- **Gemini CLI**: For massive context, complete codebase analysis

The best tool is the one that matches the question's shape:
- Building and iterating? Claude Code.
- Need validation? Codex.
- Need complete visibility? Gemini.
