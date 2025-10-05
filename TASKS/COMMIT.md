# Commit Staged Changes

### 1. Analyze Changes

1.  Run `git status` to list staged files.
2.  Run `git diff --staged` to review modifications.

### 2. Draft Commit Message

Draft a commit message using the exact format below. Match the tone and style of previous commits (`git log -n 1`).

```
<Domain>: <Brief, imperative-mood description>

<High-level summary explaining the "why" of the change.>

- <Bulleted list of specific, user-facing changes.>
- <...>

<Concluding sentence summarizing the outcome.>

<Co-authored-by line if applicable. If you are the author, use your identity. e.g., "Co-Authored-By: Gemini <gemini@google.com>">
```

### 3. Propose for Review

Propose the drafted message to the user for approval. Do not commit until approved.
