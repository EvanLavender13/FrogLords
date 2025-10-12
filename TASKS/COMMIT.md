# Commit Staged Changes

### 1. Analyze Changes

Run `scripts/review_staged_changes.ps1` to confirm staged files and review changes.

### 2. Draft Commit Message

Use the exact format below.

### 3. Propose for Review

Post the drafted message for approval. Do not commit until approved.

### 4. Commit

After approval, use PowerShell here-string syntax to avoid quoting issues:

```powershell
$msg = @"
<Domain>: <Brief description>

<Why paragraph>

- <Change 1>
- <Change 2>

<Outcome sentence>
"@

.\scripts\commit_changes.ps1 -Message $msg
```

**Example:**
```powershell
$msg = @"
character: add dash tuning

Introduces initial dash mechanic with basic timing and distance parameters.

- Add dash_speed and dash_duration to character tuning
- Implement dash trigger on Shift key
- Add cooldown timer (2s)

Dash now functional in graybox form.
"@

.\scripts\commit_changes.ps1 -Message $msg
```

**Critical:** Use here-string (`@"..."@`) syntax. Do NOT use regular quotes with escape characters.

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
