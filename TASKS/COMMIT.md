# Commit Staged Changes

### 1. Analyze Changes

Run `scripts/review_staged_changes.ps1` to confirm staged files and review changes.

### 2. Draft Commit Message

Use the exact format below.

### 3. Propose for Review

Post the drafted message for approval. Do not commit until approved.

### 4. Commit

After approval, run `scripts/commit_changes.ps1 -Message "<commit message>"`

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
