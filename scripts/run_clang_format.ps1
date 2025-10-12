#!/usr/bin/env pwsh

# Format all C/C++ sources under the src directory with clang-format.

$root = Split-Path -Parent $PSScriptRoot
$srcDir = Join-Path $root 'src'

if (-not (Test-Path -Path $srcDir -PathType Container)) {
    Write-Error "src directory not found at $srcDir"
    exit 1
}

$files = Get-ChildItem -Path $srcDir -Recurse -Include *.cpp,*.hpp,*.h -File
$filtered = @()
foreach ($file in $files) {
    if ($file.FullName.StartsWith($srcDir, [System.StringComparison]::OrdinalIgnoreCase)) {
        $filtered += $file.FullName
    }
}

if ($filtered.Count -eq 0) {
    Write-Host "No source files found under $srcDir"
    exit 0
}

Write-Host "Starting clang-format..."
Write-Host "Formatting $($filtered.Count) source file(s) in $srcDir"

$arguments = @('-i') + $filtered
& 'clang-format' @arguments

if ($LASTEXITCODE -eq 0) {
    Write-Host "clang-format completed successfully" -ForegroundColor Green
} else {
    Write-Host "clang-format failed with exit code $LASTEXITCODE" -ForegroundColor Red
}

exit $LASTEXITCODE
