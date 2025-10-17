# Commit Staged Changes

**Purpose:** Create well-formed git commits with clear messages.

**Prerequisites:** Changes staged and ready to commit

---

## 1. Analyze Changes

Run `bash scripts/bash/review_staged_changes.sh` to review staged files.

## 2. Draft Commit Message

Format:
```
<Domain>: <Brief imperative description>

<Why paragraph explaining intent>

- <User-visible change 1>
- <User-visible change 2>

<Outcome sentence>
```

## 3. Propose for Review

Post drafted message for approval. Do not commit until approved.

## 4. Execute Commit

After approval:
```bash
bash scripts/bash/commit_changes.sh -m "<message>"
```

Or for complex messages:
```bash
bash scripts/bash/commit_changes.sh -f commit_msg.txt
```

---

## Example

```bash
bash scripts/bash/commit_changes.sh -m "character: add dash tuning

Introduces initial dash mechanic with basic timing and distance parameters.

- Add dash_speed and dash_duration to character tuning
- Implement dash trigger on Shift key
- Add cooldown timer (2s)

Dash now functional in graybox form."
```

---

## Tone & Constraints

- Lead with "why" not "what"
- Imperative mood, present tense
- Domain prefix matches code area
- Only user-visible effects
- Exclude pure refactors unless API changes
