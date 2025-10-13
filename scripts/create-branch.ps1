#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Creates a new git branch with workflow-specific prefix.

.DESCRIPTION
    Creates and switches to a new git branch with the appropriate prefix:
    - feature/<name> for feature development
    - improve/<name> for improvement work (maintenance, refactors, tech debt, architectural fixes)

.PARAMETER Type
    The workflow type: feature or improve

.PARAMETER Name
    The branch name (without prefix)

.EXAMPLE
    .\create-branch.ps1 feature static-keyframe-preview

.EXAMPLE
    .\create-branch.ps1 improve yaw-direction-utilities

.EXAMPLE
    .\create-branch.ps1 improve remove-redundant-includes
#>

param (
    [Parameter(Mandatory=$true, Position=0)]
    [ValidateSet('feature', 'improve')]
    [string]$Type,

    [Parameter(Mandatory=$true, Position=1)]
    [string]$Name
)

if (-not $Name) {
    Write-Host "Error: Branch name is required." -ForegroundColor Red
    Write-Host "Usage: .\create-branch.ps1 <type> <name>" -ForegroundColor Yellow
    Write-Host "Types: feature, improve" -ForegroundColor Yellow
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
