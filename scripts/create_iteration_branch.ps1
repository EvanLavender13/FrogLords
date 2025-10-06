param (
    [Parameter(Mandatory=$true)]
    [string]$feature_name
)

if (-not $feature_name) {
    Write-Host "Error: feature_name is required."
    exit 1
}

$branch_name = "iteration/$feature_name"

Write-Host "Creating and switching to branch: $branch_name"
git checkout -b $branch_name
