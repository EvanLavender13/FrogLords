# Commit staged changes with provided message
# Supports both inline messages and message files for multi-line commits
param(
    [Parameter(Mandatory=$false)]
    [string]$Message,
    [Parameter(Mandatory=$false)]
    [string]$MessageFile
)

# Validate input
if (-not $Message -and -not $MessageFile) {
    Write-Error "Either -Message or -MessageFile must be provided"
    exit 1
}

if ($Message -and $MessageFile) {
    Write-Error "Cannot specify both -Message and -MessageFile"
    exit 1
}

if ($MessageFile) {
    # Commit using message file
    if (-not (Test-Path $MessageFile)) {
        Write-Error "Message file not found: $MessageFile"
        exit 1
    }
    git commit -F $MessageFile
} else {
    # Commit using inline message
    # Write to temp file to handle multi-line messages and special characters
    $tempFile = Join-Path $env:TEMP "commit_msg_$(Get-Date -Format 'yyyyMMddHHmmss').txt"
    try {
        # Write message to temp file (UTF-8, no BOM)
        [System.IO.File]::WriteAllText($tempFile, $Message, (New-Object System.Text.UTF8Encoding $false))
        git commit -F $tempFile
    } finally {
        # Clean up temp file
        if (Test-Path $tempFile) {
            Remove-Item $tempFile -ErrorAction SilentlyContinue
        }
    }
}
