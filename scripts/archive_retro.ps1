#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Organizes retrospective archives into RETRO_* subdirectories.

.DESCRIPTION
    Creates a RETRO_<timestamp>/ subdirectory in PLANS/ARCHIVE/ and moves:
    - The most recent RETROSPECTIVE_*.md document
    - All timestamped artifacts (FEATURE, PLAN, CODE_REVIEW, PLAN_REVIEW, IMPROVE)
    
    Auto-detects all files by pattern - no manual specification needed.
    Keeps ARCHIVE/ clean by grouping reviewed work with its analysis.

.EXAMPLE
    # Auto-detect latest retrospective and all artifacts
    .\archive_retro.ps1

.EXAMPLE
    # Specify retrospective timestamp explicitly
    .\archive_retro.ps1 -Timestamp "20251012_225921"
#>

param(
    [Parameter(Position=0)]
    [string]$Timestamp
)

$plansDir = "PLANS"
$archiveDir = "PLANS\ARCHIVE"

# If no timestamp provided, find most recent RETROSPECTIVE_*.md
if (-not $Timestamp) {
    $retroDocs = Get-ChildItem "$plansDir\RETROSPECTIVE_*.md" -ErrorAction SilentlyContinue | 
                 Sort-Object Name -Descending
    
    if ($retroDocs.Count -eq 0) {
        Write-Host "Error: No RETROSPECTIVE_*.md found in $plansDir" -ForegroundColor Red
        exit 1
    }
    
    $latestRetro = $retroDocs[0]
    $Timestamp = $latestRetro.BaseName -replace '^RETROSPECTIVE_(.+)$', '$1'
    Write-Host "Auto-detected retrospective: RETROSPECTIVE_$Timestamp.md" -ForegroundColor Cyan
} else {
    Write-Host "Using retrospective: RETROSPECTIVE_$Timestamp.md" -ForegroundColor Cyan
}

# Verify retrospective document exists
$retroDoc = "$plansDir\RETROSPECTIVE_$Timestamp.md"
if (-not (Test-Path $retroDoc)) {
    Write-Host "Error: Retrospective document not found: $retroDoc" -ForegroundColor Red
    exit 1
}

# Auto-detect all artifacts to move (timestamped workflow documents)
Write-Host "`nScanning for artifacts to organize..." -ForegroundColor Cyan
$artifactPatterns = @(
    "*_FEATURE.md",
    "*_PLAN.md", 
    "*_CODE_REVIEW.md",
    "*_PLAN_REVIEW.md",
    "*_IMPROVE_*.md"
)

$filesToMove = @()
foreach ($pattern in $artifactPatterns) {
    $matchedFiles = Get-ChildItem "$archiveDir\$pattern" -ErrorAction SilentlyContinue
    if ($matchedFiles) {
        $filesToMove += $matchedFiles
    }
}

if ($filesToMove.Count -eq 0) {
    Write-Host "No artifacts found to organize (only retrospective will be moved)" -ForegroundColor Yellow
}

# Create RETRO subdirectory
$retroSubdir = "$archiveDir\RETRO_$Timestamp"
if (Test-Path $retroSubdir) {
    Write-Host "Warning: $retroSubdir already exists. Files will be moved into it." -ForegroundColor Yellow
} else {
    New-Item -ItemType Directory -Path $retroSubdir | Out-Null
    Write-Host "Created: $retroSubdir" -ForegroundColor Green
}

# Move retrospective document first
Write-Host "`nMoving retrospective document..." -ForegroundColor Cyan
git mv $retroDoc "$retroSubdir\RETROSPECTIVE_$Timestamp.md"
if ($LASTEXITCODE -ne 0) {
    Write-Host "  Warning: git mv failed, trying regular move..." -ForegroundColor Yellow
    Move-Item $retroDoc "$retroSubdir\RETROSPECTIVE_$Timestamp.md" -Force
}
Write-Host "  Moved: RETROSPECTIVE_$Timestamp.md" -ForegroundColor Green

if ($filesToMove.Count -eq 0) {
    Write-Host "`nOrganization complete (retrospective only)!" -ForegroundColor Green
    Write-Host "Retrospective subdirectory: $retroSubdir" -ForegroundColor Cyan
    
    Write-Host "`nNext steps:" -ForegroundColor Yellow
    Write-Host "  1. Review: git status" -ForegroundColor Gray
    Write-Host "  2. Commit: git add . && git commit -m 'archive: organize RETRO_$Timestamp'" -ForegroundColor Gray
    exit 0
}

# Move artifacts
Write-Host "`nMoving $($filesToMove.Count) artifact(s)..." -ForegroundColor Cyan
$movedCount = 0
foreach ($file in $filesToMove) {
    $basename = $file.Name
    $destination = "$retroSubdir\$basename"
    
    git mv $file.FullName $destination
    if ($LASTEXITCODE -eq 0) {
        Write-Host "  Moved: $basename" -ForegroundColor Green
        $movedCount++
    } else {
        Write-Host "  Warning: git mv failed for $basename, trying regular move..." -ForegroundColor Yellow
        Move-Item $file.FullName $destination -Force
        $movedCount++
    }
}

Write-Host "`nOrganization complete!" -ForegroundColor Green
Write-Host "Retrospective subdirectory: $retroSubdir" -ForegroundColor Cyan
Write-Host "Moved: 1 retrospective + $movedCount artifact(s)" -ForegroundColor Cyan

Write-Host "`nNext steps:" -ForegroundColor Yellow
Write-Host "  1. Review: git status" -ForegroundColor Gray
Write-Host "  2. Commit: git add . && git commit -m 'archive: organize RETRO_$Timestamp'" -ForegroundColor Gray
