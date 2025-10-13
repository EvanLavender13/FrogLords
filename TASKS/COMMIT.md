# Commit Staged Changes

### 1. Analyze Changes

Run `bash scripts/bash/review_staged_changes.sh` to confirm staged files and review changes.

### 2. Draft Commit Message

Use the exact format below.

### 3. Propose for Review

Post the drafted message for approval. Do not commit until approved.

### 4. Commit

After approval, commit with the message:

```bash
bash scripts/bash/commit_changes.sh -m "<Domain>: <Brief description>

<Why paragraph>

- <Change 1>
- <Change 2>

<Outcome sentence>"
```

**Example:**
```bash
bash scripts/bash/commit_changes.sh -m "character: add dash tuning

Introduces initial dash mechanic with basic timing and distance parameters.

- Add dash_speed and dash_duration to character tuning
- Implement dash trigger on Shift key
- Add cooldown timer (2s)

Dash now functional in graybox form."
```

**For complex multi-line messages, use a temporary file:**
```bash
# Create commit_msg.txt with your message
bash scripts/bash/commit_changes.sh -f commit_msg.txt
```

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
