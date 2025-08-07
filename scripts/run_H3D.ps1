$scriptDir = $PSScriptRoot

$matlabExe = "C:\Program Files\MATLAB\R2025a\bin\matlab.exe"

# New binary path you want to use
$customExe = Join-Path $scriptDir "..\bin\PBF-FR-partitioning.exe"
$metricsExe = Join-Path $scriptDir "..\bin\PBF-FR-metrics.exe"

$outLASfolder = Join-Path $scriptDir "..\PBF-FR-H3D-results"
$outRECfolder = Join-Path $scriptDir "..\src\recognition\output"

# Path to run_H3D.ps1
$runScript = Join-Path $scriptDir "..\src\partitioning\scripts\run_H3D.ps1"

$recognitionCodeDir = Join-Path $scriptDir "..\src\recognition\code"

# Call run_H3D.ps1 with overridden exePath
Write-Host "Running $runScript with exePath = $customExe" 

# Run partioning
& "$runScript" -exePath "$customExe" -outputLASPath "$outLASfolder"

# Change to the build directory
Set-Location $recognitionCodeDir

# Run recognition
& $matlabExe -batch "main"

# Run metrics
$args = @(
    "-l", "$outLASfolder",
    "-f", "$outRECfolder"
)

& $metricsExe @args