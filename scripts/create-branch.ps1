#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Creates a new git branch with workflow-specific prefix.

.DESCRIPTION
    Creates and switches to a new git branch with the appropriate prefix:
    - feature/<name> for feature development
    - refactor/<name> for refactoring work
    - maintenance/<name> for maintenance fixes

.PARAMETER Type
    The workflow type: feature, refactor, or maintenance

.PARAMETER Name
    The branch name (without prefix)

.EXAMPLE
    .\create-branch.ps1 feature static-keyframe-preview

.EXAMPLE
    .\create-branch.ps1 refactor yaw-direction-utilities

.EXAMPLE
    .\create-branch.ps1 maintenance remove-redundant-includes
#>

param (
    [Parameter(Mandatory=$true, Position=0)]
    [ValidateSet('feature', 'refactor', 'maintenance')]
    [string]$Type,

    [Parameter(Mandatory=$true, Position=1)]
    [string]$Name
)

if (-not $Name) {
    Write-Host "Error: Branch name is required." -ForegroundColor Red
    Write-Host "Usage: .\create-branch.ps1 <type> <name>" -ForegroundColor Yellow
    Write-Host "Types: feature, refactor, maintenance" -ForegroundColor Yellow
    exit 1
}

$branch_name = "$Type/$Name"

Write-Host "Creating and switching to branch: $branch_name" -ForegroundColor Cyan
git checkout -b $branch_name

if ($LASTEXITCODE -eq 0) {
    Write-Host "`nBranch created successfully!" -ForegroundColor Green
    Write-Host "Current branch: $branch_name" -ForegroundColor Cyan
} else {
    Write-Host "`nError: Failed to create branch" -ForegroundColor Red
    exit 1
}
