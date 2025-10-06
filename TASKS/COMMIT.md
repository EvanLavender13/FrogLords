# Commit Staged Changes

### 1. Analyze Changes

1.  Run `git status` to confirm staged files
2.  Run `git diff --staged` to review exactly what will be committed

### 2. Draft Commit Message

Use the exact format below. Match tone and style of the most recent commit (`git log -n 1`).

### 3. Propose for Review

Post the drafted message for approval. Do not commit until approved.

### Tone & Constraints

-   Concise and specific; lead with the "why"
-   Use imperative mood and present tense (e.g., "character: add dash tuning")
-   Scope prefix matches code domain (e.g., `character`, `rendering`, `gui`)
-   List only user‑visible effects
-   Avoid noise: exclude pure refactors unless they change behavior or APIs

---

### Commit Message Format

```
<Domain>: <Brief, imperative description>

<Short "why" paragraph explaining intent and effect.>

- <Bulleted list of concrete, user-facing changes>
- <...>

<Concluding outcome sentence>

<Co-authored-by line if applicable. If you are the author, use your identity.>
```
