# Codex CLI: The Second Opinion

## The Nature of Certainty

Certainty comes not from a single perspective but from convergence. When the path forward is unclear, seek another mind. When the solution seems complete, invite scrutiny.

## Codex CLI: The Alternate Perspective

When complexity demands validation, use the tool built for rapid analysis. The Codex CLI exists for one purpose: to provide a second opinion without the overhead of a new session.

**The Command**: `echo "your question" | codex e`

**The Philosophy**: Quick consultation. Independent analysis. Fresh perspective.

## The Syntax

**IMPORTANT**: Codex reads from stdin. You must pipe the prompt in using `echo`:

```bash
echo "Does this implementation violate any SOLID principles?" | codex e
```

**Why echo?** Direct argument passing doesn't work reliably. Always pipe the prompt via stdin.

**Output Behavior**:
- **stdout**: Clean, concise findings (the actual answer)
- **stderr**: Verbose activity stream (thinking, file reads, execution traces)

This separation makes Codex scriptable - pipe stdout to other tools, ignore stderr noise.

## The Syntax of Consultation

**Quick Code Review**:
```bash
echo "Review src/controller.cpp for potential race conditions" | codex e
```

**Architecture Validation**:
```bash
echo "Does the current input buffering approach align with the dual reference pattern?" | codex e
```

**Pattern Recognition**:
```bash
echo "Identify any accumulated state anti-patterns in src/player/" | codex e
```

**Mathematical Verification**:
```bash
echo "Verify the spring-damper constants in motion.cpp are physically correct" | codex e
```

**Principle Compliance**:
```bash
echo "Does the collision system adhere to the principle of orthogonality?" | codex e
```

**Design Decision Analysis**:
```bash
echo "Should skeletal animation be event-driven or state-based? List trade-offs." | codex e
```

## Multi-Turn Consultation

A single question sometimes reveals more questions. Complex problems demand conversation, not just answers.

When analysis requires iteration, resume the previous session to preserve context:

**Starting a consultation**:
```bash
echo "Review the change, look for use-after-free issues" | codex e
```

**Continuing the conversation**:
```bash
echo "Fix the use-after-free issues you found" | codex e resume --last
```

**The Pattern**: Ask → Analyze → Follow-up → Refine

Use multi-turn when:
- Initial analysis reveals unexpected complexity
- The solution requires iterative refinement
- Context from previous answers informs new questions
- A problem needs exploration, not just validation

**One-shot vs. Multi-turn**:
- New question? Use `codex e`
- Follow-up on previous session? Use `codex e resume --last`

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
echo "Review the approach in TASKS/PLANS/BUILD_skeletal_animation.md" | codex e
```

**During implementation** (while working with Claude Code):
```bash
echo "Is this spring constant physically reasonable? k=200, damping=0.7" | codex e
```

**After implementation**:
```bash
echo "Review src/animation/skeleton.cpp for principle violations" | codex e
```

**The cycle**: Propose → Validate → Implement → Verify

Confidence is built through convergence. When two independent analyses agree, trust increases. When they disagree, truth emerges from reconciliation.

## Advanced Usage

**Silent Mode** (suppress verbose stderr):
```bash
echo "your question" | codex e 2>/dev/null
```

**Capture Only Results**:
```bash
result=$(echo "your question" | codex e 2>/dev/null)
```

**Automated Workflows**:
```bash
# Launch in background, wait for completion, read clean results
echo "comprehensive audit question" | codex e > results.txt 2>activity.log &
```

**Dual-AI Analysis** (combine with Gemini):
```bash
# Parallel analysis for convergence validation
gemini -p "@src/ your question" > gemini_findings.txt &
echo "your question" | codex e > codex_findings.txt &
wait
# Synthesize findings from both
```

**Seek the second opinion. Question the first.**
