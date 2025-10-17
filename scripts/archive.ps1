#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Archives workflow documents with timestamp prefix.

.DESCRIPTION
    Archives all .md files from PLANS/ to PLANS/ARCHIVE/ with timestamp prefixes.
    Excludes permanent backlog files:
    - IMPROVE_BACKLOG.md
    - DESIGN_BACKLOG.md
    - DEPENDENCY_STACK.md

.EXAMPLE
    .\archive.ps1
#>

$plansDir = "PLANS"
$archiveDir = "PLANS\ARCHIVE"

# Generate timestamp
$timestamp = Get-Date -Format "yyyyMMdd_HHmmss"

# Files that should never be archived
$excludedFiles = @(
    "IMPROVE_BACKLOG.md",
    "DESIGN_BACKLOG.md",
    "DEPENDENCY_STACK.md"
)

# Ensure archive directory exists
if (-not (Test-Path $archiveDir)) {
    New-Item -ItemType Directory -Path $archiveDir | Out-Null
    Write-Host "Created $archiveDir directory" -ForegroundColor Yellow
}

# Find all .md files in PLANS/ (excluding subdirectories)
$allFiles = Get-ChildItem "$plansDir\*.md" -ErrorAction SilentlyContinue

# Filter out excluded files
$filesToArchive = $allFiles | Where-Object { $excludedFiles -notcontains $_.Name }

if ($filesToArchive.Count -eq 0) {
    Write-Host "No workflow documents found to archive" -ForegroundColor Yellow
    exit 0
}

Write-Host "`nFound $($filesToArchive.Count) document(s) to archive:" -ForegroundColor Cyan
$filesToArchive | ForEach-Object { Write-Host "  - $($_.Name)" -ForegroundColor Gray }

# Move and rename files with timestamp prefix
Write-Host "`nArchiving with timestamp: $timestamp" -ForegroundColor Green
$archivedCount = 0
foreach ($file in $filesToArchive) {
    $newName = "${timestamp}_$($file.Name)"
    $destination = Join-Path $archiveDir $newName

    Write-Host "  $($file.Name) -> ARCHIVE\$newName"

    # Use git mv to preserve history
    git mv $file.FullName $destination

    if ($LASTEXITCODE -eq 0) {
        $archivedCount++
    } else {
        Write-Host "    Warning: git mv failed, trying regular move..." -ForegroundColor Yellow
        Move-Item $file.FullName $destination -Force
        $archivedCount++
    }
}

Write-Host "`nArchive complete!" -ForegroundColor Green
Write-Host "Archived $archivedCount file(s) to $archiveDir" -ForegroundColor Cyan

Write-Host "`nNext steps:" -ForegroundColor Yellow
Write-Host "  1. Review: git status" -ForegroundColor Gray
Write-Host "  2. Commit: git add . && git commit -m 'archive: workflow documents $timestamp'" -ForegroundColor Gray
