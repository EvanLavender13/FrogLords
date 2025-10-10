#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Archives refactor document with timestamp prefix.

.DESCRIPTION
    Moves refactor markdown file from PLANS/ to PLANS/ARCHIVE/ with a timestamp
    prefix. Unlike feature iterations, refactors use a single document that contains
    the complete history (description → plan → review → execution → code review → finalization).

.PARAMETER RefactorName
    The name of the refactor to archive (e.g., "yaw_direction_utilities")
    If not provided, will attempt to extract from current git branch (refactor/<name>)

.EXAMPLE
    .\archive-refactor.ps1 yaw_direction_utilities

.EXAMPLE
    .\archive-refactor.ps1
    # Uses current branch name if on refactor/<name> branch
#>

param(
    [Parameter(Position = 0)]
    [string]$RefactorName
)

# If no refactor name provided, try to extract from git branch
if (-not $RefactorName) {
    $currentBranch = git branch --show-current
    if ($currentBranch -match '^refactor/(.+)$') {
        $RefactorName = $Matches[1]
        Write-Host "Detected refactor name from branch: $RefactorName" -ForegroundColor Cyan
    } else {
        Write-Host "Error: No refactor name provided and not on a refactor/* branch" -ForegroundColor Red
        Write-Host "Usage: .\archive-refactor.ps1 <refactor_name>" -ForegroundColor Yellow
        Write-Host "   or: Run from a refactor/<name> branch" -ForegroundColor Yellow
        exit 1
    }
}

# Generate timestamp
$timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
Write-Host "`nTimestamp: $timestamp" -ForegroundColor Green

# Define refactor document pattern (single file per refactor)
$refactorFile = "PLANS\refactor_$RefactorName.md"

# Check if refactor document exists
if (-not (Test-Path $refactorFile)) {
    Write-Host "`nError: Refactor document not found: $refactorFile" -ForegroundColor Red
    Write-Host "Make sure the refactor has been finalized before archiving." -ForegroundColor Yellow
    exit 1
}

# Ensure archive directory exists
$archiveDir = "PLANS\ARCHIVE"
if (-not (Test-Path $archiveDir)) {
    New-Item -ItemType Directory -Path $archiveDir | Out-Null
    Write-Host "Created $archiveDir directory" -ForegroundColor Yellow
}

# Build new filename with timestamp
$basename = Split-Path $refactorFile -Leaf
$newBasename = "${timestamp}_$basename"
$destination = Join-Path $archiveDir $newBasename

Write-Host "`nArchiving refactor document:" -ForegroundColor Cyan
Write-Host "  $basename -> ARCHIVE\$newBasename"

# Use git mv to preserve history
git mv $refactorFile $destination

if ($LASTEXITCODE -eq 0) {
    Write-Host "`nArchive complete!" -ForegroundColor Green
    Write-Host "Timestamp: $timestamp" -ForegroundColor Cyan
    Write-Host "Archived to: $archiveDir\$newBasename" -ForegroundColor Cyan
} else {
    Write-Host "Warning: git mv failed, trying regular move..." -ForegroundColor Yellow
    Move-Item $refactorFile $destination -Force
    if (Test-Path $destination) {
        Write-Host "`nArchive complete!" -ForegroundColor Green
        Write-Host "Timestamp: $timestamp" -ForegroundColor Cyan
        Write-Host "Archived to: $archiveDir\$newBasename" -ForegroundColor Cyan
        Write-Host "(Note: Used regular move instead of git mv)" -ForegroundColor Yellow
    } else {
        Write-Host "`nError: Failed to move file" -ForegroundColor Red
        exit 1
    }
}

Write-Host "`nNext steps:" -ForegroundColor Yellow
Write-Host "  1. Review the changes: git status" -ForegroundColor Gray
Write-Host "  2. Commit the archive: git add . && git commit -m 'archive: $RefactorName refactor document'" -ForegroundColor Gray
Write-Host "  3. Merge refactor branch to main (if not already done)" -ForegroundColor Gray
