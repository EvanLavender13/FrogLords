# FrogLords Development Memory

## Core Philosophy

@PRINCIPLES.md

The principles are not suggestions—they are law. Every decision, every line of code, every architectural choice must trace back to these truths.

## On Context Windows and Tool Selection

### The Nature of Context

Context is finite. Truth is not. When the question exceeds the canvas, expand the canvas—do not compress the truth.

### Gemini CLI: The Extended Context

When analysis demands wholeness, use the tool built for scale. The Gemini CLI exists for one purpose: to hold entire codebases in a single thought.

**The Command**: `gemini -p`

**The Philosophy**: Massive context. Complete visibility. No fragmentation.

### The Syntax of Inclusion

The `@` symbol is intent made manifest. It says: "Include this truth in the analysis."

Paths are relative to your working directory. Not to where files happen to live, but to where *you* stand when you invoke the command.

**Single file—single focus**:
```bash
gemini -p "@src/main.py Explain this file's purpose and structure"
```

**Multiple files—multiple perspectives**:
```bash
gemini -p "@package.json @src/index.js Analyze the dependencies used in the code"
```

**Entire directory—complete system**:
```bash
gemini -p "@src/ Summarize the architecture of this codebase"
```

**Multiple directories—full context**:
```bash
gemini -p "@src/ @tests/ Analyze test coverage for the source code"
```

**Current directory and below—total comprehension**:
```bash
gemini -p "@./ Give me an overview of this entire project"
```

**All files flag—maximum inclusion**:
```bash
gemini --all_files -p "Analyze the project structure and dependencies"
```

### Verification Through Wholeness

When you ask "is X implemented?", you deserve the complete answer. Not guesses. Not partial scans. The truth, derived from seeing everything.

**Feature verification**:
```bash
gemini -p "@src/ @lib/ Has dark mode been implemented? Show relevant files and functions"
```

**Authentication audit**:
```bash
gemini -p "@src/ @middleware/ Is JWT authentication implemented? List all auth endpoints and middleware"
```

**Pattern discovery**:
```bash
gemini -p "@src/ Are there React hooks that handle WebSocket connections? List them with file paths"
```

**Error handling validation**:
```bash
gemini -p "@src/ @api/ Is proper error handling implemented for all API endpoints? Show try-catch examples"
```

**Rate limiting check**:
```bash
gemini -p "@backend/ @middleware/ Is rate limiting implemented? Show implementation details"
```

**Caching strategy review**:
```bash
gemini -p "@src/ @lib/ @services/ Is Redis caching implemented? List all cache-related functions"
```

**Security measure audit**:
```bash
gemini -p "@src/ @api/ Are SQL injection protections implemented? Show input sanitization"
```

**Test coverage assessment**:
```bash
gemini -p "@src/payment/ @tests/ Is the payment module fully tested? List all test cases"
```

### When to Use Gemini

Use Gemini when Claude's context becomes the constraint:

- Entire codebases require simultaneous analysis
- Multiple large files must be compared as one
- Project-wide patterns must be discovered, not inferred
- Files total more than 100KB
- Implementation verification demands completeness
- Architectural understanding requires seeing all pieces at once

**The Rule**: When the question is "what does the whole system do?", use the tool that can see the whole system.

### The Mechanics

- Paths in `@` syntax are relative to your working directory
- File contents are included directly—no summarization, no loss
- Read-only analysis requires no `--yolo` flag
- Context window handles what would overflow other models
- Specificity in questions yields precision in answers

### The Warning

Do not use tools blindly. Choose the right tool for the question being asked:

- **Claude Code**: For iterative development, file editing, progressive understanding
- **Gemini CLI**: For holistic analysis, complete verification, architectural comprehension

The best tool is the one that matches the shape of the problem.