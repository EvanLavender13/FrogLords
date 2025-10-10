param (
    [Parameter(Mandatory=$true)]
    [string]$refactor_name
)

if (-not $refactor_name) {
    Write-Host "Error: refactor_name is required."
    exit 1
}

$branch_name = "refactor/$refactor_name"

Write-Host "Creating and switching to branch: $branch_name"
git checkout -b $branch_name
