#!/usr/bin/env pwsh

# Run cppcheck on the project.

$root = Split-Path -Parent $PSScriptRoot
$buildDir = Join-Path $root 'build\ninja'
$compileDb = Join-Path $buildDir 'compile_commands.json'
$cppcheckBuildDir = Join-Path $root 'build\cppcheck'

if (-not (Test-Path -Path $compileDb -PathType Leaf)) {
    Write-Error "compile_commands.json not found at $compileDb. Run the CMake configure task first."
    exit 1
}

# Create cppcheck build dir if not exists
if (-not (Test-Path -Path $cppcheckBuildDir)) {
    New-Item -ItemType Directory -Path $cppcheckBuildDir | Out-Null
}

# Run cppcheck
& 'cppcheck' ('--project=' + $compileDb) ('--cppcheck-build-dir=' + $cppcheckBuildDir) '--enable=all' '--std=c++20' '--file-filter=src/**' '--suppress=missingIncludeSystem' '--suppress=*:external/**' '--check-level=exhaustive'
exit $LASTEXITCODE