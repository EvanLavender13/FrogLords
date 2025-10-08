# Review staged changes before committing

Write-Host "=== Staged Files ===" -ForegroundColor Cyan
git status --short

Write-Host "`n=== Staged Changes ===" -ForegroundColor Cyan
git diff --staged
