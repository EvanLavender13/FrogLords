# Codex CLI: The Second Opinion

## The Nature of Certainty

Certainty comes not from a single perspective but from convergence. When the path forward is unclear, seek another mind. When the solution seems complete, invite scrutiny.

## Codex CLI: The Alternate Perspective

When complexity demands validation, use the tool built for rapid analysis. The Codex CLI exists for one purpose: to provide a second opinion without the overhead of a new session.

**The Command**: `echo "your question" | codex e 2>/dev/null`

**The Philosophy**: Quick consultation. Independent analysis. Fresh perspective.

## The Syntax

**IMPORTANT**: Codex reads from stdin. You must pipe the prompt in using `echo`.

The `@` symbol includes file or directory contents in the analysis, just like Gemini.

**Single file:**
```bash
echo "@src/physics/motion.cpp Explain the spring-damper implementation" | codex e 2>/dev/null
```

**Multiple files:**
```bash
echo "@src/player/controller.cpp @src/player/state.cpp Analyze the dual reference pattern usage" | codex e 2>/dev/null
```

**Entire directory:**
```bash
echo "@src/physics/ Summarize the physics system architecture" | codex e 2>/dev/null
```

**Multiple directories:**
```bash
echo "@src/player/ @src/physics/ Analyze how player movement integrates with physics" | codex e 2>/dev/null
```

**Current directory and below:**
```bash
echo "@./ Give me an overview of the entire codebase structure" | codex e 2>/dev/null
```

**Simple question without files:**
```bash
echo "Does this implementation violate any SOLID principles?" | codex e 2>/dev/null
```

**Why echo?** Codex reads from stdin. Always pipe the prompt.

**Why 2>/dev/null?** Suppress verbose stderr (thinking, file reads, execution traces). You want the answer, not the process.

**Output Behavior**:
- **stdout**: Clean, concise findings (the actual answer)
- **stderr**: Verbose activity stream (suppressed by `2>/dev/null`)

**The Rule**: Always suppress stderr. If you need debugging, remove `2>/dev/null` temporarily.

## The Syntax of Consultation

**Quick Code Review**:
```bash
echo "@src/controller.cpp Review for potential race conditions" | codex e 2>/dev/null
```

**Architecture Validation**:
```bash
echo "Does the current input buffering approach align with the dual reference pattern?" | codex e 2>/dev/null
```

**Pattern Recognition**:
```bash
echo "@src/player/ Identify any accumulated state anti-patterns" | codex e 2>/dev/null
```

**Mathematical Verification**:
```bash
echo "@src/physics/motion.cpp Verify the spring-damper constants are physically correct" | codex e 2>/dev/null
```

**Principle Compliance**:
```bash
echo "@src/collision/ Does the collision system adhere to the principle of orthogonality?" | codex e 2>/dev/null
```

**Design Decision Analysis**:
```bash
echo "Should skeletal animation be event-driven or state-based? List trade-offs." | codex e 2>/dev/null
```

## Multi-Turn Consultation

A single question sometimes reveals more questions. Complex problems demand conversation, not just answers.

When analysis requires iteration, resume the previous session to preserve context:

**Starting a consultation**:
```bash
echo "Review the change, look for use-after-free issues" | codex e 2>/dev/null
```

**Continuing the conversation**:
```bash
echo "Fix the use-after-free issues you found" | codex e resume --last 2>/dev/null
```

**The Pattern**: Ask → Analyze → Follow-up → Refine

Use multi-turn when:
- Initial analysis reveals unexpected complexity
- The solution requires iterative refinement
- Context from previous answers informs new questions
- A problem needs exploration, not just validation

**One-shot vs. Multi-turn**:
- New question? Use `codex e 2>/dev/null`
- Follow-up on previous session? Use `codex e resume --last 2>/dev/null`

Context preservation allows depth. Shallow questions get shallow answers. Deep questions deserve conversation.

## When to Use Codex

Use Codex when you need a second opinion:

- **Before major refactoring** - Validate the approach before committing
- **When stuck on design decisions** - Get alternative perspectives
- **For complexity validation** - Ensure a solution isn't over-engineered
- **During principle conflicts** - Seek guidance when principles seem to conflict
- **After implementing a system** - Get an independent review
- **When debugging complex interactions** - Fresh eyes on difficult problems

**The Rule**: When confidence wavers, consult. When certainty seems absolute, verify.

## The Warning

Do not substitute consultation for understanding. Tools provide perspective, not truth.

- **Claude Code**: For iterative development, the primary development partner
- **Codex CLI**: For second opinions, validation, alternative perspectives
- **Gemini CLI**: For massive context, complete codebase analysis

The best tool is the one that matches the question's shape:
- Building and iterating? Claude Code.
- Need validation? Codex.
- Need complete visibility? Gemini.

## The Pattern

**Before implementing**:
```bash
echo "@TASKS/PLANS/BUILD_skeletal_animation.md Review the approach" | codex e 2>/dev/null
```

**During implementation** (while working with Claude Code):
```bash
echo "Is this spring constant physically reasonable? k=200, damping=0.7" | codex e 2>/dev/null
```

**After implementation**:
```bash
echo "@src/animation/skeleton.cpp Review for principle violations" | codex e 2>/dev/null
```

**The cycle**: Propose → Validate → Implement → Verify

Confidence is built through convergence. When two independent analyses agree, trust increases. When they disagree, truth emerges from reconciliation.

## Advanced Usage

**Verbose Mode** (when you need to debug):
```bash
echo "your question" | codex e
```

**Capture Only Results**:
```bash
result=$(echo "your question" | codex e 2>/dev/null)
```

**Background Processing**:
```bash
# Launch in background, capture clean results
echo "comprehensive audit question" | codex e 2>/dev/null > results.txt &
```

**Dual-AI Analysis** (combine with Gemini):
```bash
# Parallel analysis for convergence validation
gemini -p "@src/ your question" > gemini_findings.txt &
echo "@src/ your question" | codex e 2>/dev/null > codex_findings.txt &
wait
# Synthesize findings from both
```

**Seek the second opinion. Question the first.**
