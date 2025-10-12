<#
.SYNOPSIS
  Normalize text encodings and line endings across the repo.

.DESCRIPTION
  Rewrites selected text files to UTF-8 (no BOM) and normalizes line endings
  to LF (default) or CRLF. Skips common binary/third-party folders and NOTES/
  by default. Optionally include NOTES via -IncludeNotes.

.PARAMETER Root
  Repository root to scan. Defaults to current directory.

.PARAMETER LineEndings
  Target line endings: LF (default) or CRLF.

.PARAMETER IncludeNotes
  Include the NOTES/ directory in normalization.

.PARAMETER DryRun
  Show what would change without writing files.

.EXAMPLE
  scripts/normalize_encoding.ps1 -LineEndings LF

.EXAMPLE
  scripts/normalize_encoding.ps1 -IncludeNotes -LineEndings CRLF -DryRun
#>

[CmdletBinding()]
param(
  [string]$Root = (Get-Location).Path,
  [ValidateSet('LF','CRLF')]
  [string]$LineEndings = 'LF',
  [switch]$IncludeNotes,
  [switch]$DryRun
)

$ErrorActionPreference = 'Stop'

function Is-TextFile {
  param([string]$Path)
  try {
    $fs = [System.IO.File]::Open($Path, 'Open', 'Read', 'ReadWrite')
    try {
      $len = [Math]::Min(4096, $fs.Length)
      $buf = New-Object byte[] $len
      [void]$fs.Read($buf, 0, $len)
      # Heuristic: treat as binary if NUL present in first 4KB
      foreach ($b in $buf) { if ($b -eq 0) { return $false } }
      return $true
    } finally { $fs.Close() }
  } catch { return $false }
}

function Read-TextAutoEncoding {
  param([string]$Path)
  $bytes = [System.IO.File]::ReadAllBytes($Path)
  # Try strict UTF-8 first
  $utf8Strict = New-Object System.Text.UTF8Encoding($false, $true)
  try {
    $text = $utf8Strict.GetString($bytes)
    return @{ Text = $text; Encoding = 'UTF8' }
  } catch {
    # Fallback to Windows-1252, then re-encode to proper Unicode
    $cp1252 = [System.Text.Encoding]::GetEncoding(1252)
    $text = $cp1252.GetString($bytes)
    return @{ Text = $text; Encoding = 'CP1252' }
  }
}

function Normalize-LineEndings {
  param([string]$Text, [string]$Target)
  # Normalize to LF
  $t = $Text -replace "\r\n", "\n"
  $t = $t -replace "\r", "\n"
  if ($Target -eq 'CRLF') {
    $t = $t -replace "\n", "`r`n"
  }
  return $t
}

function Write-Utf8NoBomIfChanged {
  param([string]$Path, [string]$NewText)
  $current = [System.IO.File]::ReadAllText($Path)
  if ($current -cne $NewText) {
    if ($DryRun) { return $true }
    $utf8NoBom = New-Object System.Text.UTF8Encoding($false)
    [System.IO.File]::WriteAllText($Path, $NewText, $utf8NoBom)
    return $true
  }
  return $false
}

# Extensions considered text in this project
$textExt = @(
  '.md','.txt','.cpp','.c','.hpp','.h','.glsl','.vert','.frag',
  '.cmake','.json','.toml','.yml','.yaml','.ps1','.clang-format','.clang-tidy'
)

# Folders to skip by default
$excludeDirs = @('.git','build','generated','external','.vscode','.vs','NOTES')
if ($IncludeNotes) { $excludeDirs = $excludeDirs | Where-Object { $_ -ne 'NOTES' } }

$rootFull = (Resolve-Path $Root).Path
$files = Get-ChildItem -Path $rootFull -Recurse -File -Force |
  Where-Object {
    $rel = $_.FullName.Substring($rootFull.Length).TrimStart('\','/')
    # Skip excluded directories
    $parts = $rel -split '[\\/]'
    if ($parts.Length -gt 0 -and ($excludeDirs -contains $parts[0])) { return $false }
    # Filter by extension or known filenames
    $ext = [System.IO.Path]::GetExtension($_.Name).ToLowerInvariant()
    if ($textExt -contains $ext) { return $true }
    # Special-case names without extensions
    return ($_.Name -in @('CMakeLists.txt','.editorconfig','.gitattributes'))
  }

$changed = @()
foreach ($f in $files) {
  try {
    if (-not (Is-TextFile -Path $f.FullName)) { continue }
    $res = Read-TextAutoEncoding -Path $f.FullName
    $normalized = Normalize-LineEndings -Text $res.Text -Target $LineEndings
    if (Write-Utf8NoBomIfChanged -Path $f.FullName -NewText $normalized) {
      $changed += $f.FullName
      Write-Verbose ("Normalized: {0} (from {1})" -f $f.FullName, $res.Encoding)
    }
  } catch {
    Write-Warning ("Skip (error): {0} -> {1}" -f $f.FullName, $_.Exception.Message)
  }
}

if ($DryRun) {
  Write-Host ("Dry run: {0} files would change" -f $changed.Count)
} else {
  Write-Host ("Normalized {0} files to UTF-8 (no BOM), EOL={1}" -f $changed.Count, $LineEndings)
}

