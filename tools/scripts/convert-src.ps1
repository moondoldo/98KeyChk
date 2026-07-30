Set-StrictMode -Version 2.0
$ErrorActionPreference = "Stop"

trap {
    Write-Host ""
    Write-Host "Conversion failed."
    Write-Host $_.Exception.Message
    Read-Host "Press Enter to close"
    exit 1
}

#
# Directory settings
#
$SourceDirectory = "D:\Git\md\98KeyChk\src"
$DestinationDirectory = "D:\Git\md\98KeyChk\PC98DEV\SRC"

$SourceRoot = [System.IO.Path]::GetFullPath($SourceDirectory).TrimEnd(
    [char[]]"\/"
)
$DestinationRoot = [System.IO.Path]::GetFullPath(
    $DestinationDirectory
).TrimEnd([char[]]"\/")

if (-not (Test-Path -LiteralPath $SourceRoot -PathType Container)) {
    throw "Source directory does not exist: $SourceRoot"
}

if (-not (Test-Path -LiteralPath $DestinationRoot -PathType Container)) {
    throw "Destination directory does not exist: $DestinationRoot"
}

if ($SourceRoot -eq $DestinationRoot) {
    throw "Source and destination directories must be different."
}

$Utf8 = New-Object System.Text.UTF8Encoding($false, $true)
$ShiftJis = [System.Text.Encoding]::GetEncoding(
    932,
    [System.Text.EncoderFallback]::ExceptionFallback,
    [System.Text.DecoderFallback]::ExceptionFallback
)

$SourcePrefix = $SourceRoot + [System.IO.Path]::DirectorySeparatorChar
$ConvertedFiles = @()
$SourceFiles = @(
    Get-ChildItem -LiteralPath $SourceRoot -Recurse -File |
        Where-Object {
            $_.Extension -ieq ".c" -or $_.Extension -ieq ".h"
        }
)

foreach ($SourceFile in $SourceFiles) {
    $RelativePath = $SourceFile.FullName.Substring($SourcePrefix.Length)

    try {
        $Text = [System.IO.File]::ReadAllText($SourceFile.FullName, $Utf8)
        $Text = $Text -replace "`r`n|`r|`n", "`r`n"
        $Bytes = $ShiftJis.GetBytes($Text)
    }
    catch {
        throw "Failed to convert $RelativePath`: $($_.Exception.Message)"
    }

    $ConvertedFiles += New-Object PSObject -Property @{
        RelativePath = $RelativePath
        Bytes = $Bytes
    }
}

foreach ($ConvertedFile in $ConvertedFiles) {
    $DestinationFile = Join-Path (
        $DestinationRoot
    ) $ConvertedFile.RelativePath
    $DestinationParent = Split-Path -Parent $DestinationFile

    if (-not (Test-Path -LiteralPath $DestinationParent -PathType Container)) {
        New-Item -ItemType Directory -Path $DestinationParent -Force |
            Out-Null
    }

    [System.IO.File]::WriteAllBytes(
        $DestinationFile,
        $ConvertedFile.Bytes
    )
    Write-Host "Converted: $($ConvertedFile.RelativePath)"
}

Write-Host "Completed: $($ConvertedFiles.Count) file(s)"
