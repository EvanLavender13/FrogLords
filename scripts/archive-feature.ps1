#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Archives feature artifacts with timestamp prefix and updates internal references.

.DESCRIPTION
    Moves feature-related markdown files from PLANS/ to PLANS/ARCHIVE/ with a timestamp
    prefix, then updates all internal cross-references to use the new filenames.

.PARAMETER FeatureName
    The name of the feature to archive (e.g., "static_keyframe_preview")
    If not provided, will attempt to extract from current git branch (feature/<feature>)

.EXAMPLE
    .\archive-feature.ps1 static_keyframe_preview

.EXAMPLE
    .\archive-feature.ps1
    # Uses current branch name if on feature/<feature> branch
#>

param(
    [Parameter(Position = 0)]
    [string]$FeatureName
)

# If no feature name provided, try to extract from git branch
if (-not $FeatureName) {
    $currentBranch = git branch --show-current
    if ($currentBranch -match '^feature/(.+)$') {
        $FeatureName = $Matches[1]
        Write-Host "Detected feature name from branch: $FeatureName" -ForegroundColor Cyan
    } else {
        Write-Host "Error: No feature name provided and not on a feature/* branch" -ForegroundColor Red
        Write-Host "Usage: .\archive-feature.ps1 <feature_name>" -ForegroundColor Yellow
        Write-Host "   or: Run from a feature/<feature> branch" -ForegroundColor Yellow
        exit 1
    }
}

# Generate timestamp
$timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
Write-Host "`nTimestamp: $timestamp" -ForegroundColor Green

# Define artifact patterns
$artifactPatterns = @(
    "feature_$FeatureName.md",
    "implementation_$FeatureName.md",
    "code_review_$FeatureName.md",
    "plan_review_$FeatureName.md"
)

# Find existing artifacts
$plansDir = "PLANS"
$archiveDir = "PLANS\ARCHIVE"

# Ensure archive directory exists
if (-not (Test-Path $archiveDir)) {
    New-Item -ItemType Directory -Path $archiveDir | Out-Null
    Write-Host "Created $archiveDir directory" -ForegroundColor Yellow
}

# Find all matching files
$filesToArchive = @()
foreach ($pattern in $artifactPatterns) {
    $file = Join-Path $plansDir $pattern
    if (Test-Path $file) {
        $filesToArchive += $file
    }
}

if ($filesToArchive.Count -eq 0) {
    Write-Host "`nNo artifacts found for feature: $FeatureName" -ForegroundColor Red
    Write-Host "Looked for patterns: $($artifactPatterns -join ', ')" -ForegroundColor Yellow
    exit 1
}

Write-Host "`nFound $($filesToArchive.Count) artifact(s) to archive:" -ForegroundColor Cyan
$filesToArchive | ForEach-Object { Write-Host "  - $(Split-Path $_ -Leaf)" }

# Build mapping of old filename -> new filename (basename only, no path)
$filenameMap = @{}
foreach ($file in $filesToArchive) {
    $basename = Split-Path $file -Leaf
    $newBasename = "${timestamp}_$basename"
    $filenameMap[$basename] = $newBasename
}

# Move and rename files using git mv
Write-Host "`nMoving files to archive..." -ForegroundColor Cyan
$archivedFiles = @()
foreach ($file in $filesToArchive) {
    $basename = Split-Path $file -Leaf
    $newBasename = $filenameMap[$basename]
    $destination = Join-Path $archiveDir $newBasename

    Write-Host "  $basename -> ARCHIVE\$newBasename"

    # Use git mv to preserve history
    git mv $file $destination

    if ($LASTEXITCODE -eq 0) {
        $archivedFiles += $destination
    } else {
        Write-Host "    Warning: git mv failed, trying regular move..." -ForegroundColor Yellow
        Move-Item $file $destination -Force
        $archivedFiles += $destination
    }
}

# Update internal references in archived files
Write-Host "`nUpdating internal references..." -ForegroundColor Cyan
foreach ($archivedFile in $archivedFiles) {
    $content = Get-Content $archivedFile -Raw
    $modified = $false

    foreach ($oldBasename in $filenameMap.Keys) {
        $newBasename = $filenameMap[$oldBasename]

        # Update references in various formats:
        # - PLANS/filename.md
        # - `PLANS/filename.md`
        # - PLANS/ARCHIVE/filename.md (already archived)
        # - `filename.md` (bare reference)

        # Pattern 1: PLANS/filename.md -> PLANS/ARCHIVE/timestamp_filename.md
        $pattern1 = "PLANS/$oldBasename"
        $replacement1 = "PLANS/ARCHIVE/$newBasename"
        if ($content -match [regex]::Escape($pattern1)) {
            $content = $content -replace [regex]::Escape($pattern1), $replacement1
            $modified = $true
        }

        # Pattern 2: `filename.md` -> `timestamp_filename.md` (markdown inline code)
        $pattern2 = "``$oldBasename``"
        $replacement2 = "``$newBasename``"
        if ($content -match [regex]::Escape($pattern2)) {
            $content = $content -replace [regex]::Escape($pattern2), $replacement2
            $modified = $true
        }
    }

    if ($modified) {
        Set-Content $archivedFile -Value $content -NoNewline
        Write-Host "  Updated: $(Split-Path $archivedFile -Leaf)" -ForegroundColor Green
    }
}

Write-Host "`nArchive complete!" -ForegroundColor Green
Write-Host "Timestamp: $timestamp" -ForegroundColor Cyan
Write-Host "Archived $($archivedFiles.Count) file(s) to $archiveDir" -ForegroundColor Cyan
Write-Host "`nNext steps:" -ForegroundColor Yellow
Write-Host "  1. Review the changes: git status" -ForegroundColor Gray
Write-Host "  2. Commit the archive: git add . && git commit -m 'archive: $FeatureName feature artifacts'" -ForegroundColor Gray
