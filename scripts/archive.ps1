#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Archives workflow documents with timestamp prefix.

.DESCRIPTION
    Moves workflow documents from PLANS/ to PLANS/ARCHIVE/ with a timestamp prefix.
    Supports three workflow types with distinct naming patterns:
    - feature: Multi-file artifacts with feature-first naming (<name>_FEATURE.md, <name>_IMPLEMENTATION.md, etc.)
    - refactor: Single document with workflow-first naming (REFACTOR_<name>.md)
    - maintenance: Single document with workflow-first naming (MAINTENANCE_<name>.md)

.PARAMETER Type
    The workflow type: feature, refactor, or maintenance

.PARAMETER Name
    The name of the work to archive (e.g., "static_keyframe_preview", "yaw_direction_utilities")
    - For feature/refactor: Optional if on a matching branch (will auto-detect)
    - For maintenance: Required (maintenance typically works on main branch)

.EXAMPLE
    .\archive.ps1 feature static_keyframe_preview

.EXAMPLE
    .\archive.ps1 refactor yaw_direction_utilities

.EXAMPLE
    .\archive.ps1 maintenance remove_redundant_includes

.EXAMPLE
    .\archive.ps1 refactor
    # Auto-detects name from refactor/<name> branch (feature/refactor only)
#>

param(
    [Parameter(Mandatory=$true, Position=0)]
    [ValidateSet('feature', 'refactor', 'maintenance')]
    [string]$Type,

    [Parameter(Position=1)]
    [string]$Name
)

# If no name provided, try to extract from git branch
if (-not $Name) {
    # Maintenance items typically work on main branch, so name is required
    if ($Type -eq 'maintenance') {
        Write-Host "Error: Name is required for maintenance archiving" -ForegroundColor Red
        Write-Host "Usage: .\archive.ps1 maintenance <name>" -ForegroundColor Yellow
        Write-Host "Example: .\archive.ps1 maintenance remove_redundant_includes" -ForegroundColor Yellow
        Write-Host "`nAvailable maintenance documents in PLANS/:" -ForegroundColor Cyan
        Get-ChildItem "PLANS\MAINTENANCE_*.md" -ErrorAction SilentlyContinue | ForEach-Object {
            $docName = $_.Name -replace '^MAINTENANCE_(.+)\.md$', '$1'
            Write-Host "  - $docName" -ForegroundColor Gray
        }
        exit 1
    }
    
    # For feature/refactor, try to extract from branch
    $currentBranch = git branch --show-current
    if ($currentBranch -match "^$Type/(.+)`$") {
        $Name = $Matches[1]
        Write-Host "Detected name from branch: $Name" -ForegroundColor Cyan
    } else {
        Write-Host "Error: No name provided and not on a $Type/* branch" -ForegroundColor Red
        Write-Host "Usage: .\archive.ps1 <type> <name>" -ForegroundColor Yellow
        Write-Host "   or: Run from a $Type/<name> branch" -ForegroundColor Yellow
        Write-Host "Types: feature, refactor, maintenance" -ForegroundColor Yellow
        exit 1
    }
}

# Generate timestamp
$timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
Write-Host "`nTimestamp: $timestamp" -ForegroundColor Green

# Define artifact patterns based on type
$artifactPatterns = @()
switch ($Type) {
    'feature' {
        # Feature uses feature-first naming: <name>_WORKFLOW.md
        $artifactPatterns = @(
            "${Name}_FEATURE.md",
            "${Name}_IMPLEMENTATION.md",
            "${Name}_CODE_REVIEW.md",
            "${Name}_PLAN_REVIEW.md"
        )
    }
    'refactor' {
        # Refactor uses workflow-first naming: REFACTOR_<name>.md
        $artifactPatterns = @("REFACTOR_$Name.md")
    }
    'maintenance' {
        # Maintenance uses workflow-first naming: MAINTENANCE_<name>.md
        $artifactPatterns = @("MAINTENANCE_$Name.md")
    }
}

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
    Write-Host "`nNo artifacts found for $Type`: $Name" -ForegroundColor Red
    Write-Host "Looked for patterns: $($artifactPatterns -join ', ')" -ForegroundColor Yellow
    exit 1
}

Write-Host "`nFound $($filesToArchive.Count) artifact(s) to archive:" -ForegroundColor Cyan
$filesToArchive | ForEach-Object { Write-Host "  - $(Split-Path $_ -Leaf)" }

# Build mapping of old filename -> new filename (basename only, no path)
# Archive naming:
# - Features: timestamp_<name>_WORKFLOW.md (feature-first, grouping by feature name)
# - Refactor: timestamp_REFACTOR_<name>.md (workflow-first, grouping by type)
# - Maintenance: timestamp_MAINTENANCE_<name>.md (workflow-first, grouping by type)
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

# Update internal references in archived files (primarily for feature artifacts)
if ($Type -eq 'feature') {
    Write-Host "`nUpdating internal references..." -ForegroundColor Cyan
    foreach ($archivedFile in $archivedFiles) {
        $content = Get-Content $archivedFile -Raw
        $modified = $false

        foreach ($oldBasename in $filenameMap.Keys) {
            $newBasename = $filenameMap[$oldBasename]

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
}

Write-Host "`nArchive complete!" -ForegroundColor Green
Write-Host "Timestamp: $timestamp" -ForegroundColor Cyan
Write-Host "Archived $($archivedFiles.Count) file(s) to $archiveDir" -ForegroundColor Cyan

# Type-specific next steps
Write-Host "`nNext steps:" -ForegroundColor Yellow
Write-Host "  1. Review the changes: git status" -ForegroundColor Gray
Write-Host "  2. Commit the archive: git add . && git commit -m 'archive: $Name $Type artifacts'" -ForegroundColor Gray
if ($Type -ne 'maintenance') {
    Write-Host "  3. Merge $Type branch to main (if not already done)" -ForegroundColor Gray
}
