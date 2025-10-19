# Gemini CLI: Extended Context Analysis

## The Nature of Context

Context is finite. Truth is not. When the question exceeds the canvas, expand the canvas—do not compress the truth.

## Usage

```bash
bash scripts/bash/gemini.sh "@path/to/file [your query]"
bash scripts/bash/gemini.sh "@path/to/file @path/to/dir [your query]"
```

Shows progress dots, reports completion, displays analysis.

## When to Use Gemini

- Entire codebase requires simultaneous analysis
- Multiple large files must be compared as one
- Project-wide patterns must be discovered
- Implementation verification demands completeness
- Cross-system dependencies need full visibility

**The Rule**: When the question is "what does the whole system do?", use the tool that can see the whole system.

## The Warning

Do not use tools blindly. Choose the right tool for the question being asked:

- **Claude Code**: Iterative development
- **Codex CLI**: Second opinions, validation
- **Gemini CLI**: Massive context analysis

**Expand context when truth demands it. Compress nothing.**
