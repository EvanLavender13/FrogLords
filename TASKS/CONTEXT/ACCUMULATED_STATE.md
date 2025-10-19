# Accumulated State: The Drift Toward Falsehood

## The Nature of State

State exists in two forms:

**Truth** - Derived from authoritative sources each frame
**Accumulated** - Built up over time through modification

Truth remains truth. Accumulated state drifts toward lies.

## The Problem

When state accumulates across frames, it diverges from reality through three mechanisms:

**1. Conditional Updates**
State updated only under certain conditions preserves stale values when conditions aren't met. The gap between current reality and stored state grows silently.

**2. Numerical Drift**
Floating-point accumulation compounds error. Each operation adds uncertainty. Over time, accumulated values become approximations of approximations.

**3. Division by Accumulated Values**
When accumulated state approaches zero, division produces infinity or NaN. Mathematical operations assume non-degenerate inputs. Accumulated state cannot guarantee this.

## The Solution

**Never accumulate when you can derive.**

State should be:
- Calculated from authoritative sources each frame, OR
- Updated unconditionally every frame (preventing staleness), OR
- Protected with epsilon checks (preventing degenerate math)

**Prefer calculation over accumulation.**

A position derived from velocity and time is truth.
A position incremented each frame drifts from truth.

**If you must accumulate, update unconditionally.**

State that's sometimes updated and sometimes frozen creates divergence. Either update it every frame (staying synchronized with reality) or store the source of truth and derive the state.

**If you must use accumulated values in math, validate first.**

Before normalization: check magnitude > epsilon
Before division: check denominator > epsilon
Before inverse: check value > epsilon

Degenerate cases are not edge cases in accumulated state—they are inevitable destinations.

## The Pattern

**Replace this:**
```
accumulated_value += delta  // Only when condition is true
result = something / accumulated_value  // Division by accumulated state
```

**With this:**
```
result = derive_from_source(authoritative_state)  // Truth, not accumulation
```

**Or if accumulation is necessary:**
```
accumulated_value += delta  // Always, unconditionally
if (accumulated_value > EPSILON) {
    result = something / accumulated_value  // Safe
} else {
    result = fallback  // Explicit handling
}
```

## The Warning Signs

Accumulated state reveals itself through:

- **Conditional updates** - `if (condition) { state += delta; }`
- **Reset logic** - `if (transition) { state = 0; }` (why did it need resetting?)
- **Drift correction** - `if (error > threshold) { state = truth; }` (why did it drift?)
- **Epsilon checks added reactively** - Bug discovered, epsilon added, pattern remains

When you add epsilon checks to fix a bug, ask: "Why did this value become zero?" If the answer is "accumulation over time," the epsilon is treating symptoms, not disease.

## The Principle

**State is either truth or it is tending toward falsehood.**

Derived state is truth.
Accumulated state is decay.

Choose derivation. When accumulation is unavoidable, update unconditionally and validate before use.

**The best way to prevent accumulated state bugs is to eliminate accumulated state.**
