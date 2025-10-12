#!/usr/bin/env pwsh

# Run clang-tidy across translation units under the src directory.

$root = Split-Path -Parent $PSScriptRoot
$srcDir = Join-Path $root 'src'
$buildDir = Join-Path $root 'build\ninja'
$compileDb = Join-Path $buildDir 'compile_commands.json'

if (-not (Test-Path -Path $srcDir -PathType Container)) {
    Write-Error "src directory not found at $srcDir"
    exit 1
}

if (-not (Test-Path -Path $compileDb -PathType Leaf)) {
    Write-Error "compile_commands.json not found at $compileDb. Run the CMake configure task first."
    exit 1
}

$cppFiles = Get-ChildItem -Path $srcDir -Recurse -Include *.cpp -File |
    Where-Object { $_.FullName.StartsWith($srcDir, [System.StringComparison]::OrdinalIgnoreCase) } |
    Sort-Object FullName

if ($cppFiles.Count -eq 0) {
    Write-Host "No translation units found under $srcDir"
    exit 0
}

Write-Host "Starting clang-tidy..."
Write-Host "Analyzing $($cppFiles.Count) translation unit(s) from $srcDir"
Write-Host ""

$exitCode = 0
foreach ($file in $cppFiles) {
    $relative = $file.FullName.Substring($root.Length + 1)
    Write-Host "Analyzing: $relative"

    & 'clang-tidy' '-p' $buildDir $file.FullName
    if ($LASTEXITCODE -ne 0 -and $exitCode -eq 0) {
        $exitCode = $LASTEXITCODE
    }
}

Write-Host ""
if ($exitCode -eq 0) {
    Write-Host "clang-tidy completed successfully" -ForegroundColor Green
} else {
    Write-Host "clang-tidy found issues (exit code $exitCode)" -ForegroundColor Yellow
}

exit $exitCode
